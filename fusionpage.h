#ifndef FUSIONPAGE_H
#define FUSIONPAGE_H

#include <QDialog>
#include <QLabel>
#include "persona.h"
#include "fusion.h"

namespace Ui {
class FusionPage;
}

class FusionPage : public QDialog
{
    Q_OBJECT

public:
    explicit FusionPage(QWidget *parent = nullptr);
    ~FusionPage();
   void GetResultArcana(Persona ResPersona);
   void StartFusion(Persona result);

private:
    Ui::FusionPage *ui;
    QLabel *label;
    Persona m_result;


};

#endif // FUSIONPAGE_H
