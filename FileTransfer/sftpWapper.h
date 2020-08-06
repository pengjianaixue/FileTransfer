#pragma once

#include <QObject>
#include <string>
#include <windows.h>
//#include <WinSock2.h>
#include"..\..\FileTransfer\packages\libssh2-vc141_xp.1.8.2\lib\native\include\libssh2.h"
#include"..\..\FileTransfer\packages\libssh2-vc141_xp.1.8.2\lib\native\include\libssh2_sftp.h"
//#include <libssh2.h>
//#include <libssh2_sftp.h>
class sftpWapper : public QObject
{
	Q_OBJECT

public:
	static constexpr size_t readSizeBlock = 1024 * 8;
	struct SFPSession
	{
		std::string ipAddress{};
		std::string userName{};
		std::string password{};
		std::string ftpPath{};
		std::string publicKeyFilePath{};
		std::string rsaFilePath{};
		
	};
	sftpWapper(const SFPSession& sessionData);
	~sftpWapper();
	bool startSFTPSeesion();
	bool creatSFTPSeeion();
	std::string getFtpPath() const;
	void setFtpPath(const std::string& path);
	void shutdown();
	static void kbd_callback(const char *name, int name_len,
		const char *instruction, int instruction_len,
		int num_prompts,
		const LIBSSH2_USERAUTH_KBDINT_PROMPT *prompts,
		LIBSSH2_USERAUTH_KBDINT_RESPONSE *responses,
		void **abstract);

	// read/write API
	bool readFileFromRemote(const std::string& fileName);
	bool writeFileToRemote(const std::string& fileName);

private:
	
	SOCKET   m_socketHandler{ };
	int m_rc{};
	LIBSSH2_SESSION *m_session{ nullptr };
	LIBSSH2_SFTP *m_sftp_session{ nullptr };
	LIBSSH2_SFTP_HANDLE *m_sftp_handle{ nullptr };
	SFPSession m_sessionData{};
	struct sockaddr_in m_sin {};
};
