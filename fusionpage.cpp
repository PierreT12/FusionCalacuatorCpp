#include "fusionpage.h"
#include "ui_fusionpage.h"

FusionPage::FusionPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FusionPage)
{
    ui->setupUi(this);
}

FusionPage::~FusionPage()
{
    delete ui;
}
