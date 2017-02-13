#include "wheelwidget.h"
#include "ui_wheelwidget.h"

WheelWidget::WheelWidget(QGraphicsScene *scene, float contactAngle, float toothSpacing, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WheelWidget)
{
    ui->setupUi(this);
    this->scene = scene;
    next = 0;
    previous = 0;
    wheelCreator.setContactAngle(contactAngle);
    wheelCreator.setToothSpacing(toothSpacing);
    wheelCreator.setNumberOfTeeth(3);
    wheelCreator.setNumberOfLighteningHole(0);
    drawing = true;
}

void WheelWidget::connectGui()
{
    QObject::connect(ui->numberOfTeethSpinBox, SIGNAL(valueChanged(int)),
                     this, SLOT(setNumberOfTeeth(int)));
    QObject::connect(ui->holeDiameterDoubleSpinBox, SIGNAL(valueChanged(double)),
                     this, SLOT(setHoleDiameter(double)));
    QObject::connect(ui->numberOfLighteningHolesSpinBox, SIGNAL(valueChanged(int)),
                     this, SLOT(setNumberOfLighteningHoles(int)));
    QObject::connect(ui->armWidthDoubleSpinBox, SIGNAL(valueChanged(double)),
                     this, SLOT(setArmWidth(double)));
    QObject::connect(ui->exportPushButton, SIGNAL(clicked()),
                     this, SLOT(exportWheel()));
}

void WheelWidget::disconnectGui()
{
    QObject::disconnect(ui->numberOfTeethSpinBox, SIGNAL(valueChanged(int)),
                     this, SLOT(setNumberOfTeeth(int)));
    QObject::disconnect(ui->holeDiameterDoubleSpinBox, SIGNAL(valueChanged(double)),
                     this, SLOT(setHoleDiameter(double)));
    QObject::disconnect(ui->numberOfLighteningHolesSpinBox, SIGNAL(valueChanged(int)),
                     this, SLOT(setNumberOfLighteningHoles(int)));
    QObject::disconnect(ui->armWidthDoubleSpinBox, SIGNAL(valueChanged(double)),
                     this, SLOT(setArmWidth(double)));
    QObject::disconnect(ui->exportPushButton, SIGNAL(clicked()),
                     this, SLOT(exportWheel()));
}

void WheelWidget::drawWheel()
{
    if(!previous)
        scene->clear();
    if(next)
        next->drawWheel();

    wheel::Point center = wheelCreator.getPositionOffset();
    if(showExternalCircle)
    {
        float a = 1/std::tan(wheelCreator.getContactAngle());
        float b = wheelCreator.getPrimitiveRadius();

        scene->addLine(b/2+center.x,a*b/2+center.y,3.0/2.0*b+center.x,-a*b/2+center.y);
    }

    std::vector<wheel::Point> points = wheelCreator.getPoints();
    wheel::Point current = points[0];
    for(wheel::Point next : points)
    {
        scene->addLine(current.x, current.y, next.x, next.y);
        current = next;
    }

    float r = wheelCreator.getHoleRadius();
    float cross_size = r*1.5;
    scene->addLine(center.x-cross_size,center.y,center.x+cross_size,center.y);
    scene->addLine(center.x,center.y-cross_size,center.x,center.y+cross_size);
    scene->addEllipse(center.x - r, center.y - r, r*2, r*2);

    std::vector<std::vector<wheel::Point>> holes = wheelCreator.getLighteningHoles();
    for(std::vector<wheel::Point> points : holes)
    {
        current = points[0];
        for(wheel::Point next : points)
        {
            scene->addLine(current.x, current.y, next.x, next.y);
            current = next;
        }
    }

    r = wheelCreator.getPrimitiveRadius();
    if(showPrimitiveCircle)
        scene->addEllipse(center.x - r, center.y - r, r*2, r*2);
    r = wheelCreator.getExternalRadius();
    if(showExternalCircle)
        scene->addEllipse(center.x - r, center.y - r, r*2, r*2);
}

void WheelWidget::refreshGearsValues()
{
    WheelWidget::disconnectGui();
    wheelCreator.computeValues();
    ui->externalDiameterDoubleSpinBox->setValue(wheelCreator.getExternalRadius()*2);
    ui->primitiveDiameterDoubleSpinBox->setValue(wheelCreator.getPrimitiveRadius()*2);
    ui->holeDiameterDoubleSpinBox->setValue(wheelCreator.getHoleRadius()*2);
    ui->numberOfLighteningHolesSpinBox->setValue(wheelCreator.getNumberOfLighteningHole());
    ui->numberOfTeethSpinBox->setValue(wheelCreator.getNumberOfTeeth());
    ui->armWidthDoubleSpinBox->setValue(wheelCreator.getArmWidth());
    WheelWidget::connectGui();
    if(next)
    {
        wheelCreator.syncWith(next->wheelCreator);
        next->drawing=false;
        next->setContactAngle(wheelCreator.getContactAngle());
        next->setToothSpacing(wheelCreator.getToothSpacing());
        next->setPointResolution(wheelCreator.getPointResolution());
        next->setClearance(wheelCreator.getClearance());
        next->setShowLineOfContact(showLineOfContact);
        next->setShowExternalCircle(showExternalCircle);
        next->setShowPrimitiveCircle(showPrimitiveCircle);
        next->refreshGearsValues();
        next->drawing=true;
    }
    if(drawing)
        emit redraw();
}


void WheelWidget::setContactAngle(double alpha)
{
    wheelCreator.setContactAngle(alpha);
    WheelWidget::refreshGearsValues();
}

void WheelWidget::setNumberOfTeeth(int z)
{
    wheelCreator.setNumberOfTeeth(z);
    if(previous)
        previous->wheelCreator.syncWith(wheelCreator);
    WheelWidget::refreshGearsValues();
}

