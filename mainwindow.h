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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>

#include <QObject>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <QPushButton>

#include "wheelwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void connectGui();
    void disconnectGui();

public slots:
    void setAnimation(int state);
    void animate();
    void refreshGearsValues();
    void setToothSpacing(double p);
    void setPointResolution(int n);
    void setRotation(double alpha);
    void setClearance(double c);
    void setContactAngle(double alpha);
    void setShowExternalCircle(int st);
    void setShowLineOfContact(int st);
    void setShowPrimitiveCircle(int st);

    void handleViewCenter(int id);
    void centerView(int i);
    void manageView();
    void setAnimationSpeed(int percent);

    void licenseEvent();

private:
    Ui::MainWindow *ui;
    QGraphicsScene scene;
    QTimer timer;
    WheelWidget* firstWheel;
};

#endif // MAINWINDOW_H
