#include "dbaccess.h"


DbAccess::DbAccess(){}

//Connects to the database
void DbAccess::SetDatabaseName(const QString& path)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);
    m_db.open();
}
/////////////////////////////Settings for Listview Queries/////////////////////
////Returns a list of all Persona Names
/// Depending on what the user picked in the settings
//////////////////////////////////////////////////////////////////////////////


QStringList DbAccess::GetAllNoSpoilsDLC()
{
    QStringList list;

    QSqlQuery query("Select Name "
                    "FROM Personas_Final "
                    "WHERE Personas_Final.Spoiler = 'FALSE'"
                    "AND Personas_Final.DLC = 'FALSE'"
                    "ORDER BY Level ASC ");
    int id = query.record().indexOf("name");

    while(query.next())
    {
        QString name = query.value(id).toString();
           list.append(name);
    }
    return list;
}

//Returns a list of all Persona Names that aren't Spoilers
QStringList DbAccess::GetAllDLC()
{
    QStringList list;

    QSqlQuery query("Select Name "
                    "FROM Personas_Final "
                    "WHERE Personas_Final.Spoiler = 'FALSE' "
                    "ORDER BY Level ASC");
    int id = query.record().indexOf("name");

    while(query.next())
    {
        QString name = query.value(id).toString();
           list.append(name);
    }
    return list;
}

//Returns a list of all Persona Names that aren't DLC
QStringList DbAccess::GetAllSpoils()
{
    QStringList list;

    QSqlQuery query("Select Name "
                    "FROM Personas_Final "
                    "WHERE Personas_Final.DLC = 'FALSE' "
                    "ORDER BY Level ASC");
    int id = query.record().indexOf("name");

    while(query.next())
    {
        QString name = query.value(id).toString();
           list.append(name);
    }
    return list;
}


QStringList DbAccess::GetAll()
{
    QStringList list;

    QSqlQuery query("Select Name "
                    "FROM Personas_Final "
                    "ORDER BY Level ASC");
    int id = query.record().indexOf("name");

    while(query.next())
    {
        QString name = query.value(id).toString();
           list.append(name);
    }
    return list;
}

///////////////////////////////////////////////////////////////////////////////


///////////////////////////Search Queries/////////////////////////////////////
////Returns a list of Personas that match the text that the user provides
/// and filters it based on what the user picked in the settings
//////////////////////////////////////////////////////////////////////////////


QStringList DbAccess::NoSpoilDLCSearch(QString searchTerm)
{
    QStringList results;

    //Makes sure the query looks for an exact match
    //and anything that contains the searchTerm
    searchTerm = "%" + searchTerm + "%";

    QSqlQuery query("SELECT Personas_Final.Name "
                    "FROM Personas_Final "
                    "WHERE Personas_Final.Spoiler = 'FALSE'"
                    "AND Personas_Final.DLC = 'FALSE'"
                    "AND Personas_Final.Name LIKE ?"
                    "ORDER BY Level ASC");
        query.addBindValue(searchTerm);


    if(query.exec())
    {
        while(query.next())
        {
            QString name = query.value(0).toString();
            results.append(name);
        }
    }
    return results;
}


QStringList DbAccess::NoSpoilSearch(QString searchTerm)
{
    QStringList results;

    //Makes sure the query looks for an exact match
    //and anything that contains the searchTerm
    searchTerm = "%" + searchTerm + "%";

    QSqlQuery query("Select Name "
                    "FROM Personas_Final "
                    "WHERE Personas_Final.Spoiler = 'FALSE' "
                    "AND Personas_Final.Name LIKE ? "
                    "ORDER BY Level ASC");
        query.addBindValue(searchTerm);

        if(query.exec())
        {
            while(query.next())
            {
                QString name = query.value(0).toString();
                results.append(name);
            }
        }
    return results;
}


QStringList DbAccess::NoDLCSearch(QString searchTerm)
{
    QStringList results;

    //Makes sure the query looks for an exact match
    //and anything that contains the searchTerm
    searchTerm = "%" + searchTerm + "%";

    QSqlQuery query("Select Name "
                    "FROM Personas_Final "
                    "WHERE Personas_Final.DLC = 'FALSE' "
                    "AND Personas_Final.Name LIKE ? "
                    "ORDER BY Level ASC");
    query.addBindValue(searchTerm);

    if(query.exec())
    {
        while(query.next())
        {
            QString name = query.value(0).toString();
            results.append(name);
        }
    }
    return results;
}


