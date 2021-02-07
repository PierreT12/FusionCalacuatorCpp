#ifndef FAVORITES_H
#define FAVORITES_H

#include <QDialog>
#include <QDialog>
#include <QLabel>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QDir>
#include <QDirIterator>
#include <QDebug>
#include <QStringListModel>
#include <QItemSelectionModel>


#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>


#include "persona.h"

namespace Ui {
class favorites;
}

class favorites : public QDialog
{
    Q_OBJECT

public:
    explicit favorites(QWidget *parent = nullptr);
    ~favorites();

////////////////////Methods for Class/////////////////////////

    void GetFavs();
    void DisplayFavorite(QList<Persona> finalList);
////////////////////////////////////////////////////////////

private slots:
    void Selection (QModelIndex);

private:
    Ui::favorites *ui;
    QStringListModel *model;
    QItemSelectionModel *selectionModel;
    QStandardItemModel *modelResults;
    QStringListModel* listModel;
    QStringList *list;
    Persona m_result;
};

#endif // FAVORITES_H
