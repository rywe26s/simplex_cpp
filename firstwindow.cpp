#include "firstwindow.h"


FirstWindow::FirstWindow(QDialog *parent) : QDialog(parent)
{
    setFixedSize(minimumSize());
    setWindowFlags(Qt::WindowCloseButtonHint);

    topSpinBox = new QSpinBox(this);
    topSpinBox->setMinimum(1);
    topSpinBox->setMaximum(10);

    botSpinBox = new QSpinBox(this);
    botSpinBox->setMinimum(1);
    botSpinBox->setMaximum(10);

    mainLayout = new QFormLayout(this);
    mainLayout->addRow("Количество переменных: ", topSpinBox);
    mainLayout->addRow("Количество ограничений: ", botSpinBox);

    buttonOk = new QPushButton("OK", this);
    connect(buttonOk, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));

    MWindow = new MainWindow();

    connect(this, SIGNAL(signalButton(int, int)), MWindow, SLOT(takeMessageButton(int, int)));

    mainLayout->addWidget(buttonOk);

    setWindowTitle("Simplex");

    setLayout(mainLayout);
}

void FirstWindow::buttonClicked()
{
    emit signalButton(botSpinBox->value(), topSpinBox->value());
    close();
}

