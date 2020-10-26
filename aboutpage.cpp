#include "aboutpage.h"
#include "ui_aboutpage.h"

AboutPage::AboutPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutPage)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

AboutPage::~AboutPage()
{
    delete ui;
}
