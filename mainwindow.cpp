#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&scene);

    wheelCreator1.setContactAngle(20 * PI / 180);
    wheelCreator1.setNumberOfTeeth(12);
    wheelCreator1.setToothSpacing(50);
    wheelCreator1.setHoleRadius(10);
    wheelCreator1.setPointResolution(40);
    wheelCreator1.setClearance(0.02);
    wheelCreator1.setNumberOfLighteningHole(3);
    wheelCreator1.setArmWidth(20);
    wheelCreator1.computeValues();

    wheelCreator2.setContactAngle(20 * PI / 180);
    wheelCreator2.setNumberOfTeeth(6);
    wheelCreator2.setToothSpacing(50);
    wheelCreator2.setHoleRadius(10);
    wheelCreator2.setPointResolution(40);
    wheelCreator2.setClearance(0.02);
    wheelCreator2.setArmWidth(20);
    wheelCreator2.computeValues();

    wheelCreator1.syncWith(wheelCreator2);

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
    QObject::connect(ui->contactAngleDoubleSpinBox,SIGNAL(valueChanged(double)),
                     this, SLOT(setContactAngle(double)));
    QObject::connect(ui->numberOfTeethSpinBox,SIGNAL(valueChanged(int)),
                     this, SLOT(setNumberOfTeethG1(int)));
    QObject::connect(ui->toothSpacingDoubleSpinBox,SIGNAL(valueChanged(double)),
                     this, SLOT(setToothSpacing(double)));
    QObject::connect(ui->holeDiameterDoubleSpinBox,SIGNAL(valueChanged(double)),
                     this, SLOT(setHoleDiameterG1(double)));
    QObject::connect(ui->numberOfLighteningHolesSpinBox,SIGNAL(valueChanged(int)),
                     this, SLOT(setNumberOfLighteningHolesG1(int)));
    QObject::connect(ui->numberOfTeethSpinBox_2,SIGNAL(valueChanged(int)),
                     this, SLOT(setNumberOfTeethG2(int)));
    QObject::connect(ui->holeDiameterDoubleSpinBox_2,SIGNAL(valueChanged(double)),
                     this, SLOT(setHoleDiameterG2(double)));
    QObject::connect(ui->numberOfLighteningHolesSpinBox_2,SIGNAL(valueChanged(int)),
                     this, SLOT(setNumberOfLighteningHolesG2(int)));
    QObject::connect(ui->fileNamePushButton,SIGNAL(clicked()),
                     this, SLOT(chooseExportFileName()));
    QObject::connect(ui->exportPushButton,SIGNAL(clicked()),
                     this, SLOT(exportGraphics()));
    QObject::connect(ui->pointResolutionSpinBox,SIGNAL(valueChanged(int)),
                     this, SLOT(setPointResolution(int)));
    QObject::connect(ui->animateCheckBox,SIGNAL(stateChanged(int)),
                     this, SLOT(setAnimation(int)));
    QObject::connect(ui->wheel2GroupBox, SIGNAL(clicked()), this, SLOT(drawWheel()));
    QObject::connect(ui->rotationDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setRotation(double)));
    QObject::connect(ui->clearanceDoubleSpinBox, SIGNAL(valueChanged(double)),
                     this, SLOT(setClearance(double)));
    QObject::connect(ui->armWidthDoubleSpinBox, SIGNAL(valueChanged(double)),
                     this, SLOT(setArmWidthG1(double)));
    QObject::connect(ui->armWidthDoubleSpinBox_2, SIGNAL(valueChanged(double)),
                     this, SLOT(setArmWidthG2(double)));
}

