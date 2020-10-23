#include "fusion.h"
#include <QtDebug>
//Clean this up later

QString path = "C:\\Users\\Treffy\\Desktop\\Pesonal_Project\\Pesonal_Project\\SQL_FILES\\finaL_Database_2.db";

DbAccess fusionAccess(path);


Fusion::Fusion(Persona final)
{
    m_final = final;



}

QMultiMap<QString, QString> Fusion::StartFusion(Persona final)
{


    QStringList strList;
    QList<Persona> firstFullArcana;
    QList<Persona> secondFullArcana;

    QList<int> arcanaLvls;
    QMultiMap<QString,QString> matches;
    QMultiMap<QString,QString> finalfinal;



    //Gets a list of levels for the Target Aracana from the Database
    arcanaLvls = fusionAccess.GetArcanaLevels(final.m_arcana);
    //Gets the Pair Aracna Matches from the Database
    matches = fusionAccess.GetPairs(final.m_arcana);

    QMap<QString, QString>::iterator it;

    //Test Output
    for(it = matches.begin(); it != matches.end(); ++it)
    {

        firstFullArcana = fusionAccess.GetPersonas(it.key(),final.m_name);
        secondFullArcana = fusionAccess.GetPersonas(it.value(), final.m_name);

       strList += fusionCheck(firstFullArcana,secondFullArcana, arcanaLvls, final);


    }


    return finalfinal;
}


///Regular fusions

QStringList Fusion::fusionCheck(QList<Persona> first,
                                                 QList<Persona> second,
                                                QList<int> arcanaLvls,
                                                 Persona target)
{
    QString forResults;
    //Need a quick way to see if each is unique
    QStringList strList;
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
                    roundedLevel = RoundType(calcLevel,sameArcana,first.at(i), second.at(k), arcanaLvls);



                   correctLevel = FinalCheck(roundedLevel, target.m_level, first.at(i).m_name, second.at(k).m_name, strList);



                    if(correctLevel)
                    {


                           forResults = first.at(i).m_name + " : " + second.at(k).m_name;
                            strList.append(forResults);


                    }
                }



        }
    }

    qDebug() << strList;

    return strList;
}

///Special Fusions

QStringList Fusion::specialFusion(Persona target)
{
    QStringList specialResults;

    //Cant swithch with QStrings so I'm getting the
    //PK of the Special Fusion Persona selected
    //And using a switch case with that

    int personaSF = fusionAccess.GetPK(target.m_name);

    //None of these fusions involve math because they're
    //preset, so I just store all their fusion pairs
    //in the database
    switch (personaSF)
    {
        case 12:
        break;
        case 14:
        break;
        case 17:
        break;
        case 74:
        break;
        case 100:
        break;
        case 112:
        break;
        case 127:
        break;
        case 164:
        break;
        case 171:
        break;
        case 182:
        break;
        case 194:
        break;
        case 197:
        break;
        case 201:
        break;
        case 204:
        break;
        case 205:
        break;
        case 207:
        break;
        case 228:
        break;
        case 230:
        break;
        case 235:
        break;
        case 237:
        break;
        case 260:
        break;
        case 261:
        break;
    default:
        qDebug() << "Something has gone very wrong :{";
        break;
    };



    return specialResults;

}


///Treasure Demon Fusions
/// There is a set range of levels that could result,
/// but all in all wont be very accurate


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

bool Fusion::FinalCheck(int calcLevel, int realLevel, QString first, QString second, QStringList alreadyadded)
{
    bool correct;
    alreadyadded.sort(Qt::CaseInsensitive);
    QString testStr = second + " : " + first;

        if(calcLevel == realLevel)
            correct = true;
        else
        {
            correct = false;
        }

        if(correct)
        {

                for(int i = 0; i < alreadyadded.size(); ++i)
                {

                        if(alreadyadded.at(i).contains(testStr, Qt::CaseInsensitive))
                        {
                            correct = false;
                            break;
                        }
                        else
                        {
                            if(calcLevel == realLevel)
                                correct = true;
                            else
                            {
                                correct = false;
                            }
                        }
                    }
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



//You need to get all the Arcana Levels before this
///Only once, otherwise
//There will be a huge slowdown
int Fusion::RoundType(int calcLevel,
                      bool sameArcana,
                      Persona first,
                      Persona second,
                      QList<int> arcanaLvls)
{
    //For some reason this SQL Query is taking forever
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
int Fusion::RoundDown(int calcLevel, int lvlFirst, int lvlSecond, QList<int> arcanaLvls)
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


int Fusion::RoundUp(int calcLevel,QList<int> arcanaLvls)
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
