#include "favorites.h"
#include "ui_favorites.h"

favorites::favorites(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::favorites)
{
    ui->setupUi(this);
    //Removes help button
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    GetFavs();
}

favorites::~favorites()
{
    delete ui;
}

void favorites::GetFavs()
{


    QStringList namesList;
    QStringList fixedList;
    QDirIterator it("C:\\Users\\Treffy\\Desktop\\favs");
    while (it.hasNext()) {
        qDebug() << it.next();

        namesList.append(it.fileName());

    }

    qDebug() << "File Names uwu: ";
    for (int i = 2; i < namesList.size(); i++)
    {
        fixedList.append(namesList.at(i).split(".").first());
        qDebug() << fixedList.at(i - 2);
    }

    model = new QStringListModel(this);
    model->setStringList(fixedList);
    ui->favView->setModel(model);
    selectionModel = ui->favView->selectionModel();

    //Make Connection uwu
    connect(selectionModel,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
    this,SLOT(Selection(QModelIndex)));

}



void favorites::Selection(QModelIndex index)
{
    listModel= qobject_cast<QStringListModel*>(ui->favView->model());

    QString name = listModel->stringList().at(index.row());

    QString path = "C:\\Users\\Treffy\\Desktop\\favs\\"+name+".json";

    QList<Persona> finalList;

        QFile file( path );
           if( file.open( QIODevice::ReadOnly ) )
           {
               QByteArray bytes = file.readAll();
               file.close();

               QJsonParseError jsonError;
               QJsonDocument document = QJsonDocument::fromJson( bytes, &jsonError );
               if( jsonError.error != QJsonParseError::NoError )
               {
                  qDebug() << "fromJson failed: ";
                   return ;
               }
                QJsonObject jsonObject = document.object();
                QJsonArray jsonArray = jsonObject["list"].toArray();

                //Prints Each in list
                foreach (const QJsonValue & value, jsonArray)
                {
                    QJsonObject obj = value.toObject();
                    qDebug() << obj["name"].toString();
                    Persona per;
                    per.m_name = obj["name"].toString();
                    per.m_arcana = obj["arcana"].toString();
                    per.m_level = obj["level"].toInt();

                    finalList.append(per);

                }
               QJsonObject obj = jsonObject["result"].toObject();

               m_result.m_name = obj["name"].toString();
               m_result.m_arcana = obj["arcana"].toString();
               m_result.m_level = obj["level"].toInt();

               //qDebug() << "Result Persona: " + obj["name"].toString();

           }

           DisplayFavorite(finalList);


}


void favorites::DisplayFavorite(QList<Persona> finalList)
{
    ui->perArcFLbl->setText(m_result.m_arcana);

    ui->PerFLbl->setText(m_result.m_name);

    QString level = QString::number(m_result.m_level);
    ui->perFLvlLbl->setText(level);





    //The Standard Item Model can work as a table to hold all the
    //Results Values
    modelResults = new QStandardItemModel(finalList.size(),6,this);
    QStringList resultsHeader;
    QString first;
    QString second;
    Persona firstPersona;
    Persona secondPersona;

   //Header Titles are created
    resultsHeader << "Persona" << "Arcana" << "Level"
                  << "Persona" << "Arcana" << "Level";

    for(int i = 0; i < (finalList.size()/2); i++)
    {

        for(int col = 0; col < 6; col++)
        {

           //Creates a new Index models and sets it to the current row(i) and col
                                                   // Row, Col
           QModelIndex index = modelResults -> index(i, col, QModelIndex());

           switch (col)
           {
           case 0: // Col 1 - Persona 1: Name
               modelResults->setData(index, finalList.at(i).m_name);
               break;
           case 1: //Col 2 - Persona 1: Arcana
               modelResults->setData(index, finalList.at(i).m_arcana);
               break;
           case 2: //Col 3 - Persona 1: Level
               modelResults->setData(index, finalList.at(i).m_level);
               break;
           case 3: // Col 4 - Persona 2: Name
               modelResults->setData(index, finalList.at(i +1).m_name);
               break;
           case 4: //Col 5 - Persona 2: Arcana
               modelResults->setData(index, finalList.at(i +1).m_arcana);
               break;
           case 5: //Col 6 - Persona 2: Level
               modelResults->setData(index, finalList.at(i +1).m_level);
               break;


           }//End of switch
       }//End of col loop
   }//End of i/row loop


   //Adds header and results into UI
   modelResults->setHorizontalHeaderLabels(resultsHeader);
   ui->resultView->setModel(modelResults);

}