QStringList DbAccess::AllSearch(QString searchTerm)
{
    QStringList results;

    //Makes sure the query looks for an exact match
    //and anything that contains the searchTerm
    searchTerm = "%" + searchTerm + "%";

    QSqlQuery query("Select Name "
                    "FROM Personas_Final "
                    "WHERE Personas_Final.Name LIKE ?"
                    "ORDER BY Level ASC");
    query.addBindValue(searchTerm);

    if(query.exec())
    {
        while(query.next())
        {
            QString name = query.value(0).toString();
            results.append(name);
        }
    }
    return results;
}

////////////////////////////////////////////////////////////////////////////


/////////////////////////Displaying to Listview Queiers////////////////////
///Gets all required information from the Database when
/// a Persona is clicked
//////////////////////////////////////////////////////////////////////////

//Puts the selected Persona into a new Persona object
//And gets the basic info from the database
Persona DbAccess::GetSinglePersona(QString indexName)
{

    Persona selection;
    QSqlQuery query("SELECT Personas_Final.name, "
                    "Arcana.name, "
                    "Personas_Final.Level, "
                    "Personas_Final.Fuseable, "
                    "Personas_Final.SpecialFusion, "
                    "Personas_Final.Max_SL, "
                    "Personas_Final.Treasure "
                    "FROM Personas_Final "
                    "INNER JOIN Arcana "
                    "ON Arcana.Arcana_ID = Personas_Final.Arcana "
                    "WHERE Personas_Final.Name = ?");
    query.addBindValue(indexName);

    if(query.exec())
    {

        while(query.next())
        {

            QString temp;


            selection.m_name = query.value(0).toString();
            selection.m_arcana = query.value(1).toString();

            //The database still stores Level as a string
            //And will error out if it isn't converted

            temp = query.value(2).toString();
            selection.m_level = temp.toUInt();
            selection.m_fuseable = query.value(3).toBool();
            selection.m_sFusion = query.value(4).toBool(); //Special Fusion
            selection.m_maxSL = query.value(5).toBool(); //Max Social Link
            selection.m_treasure = query.value(6).toBool();
        }

    }
    return  selection;
}

//Returns the image assoicated with the persona
QPixmap DbAccess::GetSinglePersonaImage(QString indexName)
{
    //Creates pixmap that'll store image
    QPixmap outPixmap = QPixmap();
    //Creates Query and Persona
    QSqlQuery query("SELECT Image "
                    "FROM Personas_Final "
                    "WHERE Name = ?");
    query.addBindValue(indexName);


    if(query.exec())
    {
    QByteArray outByteArray;
        while(query.next())
        {
            outByteArray = query.value(0).toByteArray();
        }
        outPixmap.loadFromData(outByteArray);
    }
    return outPixmap;
}

//Returns a list of Persona's stat info
QStringList DbAccess::GetInfoStat(QString name)
{
    QStringList statInfo;
    QSqlQuery query("SELECT Stats_Final.Strength, "
                    "Stats_Final.Magic, "
                    "Stats_Final.Endureance, "
                    "Stats_Final.Agility, "
                    "Stats_Final.Luck "
                    "FROM Personas_Final "
                    "INNER JOIN Stats_Final "
                    "ON Stats_Final.Stat_ID = Personas_Final.Stat_ID "
                    "WHERE Name =?");
    query.addBindValue(name);

    if(query.exec())
    {
        while(query.next())
        {
               statInfo.append(query.value(0).toString()); //Stength
               statInfo.append(query.value(1).toString()); //Magic
               statInfo.append(query.value(2).toString()); //Endurance
               statInfo.append(query.value(3).toString()); //Agility
               statInfo.append(query.value(4).toString()); //Luck
        }
    }
    return statInfo;
}

