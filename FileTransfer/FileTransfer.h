#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_FileTransfer.h"

class FileTransfer : public QMainWindow
{
    Q_OBJECT

public:
    FileTransfer(QWidget *parent = Q_NULLPTR);

private:
    Ui::FileTransferClass ui;
};
