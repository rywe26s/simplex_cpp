//----------------------------------------------------------------
//Разработал: Повстяной Кирилл
//Версия 1.3
//Язык: С++
//Среда: Qt
//----------------------------------------------------------------
//Функция, используемая в программе:
//showMat - функция вывода матрицы на экран;
//tabel - функция для обработки данных введенных пользователем;
//operate - функция преобразования матрицы;
//primal - функция решения симплекс методом;
//dual - функция решения двойственным симплекс методом;
//simplex - основная функция решения симплекс метода;
//result - функция вывода решения на экран;
//buttonCliked - главная функция ввода, решения и вывода симплекс метода.
//----------------------------------------------------------------
//Переменные, используемые в программе:
//A - матрица коэффициентов;
//m - строки матрицы;
//n - столбцы матрицы;
//i,j - счетчики циклов;
//Z - массив для хранения знаков уравнения;
//sing,g,c1,c2,check - вспомогательные переменные;
//a - текущая строка матриц;
//b - текущий столбец матрицы;
//M,B - вспомогательные массивы;
//p,J,I,PI,DI,ratio - вспомогательные переменные;
//im - текущая строка матрицы;
//sum - переменная для хранения суммы;
//min - переменная для хранения минимума.
//----------------------------------------------------------------

#include "mainwindow.h"

void MainWindow::takeMessageButton(int row, int col)
{
    int i, j = 0;
    m = row;
    n = col;

    for(j = 0; j < col; j++)
    {
        label = new QLabel("x<sub>" + QString::number(j + 1) + "</sub>", this);
        label->setFont(QFont("Times", 11));
        label->setAlignment(Qt::AlignCenter);
        grid->addWidget(label, 0, j);

        lineEdit = new QLineEdit("0");
        lineEdit->setValidator(new QRegExpValidator(QRegExp("[-]{0,1}\\d{0,}\\.\\d{0,}"), this));
        linesZ += lineEdit;
        grid->addWidget(lineEdit, 1, j);
    }

    grid->addWidget(radButton1, 1, col+1);
    radButton1->setChecked(true);
    grid->addWidget(radButton2, 1, col+2);

    QStringList signs = {"<" , ">"};

    for(i = 2; i < row+2; i++)
    {
        for(j = 0; j < col; j++)
        {
            lineEdit = new QLineEdit("0");
            lineEdit->setValidator(new QRegExpValidator(QRegExp("[-]{0,1}\\d{0,}\\.\\d{0,}"), this));
            linesA.push_back(lineEdit);
            grid->addWidget(lineEdit, i, j);
        }
        comboBox = new QComboBox();
        comboBox->addItems(signs);
        comboxes += comboBox;
        grid->addWidget(comboBox, i, j+1);

        lineEdit = new QLineEdit("0");
        lineEdit->setValidator(new QRegExpValidator(QRegExp("[-]{0,1}\\d{0,}\\.\\d{0,}"), this));
        linesA.push_back(lineEdit);
        grid->addWidget(lineEdit, i, j+2);
    }

    mainLayout->addLayout(grid);
    mainLayout->addWidget(buttonGO);

    this->setLayout(mainLayout);
    this->show();
    this->resize(this->minimumSize());
}
MainWindow::MainWindow(QDialog *parent) : QDialog(parent)
{
    setFixedSize(minimumSize());
    setWindowFlags(Qt::WindowCloseButtonHint);
    setWindowTitle("Simplex");

    grid = new QGridLayout();
    mainLayout = new QVBoxLayout(this);
    radButton1 = new QRadioButton("Min", this);
    radButton2 = new QRadioButton("Max", this);
    buttonGO = new QPushButton("GO", this);

    lastWindow = new QDialog();
    textLayout = new QVBoxLayout(lastWindow);
    textEdit = new QTextEdit(lastWindow);

    connect(buttonGO, SIGNAL(clicked(bool)), this, SLOT(buttonCliked()));

    textLayout->addWidget(textEdit);
    lastWindow->setWindowTitle("Simplex");
    lastWindow->setLayout(textLayout);
    lastWindow->setFixedSize(QSize(600, 500));
    lastWindow->setWindowFlags(Qt::WindowCloseButtonHint);
    lastWindow->setModal(true);
}


//----------------------------------------------------------------
//showMat - функция вывода матрицы на экран.
//----------------------------------------------------------------
//Формальные параметры:
//A – матрица коэффициентов;
//m – строки матрицы;
//n – столбцы матрицы.
//----------------------------------------------------------------
//Локальные переменные:
//i,j – счетчики цикла.
//----------------------------------------------------------------
void MainWindow::showMat(double** A, int m, int n)
{
    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j < n; j++)
            text += QString::number(A[i][j]) + "\t";
        text += "\n";
    }
}

