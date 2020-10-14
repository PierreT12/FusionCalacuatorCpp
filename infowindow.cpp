#include "infowindow.h"
#include "ui_infowindow.h"
#include "dbaccess.h"

QString m_path = "C:\\Users\\Treffy\\Desktop\\Pesonal_Project\\Pesonal_Project\\SQL_FILES\\finaL_Database_3.db";

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

//Move to its own method



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
    Persona selection;
    QPixmap picture;
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

}

void InfoWindow::SetTableViews()
{
    modelTableStat = new QStandardItemModel(5,2,this);
    modelTableMagic = new QStandardItemModel(10,2,this);


    ui ->statView ->setModel(modelTableStat);
    ui ->magicView ->setModel(modelTableMagic);
//   QStringList stat = mainAccess.GetInfo(ui->personaName->text());
   //QStringList magic = mainAccess.GetInfo
    for(int row = 0; row < 4; row++)
        {
            int col = 1;

            QModelIndex index
   = modelTableStat->index(row,col,QModelIndex());

            modelTableStat->setData(index,2);

        }




}