void MainWindow::disconnectGui()
{
    QObject::disconnect(ui->contactAngleDoubleSpinBox,SIGNAL(valueChanged(double)),
                     this, SLOT(setContactAngle(double)));
    QObject::disconnect(ui->numberOfTeethSpinBox,SIGNAL(valueChanged(int)),
                     this, SLOT(setNumberOfTeethG1(int)));
    QObject::disconnect(ui->toothSpacingDoubleSpinBox,SIGNAL(valueChanged(double)),
                     this, SLOT(setToothSpacing(double)));
    QObject::disconnect(ui->holeDiameterDoubleSpinBox,SIGNAL(valueChanged(double)),
                     this, SLOT(setHoleDiameterG1(double)));
    QObject::disconnect(ui->numberOfLighteningHolesSpinBox,SIGNAL(valueChanged(int)),
                     this, SLOT(setNumberOfLighteningHolesG1(int)));
    QObject::disconnect(ui->numberOfTeethSpinBox_2,SIGNAL(valueChanged(int)),
                     this, SLOT(setNumberOfTeethG2(int)));
    QObject::disconnect(ui->holeDiameterDoubleSpinBox_2,SIGNAL(valueChanged(double)),
                     this, SLOT(setHoleDiameterG2(double)));
    QObject::disconnect(ui->numberOfLighteningHolesSpinBox_2,SIGNAL(valueChanged(int)),
                     this, SLOT(setNumberOfLighteningHolesG2(int)));
    QObject::disconnect(ui->fileNamePushButton,SIGNAL(clicked()),
                     this, SLOT(chooseExportFileName()));
    QObject::disconnect(ui->exportPushButton,SIGNAL(clicked()),
                     this, SLOT(exportGraphics()));
    QObject::disconnect(ui->pointResolutionSpinBox,SIGNAL(valueChanged(int)),
                     this, SLOT(setPointResolution(int)));
    QObject::disconnect(ui->animateCheckBox,SIGNAL(stateChanged(int)),
                     this, SLOT(setAnimation(int)));
    QObject::disconnect(ui->wheel2GroupBox, SIGNAL(clicked()), this, SLOT(drawWheel()));
    QObject::disconnect(ui->rotationDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setRotation(double)));
    QObject::disconnect(ui->clearanceDoubleSpinBox, SIGNAL(valueChanged(double)),
                     this, SLOT(setClearance(double)));
    QObject::disconnect(ui->armWidthDoubleSpinBox, SIGNAL(valueChanged(double)),
                     this, SLOT(setArmWidthG1(double)));
    QObject::disconnect(ui->armWidthDoubleSpinBox_2, SIGNAL(valueChanged(double)),
                     this, SLOT(setArmWidthG2(double)));
}

void MainWindow::on_actionShow_View_triggered()
{
    ui->tabWidget->setCurrentIndex(1);
    ui->groupBoxView->setFocus();
}

