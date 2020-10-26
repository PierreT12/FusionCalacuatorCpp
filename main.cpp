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


    QString path = QDir::currentPath() + "/Resources/header.png";
    qDebug() << "App path : " << path;
    //Variables for FusionPage
    QIcon icon (path);



    //Icon Set up
    w.setWindowIcon(icon);

    QMessageBox m;
    m.setText("Welcome!");
    m.setInformativeText("If you’re unfamiliar with how \n"
                         "a Persona/SMT fusion calculator works, \n"
                         "I really recommend that you checking "
                         "under Settings>Help, to get a basic understanding. \n \n"
                         "Thank you for trying my program!");
    m.setIcon(QMessageBox::Information);
    m.exec();





    //Shows main window
    w.show();

    return a.exec();
}
