#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    //Gets rid of the ugly question mark button
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    //Listeners for this page
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(Exit()));
    //Sends bool objects to InfoWindow when "Okay" is clicked
    connect(ui->buttonBox, SIGNAL(accepted()), parent, SLOT(GiveData()));

}

Settings::~Settings()
{
    delete ui;
}


//Lets the user know that their changes won't be saved
void Settings::Exit()
{
    QMessageBox noSave;
    noSave.setText("Your selections was not saved!");
    noSave.setIcon(QMessageBox::Warning);
    noSave.exec();
    this->close();

}

//Returns Spoiler bool
bool Settings::returnSpoils()
{
    //Checks Radio Button
    if(ui->yesSpoil->isChecked())
        return true;
    return false;
}

//Returns DLC bool
bool Settings::returnDLC()
{
    //Checks Radio Button
    if(ui->yesDLC->isChecked())
        return true;
    return false;
}
