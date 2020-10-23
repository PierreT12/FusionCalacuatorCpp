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
    QSqlQuery query("SELECT Personas_Final.name, Arcana.name, Personas_Final.Level,  Personas_Final.Fuseable, Personas_Final.SpecialFusion, Personas_Final.Max_SL  FROM Personas_Final INNER JOIN Arcana ON Arcana.Arcana_ID = Personas_Final.Arcana WHERE Personas_Final.Name = ?");
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
            single.m_fuseable = query.value(3).toBool();
            single.m_sFusion = query.value(4).toBool();
            single.m_maxSL = query.value(5).toBool();


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

QStringList DbAccess::GetInfoStat(QString name)
{
    //Creating List and Query
    QStringList statInfo;
    QSqlQuery query("SELECT Stats_Final.Strength, Stats_Final.Magic, Stats_Final.Endureance, Stats_Final.Agility, Stats_Final.Luck FROM Personas_Final INNER JOIN Stats_Final ON Stats_Final.Stat_ID = Personas_Final.Stat_ID WHERE Name =?");
    query.addBindValue(name);

    if(query.exec())
    {

        while(query.next())
        {
               statInfo.append(query.value(0).toString());
               statInfo.append(query.value(1).toString());
               statInfo.append(query.value(2).toString());
               statInfo.append(query.value(3).toString());
               statInfo.append(query.value(4).toString());
        }
    }


    return statInfo;
}



QStringList DbAccess::GetInfoMagic(QString name)
{
    //Creating List and Query
    QStringList magicInfo;
    QSqlQuery query("SELECT Str_Wea_Final.Phys, Str_Wea_Final.Gun, Str_Wea_Final.Fire, Str_Wea_Final.Ice, Str_Wea_Final.Electric, Str_Wea_Final.Wind, Str_Wea_Final.Psych, Str_Wea_Final.Nuke, Str_Wea_Final.Bless, Str_Wea_Final.Curse FROM Personas_Final INNER JOIN Str_Wea_Final ON Str_Wea_Final.Str_Wea_ID = Personas_Final.Str_Wea_ID WHERE Name = ?");
    query.addBindValue(name);

    if(query.exec())
    {

        while(query.next())
        {
               magicInfo.append(query.value(0).toString());
               magicInfo.append(query.value(1).toString());
               magicInfo.append(query.value(2).toString());
               magicInfo.append(query.value(3).toString());
               magicInfo.append(query.value(4).toString());
               magicInfo.append(query.value(5).toString());
               magicInfo.append(query.value(6).toString());
               magicInfo.append(query.value(7).toString());
               magicInfo.append(query.value(8).toString());
               magicInfo.append(query.value(9).toString());
        }
    }


    return magicInfo;
}

//Query works!!!
QMultiMap<QString,QString> DbAccess::GetPairs(QString arcana)
{
    QMultiMap<QString,QString> pairs;
    QSqlQuery query("SELECT Arcana.Name, Pairs.First_Arcana, Pairs.Second_Arcana FROM PairConnection  INNER JOIN Pairs ON Pairs.Pair_ID = PairConnection.Pair_ID INNER JOIN Arcana ON Arcana.Arcana_ID = PairConnection.Arcana_ID WHERE Arcana.Name = ?");
    query.addBindValue(arcana);

    if(query.exec())
    {
        while(query.next())
        {
            pairs.insert(query.value(1).toString(),query.value(2).toString());


        }
    }


    return pairs;
}

QList<Persona> DbAccess::GetPersonas(QString first, QString resName)
{

    QList<Persona> list;



    QSqlQuery queryFirst("SELECT Personas_Final.name, Arcana.name, Personas_Final.Level,  Personas_Final.Fuseable, Personas_Final.SpecialFusion, Personas_Final.Max_SL  FROM Personas_Final INNER JOIN Arcana ON Arcana.Arcana_ID = Personas_Final.Arcana WHERE Personas_Final.Fuseable = 'TRUE' AND Arcana.Name = ? AND Personas_Final.Name != ?");
    queryFirst.addBindValue(first);
    queryFirst.addBindValue(resName);

    if(queryFirst.exec())
    {

        while(queryFirst.next())
        {
            Persona tempFirst;
            QString temp;
            tempFirst.m_name = queryFirst.value(0).toString();
            tempFirst.m_arcana = queryFirst.value(1).toString();
            temp = queryFirst.value(2).toString();
            tempFirst.m_level = temp.toUInt();

            list.append(tempFirst);

//            qDebug() << tempFirst.m_name;

        }
    }










    return list;

}



QList<int> DbAccess::GetArcanaLevels(QString arcana)
{
    QList<int> levels;
    QSqlQuery query("SELECT Personas_Final.Level FROM Personas_Final INNER JOIN Arcana ON Arcana.Arcana_ID = Personas_Final.Arcana WHERE Personas_Final.Spoiler = 'FALSE' AND Arcana.Name = ?");
    query.addBindValue(arcana);


    if(query.exec())
    {

        while(query.next())
        {
            levels.append(query.value(0).toInt());
        }
    }
    return levels;
}



int DbAccess::GetPK(QString name)
{
    int pK;

    QSqlQuery query("SELECT Personas_Final.Main_ID FROM Personas_Final WHERE Name = ?");
    query.addBindValue(name);


    if(query.exec())
    {

        while(query.next())
        {
            pK = query.value(0).toInt();
        }
    }

    return pK;
}



QStringList DbAccess::GetSpecialResults(int ID)
{
    QStringList results;

    QSqlQuery query("SELECT SpeFusion.Name FROM SpecConnection INNER JOIN SpeFusion ON SpeFusion.SF_ID = SpecConnection.SF_ID INNER JOIN Personas_Final ON Personas_Final.Main_ID = SpecConnection.Main_ID WHERE Personas_Final.Main_ID = ?");
    query.addBindValue(ID);

    if(query.exec())
    {

        while(query.next())
        {
            results.append(query.value(0).toString());
        }
    }

    qDebug() << results;
    return results;
}
