#include "fusion.h"
#include <QtDebug>
//Clean this up later

QString path = "C:\\Users\\Treffy\\Desktop\\Pesonal_Project\\Pesonal_Project\\SQL_FILES\\finaL_Database_2.db";

DbAccess fusionAccess(path);

Fusion::Fusion(Persona final)
{
    m_final = final;



}

void Fusion::StartFusion(Persona final)
{


    QMultiMap<QString,QString> arcanaMatches;
    QMultiMap<QString,QString> finalMatches;
    QList<Persona> firstFullArcana;
    QList<Persona> secondFullArcana;
    //Run Database to get matching arcanas
    //Store in multimap
    //Output into multimap properly
    arcanaMatches = fusionAccess.GetPairs(final.m_arcana);

    QMap<QString, QString>::iterator it;
    QString output;
    //Test Output
    for(it = arcanaMatches.begin(); it != arcanaMatches.end(); ++it)
    {
        firstFullArcana = fusionAccess.GetPersonas(it.key(),final.m_name);
        secondFullArcana = fusionAccess.GetPersonas(it.value(), final.m_name);

        output = QString("%1 : %2").arg(it.key()).arg(it.value());
        qDebug() << output;
        //needs all of final for the round up
       finalMatches += fusionCheck(firstFullArcana,secondFullArcana, final);


    }

    QMap<QString,QString>::iterator per;
    qDebug() << "All the hard work is done";
    for(per = finalMatches.begin(); per != finalMatches.end(); ++per)
    {
        QString output;
        output = QString("%1 : %2").arg(per.key()).arg(per.value());
        qDebug() << output;
    }
}


QMultiMap<QString,QString> Fusion::fusionCheck(QList<Persona> first,
                                                 QList<Persona> second,
                                                 Persona target)
{
    QMultiMap<QString,QString> results;
    bool sameArcana;
    bool correctLevel;
    int calcLevel;
    int roundedLevel;

    for(int i = 0; i < first.size(); i++)
    {
        for(int k = 0; k < second.size(); k++)
        {
                if(!first.at(i).m_name.contains(second.at(k).m_name))
                {

                    sameArcana = SamePerArcana(first.at(i), second.at(k));
                    calcLevel = CalculateLevel(first.at(i).m_level, second.at(k).m_level);
                    roundedLevel = RoundType(calcLevel,sameArcana,first.at(i), second.at(k), target.m_arcana);
                    correctLevel = FinalCheck(roundedLevel, target.m_level);

                    if(correctLevel)
                    {
                      results.insert(first.at(i).m_name, second.at(k).m_name);
                    }
                }



        }
    }




    return results;
}



bool Fusion::SamePerArcana(Persona firstPer, Persona secondPer)
{
    bool same;

    if(firstPer.m_arcana == secondPer.m_arcana)
    {
        same = true;
    }
    else
    {
        same = false;
    }

    return same;
}

bool Fusion::FinalCheck(int calcLevel, int realLevel)
{
    bool correct;

    if(calcLevel == realLevel)
        correct = true;
    else
    {
        correct = false;
    }

    return correct;
}

///Level Calculation and Rounding Methods///

int Fusion::CalculateLevel(int first, int second)
{
    int calc;
    double templevel;

    //Works but looks ugly fix it up
    templevel = (first + second);
    templevel = templevel/2;

    //Checking to see if the number needs to have the 0.5 or 1 added
    if(floor(templevel) == templevel)
    {
        templevel = templevel + 1;
    }
    else
    {
        templevel = templevel + 0.5;
    }

    //cast it back an int
    calc = (int)templevel;
    return calc;

}




int Fusion::RoundType(int calcLevel,
                      bool sameArcana,
                      Persona first,
                      Persona second,
                      QString tagetArcana)
{
    QVector<int> arcanaLvls = fusionAccess.GetArcanaLevels(tagetArcana);
    int roundedLevel;
    if(sameArcana)
    {
       roundedLevel = RoundDown(calcLevel, first.m_level, second.m_level, arcanaLvls);
    }
    else
    {

     roundedLevel = RoundUp(calcLevel, arcanaLvls);
    }


    return roundedLevel;
}

//Isnt rounding properly for some reason
int Fusion::RoundDown(int calcLevel, int lvlFirst, int lvlSecond, QVector<int> arcanaLvls)
{
    int roundedLevel = 0;
    for(int i = 1; i< arcanaLvls.count(); i++)
    {
        if(arcanaLvls.at(i) < calcLevel &&
           arcanaLvls.at(i + 1) >= calcLevel)
        {
            if(arcanaLvls.at(i) == lvlFirst || arcanaLvls.at(i) == lvlSecond)
            {
                roundedLevel = arcanaLvls.at(i - 1);
                break;
            }
            else
            {
                roundedLevel = arcanaLvls.at(i);
                break;
            }
        }
    }







    return roundedLevel;
}


int Fusion::RoundUp(int calcLevel,QVector<int> arcanaLvls)
{
    int roundedLevel = 0;

    for(int i = 1; i< arcanaLvls.count(); i++)
    {
        if(arcanaLvls.at(i) > calcLevel)
        {
            roundedLevel = arcanaLvls.at(i);
            break;
        }
    }

    if(calcLevel >= arcanaLvls.at(arcanaLvls.count() - 1))
    {
        roundedLevel = arcanaLvls.at(arcanaLvls.count() - 1);
    }






    return roundedLevel;
}
