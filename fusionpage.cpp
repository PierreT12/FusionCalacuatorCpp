#include "fusionpage.h"
#include "ui_fusionpage.h"

FusionPage::FusionPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FusionPage)
{
    ui->setupUi(this);
     m_path = QDir::currentPath() + "/final_Database_2.db";
     fusionAccess.SetDatabaseName(m_path);
}

FusionPage::~FusionPage()
{
    this->close();
}




void FusionPage::GetResultArcana(Persona ResPersona)
{

    QString level = QString::number(ResPersona.m_level);
    QMultiMap<QString,QString> matches;

    ui->perArcFLbl->setText(ResPersona.m_arcana);
    ui->PerFLbl->setText(ResPersona.m_name);
    ui->perFLvlLbl->setText(level);

   StartFusion(ResPersona);

}

void FusionPage::StartFusion(Persona result)
{
    Fusion fusion(result);
    QStringList finalResults;

    if(result.m_sFusion)
      finalResults = fusion.SpecialFusion(result);

    else
       finalResults = fusion.StartFusion(result);

    DisplayAllResults(finalResults);

    //Do Treasure fusions

}

void FusionPage::FuseForward(Persona p1)
{
    Fusion fusion(p1);
    QStringList finalResults;



    ui->perArcFLbl->setText(p1.m_arcana);
    ui->PerFLbl->setText(p1.m_name);
    QString level = QString::number(p1.m_level);
    ui->perFLvlLbl->setText(level);





   finalResults = fusion.StartForwardFusion(p1);
   DisplayAllResults(finalResults);
}


void FusionPage::DisplayAllResults(QStringList finalResults)
{

     modelResults = new QStandardItemModel(finalResults.size(),6,this);
     QStringList resultsHeader;
     QString first;
     QString second;
     Persona firstPersona;
     Persona secondPersona;


     resultsHeader << "Persona" << "Arcana" << "Level"
                   << "Persona" << "Arcana" << "Level";
    for(int i = 0; i < finalResults.size(); i++)
    {
        first = finalResults.at(i).split(" : ").first();
        second = finalResults.at(i).split(" : ").last();

        firstPersona = fusionAccess.GetSinglePersona(first);
        secondPersona = fusionAccess.GetSinglePersona(second);


        for(int col = 0; col < 6; col++)
        {


            QModelIndex index = modelResults -> index(i, col, QModelIndex());

            switch (col)
            {
            case 0:
                modelResults->setData(index, firstPersona.m_name);
                break;
            case 1:
                modelResults->setData(index, firstPersona.m_arcana);
                break;
            case 2:
                modelResults->setData(index, firstPersona.m_level);
                break;
            case 3:
                modelResults->setData(index, secondPersona.m_name);
                break;
            case 4:
                modelResults->setData(index, secondPersona.m_arcana);
                break;
            case 5:
                modelResults->setData(index, secondPersona.m_level);
                break;


            }


        }


    }



    modelResults->setHorizontalHeaderLabels(resultsHeader);
    ui->resultView->setModel(modelResults);


}
