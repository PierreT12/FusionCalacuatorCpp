#ifndef FUSIONPAGE_H
#define FUSIONPAGE_H

#include <QDialog>
#include <QLabel>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QDir>
#include <QDirIterator>

#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>



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

///////////////////////Acutal Metods///////////////////////
   void GetResultArcana(Persona ResPersona, bool filter);

   void StartFusion(Persona result, bool filter);

   void FuseForward(Persona p1, bool filter);

   void DisplayAllResults(QStringList finalResults);

   void Special(QString text);

   void SetFavs();

   //void ReadFavorites();

   //QList<QJsonObject> ReadFavoritesTest();
/////////////////////////////////////////////////////////

    private slots:

        void AddtoFavorites();

        void DeleteFavorites();




    private:
        Ui::FusionPage *ui;
        QLabel *label;
        Persona m_result;
        QStringList m_finalResults;
        QStandardItemModel *modelResults;
        QString m_path;
        DbAccess fusionAccess;


};

#endif // FUSIONPAGE_H
