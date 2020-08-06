#include "scpCommandWapper.h"

ScpCommandWapper::ScpCommandWapper(QObject *parent)
	: QObject(parent)
{
}

ScpCommandWapper::~ScpCommandWapper()
{
}

bool ScpCommandWapper::startScp(const QString & scpCommand, FinshNotifyFunction userFinshNotifyFunction)
{
	m_scpProcessRunner.start(scpCommand);
	connect(&m_scpProcessRunner, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), userFinshNotifyFunction);
	return false;
}
