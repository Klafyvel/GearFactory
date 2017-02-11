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
    wheelCreator.setHoleRadius(10);
    wheelCreator.setPointResolution(100);
    wheelCreator.computeValues();

    MainWindow::connectGui();
    MainWindow::refreshGearsValues();
    MainWindow::drawWheel();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectGui()
{
    QObject::connect(ui->primitiveDiameterDoubleSpinBox,SIGNAL(valueChanged(double)),
                     this, SLOT(setPrimitiveDiameterG1(double)));
    QObject::connect(ui->externalDiameterDoubleSpinBox,SIGNAL(valueChanged(double)),
                     this, SLOT(setExternalDiameterG1(double)));
    QObject::connect(ui->contactAngleDoubleSpinBox,SIGNAL(valueChanged(double)),
                     this, SLOT(setContactAngleG1(double)));
    QObject::connect(ui->numberOfTeethSpinBox,SIGNAL(valueChanged(int)),
                     this, SLOT(setNumberOfTeethG1(int)));
    QObject::connect(ui->toothSpacingDoubleSpinBox,SIGNAL(valueChanged(double)),
                     this, SLOT(setToothSpacingG1(double)));
    QObject::connect(ui->holeDiameterDoubleSpinBox,SIGNAL(valueChanged(double)),
                     this, SLOT(setHoleDiameterG1(double)));
    QObject::connect(ui->numberOfLighteningHolesSpinBox,SIGNAL(valueChanged(int)),
                     this, SLOT(setNumberOfLighteningHolesG1(int)));
    QObject::connect(ui->fileNamePushButton,SIGNAL(clicked()),
                     this, SLOT(chooseExportFileName()));
    QObject::connect(ui->exportPushButton,SIGNAL(clicked()),
                     this, SLOT(exportGraphics()));
    QObject::connect(ui->pointResolutionSpinBox,SIGNAL(valueChanged(int)),
                     this, SLOT(setPointResolutionG1(int)));
}

void MainWindow::disconnectGui()
{
    QObject::disconnect(ui->primitiveDiameterDoubleSpinBox,SIGNAL(valueChanged(double)),
                     this, SLOT(setPrimitiveDiameterG1(double)));
    QObject::disconnect(ui->externalDiameterDoubleSpinBox,SIGNAL(valueChanged(double)),
                     this, SLOT(setExternalDiameterG1(double)));
    QObject::disconnect(ui->contactAngleDoubleSpinBox,SIGNAL(valueChanged(double)),
                     this, SLOT(setContactAngleG1(double)));
    QObject::disconnect(ui->numberOfTeethSpinBox,SIGNAL(valueChanged(int)),
                     this, SLOT(setNumberOfTeethG1(int)));
    QObject::disconnect(ui->toothSpacingDoubleSpinBox,SIGNAL(valueChanged(double)),
                     this, SLOT(setToothSpacingG1(double)));
    QObject::disconnect(ui->holeDiameterDoubleSpinBox,SIGNAL(valueChanged(double)),
                     this, SLOT(setHoleDiameterG1(double)));
    QObject::disconnect(ui->numberOfLighteningHolesSpinBox,SIGNAL(valueChanged(int)),
                     this, SLOT(setNumberOfLighteningHolesG1(int)));
    QObject::disconnect(ui->fileNamePushButton,SIGNAL(clicked()),
                     this, SLOT(chooseExportFileName()));
    QObject::disconnect(ui->exportPushButton,SIGNAL(clicked()),
                     this, SLOT(exportGraphics()));
    QObject::disconnect(ui->pointResolutionSpinBox,SIGNAL(valueChanged(int)),
                     this, SLOT(setPointResolutionG1(int)));
}

void MainWindow::on_actionShow_View_triggered()
{
    ui->tabWidget->setCurrentIndex(1);
    ui->groupBoxView->setFocus();
}

void MainWindow::drawWheel()
{
    scene.clear();
    std::vector<wheel::Point> points = wheelCreator.getPoints();
    wheel::Point current = points[0];
    for(wheel::Point next : points)
    {
        scene.addLine(current.x, current.y, next.x, next.y);
        current = next;
    }

    wheel::Point center = wheelCreator.getPositionOffset();
    float r = wheelCreator.getHoleRadius();
    float cross_size = r*1.5;
    scene.addLine(center.x-cross_size,center.y,center.x+cross_size,center.y);
    scene.addLine(center.x,center.y-cross_size,center.x,center.y+cross_size);

    scene.addEllipse(center.x - r, center.y - r, r*2, r*2);
}

