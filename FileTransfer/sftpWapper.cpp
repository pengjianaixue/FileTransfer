#include "sftpWapper.h"
sftpWapper::sftpWapper(const SFPSession& sessionData)
	: m_sessionData(sessionData)
{
	creatSFTPSeeion();
}

sftpWapper::~sftpWapper()
{
}

bool sftpWapper::creatSFTPSeeion()
{
#ifdef WIN32
	WSADATA wsadata;
	int err;

	err = ::WSAStartup(MAKEWORD(2, 0), &wsadata);
	if (err != 0) 
	{
		fprintf(stderr, "WSAStartup failed with error: %d\n", err);
		return false;
	}
#endif
	unsigned long hostaddr = inet_addr(m_sessionData.ipAddress.c_str());

	m_rc = libssh2_init(0);
	m_socketHandler = ::socket(AF_INET, SOCK_STREAM, 0);
	m_sin.sin_family = AF_INET;
	m_sin.sin_port = ::htons(22);
	m_sin.sin_addr.s_addr = hostaddr;
	if (::connect(m_socketHandler, reinterpret_cast<struct sockaddr*>(&m_sin),
		sizeof(struct sockaddr_in)) != 0) 
	{
		fprintf(stderr, "failed to connect!\n");
		return false;
	}
	m_session = libssh2_session_init();
	if (!m_session)
		return false;
	libssh2_session_set_blocking(m_session, 1);
	m_rc = libssh2_session_handshake(m_session, m_socketHandler);
	if (m_rc) 
	{
		fprintf(stderr, "Failure establishing SSH session: %d\n", m_rc);
		return false;
	}
	const char* fingerprint = libssh2_hostkey_hash(m_session, LIBSSH2_HOSTKEY_HASH_SHA1);
	fprintf(stderr, "Fingerprint: ");
	for (int i = 0; i < 20; i++) 
	{
		fprintf(stderr, "%02X ", (unsigned char)fingerprint[i]);
	}
	fprintf(stderr, "\n");
	char* userauthlist = libssh2_userauth_list(m_session, m_sessionData.userName.c_str(), m_sessionData.userName.length());
	fprintf(stderr, "Authentication methods: %s\n", userauthlist);
	int auth_pw{ 0 };
	if (strstr(userauthlist, "password") != NULL)
	{
		auth_pw |= 1;
	}
	if (strstr(userauthlist, "keyboard-interactive") != NULL)
	{
		auth_pw |= 2;
	}
	if (strstr(userauthlist, "publickey") != NULL)
	{
		auth_pw |= 4;
	}
	if (auth_pw & 1)
	{
		/* We could authenticate via password */
		if (libssh2_userauth_password(m_session, m_sessionData.userName.c_str(), m_sessionData.password.c_str()))
		{
			fprintf(stderr, "Authentication by password failed.\n");
			shutdown();
			return false;
		}
	}
	else if (auth_pw & 2) 
	{
		/* Or via keyboard-interactive */
		if (libssh2_userauth_keyboard_interactive(m_session, m_sessionData.userName.c_str(),
			&kbd_callback)) 
		{
			fprintf(stderr,
				"\tAuthentication by keyboard-interactive failed!\n");
			shutdown();
			return false;
		}
		else 
		{
			fprintf(stderr,
				"\tAuthentication by keyboard-interactive succeeded.\n");
		}
	}
	else if (auth_pw & 4) 
	{
		/* Or by public key */
		if (libssh2_userauth_publickey_fromfile(m_session, m_sessionData.userName.c_str(),
			m_sessionData.publicKeyFilePath.c_str(),
			m_sessionData.rsaFilePath.c_str(), m_sessionData.password.c_str())) 
		{
			fprintf(stderr, "\tAuthentication by public key failed!\n");
			shutdown();
			return false;
		}
		else 
		{
			fprintf(stderr, "\tAuthentication by public key succeeded.\n");
		}
	}
	else 
	{
		fprintf(stderr, "No supported authentication methods found!\n");
		shutdown();
		return false;
	}
	return true;

}

void sftpWapper::setFtpPath(const std::string& path)
{
	m_sessionData.ftpPath = path;
}

void sftpWapper::kbd_callback(const char * name, int name_len, const char * instruction, int instruction_len,
	int num_prompts, const LIBSSH2_USERAUTH_KBDINT_PROMPT * prompts,
	LIBSSH2_USERAUTH_KBDINT_RESPONSE * responses, void ** abstract)
{
	int i{};
	size_t n{};
	char buf[1024];
	(void)abstract;

	fprintf(stderr, "Performing keyboard-interactive authentication.\n");

	fprintf(stderr, "Authentication name: '");
	fwrite(name, 1, name_len, stderr);
	fprintf(stderr, "'\n");

	fprintf(stderr, "Authentication instruction: '");
	fwrite(instruction, 1, instruction_len, stderr);
	fprintf(stderr, "'\n");

	fprintf(stderr, "Number of prompts: %d\n\n", num_prompts);

	for (i = 0; i < num_prompts; i++)
	{
		fprintf(stderr, "Prompt %d from server: '", i);
		fwrite(prompts[i].text, 1, prompts[i].length, stderr);
		fprintf(stderr, "'\n");

		fprintf(stderr, "Please type response: ");
		fgets(buf, sizeof(buf), stdin);
		n = strlen(buf);
		while (n > 0 && strchr("\r\n", buf[n - 1]))
			n--;
		buf[n] = 0;

		responses[i].text = strdup(buf);
		responses[i].length = n;

		fprintf(stderr, "Response %d from user is '", i);
		fwrite(responses[i].text, 1, responses[i].length, stderr);
		fprintf(stderr, "'\n\n");
	}
}

bool sftpWapper::readFileFromRemote(const std::string& fileName)
{
	m_sftp_handle = libssh2_sftp_open(m_sftp_session, (m_sessionData.ftpPath+ "/" + fileName).c_str(), LIBSSH2_FXF_READ, 0);
	if (!m_sftp_handle) 
	{
		fprintf(stderr, "Unable to open file with SFTP: %ld\n",
			libssh2_sftp_last_error(m_sftp_session));
		shutdown();
		return false;
	}
	char mem[readSizeBlock]{};
	ssize_t readBytes{};
	do 
	{
		readBytes = libssh2_sftp_read(m_sftp_handle, mem, sizeof(mem));

	} while (readBytes>0);
	
	return true;
}

bool sftpWapper::writeFileToRemote(const std::string& fileName)
{

	return false;
}

std::string sftpWapper::getFtpPath() const
{
	return m_sessionData.ftpPath;
}

bool sftpWapper::startSFTPSeesion()
{	
	m_sftp_session = libssh2_sftp_init(m_session);
	if (!m_sftp_session) {
		fprintf(stderr, "Unable to init SFTP session\n");
		shutdown();
		return false;
	}
	return true;
}

void sftpWapper::shutdown()
{
	libssh2_session_disconnect(m_session, "Normal Shutdown");
	libssh2_session_free(m_session);
	
}