//----------------------------------------------------------------
//tabel - функция для обработки данных введенных пользователем.
//----------------------------------------------------------------
//Формальные параметры:
//A – матрица коэффициентов;
//Z - массив для хранения знаков уранений;
//m – строки матрицы;
//n – столбцы матрицы.
//----------------------------------------------------------------
//Локальные переменные:
//sign, g - вспомогательные переменные;
//i,j – счетчики цикла.
//----------------------------------------------------------------
void MainWindow::tabel(double** A, double* Z, int m, int n)
{
    QString sign, g = ">";

    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j < n; j++)
            A[i][j] = linesA[i * (n + 1) + j]->text().toDouble();

        sign = comboxes[i]->currentText();

        A[i][n] = linesA[i * (n + 1) + n]->text().toDouble();

        if(sign == g)
        {
            for(int j = 0; j <= n; j++)
                A[i][j] *= -1;
        }
    }

    for(int i = 0; i < n; i++)
        A[m][i] = -1 * Z[i];

    A[m][n] = 0;
}

//----------------------------------------------------------------
//operate - функция преобразования матрицы.
//----------------------------------------------------------------
//Формальные параметры:
//A – матрица коэффициентов;
//m – строки матрицы;
//n – столбцы матрицы;
//a – текущая строка матрицы;
//b – текущий столбец матрицы.
//----------------------------------------------------------------
//Локальные переменные:
//p - вспомогательная переменная;
//B - вспомогательный массив;
//i,j – счетчики цикла.
//----------------------------------------------------------------
void MainWindow::operate(double** A, int m, int n, int a, int b)
{
    double p;
    double **B;

    B = new double* [m + 1];

    for(int i = 0; i <= m; i++)
        B[i] = new double[n + 1];

    for(int i = 0; i <= m; i++)
        for(int j = 0;j <= n; j++)
            B[i][j] = A[i][j];

    p = B[a][b];
    A[a][b] = 1 / p;

    for(int i = 0; i <= m; i++)
        if(i != a)
            A[i][b] = -1 * B[i][b] / p;

    for(int j = 0; j <= n; j++)
        if(j != b)
            A[a][j] = B[a][j] / p;

    for(int i = 0; i <= m; i++)
    {
        for(int j = 0; j <= n; j++)
        {
            if((i != a) && (j != b))
                A[i][j] = ((B[a][b] * B[i][j]) - (B[a][j]*B[i][b])) / p;
        }
    }
}

//----------------------------------------------------------------
//primal - функция решения симплекс методом.
//----------------------------------------------------------------
//Формальные параметры:
//A – матрица коэффициентов;
//M - вспомогательный массив;
//m – строки матрицы;
//n – столбцы матрицы;
//im – текущая строка матрицы.
//----------------------------------------------------------------
//Локальные переменные:
//J,I,PI,ratio - вспомогательные переменные;
//sum - переменная для хранения суммы;
//min - переменная для хранения минимального значения;
//i,j – счетчики цикла.
//----------------------------------------------------------------
double MainWindow::primal(double** A, double* M, int m, int n, int im)
{
    int J = -1, I = 0;
    double min = 0, PI = 0;

    for(int j = 0; j < n; j++)
        if(A[m][j] < min)
        {
            min = A[m][j];
            J = j;
        }

    double sum = -1, ratio;

    for(int i = 0; i < m; i++)
    {
        if(A[i][J] > 0)
        {
            ratio = A[i][n] / A[i][J];
            if((static_cast<int>(sum) == -1) || (sum > ratio))
            {
                sum = ratio;
                I = i;
            }
        }
    }

    if(im == 1)
    {
        PI = A[I][n] * A[m][J] / A[I][J];
        if(PI < 0)
            PI *= -1;
        //text += "PI = " + QString::number(PI) + "\n";
        return PI;
    }

    operate(A, m, n, I, J);

    double temp;
    temp = M[I + n];
    M[I + n] = M[J];
    M[J] = temp;

    return 0;
}

