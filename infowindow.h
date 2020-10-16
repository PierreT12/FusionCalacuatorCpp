#ifndef INFOWINDOW_H
#define INFOWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QStringList>
#include <QStandardItemModel>
#include "fusionpage.h"
#include "persona.h"

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



private slots:
    void TheClick (QModelIndex);
    void FusionPress();

private:
    Ui::InfoWindow *ui;
    FusionPage *f;
    QStringListModel *model;
    QStringList *list;
    QString m_path;
    QStandardItemModel *modelTableStat;
    QStandardItemModel *modelTableMagic;
    Persona selection;

};
#endif // INFOWINDOW_H
