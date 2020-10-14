#ifndef INFOWINDOW_H
#define INFOWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QStringList>
#include <QStandardItemModel>

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



private slots:
    void TheClick (QModelIndex);

private:
    Ui::InfoWindow *ui;
    QStringListModel *model;
    QStringList *list;
    QString m_path;
    QStandardItemModel *modelTableStat;
    QStandardItemModel *modelTableMagic;

};
#endif // INFOWINDOW_H
