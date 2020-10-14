#include "dbaccess.h"



DbAccess::DbAccess(const QString& path)
{
   m_db = QSqlDatabase::addDatabase("QSQLITE");
   m_db.setDatabaseName(path);

   if (!m_db.open())
   {
      qDebug() << "Error: connection with database fail";
   }
   else
   {
      qDebug() << "Database: connection ok";
   }
}

QStringList  DbAccess::GetAll(QSqlQuery query)
{
    QStringList list;
    int id = query.record().indexOf("name");
    while(query.next())
    {
        QString name = query.value(id).toString();
           list << name;
    }

    return list;
}

Persona DbAccess::GetSinglePersona(QString indexName)
{
    //Creates Query and Persona
    QSqlQuery query("SELECT Name, Arcana, Level FROM Personas_Final WHERE Name = ?");
    query.addBindValue(indexName);
    Persona single;
    //Looks for a Persona that matches the name of the
    //Selected Persona in the ListView

//Working now
    if(query.exec())
    {

        while(query.next())
        {
            QString temp;
            single.m_name = query.value(0).toString();
            single.m_arcana = query.value(1).toString();
            //Database still stores these as strings for some reason
            //Changed it to int like a month ago :/
            temp = query.value(2).toString();
            single.m_level = temp.toUInt();

        }

    }
return single;
}




QPixmap DbAccess::GetSinglePersonaImage(QString indexName)
{
    //Creates pixmap that'll store image
    QPixmap outPixmap = QPixmap();
    //Creates Query and Persona
    QSqlQuery query("SELECT Image FROM Personas_Final WHERE Name = ?");
    query.addBindValue(indexName);

    //Looks for a Persona that matches the name of the
    //Selected Persona in the ListView

//Working now
    if(query.exec())
    {
    QByteArray outByteArray;
        while(query.next())
        {
            outByteArray = query.value( 0 ).toByteArray();
        }

        outPixmap.loadFromData( outByteArray );
    }
return outPixmap;
}