void MainWindow::on_actionExport_triggered()
{
    ui->tabWidget->setCurrentIndex(1);
    ui->groupBoxExport->setFocus();
}

void MainWindow::chooseExportFileName()
{
    if(ui->formatComboBox->currentText() == "SVG")
    {
        ui->fileNameLineEdit->setText(
                    QFileDialog::getSaveFileName(this, "Save SVG",
                                                 "gearFactory.svg",
                                                 tr("SVG files (*.svg)")));
    }
    else if(ui->formatComboBox->currentText() == "PNG")
    {
        ui->fileNameLineEdit->setText(
                    QFileDialog::getSaveFileName(this, "Save PNG",
                                                 "gearFactory.png",
                                                 tr("PNG files (*.png)")));
    }
}

void MainWindow::exportGraphics()
{
    if(ui->formatComboBox->currentText() == "SVG")
        MainWindow::exportSVG();
    else if(ui->formatComboBox->currentText() == "PNG")
        MainWindow::exportPNG();
    QMessageBox::information(this, "Export", "Image saved.");
}

void MainWindow::exportPNG()
{
    if (ui->fileNameLineEdit->text() == "")
        MainWindow::chooseExportFileName();
    QImage image(scene.sceneRect().size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    QPainter painter;
    painter.begin(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    scene.render(&painter);
    painter.end();
    image.save(ui->fileNameLineEdit->text());
}

void MainWindow::exportSVG()
{
    if (ui->fileNameLineEdit->text() == "")
        MainWindow::chooseExportFileName();
    QSvgGenerator generator;
    generator.setFileName(ui->fileNameLineEdit->text());
    generator.setSize(QSize(scene.width(), scene.height()));
    generator.setViewBox(QRect(0, 0, scene.width(), scene.height()));
    generator.setTitle("SVG Example");
    generator.setDescription("File created by SVG Example");
    QPainter painter;
    painter.begin(&generator);
    scene.render(&painter);
    painter.end();

}

void MainWindow::on_actionShow_Gear_triggered()
{
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::refreshGearsValues()
{
    MainWindow::disconnectGui();
    ui->contactAngleDoubleSpinBox->setValue(wheelCreator.getContactAngle() * 180 / PI);
    ui->externalDiameterDoubleSpinBox->setValue(wheelCreator.getExternalRadius()*2);
    ui->primitiveDiameterDoubleSpinBox->setValue(wheelCreator.getPrimitiveRadius()*2);
    ui->holeDiameterDoubleSpinBox->setValue(wheelCreator.getHoleRadius()*2);
    ui->numberOfLighteningHolesSpinBox->setValue(wheelCreator.getNumberOfLighteningHole());
    ui->numberOfTeethSpinBox->setValue(wheelCreator.getNumberOfTeeth());
    ui->toothSpacingDoubleSpinBox->setValue(wheelCreator.getToothSpacing());
    ui->pointResolutionSpinBox->setValue(wheelCreator.getPointResolution());
    MainWindow::connectGui();
}

void MainWindow::setContactAngleG1(double alpha)
{
    wheelCreator.setContactAngle(alpha * PI / 180);
    MainWindow::refreshGearsValues();
    MainWindow::drawWheel();
}

void MainWindow::setNumberOfTeethG1(int z)
{
    wheelCreator.setNumberOfTeeth(z);
    MainWindow::refreshGearsValues();
    MainWindow::drawWheel();
}
void MainWindow::setToothSpacingG1(double p)
{
    wheelCreator.setToothSpacing(p);
    MainWindow::refreshGearsValues();
    MainWindow::drawWheel();
}
void MainWindow::setHoleDiameterG1(double d)
{
    wheelCreator.setHoleRadius(d/2);
    MainWindow::refreshGearsValues();
    MainWindow::drawWheel();
}
void MainWindow::setNumberOfLighteningHolesG1(int n)
{
    wheelCreator.setNumberOfLighteningHole(n);
    MainWindow::refreshGearsValues();
    MainWindow::drawWheel();
}
void MainWindow::setPrimitiveDiameterG1(double d)
{
    wheelCreator.setPrimitiveRadius(d/2);
    MainWindow::refreshGearsValues();
    MainWindow::drawWheel();
}
void MainWindow::setExternalDiameterG1(double d)
{
    wheelCreator.setExternalRadius(d/2);
    MainWindow::refreshGearsValues();
    MainWindow::drawWheel();
}
void MainWindow::setPointResolutionG1(int n)
{
    wheelCreator.setPointResolution(n);
    MainWindow::refreshGearsValues();
    MainWindow::drawWheel();
}
