#include "fusion.h"
#include <QtDebug>
//Clean this up later





Fusion::Fusion(Persona final)
{
    m_final = final;



}

void Fusion::StartFusion(Persona final)
{
    QString path = "C:\\Users\\Treffy\\Desktop\\Pesonal_Project\\Pesonal_Project\\SQL_FILES\\finaL_Database_2.db";
    DbAccess fusionAccess(path);
    QMultiMap<QString,QString> arcanaMatches;
    QMultiMap<Persona,Persona> draftMatches;
    //Run Database to get matching arcanas
    //Store in multimap
    //Output into multimap properly
    arcanaMatches = fusionAccess.GetPairs(final.m_arcana);

    QMap<QString, QString>::iterator it;
    QString output;
    //Test Output
    for(it = arcanaMatches.begin(); it != arcanaMatches.end(); ++it)
    {
        draftMatches = fusionAccess.GetPersonas(it.key(),it.value());
        output = QString("%1 : %2").arg(it.key()).arg(it.value());
        qDebug() << output;
    }
}
