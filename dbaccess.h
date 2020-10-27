#ifndef DBACCESS_H
#define DBACCESS_H
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlIndex>
#include <QMultiMap>
#include <QPixmap>
#include <QLinkedList>
#include "persona.h"
class DbAccess
{

public:
    DbAccess();

    void SetDatabaseName(const QString& path);

//////////////////Settings for Listview Queries///////////////////
    QStringList GetAllNoSpoilsDLC();
    QStringList GetAllDLC();
    QStringList GetAllSpoils();
////////////////////////////////////////////////////////////////

///////////////////Search Queries/////////////////////////////
    QStringList NoSpoilDLCSearch(QString searchTerm);
    QStringList NoSpoilSearch(QString searchTerm);
    QStringList NoDLCSearch(QString searchTerm);
    QStringList AllSearch(QString searchTerm);
////////////////////////////////////////////////////////////


////////////////Displaying to Listview Queiers///////////////
    QStringList GetAll();
    Persona GetSinglePersona(QString indexName);
    QPixmap GetSinglePersonaImage(QString indexName);
    QStringList GetInfoStat(QString name);
    QStringList GetInfoMagic(QString name);
/////////////////////////////////////////////////////////////


////////////////////Fusion Queries//////////////////////////



/////////////////Normal Fusion Queries///////////////////

///////All DLC Normal Fusion Queries
    QMultiMap<QString,QString> GetPairs(QString arcana);
    QList<Persona> GetPersonas(QString first, QString resName);
    QList<int> GetArcanaLevels(QString arcana);

///////////////No DLC Normal Fusion Queries
    QList<Persona> GetPersonasNoDLC(QString first, QString resName);
    QList<int> GetArcanaLevelsNoDLC(QString arcana);

/////////////////////////////////////////////////////////


/////////////////Forward Fusion Queries///////////////////

///////All DLC Forward Fusion Queries
    QList<Persona> FFGetPersonas(Persona p1);
    Persona GetResultPersona(QString arcana, int level);
    QString GetTarget(QString firstArc, QString secondArc);

///////////////No DLC Forward Fusion Queries
    QList<Persona> FFGetPersonasNoDLC(Persona p1);
    Persona GetResultPersonaNoDLC(QString arcana, int level);

/////////////////////////////////////////////////////////

/////////////////Special Fusion Queries///////////////////
    int GetPK(QString name);
    QStringList GetSpecialResults(int ID);
/////////////////////////////////////////////////////////

private:
    QSqlDatabase m_db;

};

#endif // DBACCESS_H
