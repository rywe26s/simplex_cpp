#ifndef FIRSTWINDOW_H
#define FIRSTWINDOW_H

#include <QDialog>
#include <QSpinBox>
#include <QFormLayout>
#include <QIcon>
#include "mainwindow.h"


class FirstWindow : public QDialog
{
    Q_OBJECT
public:
    explicit FirstWindow(QDialog *parent = nullptr);

signals:
    void signalButton(int row, int col);

public slots:
    void buttonClicked();

private:
    MainWindow *MWindow;
    QSpinBox *topSpinBox,
             *botSpinBox;
    QPushButton *buttonOk;
    QFormLayout  *mainLayout;
};

#endif // FIRSTWINDOW_H
