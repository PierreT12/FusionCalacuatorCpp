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
    DbAccess(const QString& path);
    void SetDatabaseName(const QString& path);

    QStringList GetAllNoSpoilsDLC();
    QStringList GetAllDLC();
    QStringList GetAllSpoils();

    QStringList NoSpoilDLCSearch(QString searchTerm);
    QStringList NoSpoilSearch(QString searchTerm);
    QStringList NoDLCSearch(QString searchTerm);
    QStringList AllSearch(QString searchTerm);



    QStringList GetAll();
    Persona GetSinglePersona(QString indexName);
    QPixmap GetSinglePersonaImage(QString indexName);
    QStringList GetInfoStat(QString name);
    QStringList GetInfoMagic(QString name);

    QMultiMap<QString,QString> GetPairs(QString arcana);
    QList<Persona> GetPersonas(QString first, QString resName);
    QList<int> GetArcanaLevels(QString arcana);

    QList<Persona> GetPersonasNoDLC(QString first, QString resName);
    QList<int> GetArcanaLevelsNoDLC(QString arcana);

    QList<Persona> FFGetPersonas(Persona p1);
    Persona GetResultPersona(QString arcana, int level);
    QString GetTarget(QString firstArc, QString secondArc);

    QList<Persona> FFGetPersonasNoDLC(Persona p1);
    Persona GetResultPersonaNoDLC(QString arcana, int level);




    int GetPK(QString name);
    QStringList GetSpecialResults(int ID);

private:
    QSqlDatabase m_db;

};

#endif // DBACCESS_H
