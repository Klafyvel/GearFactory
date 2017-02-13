#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>

#include <QObject>
#include <QMainWindow>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QPainter>
#include <QMessageBox>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextStream>
#include <QTimer>
#include "wheelcreator.h"

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
    void exportSVG();
    void exportPNG();
    void exportPDF();

public slots:
    void drawWheel();
    void refreshGearsValues();

    void setPrimitiveDiameterG1(double d);
    void setExternalDiameterG1(double d);
    void setNumberOfTeethG1(int z);
    void setHoleDiameterG1(double d);
    void setNumberOfLighteningHolesG1(int n);
    void setArmWidthG1(double w);

    void setPrimitiveDiameterG2(double d);
    void setExternalDiameterG2(double d);
    void setNumberOfTeethG2(int z);
    void setHoleDiameterG2(double d);
    void setNumberOfLighteningHolesG2(int n);
    void setArmWidthG2(double w);

    void setPointResolution(int n);
    void setToothSpacing(double p);
    void setContactAngle(double alpha);
    void setClearance(double c);

    void setRotation(double alpha);

    void setAnimation(int state);
    void animate();

    void chooseExportFileName();
    void exportGraphics();

private slots:
    void on_actionShow_View_triggered();

    void on_actionExport_triggered();

    void on_actionShow_Gear_triggered();

private:
    Ui::MainWindow *ui;
    wheel::WheelCreator wheelCreator1;
    wheel::WheelCreator wheelCreator2;
    QGraphicsScene scene;
    QTimer timer;
};

#endif // MAINWINDOW_H
