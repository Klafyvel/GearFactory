#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&scene);
    wheelCreator.setContactAngle(20 * PI / 180);
    wheelCreator.setNumberOfTeeth(12);
    wheelCreator.setToothSpacing(50);
    wheelCreator.computeValues();

    MainWindow::drawWheel();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionShow_View_triggered()
{

}

void MainWindow::drawWheel()
{
    std::vector<wheel::Point> points = wheelCreator.getPoints();
    wheel::Point current = points[0];
    for(wheel::Point next : points)
    {
        scene.addLine(current.x, current.y, next.x, next.y);
        current = next;
    }
}