//----------------------------------------------------------------
//dual - функция решения двойственным симплекс методом.
//----------------------------------------------------------------
//Формальные параметры:
//A – матрица коэффициентов;
//M - вспомогательный массив;
//m – строки матрицы;
//n – столбцы матрицы;
//im – текущая строка матрицы.
//----------------------------------------------------------------
//Локальные переменные:
//J,I,DI,ratio - вспомогательные переменные;
//sum - переменная для хранения суммы;
//min - переменная для хранения минимального значения;
//i,j – счетчики цикла.
//----------------------------------------------------------------
double MainWindow::dual(double ** A, double* M, int m, int n, int im) {
    int I = -1, J = 0;
    double min = 0, DI = 0;

    for(int i = 0; i < m; i++)
        if(A[i][n] < min)
        {
            min = A[i][n];
            I = i;
        }

    double sum = -1, ratio = 0;

    for(int j = 0; j < m; j++)
    {
        if(A[I][j] < 0) {
            ratio = A[m][j] / A[I][j];
            if((static_cast<int>(sum) == -1) || (sum > ratio))
            {
                sum = ratio;
                J = j;
            }
        }
    }

    if(im == 1)
    {
        DI = A[I][n] * A[m][J] / A[I][J];
        if(DI < 0)
            DI *= -1;
        //text += "DI = " + QString::number(DI) + "\n";
        return DI;
    }

    operate(A, m, n, I, J);

    double temp;
    temp = M[I + n];
    M[I + n] = M[J];
    M[J] = temp;

    return 0;
}

//----------------------------------------------------------------
//simplex - основная функция решения симплекс метода.
//----------------------------------------------------------------
//Формальные параметры:
//A – матрица коэффициентов;
//M - вспомогательный массив;
//m – строки матрицы;
//n – столбцы матрицы.
//----------------------------------------------------------------
//Локальные переменные:
//PI,DI,c1,c2 - вспомогательные переменные;
//i,j – счетчики цикла.
//----------------------------------------------------------------
void MainWindow::simplex(double** A, double* M, int m, int n) {

    int c1 = 0, c2 = 0;

    for(int i = 0; i < m; i++)
        if(A[i][n] < 0)
            c1 = 1;

    for(int j = 0; j < n; j++)
        if(A[m][j] < 0)
            c2 = 1;

    if((c1 == 0) && (c2 == 0))
        return;
    else if(c1 == 0)
        primal(A, M, m, n, 0);
    else if(c2 == 0)
        dual(A, M, m, n, 0);
    else
    {
        double PI, DI;

        PI = primal(A, M, m, n, 1);
        DI = dual(A, M, m, n, 1);

        if(PI >= DI)
            primal(A, M, m, n, 0);
        else
            dual(A, M, m, n, 0);
    }

    text += "\nИтерация " + QString::number(itr++) + "\n";
    showMat(A, m+1, n+1);

    simplex(A, M, m, n);
}

//----------------------------------------------------------------
//result - функция вывода решения на экран.
//----------------------------------------------------------------
//Формальные параметры:
//A – матрица коэффициентов;
//M - вспомогательный массив;
//m – строки матрицы;
//n – столбцы матрицы.
//----------------------------------------------------------------
//Локальные переменные:
//i – счетчик цикла.
//----------------------------------------------------------------
void MainWindow::result(double** A, double* M, int m, int n) {
    text += "\nZ = " + QString::number(A[m][n]);

    if (radButton1->isChecked() == true)
        text += " -> min";
    else
        text += " -> max";

    text += "\n\nНа:\n";

    for(int i = 0; i < m; i++)
        text += "X" + QString::number(M[i + n] + 1) + " = " + QString::number(A[i][n]) + "\n";
    for(int i = 0; i < n; i++)
        text += "X" + QString::number(M[i] + 1) + " = 0\n";
}

//----------------------------------------------------------------
//buttonCliked - главная функция ввода, решения и вывода симплекс метода.
//----------------------------------------------------------------
//Локальные переменные:
//check - вспомогательная переменная;
//A - матрица;
//Z - массив знаков;
//M - вспомогательный массив;
//i – счетчик цикла.
//----------------------------------------------------------------
void MainWindow::buttonCliked()
{
    int i = 0, check = 1;

    itr = 1;

    text.clear();
    text.squeeze();


    if (radButton2->isChecked() == true)
        check = 0;

    Z = new double [n];

    A = new double* [m + 1];
    for(i = 0; i <= m; i++)
        A[i] = new double [n + 1];

    M = new double [n + m];
    for(i = 0; i < m + n; i++)
        M[i] = i;

    for(i = 0; i < n; i++)
        Z[i] = linesZ[i]->text().toDouble();

    if(check==1)
        for(i = 0; i < n; i++)
            Z[i] = -1 * Z[i];

    text += "Исходная матрица:\n";
    tabel(A, Z, m, n);

    showMat(A, m + 1, n + 1);

    simplex(A, M, m, n);

    if(check==1)
        A[m][n] *= -1;

    text += "\nРешение:\n";
    showMat(A, m+1, n+1);

    result(A, M, m, n);

    textEdit->setText(text);

    lastWindow->show();

    for (int i = 0; i < m + 1; i++) //Чистим память
        delete [] A[i];

    delete [] A;
    delete [] M;
    delete [] Z;
}
