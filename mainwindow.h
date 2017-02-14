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

private slots:
    void on_actionShow_View_triggered();
    void on_actionShow_Gear_triggered();

private:
    Ui::MainWindow *ui;
    QGraphicsScene scene;
    QTimer timer;
    WheelWidget* firstWheel;
};

#endif // MAINWINDOW_H
