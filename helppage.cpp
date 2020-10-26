#include "helppage.h"
#include "ui_helppage.h"

Help::Help(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Help)
{
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
}

Help::~Help()
{
    delete ui;
}
