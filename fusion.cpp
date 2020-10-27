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
//Returns a List of all the correct results from the Reverse Fusion
QStringList Fusion::StartFusion(Persona target, bool filter)
{
    QStringList finalMatch;

    QList<Persona> firstFullArcana;
    QList<Persona> secondFullArcana;
    QList<int> arcanaLvls;

    QMultiMap<QString,QString> matches;




    //Does a filter check to see if DLC Personas will also be pulled
    //And gets all the Persona's Levels for that specific Arcana
    if(filter)
        arcanaLvls = fusionAccess.GetArcanaLevels(target.m_arcana);
    else
        arcanaLvls = fusionAccess.GetArcanaLevelsNoDLC(target.m_arcana);

    //Gets the Pair Aracna Matches from the Database
    matches = fusionAccess.GetPairs(target.m_arcana);



    QMap<QString, QString>::iterator it;

    //This is run for every pair
    for(it = matches.begin(); it != matches.end(); ++it)
    {
        //Gets all Personas from that Arcana depending on what
        //Filter is being used
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

        //Returns the two Personas that will result in the Target Persona (The one the user clicked)
        finalMatch+= FusionCheck(firstFullArcana,secondFullArcana, arcanaLvls, target);
    }
    return finalMatch;
}

//Forward Fusion
//Returns a List of all the correct results from the Forward Fusion
QStringList Fusion::StartForwardFusion(Persona p1, bool filter)
{

    QList<Persona> allPersonas;
    QStringList finalMatch;
    QString output;

    //Gets ALL Personas depending on filter
    if(filter)
        allPersonas = fusionAccess.FFGetPersonas(p1);
    else
        allPersonas = fusionAccess.FFGetPersonasNoDLC(p1);


    //Every Persona in the list is checked so that's why
    //It takes a lot longer compared to reverse fusion
    for(int i = 0; i < allPersonas.size(); i++)
    {
        //Returns a string of the two correct Personas
        output = FFCheck(p1, allPersonas.at(i), filter);

        //Sometimes Nothing is returned so we don't wanna add
        //That to the list
        if(!output.isEmpty())
        {
            finalMatch.append(output);
        }
    }
    return finalMatch;
}

/////////////////////////////////The Checks/////////////////////////////////////

//Reverse Fusion Check
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
            //This is here as a failsafe incase the query messes up
            if(!first.at(i).m_name.contains(second.at(k).m_name))
            {

                sameArcana = SamePerArcana(first.at(i), second.at(k));


                calcLevel = CalculateLevel(first.at(i).m_level, second.at(k).m_level);

                //Finds out what rounding to do and performs it
                roundedLevel = RoundType(calcLevel,
                                         sameArcana,
                                         first.at(i),
                                         second.at(k),
                                         arcanaLvls);

                //Checks that the results match the Target Persona
                //And that the results aren't being double added
                correctLevel = FinalCheck(roundedLevel,
                                          target.m_level,
                                          first.at(i).m_name,
                                          second.at(k).m_name,
                                          finalListMatch);

                //If everything is all good, the result is added to a list
                if(correctLevel)
                {
                    results = first.at(i).m_name + " : " + second.at(k).m_name;
                    finalListMatch.append(results);

                }//Correct Check
            }//Name Check
        }//Second Arcana For Loop
    }//First Arcana For Loop


    return finalListMatch;
}



//Forward Fusion Check
QString Fusion::FFCheck(Persona p1, Persona p2, bool filter)
{
    QString results;
    QString targetArcana;
    QList<int> arcanaLvls;
    Persona outputPersona;
    bool sameArcana;
    int calcLevel;
    int roundedLevel;

    //Gets the Target Arcana if these two Arcanas are combinded
    targetArcana = fusionAccess.GetTarget(p1.m_arcana, p2.m_arcana);

    //Do a null and empty check because sometimes there is no result
    //From GetTarget
    if(!targetArcana.isEmpty())
    {
        //Does the same thing that it does in Reverse Fusion
        arcanaLvls = fusionAccess.GetArcanaLevels(targetArcana);

        sameArcana = SamePerArcana(p1, p2);

        calcLevel = CalculateLevel(p1.m_level, p2.m_level);

        //Finds out what rounding to do and performs it
        roundedLevel = RoundType(calcLevel,
                                 sameArcana,
                                 p1,
                                 p2,
                                 arcanaLvls);

        //The result can be different depending on DLC Characters
        //So this check is in place to make sure it stays in line
        //With the settings the user has set
        if(filter)
            outputPersona = fusionAccess.GetResultPersona(targetArcana,roundedLevel);
        else
            outputPersona = fusionAccess.GetResultPersonaNoDLC(targetArcana,roundedLevel);

        if(!outputPersona.m_name.isEmpty())
            results = p2.m_name + " : " + outputPersona.m_name;
    }
    return results;
}
//////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////Special Fusions////////////////////////////////////////////////

