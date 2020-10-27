#ifndef PERSONA_H
#define PERSONA_H
#include <QString>


class Persona
{

public:
    //Testing out making all these public
    //need to be accessed in other classes uwu
    QString m_name;
    QString m_arcana;
    int m_level;
    bool m_spoiler = false;
    bool m_fuseable;
    bool m_sFusion;
    bool m_maxSL;
    bool m_treasure;




Persona();


private:

};

#endif // PERSONA_H
