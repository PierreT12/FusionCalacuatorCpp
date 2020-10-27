#ifndef INFOWINDOW_H
#define INFOWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QStringList>
#include <QStandardItemModel>
#include <QDir>
#include <QItemSelectionModel>

#include "fusionpage.h"
#include "persona.h"
#include "Settings.h"
#include "aboutpage.h"
#include "helppage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class InfoWindow; }
QT_END_NAMESPACE

class InfoWindow : public QMainWindow
{
    Q_OBJECT

public:

    InfoWindow(QWidget *parent = nullptr);
    ~InfoWindow();
    QStringList DataConnection();
    void AddToView(QStringList list);
    void SetTableViews();
    void AddToUi(QStandardItemModel* models,
                 QStringList list);
    void AddToUiInfo(QStandardItemModel* models,
                     QStringList list);

    bool SendBools();

private slots:
    void TheClick (QModelIndex);
    void FusionPress();
    void ForwardPress();
    void SearchPress();
    void Exit();
    void GiveData();
    void OpenAbout();
    void OpenHelp();
    void OpenSettings();

private:
    Ui::InfoWindow *ui;
    FusionPage *f;
    Settings *s;
    AboutPage *a;
    Help *h;
    QStringListModel *model;
    QItemSelectionModel *selectionModel;
    QStringListModel* listModel;
    QStringList *list;
    QString m_path;
    DbAccess mainAccess;
    QStandardItemModel *modelTableStat;
    QStandardItemModel *modelTableMagic;
    Persona selection;

};
#endif // INFOWINDOW_H
