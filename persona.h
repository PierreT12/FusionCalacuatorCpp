#ifndef PERSONA_H
#define PERSONA_H
#include <QString>


class Persona
{

public:
    QString m_name;
    QString m_arcana;
    int m_level;
    bool m_spoiler = false;
    bool m_fuseable;
    bool m_sFusion;
    bool m_maxSL;
    bool m_treasure = false;




Persona();


private:

};

#endif // PERSONA_H
