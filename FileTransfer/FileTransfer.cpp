#include "FileTransfer.h"
#include "stdafx.h"

FileTransfer::FileTransfer(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	m_serverInfoConfigrationForm = std::make_unique<ServerInfoConfigration>();
	m_serverInfoConfigrationForm->hide();
	connect(this->ui.actionServerConfigration, &QAction::triggered, this->m_serverInfoConfigrationForm.get(), [&]() {this->m_serverInfoConfigrationForm->show(); });
}
