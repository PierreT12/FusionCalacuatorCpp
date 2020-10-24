#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(Exit()));
    connect(ui->buttonBox, SIGNAL(accepted()), parent, SLOT(GiveData()));

}

Settings::~Settings()
{
    delete ui;
}



void Settings::Exit()
{
    QMessageBox noSave;
    noSave.setText("Your selections will not be saved!");
    noSave.setIcon(QMessageBox::Warning);
    noSave.exec();
    this->close();

}

bool Settings::returnSpoils()
{
    if(ui->yesSpoil->isChecked())
        return true;
    return false;
}

bool Settings::returnDLC()
{
    if(ui->yesDLC->isChecked())
        return true;
    return false;
}
