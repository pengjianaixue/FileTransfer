#pragma once

#include <QObject>
#include <QProcess>
class ScpCommandWapper : public QObject
{
	Q_OBJECT

public:
	using FinshNotifyFunction = std::function<void(int, QProcess::ExitStatus)>;
	ScpCommandWapper(QObject *parent);
	~ScpCommandWapper();
	bool startScp(const QString& scpCommand, FinshNotifyFunction userFinshNotifyFunction);
private:
	QProcess m_scpProcessRunner;
};