void MainWindow::drawWheel()
{
    scene.clear();
    std::vector<wheel::Point> points = wheelCreator1.getPoints();
    wheel::Point current = points[0];
    for(wheel::Point next : points)
    {
        scene.addLine(current.x, current.y, next.x, next.y);
        current = next;
    }

    wheel::Point center = wheelCreator1.getPositionOffset();
    float r = wheelCreator1.getHoleRadius();
    float cross_size = r*1.5;
    scene.addLine(center.x-cross_size,center.y,center.x+cross_size,center.y);
    scene.addLine(center.x,center.y-cross_size,center.x,center.y+cross_size);
    scene.addEllipse(center.x - r, center.y - r, r*2, r*2);

    std::vector<std::vector<wheel::Point>> holes = wheelCreator1.getLighteningHoles();
    for(std::vector<wheel::Point> points : holes)
    {
        wheel::Point current = points[0];
        for(wheel::Point next : points)
        {
            scene.addLine(current.x, current.y, next.x, next.y);
            current = next;
        }
    }

    r = wheelCreator1.getPrimitiveRadius();
    if(!!ui->showPrimitiveCircleCheckBox->checkState())
        scene.addEllipse(center.x - r, center.y - r, r*2, r*2);
    r = wheelCreator1.getExternalRadius();
    if(!!ui->showExternalCircleCheckBox->checkState())
        scene.addEllipse(center.x - r, center.y - r, r*2, r*2);

    if(!ui->wheel2GroupBox->isChecked())
        return;

    std::vector<wheel::Point> points2 = wheelCreator2.getPoints();
    current = points2[0];
    for(wheel::Point next : points2)
    {
        scene.addLine(current.x, current.y, next.x, next.y);
        current = next;
    }

    center = wheelCreator2.getPositionOffset();
    r = wheelCreator2.getHoleRadius();
    float cross_size2 = r*1.5;
    scene.addLine(center.x-cross_size2,center.y,center.x+cross_size2,center.y);
    scene.addLine(center.x,center.y-cross_size2,center.x,center.y+cross_size2);
    scene.addEllipse(center.x - r, center.y - r, r*2, r*2);

    holes = wheelCreator2.getLighteningHoles();
    for(std::vector<wheel::Point> points : holes)
    {
        wheel::Point current = points[0];
        for(wheel::Point next : points)
        {
            scene.addLine(current.x, current.y, next.x, next.y);
            current = next;
        }
    }

    r = wheelCreator2.getPrimitiveRadius();
    if(!!ui->showPrimitiveCircleCheckBox->checkState())
        scene.addEllipse(center.x - r, center.y - r, r*2, r*2);
    r = wheelCreator2.getExternalRadius();
    if(!!ui->showExternalCircleCheckBox->checkState())
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
    else if(ui->formatComboBox->currentText() == "PDF")
    {
        ui->fileNameLineEdit->setText(
                    QFileDialog::getSaveFileName(this, "Save PDF",
                                                 "gearFactory.pdf",
                                                 tr("PDF files (*.pdf)")));
    }
}

void MainWindow::exportGraphics()
{
    if(ui->formatComboBox->currentText() == "SVG")
        MainWindow::exportSVG();
    else if(ui->formatComboBox->currentText() == "PNG")
        MainWindow::exportPNG();
    else if(ui->formatComboBox->currentText() == "PDF")
        MainWindow::exportPDF();
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

    QMessageBox::information(this, "Export", "Image saved.");
}

void MainWindow::exportSVG()
{
    if (ui->fileNameLineEdit->text() == "")
        MainWindow::chooseExportFileName();
    QString result = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>";
    result.append("<svg width=\"");
    result.append(QString::number(this->wheelCreator1.getExternalRadius()*2));
    result.append("mm\" height=\"");
    result.append(QString::number(this->wheelCreator1.getExternalRadius()*2));
    result.append("mm\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\"  version=\"1.2\" baseProfile=\"tiny\">\n");
    result.append("<title>Gear factory</title>\n");
    result.append("<desc>File created by Gear Factory.</desc>\n");

    result.append("<g>\n");

    float r_ext = this->wheelCreator1.getExternalRadius();

    // Tooth
    result.append("<g>\n");
    wheel::Point previous;
    wheel::Point first;
    bool isFirst = true;
    for(wheel::Point p : this->wheelCreator1.getPoints())
    {
        if(isFirst)
        {
            isFirst = false;
            first = p;
        }
        else
        {
            result.append("<line x1=\"");
            result.append(QString::number(previous.x + r_ext));
            result.append("mm\" y1=\"");
            result.append(QString::number(previous.y + r_ext));
            result.append("mm\" x2=\"");
            result.append(QString::number(p.x + r_ext));
            result.append("mm\" y2=\"");
            result.append(QString::number(p.y + r_ext));
            result.append("mm\" style=\"stroke:rgb(0,0,0); stroke-width:1px\"/>\n");
        }

        previous = p;
    }
    result.append("<line x1=\"");
    result.append(QString::number(previous.x + r_ext));
    result.append("mm\" y1=\"");
    result.append(QString::number(previous.y + r_ext));
    result.append("mm\" x2=\"");
    result.append(QString::number(first.x + r_ext));
    result.append("mm\" y2=\"");
    result.append(QString::number(first.y + r_ext));
    result.append("mm\" style=\"stroke:rgb(0,0,0); stroke-width:1px\"/>\n");
    result.append("</g>\n");

    // Center
    result.append("<circle cx=\"");
    result.append(QString::number(r_ext));
    result.append("mm\" cy=\"");
    result.append(QString::number(r_ext));
    result.append("mm\" r=\"");
    result.append(QString::number(this->wheelCreator1.getHoleRadius()));
    result.append("mm\" stroke=\"black\" fill=\"none\"/>\n");

    result.append("<line x1=\"");
    result.append(QString::number(r_ext));
    result.append("mm\" y1=\"");
    result.append(QString::number(r_ext-1.5*this->wheelCreator1.getHoleRadius()));
    result.append("mm\" x2=\"");
    result.append(QString::number(r_ext));
    result.append("mm\" y2=\"");
    result.append(QString::number(r_ext+1.5*this->wheelCreator1.getHoleRadius()));
    result.append("mm\" style=\"stroke:rgb(0,0,0); stroke-width:1px\"/>\n");
    result.append("<line x1=\"");
    result.append(QString::number(r_ext-1.5*this->wheelCreator1.getHoleRadius()));
    result.append("mm\" y1=\"");
    result.append(QString::number(r_ext));
    result.append("mm\" x2=\"");
    result.append(QString::number(r_ext+1.5*this->wheelCreator1.getHoleRadius()));
    result.append("mm\" y2=\"");
    result.append(QString::number(r_ext));
    result.append("mm\" style=\"stroke:rgb(0,0,0); stroke-width:1px\"/>\n");

    result.append("</g>\n");
    result.append("</svg>\n");

    // Export
    QFile file(ui->fileNameLineEdit->text());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Export", "Could not open this file.");
        return;
    }
    QTextStream out(&file);
    out << result;
    QMessageBox::information(this, "Export", "Image saved.");
}

