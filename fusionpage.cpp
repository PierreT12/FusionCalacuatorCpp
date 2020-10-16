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
    this->close();

}




void FusionPage::GetResultArcana(Persona ResPersona)
{
    m_result = ResPersona;
    QString level = QString::number(m_result.m_level);


    ui->perArcFLbl->setText(m_result.m_arcana);
    ui->PerFLbl->setText(m_result.m_name);
    ui->perFLvlLbl->setText(level);

    StartFusion(m_result);


}

void FusionPage::StartFusion(Persona result)
{

    //Write SqLite Query to get Matching Arcana Pair
    Fusion fusion(result);

    fusion.StartFusion(result);
    //Write SqLite Query to get all Personas from Each
    //Mathcing Pair




}
