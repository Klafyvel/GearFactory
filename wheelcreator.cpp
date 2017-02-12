#include "wheelcreator.h"
namespace wheel {

WheelCreator::WheelCreator() : positionOffset(0,0)
{

}

float WheelCreator::getPrimitiveRadius() const
{
    return this->primitiveRadius;
}

float WheelCreator::getExternalRadius() const
{
    return this->externalRadius;
}

float WheelCreator::getContactAngle() const
{
    return this->contactAngle;
}

int WheelCreator::getNumberOfTeeth() const
{
    return this->numberOfTeeth;
}

float WheelCreator::getToothSpacing() const
{
    return this->toothSpacing;
}

float WheelCreator::getHoleRadius() const
{
    return this->holeRadius;
}

int WheelCreator::getNumberOfLighteningHole() const
{
    return this->numberOfLighteningHole;
}

Point WheelCreator::getPositionOffset() const
{
    return this->positionOffset;
}

float WheelCreator::getRotationOffset() const
{
    return this->rotationOffset;
}

std::vector<Point> WheelCreator::computeATooth(float begin) const
{
    std::vector<Point>result(0);

    float m = this->toothSpacing / PI;
    float r_base = this->primitiveRadius * cos(this->contactAngle);
    float r_foot = this->externalRadius - 2.5/2 * m;
    float tooth_domain_max = std::acos(r_base/this->externalRadius);
    float tooth_domain_min = (r_foot<r_base)?0:std::acos(r_base/r_foot);
    float deport = PI / this->numberOfTeeth / 2 + std::tan(this->contactAngle) - this->contactAngle; //- std::tan(tooth_domain_min) + tooth_domain_min ;
    float top_max = begin+deport - std::tan(tooth_domain_max) + tooth_domain_max;

    for(float u=0; u<this->pointResolution; u++)
    {
        float t = (1-u/this->pointResolution) * begin + u/this->pointResolution * top_max;
        float x = this->positionOffset.x + this->externalRadius*cos(t);
        float y = this->positionOffset.y + this->externalRadius*sin(t);
        result.push_back(Point(x,y));
    }

    for(float u=0; u<this->pointResolution; u++)
    {
        float t = (1-u/this->pointResolution) * (-tooth_domain_max) + u/this->pointResolution * (-tooth_domain_min);
        float x = r_base / cos(t) * cos(begin+deport+std::tan(t)-t) + this->positionOffset.x;
        float y = r_base / cos(t) * sin(begin+deport+std::tan(t)-t) + this->positionOffset.y;
        result.push_back(Point(x,y));
    }

    for(float u=0; u<this->pointResolution; u++)
    {
        float t = (1-u/this->pointResolution) * (begin+deport) + u/this->pointResolution * (begin+2*PI/this->numberOfTeeth - deport);
        float x = r_foot * cos(t) + this->positionOffset.x;
        float y = r_foot * sin(t) + this->positionOffset.y;
        result.push_back(Point(x,y));
    }

    for(float u=0; u<this->pointResolution; u++)
    {
        float t = u/this->pointResolution * tooth_domain_max + (1-u/this->pointResolution) * tooth_domain_min;
        float x = r_base / cos(t) * cos(begin-deport+2*PI/this->numberOfTeeth+std::tan(t)-t) + this->positionOffset.x;
        float y = r_base / cos(t) * sin(begin-deport+2*PI/this->numberOfTeeth+std::tan(t)-t) + this->positionOffset.y;
        result.push_back(Point(x,y));
    }

    for(float u=0; u<this->pointResolution; u++)
    {
        float t = (1-u/this->pointResolution) * (begin+2*PI/this->numberOfTeeth-deport+std::tan(tooth_domain_max)-tooth_domain_max) + u/this->pointResolution * (begin+2*PI/this->numberOfTeeth);
        float x = this->positionOffset.x + this->externalRadius*cos(t);
        float y = this->positionOffset.y + this->externalRadius*sin(t);
        result.push_back(Point(x,y));
    }
    return result;
}

std::vector<Point> WheelCreator::getPoints() const
{
    std::vector<Point> result(0);
    for(int begin=0; begin<this->numberOfTeeth; begin++)
    {
        std::vector<Point> tooth = WheelCreator::computeATooth(begin*2*PI/this->numberOfTeeth+this->rotationOffset);
        result.insert(result.end(),tooth.begin(), tooth.end());
    }
    return result;
}

int WheelCreator::getPointResolution() const
{
    return this->pointResolution;
}

void WheelCreator::computeValues()
{
    this->primitiveRadius = this->numberOfTeeth * this->toothSpacing / PI / 2;
    float m = this->toothSpacing / PI;
    this->externalRadius = this->primitiveRadius + m / 2;
}

void WheelCreator::setPrimitiveRadius(float r)
{
    float m = this->toothSpacing / PI;
    this->primitiveRadius = r;
    this->numberOfTeeth = r * 2 * PI / this->toothSpacing;
    this->externalRadius = this->primitiveRadius + m/2;
}

void WheelCreator::setExternalRadius(float r)
{
    this->externalRadius = r;
    float m = this->toothSpacing / PI;
    this->primitiveRadius = r - m/2;
    this->numberOfTeeth = this->primitiveRadius * 2 * PI / this->toothSpacing;
}

void WheelCreator::setContactAngle(float alpha)
{
    this->contactAngle = alpha;
    WheelCreator::computeValues();
}

void WheelCreator::setNumberOfTeeth(int z)
{
    this->numberOfTeeth = z;
    WheelCreator::computeValues();
}

void WheelCreator::setToothSpacing(float p)
{
    this->toothSpacing = p;
    WheelCreator::computeValues();
}

void WheelCreator::setHoleRadius(float r)
{
    this->holeRadius = r;
}

void WheelCreator::setNumberOfLighteningHole(int n)
{
    this->numberOfLighteningHole = n;
}

void WheelCreator::setPositionOffset(float x, float y)
{
    WheelCreator::setPositionOffset(Point(x,y));
}

void WheelCreator::setPositionOffset(Point p)
{
    this->positionOffset = p;
}

void WheelCreator::setPointResolution(int n)
{
    this->pointResolution = n;
}

void WheelCreator::setRotationOffset(float alpha)
{
    this->rotationOffset = alpha;
}

void WheelCreator::syncWith(WheelCreator &wheel) const
{
    float d = this->primitiveRadius + wheel.primitiveRadius;
    float r = this->primitiveRadius/wheel.primitiveRadius;
    wheel.setPositionOffset(this->positionOffset.x+d, this->positionOffset.y);
    wheel.setRotationOffset((float)(this->numberOfTeeth-wheel.numberOfTeeth+1)*PI/wheel.numberOfTeeth - this->rotationOffset * r);
}
}
