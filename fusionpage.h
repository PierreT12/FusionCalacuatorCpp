#ifndef FUSIONPAGE_H
#define FUSIONPAGE_H

#include <QDialog>

namespace Ui {
class FusionPage;
}

class FusionPage : public QDialog
{
    Q_OBJECT

public:
    explicit FusionPage(QWidget *parent = nullptr);
    ~FusionPage();

private:
    Ui::FusionPage *ui;
};

#endif // FUSIONPAGE_H
