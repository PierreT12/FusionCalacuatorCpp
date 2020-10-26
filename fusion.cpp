#include "fusion.h"
#include <QtDebug>
//Clean this up later



Fusion::Fusion(Persona target)
{
    //Sets up the database
    m_path = QDir::currentPath() + "/Resources/final_Database_2.db";
    fusionAccess.SetDatabaseName(m_path);
}

//Reverse Fusion
QStringList Fusion::StartFusion(Persona target, bool filter)
{
    QStringList finalMatch;

    QList<Persona> firstFullArcana;
    QList<Persona> secondFullArcana;
    QList<int> arcanaLvls;

    QMultiMap<QString,QString> matches;



    //Gets a list of levels for the Target Aracana from the Database
    if(filter)
        arcanaLvls = fusionAccess.GetArcanaLevels(target.m_arcana);
    else
        arcanaLvls = fusionAccess.GetArcanaLevelsNoDLC(target.m_arcana);

    //Gets the Pair Aracna Matches from the Database
    matches = fusionAccess.GetPairs(target.m_arcana);



    QMap<QString, QString>::iterator it;

    for(it = matches.begin(); it != matches.end(); ++it)
    {
        if(filter)
        {
            firstFullArcana = fusionAccess.GetPersonas(it.key(),target.m_name);
            secondFullArcana = fusionAccess.GetPersonas(it.value(), target.m_name);
        }
        else
        {
            firstFullArcana = fusionAccess.GetPersonasNoDLC(it.key(),target.m_name);
            secondFullArcana = fusionAccess.GetPersonasNoDLC(it.value(), target.m_name);
        }


        finalMatch+= FusionCheck(firstFullArcana,secondFullArcana, arcanaLvls, target);
    }
    return finalMatch;
}

//Forward Fusion

QStringList Fusion::StartForwardFusion(Persona p1, bool filter)
{

    QList<Persona> allPersonas;
    QStringList finalMatch;
    QString output;

    if(filter)
        allPersonas = fusionAccess.FFGetPersonas(p1);
    else
        allPersonas = fusionAccess.FFGetPersonasNoDLC(p1);


    for(int i = 0; i < allPersonas.size(); i++)
    {
        output = FFCheck(p1, allPersonas.at(i), filter);
        if(!output.isEmpty())
        {
            finalMatch.append(output);
            qDebug() << output;
        }
    }


    return finalMatch;
}

///Regular Fusions

QStringList Fusion::FusionCheck(QList<Persona> first,
                                QList<Persona> second,
                                QList<int> arcanaLvls,
                                Persona target)
{
    QString results;
    QStringList finalListMatch;
    bool sameArcana;
    bool correctLevel;
    int calcLevel;
    int roundedLevel;

    //Nested for loops so that every possible combo is checked
    for(int i = 0; i < first.size(); i++)
    {
        for(int k = 0; k < second.size(); k++)
        {
            if(!first.at(i).m_name.contains(second.at(k).m_name))
            {
                sameArcana = SamePerArcana(first.at(i), second.at(k));


                calcLevel = CalculateLevel(first.at(i).m_level, second.at(k).m_level);


                roundedLevel = RoundType(calcLevel,
                                         sameArcana,
                                         first.at(i),
                                         second.at(k),
                                         arcanaLvls);


                correctLevel = FinalCheck(roundedLevel,
                                          target.m_level,
                                          first.at(i).m_name,
                                          second.at(k).m_name,
                                          finalListMatch);

                if(correctLevel)
                {
                    results = first.at(i).m_name + " : " + second.at(k).m_name;
                    finalListMatch.append(results);
                }
            }
        }
    }

    qDebug() << finalListMatch;

    return finalListMatch;
}

///Forward Fusions
QString Fusion::FFCheck(Persona p1, Persona p2, bool filter)
{
    QString results;
    QString targetArcana;
    QList<int> arcanaLvls;
    Persona outputPersona;
    bool sameArcana;
    int calcLevel;
    int roundedLevel;

    targetArcana = fusionAccess.GetTarget(p1.m_arcana, p2.m_arcana);

    //Do a null and empty check
    //Then do all the fun fusey stuff
    if(!targetArcana.isEmpty())
    {
        arcanaLvls = fusionAccess.GetArcanaLevels(targetArcana);
        sameArcana = SamePerArcana(p1, p2);
        calcLevel = CalculateLevel(p1.m_level, p2.m_level);
        roundedLevel = RoundType(calcLevel,
                                 sameArcana,
                                 p1,
                                 p2,
                                 arcanaLvls);
        if(filter)
            outputPersona = fusionAccess.GetResultPersona(targetArcana,roundedLevel);
        else
            outputPersona = fusionAccess.GetResultPersonaNoDLC(targetArcana,roundedLevel);

        if(!outputPersona.m_name.isEmpty())
            results = p2.m_name + " : " + outputPersona.m_name;

    }






    return results;
}