void MainWindow::exportPDF()
{
    if (ui->fileNameLineEdit->text() == "")
        MainWindow::chooseExportFileName();
    QPrinter printer(QPrinter::HighResolution);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(ui->fileNameLineEdit->text());

    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter;
        painter.begin(&printer);
        scene.render(&painter);
        painter.end();
      }
    QMessageBox::information(this, "Export", "Image saved.");
}

void MainWindow::on_actionShow_Gear_triggered()
{
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::refreshGearsValues()
{
    wheelCreator1.syncWith(wheelCreator2);

    MainWindow::disconnectGui();

    ui->contactAngleDoubleSpinBox->setValue(wheelCreator1.getContactAngle() * 180 / PI);
    ui->externalDiameterDoubleSpinBox->setValue(wheelCreator1.getExternalRadius()*2);
    ui->primitiveDiameterDoubleSpinBox->setValue(wheelCreator1.getPrimitiveRadius()*2);
    ui->holeDiameterDoubleSpinBox->setValue(wheelCreator1.getHoleRadius()*2);
    ui->numberOfLighteningHolesSpinBox->setValue(wheelCreator1.getNumberOfLighteningHole());
    ui->numberOfTeethSpinBox->setValue(wheelCreator1.getNumberOfTeeth());
    ui->toothSpacingDoubleSpinBox->setValue(wheelCreator1.getToothSpacing());
    ui->pointResolutionSpinBox->setValue(wheelCreator1.getPointResolution());
    ui->armWidthDoubleSpinBox->setValue(wheelCreator1.getArmWidth());

    ui->externalDiameterDoubleSpinBox_2->setValue(wheelCreator2.getExternalRadius()*2);
    ui->primitiveDiameterDoubleSpinBox_2->setValue(wheelCreator2.getPrimitiveRadius()*2);
    ui->holeDiameterDoubleSpinBox_2->setValue(wheelCreator2.getHoleRadius()*2);
    ui->numberOfLighteningHolesSpinBox_2->setValue(wheelCreator2.getNumberOfLighteningHole());
    ui->numberOfTeethSpinBox_2->setValue(wheelCreator2.getNumberOfTeeth());
    ui->armWidthDoubleSpinBox_2->setValue(wheelCreator2.getArmWidth());

    ui->clearanceDoubleSpinBox->setValue(wheelCreator1.getClearance()*100);

    MainWindow::connectGui();
}

void MainWindow::setContactAngle(double alpha)
{
    wheelCreator1.setContactAngle(alpha * PI / 180);
    wheelCreator2.setContactAngle(alpha * PI / 180);
    MainWindow::refreshGearsValues();
    MainWindow::drawWheel();
}

void MainWindow::setNumberOfTeethG1(int z)
{
    wheelCreator1.setNumberOfTeeth(z);
    MainWindow::refreshGearsValues();
    MainWindow::drawWheel();
}
void MainWindow::setToothSpacing(double p)
{
    wheelCreator1.setToothSpacing(p);
    wheelCreator2.setToothSpacing(p);
    MainWindow::refreshGearsValues();
    MainWindow::drawWheel();
}
void MainWindow::setHoleDiameterG1(double d)
{
    wheelCreator1.setHoleRadius(d/2);
    MainWindow::refreshGearsValues();
    MainWindow::drawWheel();
}
void MainWindow::setNumberOfLighteningHolesG1(int n)
{
    wheelCreator1.setNumberOfLighteningHole(n);
    MainWindow::refreshGearsValues();
    MainWindow::drawWheel();
}
void MainWindow::setPrimitiveDiameterG1(double d)
{
    wheelCreator1.setPrimitiveRadius(d/2);
    MainWindow::refreshGearsValues();
    MainWindow::drawWheel();
}
void MainWindow::setExternalDiameterG1(double d)
{
    wheelCreator1.setExternalRadius(d/2);
    MainWindow::refreshGearsValues();
    MainWindow::drawWheel();
}
void MainWindow::setPointResolution(int n)
{
    wheelCreator1.setPointResolution(n);
    wheelCreator2.setPointResolution(n);
    MainWindow::refreshGearsValues();
    MainWindow::drawWheel();
}

void MainWindow::setNumberOfTeethG2(int z)
{
    wheelCreator2.setNumberOfTeeth(z);
    MainWindow::refreshGearsValues();
    MainWindow::drawWheel();
}

void MainWindow::setHoleDiameterG2(double d)
{
    wheelCreator2.setHoleRadius(d/2);
    MainWindow::refreshGearsValues();
    MainWindow::drawWheel();
}
void MainWindow::setNumberOfLighteningHolesG2(int n)
{
    wheelCreator2.setNumberOfLighteningHole(n);
    MainWindow::refreshGearsValues();
    MainWindow::drawWheel();
}
void MainWindow::setPrimitiveDiameterG2(double d)
{
    wheelCreator2.setPrimitiveRadius(d/2);
    MainWindow::refreshGearsValues();
    MainWindow::drawWheel();
}
void MainWindow::setExternalDiameterG2(double d)
{
    wheelCreator2.setExternalRadius(d/2);
    MainWindow::refreshGearsValues();
    MainWindow::drawWheel();
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
    float d = (float)(ui->animationSpeedSpinBox->value())/100*PI/30;
    wheelCreator1.setRotationOffset(wheelCreator1.getRotationOffset()+d);
    wheelCreator1.syncWith(wheelCreator2);
    MainWindow::drawWheel();
}
void MainWindow::setRotation(double alpha)
{
    wheelCreator1.setRotationOffset(alpha*PI/180);
    wheelCreator1.syncWith(wheelCreator2);
    MainWindow::drawWheel();
}

void MainWindow::setClearance(double c)
{
    wheelCreator1.setClearance(c/100);
    wheelCreator2.setClearance(c/100);
    MainWindow::refreshGearsValues();
    MainWindow::drawWheel();
}

void MainWindow::setArmWidthG1(double w)
{
    wheelCreator1.setArmWidth(w);
    MainWindow::refreshGearsValues();
    MainWindow::drawWheel();
}

void MainWindow::setArmWidthG2(double w)
{
    wheelCreator2.setArmWidth(w);
    MainWindow::refreshGearsValues();
    MainWindow::drawWheel();
}
