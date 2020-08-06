#pragma once

#include <QtWidgets/QMainWindow>
#include <QtCore/qfilesystemwatcher.h>
#include <QtNetwork/QNetworkConfiguration>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkDiskCache>
#include "ServerInfoConfigration.h"
#include "ui_FileTransfer.h"

class FileTransfer : public QMainWindow
{
    Q_OBJECT
	
public:
    FileTransfer(QWidget *parent = Q_NULLPTR);
	
private:
    Ui::FileTransferClass ui;
	QFileSystemWatcher m_fileWatcher;
	std::unique_ptr<ServerInfoConfigration> m_serverInfoConfigrationForm;
};