void WheelWidget::setToothSpacing(double p)
{
    wheelCreator.setToothSpacing(p);
}

void WheelWidget::setHoleDiameter(double d)
{
    wheelCreator.setHoleRadius(d/2);
    WheelWidget::refreshGearsValues();
}

void WheelWidget::setNumberOfLighteningHoles(int n)
{
    wheelCreator.setNumberOfLighteningHole(n);
    WheelWidget::refreshGearsValues();
}

void WheelWidget::setPrimitiveDiameter(double d)
{
    wheelCreator.setPrimitiveRadius(d/2);
    WheelWidget::refreshGearsValues();
}

void WheelWidget::setExternalDiameter(double d)
{
    wheelCreator.setExternalRadius(d/2);
    WheelWidget::refreshGearsValues();
}

void WheelWidget::setPointResolution(int n)
{
    wheelCreator.setPointResolution(n);
    WheelWidget::refreshGearsValues();
}

void WheelWidget::setClearance(double c)
{
    wheelCreator.setClearance(c);
}

void WheelWidget::setArmWidth(double w)
{
    wheelCreator.setArmWidth(w);
    WheelWidget::refreshGearsValues();
}

void WheelWidget::setRotation(double alpha)
{
    wheelCreator.setRotationOffset(alpha);
}

void WheelWidget::rotate(double step)
{
    WheelWidget::setRotation(wheelCreator.getRotationOffset()+step);
}

void WheelWidget::exportWheel()
{
    if(ExportDialog::exportWheel(*this, this))
        QMessageBox::information(this, "Export", "Image saved.");
    else
        QMessageBox::critical(this, "Export", "Could not save.");
}

void WheelWidget::setShowLineOfContact(bool st)
{
    showLineOfContact = st;
}

void WheelWidget::setShowPrimitiveCircle(bool st)
{
    showPrimitiveCircle = st;
}

void WheelWidget::setShowExternalCircle(bool st)
{
    showExternalCircle = st;
}

void WheelWidget::exportSVG(QString filename)
{
    float r_ext = wheelCreator.getExternalRadius();
    QString result = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>";
    result.append("<svg width=\"");
    result.append(QString::number(r_ext*2));
    result.append("mm\" height=\"");
    result.append(QString::number(r_ext*2));
    result.append("mm\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\"  version=\"1.2\" baseProfile=\"tiny\">\n");
    result.append("<title>Gear factory</title>\n");
    result.append("<desc>File created by Gear Factory.</desc>\n");

    result.append("<g>\n");

    // Tooth
    result.append("<g>\n");
    wheel::Point previous;
    wheel::Point first;
    bool isFirst = true;
    for(wheel::Point p : wheelCreator.getPoints())
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
    result.append(QString::number(wheelCreator.getHoleRadius()));
    result.append("mm\" stroke=\"black\" fill=\"none\"/>\n");

    result.append("<line x1=\"");
    result.append(QString::number(r_ext));
    result.append("mm\" y1=\"");
    result.append(QString::number(r_ext-1.5*wheelCreator.getHoleRadius()));
    result.append("mm\" x2=\"");
    result.append(QString::number(r_ext));
    result.append("mm\" y2=\"");
    result.append(QString::number(r_ext+1.5*wheelCreator.getHoleRadius()));
    result.append("mm\" style=\"stroke:rgb(0,0,0); stroke-width:1px\"/>\n");
    result.append("<line x1=\"");
    result.append(QString::number(r_ext-1.5*wheelCreator.getHoleRadius()));
    result.append("mm\" y1=\"");
    result.append(QString::number(r_ext));
    result.append("mm\" x2=\"");
    result.append(QString::number(r_ext+1.5*wheelCreator.getHoleRadius()));
    result.append("mm\" y2=\"");
    result.append(QString::number(r_ext));
    result.append("mm\" style=\"stroke:rgb(0,0,0); stroke-width:1px\"/>\n");

    result.append("</g>\n");
    result.append("</svg>\n");

    // Export
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Export", "Could not open this file.");
        return;
    }
    QTextStream out(&file);
    out << result;
}

void WheelWidget::setNext(WheelWidget *next)
{
    next = next;
}

WheelWidget* WheelWidget::addWheel(QWidget* parent)
{
    if(next)
        return next->addWheel(parent);
    next = new WheelWidget(scene, wheelCreator.getContactAngle(), wheelCreator.getToothSpacing(), parent);
    if(!next)
        return 0;
    next->setContactAngle(wheelCreator.getContactAngle());
    next->setNumberOfTeeth(wheelCreator.getNumberOfTeeth());
    next->setToothSpacing(wheelCreator.getToothSpacing());
    next->setHoleDiameter(wheelCreator.getHoleRadius()*2);
    next->setPointResolution(wheelCreator.getPointResolution());
    next->setClearance(wheelCreator.getClearance());
    next->setNumberOfLighteningHoles(wheelCreator.getNumberOfLighteningHole());
    next->setArmWidth(wheelCreator.getArmWidth());
    next->setShowLineOfContact(showLineOfContact);
    next->setShowExternalCircle(showExternalCircle);
    next->setShowPrimitiveCircle(showPrimitiveCircle);
    next->previous = this;
    QObject::connect(next, SIGNAL(redraw()), this, SLOT(askForRedraw()));
    return next;
}

void WheelWidget::askForRedraw()
{
    if(previous)
        emit redraw();
    else
        WheelWidget::drawWheel();
}

void WheelWidget::setDrawing(bool st)
{
    this->drawing = st;
}

WheelWidget::~WheelWidget()
{
    delete ui;
}
