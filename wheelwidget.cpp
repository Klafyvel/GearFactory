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

#include "wheelwidget.h"
#include "ui_wheelwidget.h"

WheelWidget::WheelWidget(QGraphicsScene *scene, QTabWidget* tab, int i, float contactAngle, float toothSpacing, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WheelWidget)
{
    ui->setupUi(this);
    this->i = i;

    ui->deletePushButton->setEnabled(false);
    ui->stackedCheckBox->setEnabled(false);
    this->scene = scene;
    next = 0;
    previous = 0;
    wheelCreator.setContactAngle(contactAngle);
    wheelCreator.setToothSpacing(toothSpacing);
    wheelCreator.setNumberOfTeeth(3);
    wheelCreator.setNumberOfLighteningHole(0);
    drawing = true;
    this->tab = tab;
    currentPen = QPen();
    WheelWidget::setNoFocusPen();
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
    QObject::connect(ui->addPushButton, SIGNAL(clicked()), this, SLOT(addWheel()));
    if(previous)
        QObject::connect(ui->deletePushButton, SIGNAL(clicked()), previous, SLOT(delWheel()));
    QObject::connect(tab, SIGNAL(currentChanged(int)), this, SLOT(setFocusPen(int)));
    QObject::connect(ui->addStackedPushButton, SIGNAL(clicked()), this, SLOT(addStackedWheel()));
    QObject::connect(ui->stackedCheckBox, SIGNAL(stateChanged(int)), this, SLOT(setStacked(int)));
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
    QObject::disconnect(ui->addPushButton, SIGNAL(clicked()), this, SLOT(addWheel()));
    if(previous)
        QObject::disconnect(ui->deletePushButton, SIGNAL(clicked()), previous, SLOT(delWheel()));
    QObject::disconnect(tab, SIGNAL(currentChanged(int)), this, SLOT(setFocusPen(int)));
    QObject::disconnect(ui->addStackedPushButton, SIGNAL(clicked()), this, SLOT(addStackedWheel()));
    QObject::disconnect(ui->stackedCheckBox, SIGNAL(stateChanged(int)), this, SLOT(setStacked(int)));
}

void WheelWidget::drawWheel()
{
    if(!previous)
        scene->clear();
    if(next)
        next->drawWheel();
    wheel::Point center = wheelCreator.getPositionOffset();
    if(showLineOfContact)
    {
        float a = 1/std::tan(wheelCreator.getContactAngle());
        float b = wheelCreator.getPrimitiveRadius();

        scene->addLine(3.0*b/4+center.x,a*(3.0*b/4-b)+center.y,5.0/4*b+center.x,a*(5.0*b/4-b)+center.y, currentPen);
    }

    std::vector<wheel::Point> points = wheelCreator.getPoints();
    wheel::Point current = points[0];
    for(wheel::Point next : points)
    {
        scene->addLine(current.x, current.y, next.x, next.y, currentPen);
        current = next;
    }

    float r = wheelCreator.getHoleRadius();
    float cross_size = r*1.5;
    scene->addLine(center.x-cross_size,center.y,center.x+cross_size,center.y, currentPen);
    scene->addLine(center.x,center.y-cross_size,center.x,center.y+cross_size, currentPen);
    scene->addEllipse(center.x - r, center.y - r, r*2, r*2, currentPen);

    std::vector<std::vector<wheel::Point>> holes = wheelCreator.getLighteningHoles();
    for(std::vector<wheel::Point> points : holes)
    {
        current = points[0];
        for(wheel::Point next : points)
        {
            scene->addLine(current.x, current.y, next.x, next.y, currentPen);
            current = next;
        }
    }

    r = wheelCreator.getPrimitiveRadius();
    if(showPrimitiveCircle)
        scene->addEllipse(center.x - r, center.y - r, r*2, r*2, currentPen);
    r = wheelCreator.getExternalRadius();
    if(showExternalCircle)
        scene->addEllipse(center.x - r, center.y - r, r*2, r*2, currentPen);
}