///Special Fusions

QStringList Fusion::SpecialFusion(Persona target)
{
    QStringList specialResults;


    int personaSF = fusionAccess.GetPK(target.m_name);

    specialResults = fusionAccess.GetSpecialResults(personaSF);


    return specialResults;

}

///Treasure Demon Fusions
/// There is a set range of levels that could result,
/// but all in all wont be very accurate
/// Not planning on implementing it right now


bool Fusion::SamePerArcana(Persona first,
                           Persona second)
{
    bool sameArcana;

    if(first.m_arcana == second.m_arcana)
        sameArcana = true;
    else
        sameArcana = false;

    return sameArcana;
}

bool Fusion::FinalCheck(int calcLevel,
                        int realLevel,
                        QString first,
                        QString second,
                        QStringList finalListMatch)
{
    finalListMatch.sort(Qt::CaseInsensitive);
    bool correct;
    QString testStr = second + " : " + first;

    correct = MathCheck(calcLevel, realLevel);

    if(correct)
    {
        for(int i = 0; i < finalListMatch.size(); ++i)
        {
            if(finalListMatch.at(i).contains(testStr, Qt::CaseInsensitive))
            {
                correct = false;
                break;
            }
            else
                correct = MathCheck(calcLevel, realLevel);
        }
    }
    return correct;
}


bool Fusion::MathCheck(int calcLevel,
                       int realLevel)
{
    bool correct;

    if(calcLevel == realLevel)
        correct = true;
    else
        correct = false;


    return correct;
}



///Level Calculation and Rounding Methods///

int Fusion::CalculateLevel(int first,
                           int second)
{
    int calcLevel;
    double templevel;

    //Works but looks ugly fix it up
    templevel = (first + second);
    templevel = templevel/2;

    //Checking to see if the number is whole or not
    if(floor(templevel) == templevel)
        templevel = templevel + 1;

    else
        templevel = templevel + 0.5;


    //cast it back an int
    calcLevel = (int)templevel;
    return calcLevel;

}




int Fusion::RoundType(int calcLevel,
                      bool sameArcana,
                      Persona first,
                      Persona second,
                      QList<int> arcanaLvls)
{

    int roundedLevel;

    if(sameArcana)
        roundedLevel = RoundDown(calcLevel,
                                 first.m_level,
                                 second.m_level,
                                 arcanaLvls);

    else
     roundedLevel = RoundUp(calcLevel, arcanaLvls);


    return roundedLevel;
}


int Fusion::RoundDown(int calcLevel,
                      int lvlFirst,
                      int lvlSecond,
                      QList<int> arcanaLvls)
{
    int roundedLevel = 0;


    for(int i = 0; i< arcanaLvls.count(); i++)
    {
        if(arcanaLvls.at(i) < calcLevel &&
           (arcanaLvls.at(i + 1) >= calcLevel))
        {
            if(arcanaLvls.at(i) == lvlFirst || arcanaLvls.at(i) == lvlSecond)
            {
                if((i - 1) < 0)
                    break;
                else
                {
                    roundedLevel = arcanaLvls.at(i - 1);
                    break;
                }
            }
            else
            {
                roundedLevel = arcanaLvls.at(i);
                break;
            }
        }
        else
        {
            if(arcanaLvls.at(i) == calcLevel ||
               arcanaLvls.at(arcanaLvls.count() - 1) < calcLevel)
                    break;

        }
    }


    return roundedLevel;
}


int Fusion::RoundUp(int calcLevel,
                    QList<int> arcanaLvls)
{
    int roundedLevel = 0;

    for(int i = 0; i< arcanaLvls.count(); i++)
    {
        if(arcanaLvls.at(i) >= calcLevel)
        {
            roundedLevel = arcanaLvls.at(i);
            break;
        }
    }

    if(calcLevel >= arcanaLvls.at(arcanaLvls.count() - 1))
        roundedLevel = arcanaLvls.at(arcanaLvls.count() - 1);

    return roundedLevel;
}
