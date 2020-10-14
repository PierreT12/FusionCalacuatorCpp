#include "infowindow.h"
#include "fusionpage.h"
#include "dbaccess.h"
#include <QApplication>
#include <QSqlQuery>

void OpenConnection(std::string);

int main(int argc, char *argv[])
{
    //UI Set up
    QApplication a(argc, argv);
    //Sets up winodws and other variables
    //Variables for InfoWindow
    InfoWindow w;
    QStringListModel model;
    QStringList list;
    //Variables for FusionPage
    FusionPage f;
    QIcon icon ("C:\\Users\\Treffy\\Desktop\\Pesonal_Project\\Pesonal_Project\\Persona_Images\\header.png");
    QString path = "C:\\Users\\Treffy\\Desktop\\Pesonal_Project\\Pesonal_Project\\SQL_FILES\\finaL_Database_3.db";


    //Icon Set up
    w.setWindowIcon(icon);
    f.setWindowIcon(icon);




    //Shows main window
    w.show();
    return a.exec();
}
