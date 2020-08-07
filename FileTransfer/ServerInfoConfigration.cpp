#include "ServerInfoConfigration.h"

ServerInfoConfigration::ServerInfoConfigration(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
    initUi();
    QObject::connect(ui.lineEdit_IP,&QLineEdit::textChanged,this,&ServerInfoConfigration::sendInformationUpdate);
    QObject::connect(ui.lineEdit_UserName, &QLineEdit::textChanged, this, &ServerInfoConfigration::sendInformationUpdate);
    QObject::connect(ui.lineEdit_Password, &QLineEdit::textChanged, this, &ServerInfoConfigration::sendInformationUpdate);
    QObject::connect(ui.lineEdit_LocalPath, &QLineEdit::textChanged, this, &ServerInfoConfigration::sendInformationUpdate);
    QObject::connect(ui.lineEdit_RemotePath, &QLineEdit::textChanged, this, &ServerInfoConfigration::sendInformationUpdate);
}

ServerInfoConfigration::~ServerInfoConfigration()
{

}

void ServerInfoConfigration::initUi()
{

    ui.lineEdit_IP->setProperty("name", "IPAddress");
    ui.lineEdit_UserName->setProperty("name", "UserName");
    ui.lineEdit_Password->setProperty("name", "Password");
    ui.lineEdit_LocalPath->setProperty("name", "LocalPath");
    ui.lineEdit_RemotePath->setProperty("name", "RemotePath");
}

void ServerInfoConfigration::sendInformationUpdate(const QString& info)
{
    QString infoType = QObject::sender()->property("name").toString();
    emit informationUpdate(InfortypeMap[infoType], info);
}
