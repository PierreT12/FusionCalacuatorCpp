#ifndef MULTITHREADFUSION_H
#define MULTITHREADFUSION_H

#include <QObject>
#include <QThread>
#include <QMultiMap>
#include <QDebug>
#include <QString>
#include <QMetaType>


#include "persona.h"
#include "fusion.h"

class MultiThreadFusion : public QObject
{
    Q_OBJECT

public:

    MultiThreadFusion();
    ~MultiThreadFusion();
    void RequestPersonas(Persona final);



public slots:
    //Takes in the matches from
   void MatchThem(Persona final);



signals:
   void ListReady(QMultiMap<QString,QString> matches);


private:
   QThread t;



};

#endif // MULTITHREADFUSION_H
