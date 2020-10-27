#ifndef FUSIONPAGE_H
#define FUSIONPAGE_H

#include <QDialog>
#include <QLabel>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QDir>


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
   void GetResultArcana(Persona ResPersona, bool filter);
   void StartFusion(Persona result, bool filter);
   void FuseForward(Persona p1, bool filter);
   void DisplayAllResults(QStringList finalResults);




private:
    Ui::FusionPage *ui;
    QLabel *label;
    Persona m_result;
    QStandardItemModel *modelResults;
    QString m_path;
    DbAccess fusionAccess;


};

#endif // FUSIONPAGE_H
