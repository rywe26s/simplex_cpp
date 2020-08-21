#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QString>
#include <QVector>
#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QStringList>
#include <QRadioButton>
#include <QRegExpValidator>


class MainWindow : public QDialog
{
    Q_OBJECT
public:
    explicit MainWindow(QDialog *parent = nullptr);

    MainWindow(int m, int n){
        this->m = m;
        this->n = n;
    }

signals:

public slots:
    void takeMessageButton(int row, int col);
    void buttonCliked();

private:
    int m = 0, n = 0, itr = 1;
    double *Z, *M, **A;

    QString text;
    QLabel *label;
    QGridLayout *grid;
    QLineEdit *lineEdit;
    QTextEdit *textEdit; 
    QDialog *lastWindow;
    QComboBox *comboBox; 
    QPushButton *buttonGO;
    QVBoxLayout *mainLayout,
                *textLayout;
    QRadioButton *radButton1,
                 *radButton2;
    QVector <QLineEdit*> linesA,
                         linesZ;
    QVector <QComboBox*> comboxes;

    void showMat(double** A, int m, int n);
    void result(double** A, double* M, int m, int n);
    void tabel(double** A, double* Z, int m, int n);
    void simplex(double** A, double* M, int m, int n);
    void operate(double** A, int m, int n, int a, int b);
    double dual(double ** A, double* M, int m, int n, int im);
    double primal(double** A, double* M, int m, int n, int im);
};

#endif // MAINWINDOW_H
