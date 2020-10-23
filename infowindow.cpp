#include "infowindow.h"
#include "ui_infowindow.h"
#include "dbaccess.h"


QString m_path = "C:\\Users\\Treffy\\Desktop\\Pesonal_Project\\Pesonal_Project\\SQL_FILES\\finaL_Database_2.db";



//Connects to the database as soon as the program runs for the first time
//Needs to be global to this class so other methods can call it
//Otherwise server will keep randomly ending connections
DbAccess mainAccess(m_path);

InfoWindow::InfoWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::InfoWindow)
{
    ui->setupUi(this);

 AddToView(DataConnection());
 SetTableViews();

connect(ui ->fuisonButton, SIGNAL(clicked()), this, SLOT(FusionPress()));




}

InfoWindow::~InfoWindow()
{
    delete ui;
}
QStringList InfoWindow::DataConnection()
{
    QStringList list;



    //Does a basic query to get all the persona names as soon as
    //The database connection is made
    QSqlQuery query("Select Name FROM Personas_Final ORDER BY Level ASC");
    list = mainAccess.GetAll(query);

    return list;
}

void InfoWindow::AddToView(QStringList list)
{

        // Create model for list
        model = new QStringListModel(this);
        //Add Data to the model
        model->setStringList(list);

        //Attach Model to ListView
        //This will cause Personas to acutally Display
        ui -> personaView ->setModel(model);
        //Sets up a listener to wait for the user to click on a persona
        ui->personaView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        connect(ui->personaView, SIGNAL(clicked(QModelIndex)),this,SLOT(TheClick(QModelIndex)));
}
void InfoWindow::TheClick (QModelIndex index)
{
    //Variables that'll get instanced in a hot sec
    QPixmap picture;
    modelTableStat = new QStandardItemModel(5,2,this);
    modelTableMagic = new QStandardItemModel(10,2,this);
    QStringList stats;
    QStringList magic;
    //Cast model to QStringModel
    QStringListModel* listModel= qobject_cast<QStringListModel*>(ui->personaView ->model());

    QString name = listModel->stringList().at(index.row());

    //Gets the selected persona from the database
    selection = mainAccess.GetSinglePersona(name);

    picture = mainAccess.GetSinglePersonaImage(name);

    //Displays all of the basic Persona Info to the InfoWindow
    ui ->personaName->setText(selection.m_name);
    ui ->personaArcana->setText(selection.m_arcana);
    //Have to convert back to a string
    QString level = QString::number(selection.m_level);
    ui -> personaLevel->setText(level);

    ui->personaImage->setPixmap(picture);
    SetTableViews();



    stats = mainAccess.GetInfoStat(selection.m_name);
    AddToUiInfo(modelTableStat,stats);
    magic = mainAccess.GetInfoMagic(selection.m_name);
    AddToUiInfo(modelTableMagic,magic);


}
void InfoWindow::FusionPress()
{




    f = new FusionPage(this);



    if(!selection.m_fuseable)
    {
        QMessageBox noFuse;
        noFuse.setText("Sorry this Persona cannot be fused.");
        noFuse.setInformativeText("Please try a different Persona!");
        noFuse.setIcon(QMessageBox::Critical);
        noFuse.exec();

    }
    else
    {
        f->GetResultArcana(selection);
        f->show();
    }
}



void InfoWindow::SetTableViews()
{
    modelTableStat = new QStandardItemModel(5,2,this);
    modelTableMagic = new QStandardItemModel(10,2,this);
    QStringList stats;
    QStringList magic;
    QStringList statBasic;
    QStringList magicBasic;


    stats <<"Stat" << "Value";
    statBasic << "Strength" << "Magic"
              << "Endurance" << "Agility"
              << "Luck";
    modelTableStat->setHorizontalHeaderLabels(stats);

    ui ->statView ->setModel(modelTableStat);
    AddToUi(modelTableStat, statBasic);



    magic <<"Magic" << "Value";
    //This is just the names for right now
    //Later will be the images
    magicBasic <<"Physical" << "Gun"
              <<"Fire" <<"Ice"
              << "Elec" << "Wind"
              << "Psych" << "Nuke"
              << "Bless" << "Curse";
    modelTableMagic->setHorizontalHeaderLabels(magic);
    ui ->magicView ->setModel(modelTableMagic);
    AddToUi(modelTableMagic, magicBasic);









}

void InfoWindow::AddToUi(QStandardItemModel* models,
                         QStringList list)
{



    for(int row = 0; row < list.size(); row++)
        {
            int col = 0;




            QModelIndex index
   = models -> index(row, col, QModelIndex());

            models->setData(index,list.at(row));

        }




}



void InfoWindow::AddToUiInfo(QStandardItemModel* models,
                             QStringList list)
{



    for(int row = 0; row < list.size(); row++)
        {
            int col = 1;




            QModelIndex index
   = models -> index(row, col, QModelIndex());

            models->setData(index,list.at(row));


        }




}
