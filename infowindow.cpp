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

    //Database Set-Up
    m_path = QDir::currentPath() + "/Resources/final_Database_2.db";
    mainAccess.SetDatabaseName(m_path);

    ui->setupUi(this);

    //Adds all Personas to View using the default No Spoilers/DLC
    AddToView(mainAccess.GetAllNoSpoilsDLC());

    //Sets up the default Stat & Weakness tables
    SetTableViews();



//////////////All of signal/slot listeners///////////////////////////////

          /////////////Waits for mouse click//////////////////
    connect(ui-> fuisonButton, SIGNAL(clicked()), this, SLOT(FusionPress()));
    connect(ui->forwardFusion, SIGNAL(clicked()), this, SLOT(ForwardPress()));
    connect(ui->searchBtn, SIGNAL(clicked()), this, SLOT(SearchPress()));
    connect(ui-> actionExit, SIGNAL(triggered()), this, SLOT(Exit()));
    connect(ui-> actionAbout, SIGNAL(triggered()), this, SLOT(OpenAbout()));
    connect(ui-> actionHelp, SIGNAL(triggered()), this, SLOT(OpenHelp()));
    connect(ui-> actionSettings, SIGNAL(triggered()), this, SLOT(OpenSettings()));
        /////////Waits for Enter to be pressed/////////
    connect(ui->serachBox, SIGNAL(returnPressed()),this, SLOT(SearchPress()));

/////////////////////////////////////////////////////////////////////////
}

InfoWindow::~InfoWindow()
{
    delete ui;
}


////////////All of the slot methods////////////////////


//Displays the Persona that the user selected
void InfoWindow::TheClick (QModelIndex index)
{

    QPixmap picture;
    modelTableStat = new QStandardItemModel(5,2,this);
    modelTableMagic = new QStandardItemModel(10,2,this);
    QStringList stats;
    QStringList magic;

    //Cast model to QStringModel so program can get information from it
    listModel= qobject_cast<QStringListModel*>(ui->personaView ->model());

    //Need this to query the database
    QString name = listModel->stringList().at(index.row());

    //Gets the selected persona from the database
    selection = mainAccess.GetSinglePersona(name);
    picture = mainAccess.GetSinglePersonaImage(name);

    //Displays all of the basic Persona Info to the InfoWindow
    ui ->personaName->setText(selection.m_name);
    ui ->personaArcana->setText(selection.m_arcana);

    //level needs to be a QString again :/
    QString level = QString::number(selection.m_level);
    ui -> personaLevel->setText(level);

    //Displays Persona's Image, and keeps the Aspect ration
    ui->personaImage->setPixmap(picture.scaled(ui->personaImage->size(),Qt::KeepAspectRatio));


    //Default set up for Tables
    SetTableViews();


    //Gets the stat and magic info
    stats = mainAccess.GetInfoStat(selection.m_name);
    magic = mainAccess.GetInfoMagic(selection.m_name);

    //Adds the data to their respective tables
    AddToUiInfo(modelTableStat,stats);
    AddToUiInfo(modelTableMagic,magic);


}

//Performs Reverse Fusion and shows Fusion Window
void InfoWindow::FusionPress()
{
    //Creates new Fusion Page
    f = new FusionPage(this);


    //Checks to see if persona is even fuseable
    if(!selection.m_fuseable)
    {
        QMessageBox noFuse;
        noFuse.setText("Sorry this Persona cannot be fused.");
        noFuse.setInformativeText("Please try a different Persona!");
        noFuse.setIcon(QMessageBox::Critical);
        noFuse.exec();
    }
    //Checks to see if persona is Treasure Demon
    else if(selection.m_treasure)
    {
        QMessageBox noFuse;
        noFuse.setText("Sorry this Persona cannot be fused.");
        noFuse.setInformativeText("This is a Treasure Demon and cannot be Reverse Fused!");
        noFuse.setIcon(QMessageBox::Critical);
        noFuse.exec();
    }
    else
    {
        //Sets the cursor to wait so the user can't spam click buttons
        this->setCursor(Qt::WaitCursor);

        //This gets the Arcana Pairs needed
        //and the fusion begins
        f->GetResultArcana(selection, SendBools());
        f->show();

        this->setCursor(Qt::ArrowCursor);
    }




}

