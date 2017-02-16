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

#ifndef WHEELWIDGET_H
#define WHEELWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QTabWidget>

#include "wheelcreator.h"
#include "exportdialog.h"

namespace Ui {
class WheelWidget;
}

class WheelWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WheelWidget(QGraphicsScene* scene, QTabWidget* tab=0, int i=0, float contactAngle=0, float toothSpacing=1, QWidget *parent = 0);
    ~WheelWidget();
    void exportSVG(QString filename);
    void incrI();
    void decrI();

    QRect boundingRect() const;
    QRect fullSize() const;

signals:
    void redraw();

public slots:
    void addStackedWheel();
    void addWheel();
    void delWheel();
    void askForRedraw();
    void drawWheel();
    void connectGui();
    void disconnectGui();

    void refreshGearsValues();

    void setPrimitiveDiameter(double d);
    void setExternalDiameter(double d);
    void setNumberOfTeeth(int z);
    void setHoleDiameter(double d);
    void setNumberOfLighteningHoles(int n);
    void setArmWidth(double w);
    void setPointResolution(int n);
    void setToothSpacing(double p);
    void setContactAngle(double alpha);
    void setClearance(double c);
    void setRotation(double alpha);
    void rotate(double step);
    void setDrawing(bool st);
    void setStacked(int i);

    void exportWheel();

    void setShowLineOfContact(bool st);
    void setShowPrimitiveCircle(bool st);
    void setShowExternalCircle(bool st);
    void setNext(WheelWidget* next);

    void setFocusPen();
    void setNoFocusPen();
    void setFocusPen(int i);

private:
    Ui::WheelWidget *ui;
    WheelWidget* next;
    WheelWidget* previous;
    QGraphicsScene* scene;

    wheel::WheelCreator wheelCreator;

    bool showLineOfContact;
    bool showPrimitiveCircle;
    bool showExternalCircle;

    int i;
    QTabWidget* tab;

    bool drawing;

    QPen currentPen;
};

#endif // WHEELWIDGET_H
