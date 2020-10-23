#ifndef FUSION_H
#define FUSION_H
#include "persona.h"
#include "dbaccess.h"
#include <QMultiMap>

class Fusion
{
public:
    Fusion(Persona final);


   //Acutal Main Fusion Methods
   QMultiMap<QString, QString> StartFusion(Persona final);

   QStringList  fusionCheck(QList<Persona> first,
                                           QList<Persona> second,
                                           QList<int> arcanaLvls,
                                           Persona target);
   QStringList specialFusion(Persona target);

   //Doing Queries or Math Methods
    bool SamePerArcana(Persona firstPer,
                       Persona secondPer);
    int CalculateLevel(int first,
                       int second);
    bool FinalCheck(int calcLevel,
                    int realLevel,
                    QString first,
                    QString second,
                    QStringList alreadyadded);


    //Rounding Methods
    int RoundType(int CalcLevel,
                  bool sameArcana,
                  Persona first,
                  Persona second,
                  QList<int> arcanaLvls);

    int RoundUp(int calcLevel,QList<int> arcanaLvls);

    int RoundDown(int calcLevel,
                  int lvlFirst,
                  int lvlSecond,
                  QList<int> arcanaLvls);



private:
    Persona m_final;
    Persona m_first;
    Persona m_second;

};

#endif // FUSION_H
