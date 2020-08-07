#pragma once

#include <QWidget>
#include "ui_ServerInfoConfigration.h"

class ServerInfoConfigration : public QWidget
{
	Q_OBJECT

public:
	ServerInfoConfigration(QWidget *parent = Q_NULLPTR);
	~ServerInfoConfigration();
    enum  class Infortype:int
    {
        IPAddress,
        UserName,
        PassWord,
        LocalPath,
        RemotePath
    };
signals:
    void informationUpdate(Infortype infortype, const QString& info);
public slots:
    void sendInformationUpdate(const QString& info);

private:
	Ui::ServerInfoConfigration ui;
    void initUi();
    inline static std::map<QString, Infortype> InfortypeMap{ {"IPAddress",Infortype::IPAddress},{"UserName",Infortype::UserName},
    {"Password",Infortype::PassWord},{"LocalPath",Infortype::LocalPath},{"RemotePath",Infortype::RemotePath} };
    
};
