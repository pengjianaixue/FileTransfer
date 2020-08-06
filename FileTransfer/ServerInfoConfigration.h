#pragma once

#include <QWidget>
#include "ui_ServerInfoConfigration.h"

class ServerInfoConfigration : public QWidget
{
	Q_OBJECT

public:
	ServerInfoConfigration(QWidget *parent = Q_NULLPTR);
	~ServerInfoConfigration();

private:
	Ui::ServerInfoConfigration ui;
};