//Performs Forward Fusion and shows Fusion Window
void InfoWindow::ForwardPress()
{

    f = new FusionPage(this);

    //Checks to see if persona is even fuseable
    if(!selection.m_fuseable)
    {
        QMessageBox noFuse;
        noFuse.setText("Sorry this Persona cannot be forward fused.");
        noFuse.setInformativeText("Please try a different Persona!");
        noFuse.setIcon(QMessageBox::Critical);
        noFuse.exec();
    }
    //Checks to see if persona is Treasure Demon
    else if(selection.m_treasure)
    {
        QMessageBox noFuse;
        noFuse.setText("Sorry this Persona cannot be fused at this time!");
        noFuse.setInformativeText("I haven't implemented Treasure Demons Forward Fusing yet."
                                  "\n"
                                  "Sorry!");
        noFuse.setIcon(QMessageBox::Critical);
        noFuse.exec();
    }
    else
    {


        this->setCursor(Qt::WaitCursor);

        //Creates a Message Box that prevents it from going into
        //"Not Responding" mode while the calcuations are done

        QMessageBox pleaseWait;
        pleaseWait.setInformativeText("Your Results are loading, please wait!");
        pleaseWait.setIcon(QMessageBox::Information);
        pleaseWait.setWindowTitle("Please Wait!");

        //Prevents the user from closing it
        pleaseWait.setCursor(Qt::WaitCursor);
        pleaseWait.setStandardButtons(0);

        //Cant use .exec() otherwise the next part of the method
        //Won't start until the user closes this Message Box
        pleaseWait.show();

        //Starts the Forward Fusion and displays the results
        f->FuseForward(selection, SendBools());
        f->show();

        //Closes the MessageBox when the Fusion Page shows up
        if(f->isVisible() && pleaseWait.isVisible())
            pleaseWait.close();


        this->setCursor(Qt::ArrowCursor);
    }
}

//Runs the Search Methods based on what filters are turned on/off
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

//returns the DLC bool
bool InfoWindow::SendBools()
{
    bool filter;

    filter = showdlc;

    return filter;
}

//Closes the program
void InfoWindow::Exit()
{
    this->close();
}

//Open Settings
void InfoWindow::OpenSettings()
{

    s = new Settings(this);

    s->show();

}

//Opens About
void InfoWindow::OpenAbout()
{
    a = new AboutPage(this);

    a->show();
}

//Opens Help
void InfoWindow::OpenHelp()
{
    h = new Help(this);
    h->show();
}

////////////////////////////////////////////////////////

/////////////////////UI Methods/////////////////////////

//Adds the DataConnection() List to the ListView
void InfoWindow::AddToView(QStringList list)
{
        // Create model for list
        model = new QStringListModel(this);

        //Add Data to the model
        model->setStringList(list);

        //Attach Model to ListView
        //So user can see the list of Personas
        ui -> personaView ->setModel(model);

        //Makes sure there is no edit trigger
        ui->personaView->setEditTriggers(QAbstractItemView::NoEditTriggers);

        //Create new Item Selection Model and ties it to the listview
        selectionModel = ui->personaView->selectionModel();

        //Takes Up/Down keys as well as clicking
        connect(selectionModel,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
        this,SLOT(TheClick(QModelIndex)));


}


//Sets up Default TableView for both tables
void InfoWindow::SetTableViews()
{

    QStringList statsHeader;
    QStringList magic;
    QStringList statBasic;
    QStringList magicBasic;

    //Row and Col values are hardcoded int because
    //they never change unliked the fusion results
    modelTableStat = new QStandardItemModel(5,2,this);
    modelTableMagic = new QStandardItemModel(10,2,this);

    //Header and Row 1 Value Creation

    statsHeader <<"Stat" << "Value";

    statBasic << "Strength"
              << "Magic"
              << "Endurance"
              << "Agility"
              << "Luck";



    magic <<"Magic" << "Value";
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

    //Sets Headers
    modelTableStat->setHorizontalHeaderLabels(statsHeader);
    modelTableMagic->setHorizontalHeaderLabels(magic);

    //Sets Model to their respective views
    ui ->statView ->setModel(modelTableStat);
    ui ->magicView ->setModel(modelTableMagic);

    //Actually adds the data the to Model
    AddToUi(modelTableStat, statBasic);
    AddToUi(modelTableMagic, magicBasic);

}


//Sets Data for each row in the first col
void InfoWindow::AddToUi(QStandardItemModel* models,QStringList list)
{
    int col = 0;
    for(int row = 0; row < list.size(); row++)
    {
        //Gets the current index
        QModelIndex index = models -> index(row,col,QModelIndex());

        models->setData(index,list.at(row));
    }
}


//Sets the Persona specfic info to col 2
void InfoWindow::AddToUiInfo(QStandardItemModel* models, QStringList list)
{
    int col = 1;

    for(int row = 0; row < list.size(); row++)
    {
        //Gets the current index
        QModelIndex index = models -> index(row,col,QModelIndex());

        models->setData(index,list.at(row));
    }
}

//////////////////////////////////////////////////////////

////////////////////Other Methods/////////////////////////

//Gets Data from the Settings Window
//Displays Persons to listView based on the bools
void InfoWindow::GiveData()
{
    showspoilers = s->returnSpoils();
    showdlc = s->returnDLC();

    if(showspoilers && showdlc)
        AddToView(mainAccess.GetAll()); //DLC & Spoiler
    else if(showspoilers)
       AddToView(mainAccess.GetAllSpoils()); //Spoilers
    else if(showdlc)
        AddToView(mainAccess.GetAllDLC()); //DLC
    else
        AddToView(mainAccess.GetAllNoSpoilsDLC()); //No DLC & Spoiler


}
