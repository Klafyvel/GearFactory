#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
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
    void drawWheel();

private slots:
    void on_actionShow_View_triggered();

private:
    Ui::MainWindow *ui;
    wheel::WheelCreator wheelCreator;
    QGraphicsScene scene;
};

#endif // MAINWINDOW_H
