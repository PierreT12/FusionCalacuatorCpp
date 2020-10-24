#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();
    bool returnSpoils();
    bool returnDLC();

public slots:
    void Exit();

private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H
