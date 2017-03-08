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

#ifndef WHEELCREATOR_H
#define WHEELCREATOR_H

#define PI 3.1415

#include <iostream>
#include <cmath>
#include <vector>
#include <QString>

namespace wheel {

const int POINT_RES = 100;

typedef struct Point
{
    Point() : x(0), y(0) {}
    Point(float x, float y) : x(x), y(y) {}
    float x;
    float y;
} Point;


class WheelCreator
{
public:
    WheelCreator();

    std::vector<Point> toothProfile() const;
    std::vector<Point> computeATooth(float begin) const;
    std::vector<Point> getPoints();
    std::vector<Point> computeAHole(float begin) const;
    std::vector<std::vector<Point>> getLighteningHoles();
    QString svg(Point offset, int i=0);

    void computeValues();

    void syncWith(WheelCreator& wheel, bool stacked=false) const;

    // Getters
    float getPrimitiveRadius() const;
    float getExternalRadius() const;
    float getContactAngle() const;
    int getNumberOfTeeth() const;
    float getToothSpacing() const;
    float getHoleRadius() const;
    int getNumberOfLighteningHole() const;
    Point getPositionOffset() const;
    int getPointResolution() const;
    float getRotationOffset() const;
    float getClearance() const;
    float getArmWidth() const;
    float getBaseRadius() const;

    // Setters
    void setPrimitiveRadius(float r);
    void setExternalRadius(float r);
    void setContactAngle(float alpha);
    void setNumberOfTeeth(int z);
    void setToothSpacing(float p);
    void setHoleRadius(float r);
    void setNumberOfLighteningHole(int n);
    void setPositionOffset(Point p);
    void setPositionOffset(float x, float y);
    void setPointResolution(int n);
    void setRotationOffset(float alpha);
    void setClearance(float c);
    void setArmWidth(float w);

protected:
    float primitiveRadius;
    float externalRadius;
    float contactAngle;
    int numberOfTeeth;
    float toothSpacing;
    float holeRadius;
    int numberOfLighteningHole;
    Point positionOffset;
    float rotationOffset;
    int pointResolution;
    float clearance;
    float armWidth;
    bool needToRecomputeTooth;
    bool needToRecomputeHoles;
    std::vector<Point> hole;
    std::vector<Point> tooth;
};

}
#endif // WHEELCREATOR_H
