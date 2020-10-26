#ifndef FUSION_H
#define FUSION_H
#include <QMultiMap>
#include <QDir>

#include "persona.h"
#include "dbaccess.h"


class Fusion
{
public:
    Fusion(Persona final);


   //Acutal Main Fusion Methods
   QStringList StartFusion(Persona target, bool filter);

   QStringList StartForwardFusion(Persona p1, bool filter);

   QStringList FusionCheck(QList<Persona> first,
                                           QList<Persona> second,
                                           QList<int> arcanaLvls,
                                           Persona target);

   QStringList SpecialFusion(Persona target);

   QString FFCheck(Persona p1, Persona p2, bool filter);

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

    bool MathCheck(int calcLevel,int realLevel);


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
    QString m_path;
    DbAccess fusionAccess;


};

#endif // FUSION_H