//Returns a list of Persona's magic Strengths/Weaknesses
QStringList DbAccess::GetInfoMagic(QString name)
{
    QStringList magicInfo;
    QSqlQuery query("SELECT Str_Wea_Final.Phys, "
                    "Str_Wea_Final.Gun, "
                    "Str_Wea_Final.Fire, "
                    "Str_Wea_Final.Ice, "
                    "Str_Wea_Final.Electric, "
                    "Str_Wea_Final.Wind, "
                    "Str_Wea_Final.Psych, "
                    "Str_Wea_Final.Nuke, "
                    "Str_Wea_Final.Bless, "
                    "Str_Wea_Final.Curse "
                    "FROM Personas_Final "
                    "INNER JOIN Str_Wea_Final "
                    "ON Str_Wea_Final.Str_Wea_ID = Personas_Final.Str_Wea_ID "
                    "WHERE Name = ?");
    query.addBindValue(name);

    if(query.exec())
    {

        while(query.next())
        {
               magicInfo.append(query.value(0).toString()); //Physical
               magicInfo.append(query.value(1).toString()); //Gun
               magicInfo.append(query.value(2).toString()); //Fire
               magicInfo.append(query.value(3).toString()); //Ice
               magicInfo.append(query.value(4).toString()); //Electric
               magicInfo.append(query.value(5).toString()); //Wind
               magicInfo.append(query.value(6).toString()); //Psych
               magicInfo.append(query.value(7).toString()); //Nuke
               magicInfo.append(query.value(8).toString()); //Bless
               magicInfo.append(query.value(9).toString()); //Curse
        }
    }
    return magicInfo;
}
//////////////////////////////////////////////////////////////////////////


////////////////////////Fusion Queries///////////////////////////////////


///////////////////////Normal Fusion Queries////////////////////////////

        ////////////All DLC Normal Fusion Queries///////////

//Returns a mutlimap of all possible Arcana Pairs for
//the Arcana given
QMultiMap<QString,QString> DbAccess::GetPairs(QString arcana)
{
    //Had to use multimap because the Arcana Matches
    //don't have unique keys
    QMultiMap<QString,QString> pairs;

    //Had to create a Many to many realtionship
    //because each arcana has many matches
    QSqlQuery query("SELECT Arcana.Name, "
                    "Pairs.First_Arcana, "
                    "Pairs.Second_Arcana "
                    "FROM PairConnection  "
                    "INNER JOIN Pairs "
                    "ON Pairs.Pair_ID = PairConnection.Pair_ID "
                    "INNER JOIN Arcana ON Arcana.Arcana_ID = PairConnection.Arcana_ID "
                    "WHERE Arcana.Name = ?");
    query.addBindValue(arcana);

    if(query.exec())
    {
        while(query.next())
        {
            //            First Arcana Name         Second Arcana Name
            pairs.insert(query.value(1).toString(),query.value(2).toString());
        }
    }
    return pairs;
}

//Returns a list of Personas from one of the pair Arcanas
//But doesn't include the Result Persona itself if they share the same Arcana
QList<Persona> DbAccess::GetPersonas(QString arcana, QString resName)
{
    QList<Persona> list;
    //Have to exculde the Result Persona from the list for accuracy
    QSqlQuery query("SELECT Personas_Final.name, "
                         "Arcana.name, Personas_Final.Level, "
                         "Personas_Final.Fuseable, "
                         "Personas_Final.SpecialFusion, "
                         "Personas_Final.Max_SL "
                         "FROM Personas_Final "
                         "INNER JOIN Arcana "
                         "ON Arcana.Arcana_ID = Personas_Final.Arcana "
                         "WHERE Personas_Final.Fuseable = 'TRUE' "
                         "AND Arcana.Name = ? "
                         "AND Personas_Final.Name != ?");
    query.addBindValue(arcana);
    query.addBindValue(resName);

    if(query.exec())
    {
        Persona tempPers;
        QString temp;

        while(query.next())
        {
            tempPers.m_name = query.value(0).toString();
            tempPers.m_arcana = query.value(1).toString();
            temp = query.value(2).toString();
            tempPers.m_level = temp.toUInt();

            list.append(tempPers);
        }
    }
    return list;
}

