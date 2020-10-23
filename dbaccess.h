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
    DbAccess(const QString& path);
    QStringList GetAll(QSqlQuery query);
    Persona GetSinglePersona(QString indexName);
    QPixmap GetSinglePersonaImage(QString indexName);
    QStringList GetInfoStat(QString name);
    QStringList GetInfoMagic(QString name);
    QMultiMap<QString,QString> GetPairs(QString arcana);
    QList<Persona> GetPersonas(QString first, QString resName);
    QList<int> GetArcanaLevels(QString arcana);
    int GetPK(QString name);
    QStringList GetSpecialResults(int ID);
private:
    QSqlDatabase m_db;

};

#endif // DBACCESS_H
