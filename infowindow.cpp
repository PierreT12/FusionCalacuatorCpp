#include "infowindow.h"
#include "ui_infowindow.h"
#include "dbaccess.h"




//Need to be global because
//three diff methods use
bool showspoilers = false;
bool showdlc = false;



InfoWindow::InfoWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::InfoWindow)
{
 m_path = QDir::currentPath() + "/final_Database_2.db";

   mainAccess.SetDatabaseName(m_path);

    ui->setupUi(this);

 AddToView(DataConnection());
 SetTableViews();




//All of signal/slot listeners(?)
connect(ui-> fuisonButton, SIGNAL(clicked()), this, SLOT(FusionPress()));
connect(ui->forwardFusion, SIGNAL(clicked()), this, SLOT(ForwardPress()));
connect(ui->searchBtn, SIGNAL(clicked()), this, SLOT(SearchPress()));
connect(ui-> actionExit, SIGNAL(triggered()), this, SLOT(Exit()));
connect(ui-> actionAbout, SIGNAL(triggered()), this, SLOT(OpenAbout()));
connect(ui-> actionHelp, SIGNAL(triggered()), this, SLOT(OpenHelp()));
connect(ui-> actionSettings, SIGNAL(triggered()), this, SLOT(OpenSettings()));



}

InfoWindow::~InfoWindow()
{
    delete ui;
}

QStringList InfoWindow::DataConnection()
{
    QStringList list;
    list = mainAccess.GetAllNoSpoilsDLC();

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

        //Create new Item Selection Model
        selectionModel = ui->personaView->selectionModel();

        //Takes Up/Down keys as well as clicking
        connect(selectionModel,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
        this,SLOT(TheClick(QModelIndex)));


}

///All of the slot methods
//Button/ListView Presses on InfoPage
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




    ui->personaImage->setPixmap(picture.scaled(ui->personaImage->size(),Qt::KeepAspectRatio));



    SetTableViews();



    stats = mainAccess.GetInfoStat(selection.m_name);
    magic = mainAccess.GetInfoMagic(selection.m_name);

    AddToUiInfo(modelTableStat,stats);
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
        this->setCursor(Qt::WaitCursor);
        f->GetResultArcana(selection);
        f->show();
        this->setCursor(Qt::ArrowCursor);
    }




}

void InfoWindow::ForwardPress()
{

    f = new FusionPage(this);

    if(!selection.m_fuseable || !selection.m_spoiler)
    {
        QMessageBox noFuse;
        noFuse.setText("Sorry this Persona cannot be forward fused.");
        noFuse.setInformativeText("Please try a different Persona!");
        noFuse.setIcon(QMessageBox::Critical);
        noFuse.exec();
    }
    else
    {
        this->setCursor(Qt::WaitCursor);
        f->FuseForward(selection);
        f->show();
        this->setCursor(Qt::ArrowCursor);
    }
}

void InfoWindow::SearchPress()
{
    QString search = ui->serachBox->text();
    if(showspoilers && showdlc)
        AddToView(mainAccess.AllSearch(search));

    else if(showspoilers)
        AddToView(mainAccess.NoDLCSearch(search));

    else if (showdlc)
        AddToView(mainAccess.NoSpoilSearch(search));

    else
        AddToView(mainAccess.NoSpoilDLCSearch(search));

}



//MenuBar Button Presses
void InfoWindow::Exit()
{
    this->close();
}
void InfoWindow::OpenSettings()
{

    s = new Settings(this);

    s->show();

}
void InfoWindow::OpenAbout()
{
    a = new AboutPage(this);

    a->show();
}
void InfoWindow::OpenHelp()
{
    h = new Help(this);
    h->show();
}
void InfoWindow::GiveData()
{
    showspoilers = s->returnSpoils();
    showdlc = s->returnDLC();

    if(showspoilers && showdlc)
        AddToView(mainAccess.GetAll());
    else if(showspoilers)
       AddToView(mainAccess.GetAllSpoils());
    else if(showdlc)
        AddToView(mainAccess.GetAllDLC());
    else
        AddToView(mainAccess.GetAllNoSpoilsDLC());


}


void InfoWindow::SetTableViews()
{

    QStringList statsHeader;
    QStringList magic;
    QStringList statBasic;
    QStringList magicBasic;

    modelTableStat = new QStandardItemModel(5,2,this);
    modelTableMagic = new QStandardItemModel(10,2,this);

    statsHeader <<"Stat" << "Value";

    statBasic << "Strength"
              << "Magic"
              << "Endurance"
              << "Agility"
              << "Luck";

    modelTableStat->setHorizontalHeaderLabels(statsHeader);

    ui ->statView ->setModel(modelTableStat);
    AddToUi(modelTableStat, statBasic);


    magic <<"Magic" << "Value";
    //This is just the names for right now
    //Later will be the images
    magicBasic <<"Physical"
              << "Gun"
              <<"Fire"
              <<"Ice"
              << "Elec"
              << "Wind"
              << "Psych"
              << "Nuke"
              << "Bless"
              << "Curse";

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

            QModelIndex index = models -> index(row,
                                                col,
                                                QModelIndex());

            models->setData(index,list.at(row));

        }




}



void InfoWindow::AddToUiInfo(QStandardItemModel* models,
                             QStringList list)
{

    for(int row = 0; row < list.size(); row++)
        {
            int col = 1;

            QModelIndex index = models -> index(row,
                                                col,
                                                QModelIndex());

            models->setData(index,list.at(row));

        }
}
