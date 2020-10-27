#include "fusionpage.h"
#include "ui_fusionpage.h"

FusionPage::FusionPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FusionPage)
{
    ui->setupUi(this);
    //Removes help button
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    //Gets the relative path for the database
    m_path = QDir::currentPath() + "/Resources/final_Database_2.db";
    fusionAccess.SetDatabaseName(m_path);
}

FusionPage::~FusionPage()
{
    this->close();
}



//Displays the selected Persona information from the previous page
//on to this page and starts the fusion
void FusionPage::GetResultArcana(Persona ResPersona, bool filter)
{

    QString level = QString::number(ResPersona.m_level);
    QMultiMap<QString,QString> matches;

    ui->perArcFLbl->setText(ResPersona.m_arcana);
    ui->PerFLbl->setText(ResPersona.m_name);
    ui->perFLvlLbl->setText(level);

    StartFusion(ResPersona, filter);

}


//Adds Text to Label and starts certain fusions
//depending on the Persona's type
void FusionPage::StartFusion(Persona result, bool filter)
{
    Fusion fusion(result);
    QStringList finalResults;
    QString text;


    if(result.m_maxSL) //Persona is Max Social Link?
    {
        text = "This Persona requires a Max Social Link!";
        ui->mlLbl->setText(text);

        if(result.m_sFusion) //Persona Special Fusion?
        {
            Special(text);
            finalResults = fusion.SpecialFusion(result);
        } // Special Fusion close
        else
            finalResults = fusion.StartFusion(result,filter);
    } // MaxSL close
    else
        if(result.m_sFusion) //Persona Special Fusion?
        {
            Special(text);
            finalResults = fusion.SpecialFusion(result);

        }// Special Fusion 2 close
    else
       finalResults = fusion.StartFusion(result,filter); //Default to this is all else fails


    DisplayAllResults(finalResults);


}

//Displays selected Persona's information from the previous page
//And then just does the forward fuse
void FusionPage::FuseForward(Persona p1, bool filter)
{
    Fusion fusion(p1);
    QStringList finalResults;


    ui->perArcFLbl->setText(p1.m_arcana);
    ui->PerFLbl->setText(p1.m_name);
    QString level = QString::number(p1.m_level);
    ui->perFLvlLbl->setText(level);


    finalResults = fusion.StartForwardFusion(p1, filter);

   DisplayAllResults(finalResults);
}

//Puts all the fusions results on the screen for the user to see
void FusionPage::DisplayAllResults(QStringList finalResults)
{

     //The Standard Item Model can work as a table to hold all the
     //Results Values
     modelResults = new QStandardItemModel(finalResults.size(),6,this);
     QStringList resultsHeader;
     QString first;
     QString second;
     Persona firstPersona;
     Persona secondPersona;

    //Header Titles are created
     resultsHeader << "Persona" << "Arcana" << "Level"
                   << "Persona" << "Arcana" << "Level";

     for(int i = 0; i < finalResults.size(); i++)
     {
         //Gets each Persona Name and pulls up their information
         first = finalResults.at(i).split(" : ").first();
         second = finalResults.at(i).split(" : ").last();

         firstPersona = fusionAccess.GetSinglePersona(first);
         secondPersona = fusionAccess.GetSinglePersona(second);


         for(int col = 0; col < 6; col++)
         {

            //Creates a new Index models and sets it to the current row(i) and col
                                                    // Row, Col
            QModelIndex index = modelResults -> index(i, col, QModelIndex());

            switch (col)
            {
            case 0: // Col 1 - Persona 1: Name
                modelResults->setData(index, firstPersona.m_name);
                break;
            case 1: //Col 2 - Persona 1: Arcana
                modelResults->setData(index, firstPersona.m_arcana);
                break;
            case 2: //Col 3 - Persona 1: Level
                modelResults->setData(index, firstPersona.m_level);
                break;
            case 3: // Col 4 - Persona 2: Name
                modelResults->setData(index, secondPersona.m_name);
                break;
            case 4: //Col 5 - Persona 2: Arcana
                modelResults->setData(index, secondPersona.m_arcana);
                break;
            case 5: //Col 6 - Persona 2: Level
                modelResults->setData(index, secondPersona.m_level);
                break;


            }//End of switch
        }//End of col loop
    }//End of i/row loop


    //Adds header and results into UI
    modelResults->setHorizontalHeaderLabels(resultsHeader);
    ui->resultView->setModel(modelResults);


}


//Just sets the Specail Fusion Label
//Did this so the if/else statments looked a bit cleaner
void FusionPage::Special(QString text)
{
    text = "This is a Special Fusion "
                  "All of these Personas are required!";
    ui->spFusion->setText(text);
}
