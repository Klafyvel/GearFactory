#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&scene);

    firstWheel = new WheelWidget(&scene);
    if(!firstWheel)
            return;
    firstWheel->setDrawing(false);
    firstWheel->setToothSpacing(ui->toothSpacingDoubleSpinBox->value());
    firstWheel->setContactAngle(ui->contactAngleDoubleSpinBox->value()*PI/180);
    firstWheel->setClearance(ui->clearanceDoubleSpinBox->value()/100);
    firstWheel->setPointResolution(ui->pointResolutionSpinBox->value());
    firstWheel->setNumberOfTeeth(12);
    firstWheel->setHoleDiameter(20);
    firstWheel->setNumberOfLighteningHoles(3);
    firstWheel->setArmWidth(20);
    firstWheel->setDrawing(true);
    QObject::connect(firstWheel, SIGNAL(redraw()), firstWheel, SLOT(askForRedraw()));


    addWheelPushButton = new QPushButton("Add..");
    ui->tabWidget->setCornerWidget(addWheelPushButton);
    ui->tabWidget->insertTab(0, firstWheel, "Wheel 1");

    MainWindow::connectGui();
    MainWindow::refreshGearsValues();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectGui()
{
    QObject::connect(ui->contactAngleDoubleSpinBox,SIGNAL(valueChanged(double)),
                     this, SLOT(setContactAngle(double)));
    QObject::connect(ui->toothSpacingDoubleSpinBox,SIGNAL(valueChanged(double)),
                     this, SLOT(setToothSpacing(double)));
    QObject::connect(ui->pointResolutionSpinBox,SIGNAL(valueChanged(int)),
                     this, SLOT(setPointResolution(int)));
    QObject::connect(ui->animateCheckBox,SIGNAL(stateChanged(int)),
                     this, SLOT(setAnimation(int)));
    QObject::connect(ui->rotationDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setRotation(double)));
    QObject::connect(ui->clearanceDoubleSpinBox, SIGNAL(valueChanged(double)),
                     this, SLOT(setClearance(double)));
    QObject::connect(addWheelPushButton, SIGNAL(clicked()), this, SLOT(addWheel()));
}

void MainWindow::disconnectGui()
{
    QObject::disconnect(ui->contactAngleDoubleSpinBox,SIGNAL(valueChanged(double)),
                     this, SLOT(setContactAngle(double)));
    QObject::disconnect(ui->toothSpacingDoubleSpinBox,SIGNAL(valueChanged(double)),
                     this, SLOT(setToothSpacing(double)));
    QObject::disconnect(ui->pointResolutionSpinBox,SIGNAL(valueChanged(int)),
                     this, SLOT(setPointResolution(int)));
    QObject::disconnect(ui->animateCheckBox,SIGNAL(stateChanged(int)),
                     this, SLOT(setAnimation(int)));
    QObject::disconnect(ui->rotationDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setRotation(double)));
    QObject::disconnect(ui->clearanceDoubleSpinBox, SIGNAL(valueChanged(double)),
                     this, SLOT(setClearance(double)));
    QObject::disconnect(addWheelPushButton, SIGNAL(clicked()), this, SLOT(addWheel()));
}

void MainWindow::on_actionShow_View_triggered()
{
    ui->groupBoxView->setFocus();
}

void MainWindow::on_actionShow_Gear_triggered()
{
    ui->gearGroupBox->setFocus();
}

void MainWindow::refreshGearsValues()
{
    MainWindow::disconnectGui();
    firstWheel->refreshGearsValues();
    MainWindow::connectGui();

}

void MainWindow::setContactAngle(double alpha)
{
    firstWheel->setContactAngle(alpha* PI / 180);
    firstWheel->refreshGearsValues();
}

void MainWindow::setToothSpacing(double p)
{
    firstWheel->setToothSpacing(p);
    firstWheel->refreshGearsValues();
}

void MainWindow::setPointResolution(int n)
{
    firstWheel->setPointResolution(n);
    firstWheel->refreshGearsValues();
}

void MainWindow::setAnimation(int state)
{
    if(!!state)
    {
        QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(animate()));
        timer.start(20);
    }
    else
    {
        QObject::disconnect(&timer, SIGNAL(timeout()), this, SLOT(animate()));
        timer.stop();
    }
}

void MainWindow::animate()
{
    float d = (float)(ui->animationSpeedSpinBox->value())/100 * PI/30;
    firstWheel->rotate(d);
    firstWheel->refreshGearsValues();
}

void MainWindow::setRotation(double alpha)
{
    firstWheel->setRotation(alpha * PI/180);
    firstWheel->refreshGearsValues();
}

void MainWindow::setClearance(double c)
{
    firstWheel->setClearance(c/100);
    firstWheel->refreshGearsValues();
}

void MainWindow::addWheel()
{
    WheelWidget* wheel = firstWheel->addWheel();
    if (!wheel)
    {
        QMessageBox::critical(this, "Error", "Could not add a wheel...");
        return;
    }
    int i = ui->tabWidget->count();
    ui->tabWidget->insertTab(i, wheel, QString("Wheel ").append(QString::number(i+1)));
    ui->tabWidget->setCurrentIndex(i);
    firstWheel->refreshGearsValues();
}