//Returns a list of special fusion Resutls
//This is quick to pull because there's no calculations required
QStringList Fusion::SpecialFusion(Persona target)
{
    QStringList specialResults;

    //Gets the Primary Key
    int personaSF = fusionAccess.GetPK(target.m_name);

    //Gets a list of the results
    specialResults = fusionAccess.GetSpecialResults(personaSF);


    return specialResults;

}

///Treasure Demon Fusions
/// There is a set range of levels that could result,
/// but all in all wont be very accurate
/// Not planning on implementing it right now

/////////////////Calcuations and Final Checks/////////////////////////

//Returns bool based on if the two Personas
//Have matching Arcanas
//This affects rounding so its pretty important to have
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

//This makes sure that repeats aren't included
//And that the results and Target Persona match
bool Fusion::FinalCheck(int calcLevel,
                        int realLevel,
                        QString first,
                        QString second,
                        QStringList finalListMatch)
{
    finalListMatch.sort(Qt::CaseInsensitive);

    bool correct;
    //Getting the string to make sure the reverse
    //Doesn't already exist in the correct result list
    QString testStr = second + " : " + first;

    //Making sure the levels match
    correct = MathCheck(calcLevel, realLevel);

    if(correct)
    {
        //Making sure that the same results, doesn't show up
        //ANYWHERE in the list
        for(int i = 0; i < finalListMatch.size(); ++i)
        {
            if(finalListMatch.at(i).contains(testStr, Qt::CaseInsensitive))
            {
                correct = false;
                break;
            }
            else
                //Checks the math again for piece of mind
                correct = MathCheck(calcLevel, realLevel);
        }
    }
    return correct;
}


//Checks to make sure the Result Level and Real Level Match
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

//Returns the unrounded Result Level
int Fusion::CalculateLevel(int first,
                           int second)
{
    //Formula
    // RL = (lvl1 + lvl2)/2 + 0.5 or 1

    int calcLevel;
    double templevel;

    //Gets the mean of the levels
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



//Passes the unrounded Result Level to the
//Right Rounding Type
int Fusion::RoundType(int calcLevel,
                      bool sameArcana,
                      Persona first,
                      Persona second,
                      QList<int> arcanaLvls)
{

    int roundedLevel;

    if(sameArcana) //They have the same arcana
        roundedLevel = RoundDown(calcLevel,
                                 first.m_level,
                                 second.m_level,
                                 arcanaLvls);

    else //Different arcanas
     roundedLevel = RoundUp(calcLevel, arcanaLvls);


    return roundedLevel;
}

//Round the unrounded level to the next lowest level
//On the Arcana Level List
int Fusion::RoundDown(int calcLevel,
                      int lvlFirst,
                      int lvlSecond,
                      QList<int> arcanaLvls)
{
    //Sets it to 0 for piece of mind
    int roundedLevel = 0;


    for(int i = 0; i< arcanaLvls.count(); i++)
    {
        //Needs to check the next level in arcanaLvls
        //So it does the correct round down
        if(arcanaLvls.at(i) < calcLevel &&
           (arcanaLvls.at(i + 1) >= calcLevel))
        {
            if(arcanaLvls.at(i) == lvlFirst || arcanaLvls.at(i) == lvlSecond)
                //A Target Persona's pairing result can't include itself
                //So it'll try to round down to the next lowest one if that's true
            {
                if((i - 1) < 0) //Makes sure its not roundind down to nothing
                    break;
                else
                {
                    roundedLevel = arcanaLvls.at(i - 1);
                    break;
                }
            }// Matches a pair if statment
            else
            {
                roundedLevel = arcanaLvls.at(i);
                break;
            }
        }// < and >= if statmennt
        else
        {
            //A check to see if its an exact match or at the Highest index
            if(arcanaLvls.at(i) == calcLevel ||
               arcanaLvls.at(arcanaLvls.count() - 1) < calcLevel)
                    break;

        }
    }//Arcana Level For Loop
    return roundedLevel;
}

//Round the unrounded level to the next highest level
//On the Arcana Level List
int Fusion::RoundUp(int calcLevel,
                    QList<int> arcanaLvls)
{
    int roundedLevel = 0;

    //Loops to tosee if the arcanaLvl is greater
    //And rounding to it if true
    for(int i = 0; i< arcanaLvls.count(); i++)
    {
        if(arcanaLvls.at(i) >= calcLevel)
        {
            roundedLevel = arcanaLvls.at(i);
            break;
        }
    }

    //Failsafe if there is no next highest level
    if(calcLevel >= arcanaLvls.at(arcanaLvls.count() - 1))
        roundedLevel = arcanaLvls.at(arcanaLvls.count() - 1);

    return roundedLevel;
}




