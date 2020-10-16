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



private:
    Persona m_final;
    Persona m_first;
    Persona m_second;

};

#endif // FUSION_H