void WheelWidget::setStacked(int i)
{
    WheelWidget::refreshGearsValues();
    if(previous)
    {
        previous->setNumberOfLighteningHoles(0);
    }
    WheelWidget::setNumberOfLighteningHoles(0);
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
    if(previous)
        previous->wheelCreator.syncWith(wheelCreator, ui->stackedCheckBox->isChecked());
    if(next)
    {
        wheelCreator.syncWith(next->wheelCreator, next->ui->stackedCheckBox->isChecked());
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
        previous->wheelCreator.syncWith(wheelCreator, ui->stackedCheckBox->isChecked());
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
    result.append(QString::number(2*r_ext+5));
    result.append("mm\" height=\"");
    result.append(QString::number(2*r_ext+10));
    result.append("mm\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\"");
    result.append(" version=\"1.2\" baseProfile=\"tiny\" viewBox=\"0 0 ");
    result.append(QString::number(2*r_ext+5)).append(" ");
    result.append(QString::number(2*r_ext+10)).append("\" >\n");
    result.append("<title>Gear factory</title>\n");
    result.append("<desc>File created by Gear Factory.</desc>\n");

    result.append(wheelCreator.svg(wheel::Point(r_ext+2.5, r_ext+2.5),i));

    result.append("<g transform=\"translate(2.5,").append(QString::number(2*r_ext+7));
    result.append(")\" style=\"font-size:1mm\" class=\"wheelDescr\" id=\"descrWheel").append(QString::number(i)).append("\">\n");
    result.append("<text>Tooth spacing : ").append(QString::number(wheelCreator.getToothSpacing())).append("mm ");
    result.append("Primitive radius : ").append(QString::number(wheelCreator.getPrimitiveRadius())).append("mm ");
    result.append("External radius : ").append(QString::number(wheelCreator.getExternalRadius())).append("mm ");
    result.append("Number of teeth : ").append(QString::number(wheelCreator.getNumberOfTeeth())).append("</text>\n");
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

void WheelWidget::addStackedWheel()
{
    WheelWidget::addWheel();
    next->ui->stackedCheckBox->setChecked(true);
    WheelWidget::refreshGearsValues();
}

void WheelWidget::addWheel()
{
    WheelWidget* new_next = next;
    next = new WheelWidget(scene, tab, i, wheelCreator.getContactAngle(), wheelCreator.getToothSpacing());
    if(!next)
        return;
    next->ui->deletePushButton->setEnabled(true);
    next->ui->stackedCheckBox->setEnabled(true);
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
    next->tab = tab;
    next->next = new_next;
    if(next->next)
    {
        next->next->previous = next;
        QObject::disconnect(next->next->ui->deletePushButton, SIGNAL(clicked()), this, SLOT(delWheel()));
        QObject::connect(next->next->ui->deletePushButton, SIGNAL(clicked()), next, SLOT(delWheel()));

        QObject::disconnect(next->next, SIGNAL(redraw()), this, SLOT(askForRedraw()));
        QObject::connect(next->next, SIGNAL(redraw()), next, SLOT(askForRedraw()));
    }
    tab->insertTab(i+1, next, QString("Wheel ").append(QString::number(i+2)));
    next->incrI();
    next->ui->stackedCheckBox->setChecked(false);
    WheelWidget::refreshGearsValues();
    QObject::connect(next, SIGNAL(redraw()), this, SLOT(askForRedraw()));
    QObject::connect(next->ui->deletePushButton, SIGNAL(clicked()), this, SLOT(delWheel()));
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

void WheelWidget::incrI()
{
    i ++;
    if(tab)
        tab->setTabText(i, QString("Wheel ").append(QString::number(i+1)));
    if(next)
        next->incrI();

}

void WheelWidget::decrI()
{
    i --;
    if(tab)
        tab->setTabText(i+1, QString("Wheel ").append(QString::number(i+1)));
    if(next)
        next->decrI();

}

void WheelWidget::delWheel()
{
    if(!next)
        return;
    WheelWidget* toBeDeleted = next;
    next = next->next;
    toBeDeleted->next = 0;
    if(next)
    {
        next->ui->stackedCheckBox->setChecked(false);
        QObject::connect(next->ui->deletePushButton,SIGNAL(clicked()), this, SLOT(delWheel()));
        QObject::connect(next,SIGNAL(redraw()), this, SLOT(askForRedraw()));
        next->previous = this;
        next->decrI();
    }
    delete toBeDeleted;
    WheelWidget::refreshGearsValues();
}

WheelWidget::~WheelWidget()
{
    delete ui;
}

void WheelWidget::setFocusPen()
{
    currentPen.setColor(QColor(0,0,0));
    currentPen.setWidth(1);
}

void WheelWidget::setNoFocusPen()
{
    currentPen.setColor(QColor(0,100,150));
    currentPen.setWidth(0);
}

void WheelWidget::setFocusPen(int i)
{
    if(i == this->i)
        WheelWidget::setFocusPen();
    else
        WheelWidget::setNoFocusPen();
    emit redraw();
}

QRect WheelWidget::boundingRect() const
{
    wheel::Point p = wheelCreator.getPositionOffset();
    float r = wheelCreator.getExternalRadius();
    return QRect(p.x-r, p.y-r, 2*r,2*r);
}

QRect WheelWidget::fullSize() const
{
    QRect res(0,0,0,0);
    float r = wheelCreator.getExternalRadius();
    res.setWidth(2*r);
    res.setHeight(2*r);
    if(next)
    {
        QRect nextRes = next->fullSize();
        if(ui->stackedCheckBox->isChecked())
        {
            res.setWidth(nextRes.width());
        }
        else
            res.setWidth(res.width() + nextRes.width());
        res.setHeight(std::max(nextRes.height(), res.height()));
    }
    return res;
}
