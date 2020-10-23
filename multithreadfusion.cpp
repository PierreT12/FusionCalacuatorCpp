#include "multithreadfusion.h"



MultiThreadFusion::MultiThreadFusion()
{
    moveToThread(&t);
    t.start();

}



MultiThreadFusion::~MultiThreadFusion()
{
    qDebug() << "Peace out Home Slice uwu!";
    t.quit();
    t.wait();
}

void MultiThreadFusion::RequestPersonas(Persona final)
{

    qRegisterMetaType<Persona>("Persona");

    QMetaObject::invokeMethod(this, "MatchThem", Q_ARG(Persona,final));
}


void MultiThreadFusion::MatchThem( Persona final)
{
    //Gotta manually put this in here because invoke methods gets
    //real pissy with QMultiMap
    QString path = "C:\\Users\\Treffy\\Desktop\\Pesonal_Project\\Pesonal_Project\\SQL_FILES\\finaL_Database_2.db";

    QMultiMap<QString, QString> output;
    QMultiMap<QString,QString> matches;
    DbAccess fusionAccess(path);
    matches = fusionAccess.GetPairs(final.m_arcana);




    //This will do all the fusion heavly lifting
    Fusion fusion(final);

   output = fusion.StartFusion(matches,final);
    _sleep(10);
    qDebug("Fuck yea!!! It hopefully worked!");
    emit ListReady(output);


}

