#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QMainWindow>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QSvgGenerator>
#include <QPainter>
#include <QMessageBox>
#include <QPrinter>
#include <QPrintDialog>
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
    void setContactAngleG1(double alpha);
    void setNumberOfTeethG1(int z);
    void setToothSpacingG1(double p);
    void setHoleDiameterG1(double d);
    void setNumberOfLighteningHolesG1(int n);
    void setPointResolutionG1(int n);

    void chooseExportFileName();
    void exportGraphics();

private slots:
    void on_actionShow_View_triggered();

    void on_actionExport_triggered();

    void on_actionShow_Gear_triggered();

private:
    Ui::MainWindow *ui;
    wheel::WheelCreator wheelCreator;
    QGraphicsScene scene;
};

#endif // MAINWINDOW_H
