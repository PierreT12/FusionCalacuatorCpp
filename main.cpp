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
    QIcon icon ("C:\\Users\\Treffy\\Desktop\\Pesonal_Project\\Pesonal_Project\\Persona_Images\\header.png");



    //Icon Set up
    w.setWindowIcon(icon);






    //Shows main window
    w.show();

    return a.exec();
}
