#include "aboutpage.h"
#include "ui_aboutpage.h"

AboutPage::AboutPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutPage)
{
    ui->setupUi(this);
    //Gets rid of the Question Mark
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

AboutPage::~AboutPage()
{
    delete ui;
}
