#ifndef WHEELCREATOR_H
#define WHEELCREATOR_H

#define PI 3.1415

#include <iostream>
#include <cmath>
#include <vector>

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

    std::vector<Point> computeATooth(float begin) const;
    std::vector<Point> getPoints() const;

    void computeValues();

    void syncWith(WheelCreator& wheel) const;

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
};

}
#endif // WHEELCREATOR_H
