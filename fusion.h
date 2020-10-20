#ifndef FUSION_H
#define FUSION_H
#include "persona.h"
#include "dbaccess.h"
#include <QMultiMap>

class Fusion
{
public:
    Fusion(Persona final);
    void StartFusion(Persona final);
    QMultiMap<QString,QString> fusionCheck(QList<Persona> first,
                                           QList<Persona> second,
                                           Persona target);

    bool SamePerArcana(Persona firstPer, Persona secondPer);
    int CalculateLevel(int first, int second);
    bool FinalCheck(int calcLevel, int realLevel);

    //Rounding Methods
    int RoundType(int CalcLevel, bool sameArcana, Persona first, Persona second, QString targetArcana);
    //Needs a target Arcana List
    int RoundUp(int calcLevel,QVector<int> arcanaLvls);
    //Just needs the two other levels
    int RoundDown(int calcLevel,
                  int lvlFirst,
                  int lvlSecond,
                  QVector<int> arcanaLvls);



private:
    Persona m_final;
    Persona m_first;
    Persona m_second;

};

#endif // FUSION_H