//Returns a list of all of the levels of all Fuseable Personas in an Arcana
//This is mega important for the rounding portion
QList<int> DbAccess::GetArcanaLevels(QString arcana)
{
    QList<int> levels;

    QSqlQuery query("SELECT Personas_Final.Level "
                    "FROM Personas_Final "
                    "INNER JOIN Arcana "
                    "ON Arcana.Arcana_ID = Personas_Final.Arcana "
                    "WHERE Personas_Final.Spoiler = 'FALSE' "
                    "AND Personas_Final.Fuseable = 'TRUE' "
                    "AND Arcana.Name = ?");
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


        ////////No DLC Normal Fusion Queries//////////////

//Does the exact same thing as GetPersonas but without DLC Characters
QList<Persona> DbAccess::GetPersonasNoDLC(QString first, QString resName)
{
    QList<Persona> list;

    QSqlQuery queryFirst("SELECT Personas_Final.name, "
                         "Arcana.name, Personas_Final.Level, "
                         "Personas_Final.Fuseable, "
                         "Personas_Final.SpecialFusion, "
                         "Personas_Final.Max_SL "
                         "FROM Personas_Final "
                         "INNER JOIN Arcana "
                         "ON Arcana.Arcana_ID = Personas_Final.Arcana "
                         "WHERE Personas_Final.Fuseable = 'TRUE' "
                         "AND Personas_Final.DLC = 'FALSE' "
                         "AND Arcana.Name = ? "
                         "AND Personas_Final.Name != ?");
    queryFirst.addBindValue(first);
    queryFirst.addBindValue(resName);

    if(queryFirst.exec())
    {
        Persona tempPers;
        QString temp;

        while(queryFirst.next())
        {
            tempPers.m_name = queryFirst.value(0).toString();
            tempPers.m_arcana = queryFirst.value(1).toString();
            temp = queryFirst.value(2).toString();
            tempPers.m_level = temp.toUInt();

            list.append(tempPers);
        }
    }
    return list;
}


//Does the exact same thing as GetArcanaLevels but without DLC Characters
//This has a major impact on rounding so is hella needed
QList<int> DbAccess::GetArcanaLevelsNoDLC(QString arcana)
{
    QList<int> levels;

    QSqlQuery query("SELECT Personas_Final.Level "
                    "FROM Personas_Final "
                    "INNER JOIN Arcana "
                    "ON Arcana.Arcana_ID = Personas_Final.Arcana "
                    "WHERE Personas_Final.Spoiler = 'FALSE' "
                    "AND Personas_Final.Fuseable = 'TRUE' "
                    "AND Personas_Final.DLC = 'FALSE' "
                    "AND Arcana.Name = ?");
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

////////////////////////////////////////////////////////////////////////

/////////////////Special Fusion Queries//////////////////////////////

//Gets the Primary Key for a specific Persona
int DbAccess::GetPK(QString name)
{
    int pK = 0;

    QSqlQuery query("SELECT Personas_Final.Main_ID "
                    "FROM Personas_Final "
                    "WHERE Name = ?");
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

//Returns a list of all the Personas needed
//For a Special Fusion
QStringList DbAccess::GetSpecialResults(int ID)
{
    QStringList results;

    //Special Fusions have multiple results
    //So a Many-to-Many is needed
    QSqlQuery query("SELECT SpeFusion.Name "
                    "FROM SpecConnection "
                    "INNER JOIN SpeFusion "
                    "ON SpeFusion.SF_ID = SpecConnection.SF_ID "
                    "INNER JOIN Personas_Final "
                    "ON Personas_Final.Main_ID = SpecConnection.Main_ID "
                    "WHERE Personas_Final.Main_ID = ?");
    query.addBindValue(ID);

    if(query.exec())
    {
        while(query.next())
        {
            results.append(query.value(0).toString());
        }
    }
    return results;
}




//////////////////////////Forward Fusion Queries//////////////////////////////

            ////////////All DLC Forward Fusion Queries/////////////

//Returns a list of every Persona execept p1
//if p1 is returned, the results may be wrong
QList<Persona> DbAccess::FFGetPersonas(Persona p1)
{
    QList<Persona> list;

    QSqlQuery query("SELECT Personas_Final.name, "
                    "Arcana.name, "
                    "Personas_Final.Level "
                    "FROM Personas_Final "
                    "INNER JOIN Arcana "
                    "ON Arcana.Arcana_ID = Personas_Final.Arcana "
                    "WHERE Personas_Final.Fuseable = 'TRUE' "
                    "AND Personas_Final.Name != ?");
    query.addBindValue(p1.m_name);


    if(query.exec())
    {
        Persona tempPers;
        QString temp;

        while(query.next())
        {
            //Dont need all the bool information in this object

            tempPers.m_name = query.value(0).toString();
            tempPers.m_arcana = query.value(1).toString();
            temp = query.value(2).toString();
            tempPers.m_level = temp.toUInt();

            list.append(tempPers);
        }
    }
    return list;
}

//Returns the Targect Arcana based on the two provided
QString DbAccess::GetTarget(QString firstArc, QString secondArc)
{
    QString results;
    QSqlQuery query("SELECT Arcana.Name "
                    "FROM PairConnection "
                    "INNER JOIN Pairs "
                    "ON Pairs.Pair_ID = PairConnection.Pair_ID "
                    "INNER JOIN Arcana "
                    "ON Arcana.Arcana_ID = PairConnection.Arcana_ID "
                    "WHERE Pairs.First_Arcana = ? "
                    "AND Pairs.Second_Arcana = ?");
    query.addBindValue(firstArc);
    query.addBindValue(secondArc);

    if(query.exec())
    {
        while(query.next())
        {
            results = query.value(0).toString();
        }
    }
    return results;
}

//Returns Persona by using the arcana and level to query
//An Arcna won't have multiple personas at the same base level
Persona DbAccess::GetResultPersona(QString arcana, int level)
{
    Persona result;
    QSqlQuery query("SELECT Personas_Final.name, "
                    "Arcana.name, "
                    "Personas_Final.Level, "
                    "Personas_Final.Fuseable, "
                    "Personas_Final.SpecialFusion, "
                    "Personas_Final.Max_SL "
                    "FROM Personas_Final "
                    "INNER JOIN Arcana "
                    "ON Arcana.Arcana_ID = Personas_Final.Arcana "
                    "WHERE Arcana.Name = ? "
                    "AND Personas_Final.Fuseable = 'TRUE' "
                    "AND Personas_Final.Spoiler = 'FALSE'"
                    "AND Personas_Final.SpecialFusion = 'FALSE' "
                    "AND Personas_Final.Level = ?");
    query.addBindValue(arcana);
    query.addBindValue(level);


    if(query.exec())
    {
        while(query.next())
        {

            QString temp;
            result.m_name = query.value(0).toString();    //Name
            result.m_arcana = query.value(1).toString();  //Arcana
            result.m_level = temp.toUInt();              //Level
            result.m_fuseable = query.value(3).toBool(); //Fuseable
            result.m_sFusion = query.value(4).toBool(); //Special Fusion
            result.m_maxSL = query.value(5).toBool(); //Max Social Link

        }
    }
    return result;
}



           /////////////No DLC Forward Fusion Queries/////////////

//Does the same thing as FFGetPersonas, just without the DLC Personas
QList<Persona> DbAccess::FFGetPersonasNoDLC(Persona p1)
{
    QList<Persona> list;

    QSqlQuery query("SELECT Personas_Final.name, "
                    "Arcana.name, "
                    "Personas_Final.Level "
                    "FROM Personas_Final "
                    "INNER JOIN Arcana "
                    "ON Arcana.Arcana_ID = Personas_Final.Arcana "
                    "WHERE Personas_Final.Fuseable = 'TRUE' "
                    "AND Personas_Final.DLC = 'FALSE' "
                    "AND Personas_Final.Name != 'Satanael'"
                    "AND Personas_Final.Name != ?");
    query.addBindValue(p1.m_name);


    if(query.exec())
    {
        Persona tempPers;
        QString temp;

        while(query.next())
        {
            tempPers.m_name = query.value(0).toString();
            tempPers.m_arcana = query.value(1).toString();
            temp = query.value(2).toString();
            tempPers.m_level = temp.toUInt();

            list.append(tempPers);
        }
    }

    return list;
}

//Does the same thing as GetResultPersona, just without the DLC Personas
Persona DbAccess::GetResultPersonaNoDLC(QString arcana, int level)
{
    Persona result;
    QSqlQuery query("SELECT Personas_Final.name, "
                    "Arcana.name, "
                    "Personas_Final.Level, "
                    "Personas_Final.Fuseable, "
                    "Personas_Final.SpecialFusion, "
                    "Personas_Final.Max_SL "
                    "FROM Personas_Final "
                    "INNER JOIN Arcana "
                    "ON Arcana.Arcana_ID = Personas_Final.Arcana "
                    "WHERE Arcana.Name = ? "
                    "AND Personas_Final.Fuseable = 'TRUE' "
                    "AND Personas_Final.Spoiler = 'FALSE'"
                    "AND Personas_Final.SpecialFusion = 'FALSE' "
                    "AND Personas_Final.DLC = 'FALSE' "
                    "AND Personas_Final.Level = ?");
    query.addBindValue(arcana);
    query.addBindValue(level);


    if(query.exec())
    {
        while(query.next())
        {
            QString temp;
            result.m_name = query.value(0).toString();    //Name
            result.m_arcana = query.value(1).toString();  //Arcana
            result.m_level = temp.toUInt();              //Level
            result.m_fuseable = query.value(3).toBool(); //Fuseable
            result.m_sFusion = query.value(4).toBool(); //Special Fusion
            result.m_maxSL = query.value(5).toBool(); //Max Social Link
        }

    }
    return result;
}



/////////////////////////////////////////////////////////////////////////////
