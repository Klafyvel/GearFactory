/*
Gear Factory
Copyright (C) 2017  Hugo LEVY-FALK

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&scene);

    firstWheel = new WheelWidget(&scene, ui->tabWidget, 0);
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
    firstWheel->setShowExternalCircle(false);
    firstWheel->setShowLineOfContact(false);
    firstWheel->setShowPrimitiveCircle(true);
    firstWheel->setDrawing(true);
    QObject::connect(firstWheel, SIGNAL(redraw()), this, SLOT(manageView()));

    ui->tabWidget->insertTab(0,firstWheel,"Wheel 1");

    MainWindow::connectGui();
    MainWindow::refreshGearsValues();
    MainWindow::manageView();
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
    QObject::connect(this->ui->showExternalCircleCheckBox, SIGNAL(stateChanged(int)), this, SLOT(setShowExternalCircle(int)));
    QObject::connect(this->ui->showLineOfContactCheckBox, SIGNAL(stateChanged(int)), this, SLOT(setShowLineOfContact(int)));
    QObject::connect(this->ui->showPrimitiveCircleCheckBox, SIGNAL(stateChanged(int)), this, SLOT(setShowPrimitiveCircle(int)));
    QObject::connect(ui->viewButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(handleViewCenter(int)));
    QObject::connect(ui->animationSpeedSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setAnimationSpeed(int)));
    QObject::connect(ui->licensePushButton, SIGNAL(clicked()), this, SLOT(licenseEvent()));
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
    QObject::disconnect(this->ui->showExternalCircleCheckBox, SIGNAL(stateChanged(int)), this, SLOT(setShowExternalCircle(int)));
    QObject::disconnect(this->ui->showLineOfContactCheckBox, SIGNAL(stateChanged(int)), this, SLOT(setShowLineOfContact(int)));
    QObject::disconnect(this->ui->showPrimitiveCircleCheckBox, SIGNAL(stateChanged(int)), this, SLOT(setShowPrimitiveCircle(int)));
    QObject::disconnect(ui->viewButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(handleViewCenter(int)));
    QObject::disconnect(ui->animationSpeedSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setAnimationSpeed(int)));
    QObject::disconnect(ui->licensePushButton, SIGNAL(clicked()), this, SLOT(licenseEvent()));
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
    float d = PI/300;
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

void MainWindow::setShowExternalCircle(int st)
{
    firstWheel->setShowExternalCircle(!!st);
    firstWheel->refreshGearsValues();
}

void MainWindow::setShowLineOfContact(int st)
{
    firstWheel->setShowLineOfContact(!!st);
    firstWheel->refreshGearsValues();
}

void MainWindow::setShowPrimitiveCircle(int st)
{
    firstWheel->setShowPrimitiveCircle(!!st);
    firstWheel->refreshGearsValues();
}

void MainWindow::centerView(int i)
{
    WheelWidget* currentWheel = static_cast<WheelWidget*>(ui->tabWidget->widget(i));
    ui->graphicsView->fitInView(currentWheel->boundingRect(), Qt::KeepAspectRatio);
}

void MainWindow::handleViewCenter(int)
{
    MainWindow::manageView();
}

void MainWindow::manageView()
{
    if(ui->viewButtonGroup->checkedId() == -3)
    {
        MainWindow::centerView(ui->tabWidget->currentIndex());
    }
    else
    {
        QRect rect = firstWheel->fullSize();
        rect.setTop(-rect.height()/2);
        rect.setLeft(-rect.width()/2);
        ui->graphicsView->fitInView(rect, Qt::KeepAspectRatio);
    }
    firstWheel->askForRedraw();
}

void MainWindow::setAnimationSpeed(int percent)
{
    timer.setInterval((1-percent/100)*70 + percent/100*5);
}

void MainWindow::licenseEvent()
{
    QMessageBox::about(this, "License", "Gear Factory\nCopyright (C) 2017  Hugo LEVY-FALK\n\nThis program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.\n\nThis program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.\n\nYou should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.\n");
}
