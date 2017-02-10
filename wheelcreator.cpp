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

std::vector<Point> WheelCreator::computeATooth(float begin) const
{
    std::vector<Point>result(0);

    float m = this->toothSpacing / PI;
    float deport = PI / this->numberOfTeeth / 2 + std::tan(this->contactAngle) - this->contactAngle;
    float r_base = this->primitiveRadius * cos(this->contactAngle);
    float r_foot = this->externalRadius - 2.5/2 * m;
    float tooth_domain_max = std::acos(r_base/this->externalRadius);
    float top_max = begin+deport - std::tan(tooth_domain_max) + tooth_domain_max;

    for(float u=0; u<POINT_RES; u++)
    {
        float t = (1-u/POINT_RES) * begin + u/POINT_RES * top_max;
        float x = this->positionOffset.x + this->externalRadius*cos(t);
        float y = this->positionOffset.y + this->externalRadius*sin(t);
        result.push_back(Point(x,y));
    }

    for(float u=0; u<POINT_RES; u++)
    {
        float t = (1-u/POINT_RES) * (-tooth_domain_max);
        float x = r_base / cos(t) * cos(begin+deport+std::tan(t)-t) + this->positionOffset.x;
        float y = r_base / cos(t) * sin(begin+deport+std::tan(t)-t) + this->positionOffset.y;
        result.push_back(Point(x,y));
    }

    for(float u=0; u<POINT_RES; u++)
    {
        float t = (1-u/POINT_RES) * (begin+deport) + u/POINT_RES * (begin+2*PI/this->numberOfTeeth - deport);
        float x = r_foot * cos(t) + this->positionOffset.x;
        float y = r_foot * sin(t) + this->positionOffset.y;
        result.push_back(Point(x,y));
    }

    for(float u=0; u<POINT_RES; u++)
    {
        float t = u/POINT_RES * tooth_domain_max;
        float x = r_base / cos(t) * cos(begin-deport+2*PI/this->numberOfTeeth+std::tan(t)-t) + this->positionOffset.x;
        float y = r_base / cos(t) * sin(begin-deport+2*PI/this->numberOfTeeth+std::tan(t)-t) + this->positionOffset.y;
        result.push_back(Point(x,y));
    }

    for(float u=0; u<POINT_RES; u++)
    {
        float t = (1-u/POINT_RES) * (begin+2*PI/this->numberOfTeeth-deport+std::tan(tooth_domain_max)-tooth_domain_max) + u/POINT_RES * (begin+2*PI/this->numberOfTeeth);
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
        std::vector<Point> tooth = WheelCreator::computeATooth(begin*2*PI/this->numberOfTeeth);
        result.insert(result.end(),tooth.begin(), tooth.end());
    }
    return result;
}

void WheelCreator::setContactAngle(float alpha)
{
    this->contactAngle = alpha;
}

void WheelCreator::setNumberOfTeeth(int z)
{
    this->numberOfTeeth = z;
}

void WheelCreator::setToothSpacing(float p)
{
    this->toothSpacing = p;
}

void WheelCreator::computeValues()
{
    this->primitiveRadius = this->numberOfTeeth * this->toothSpacing / PI / 2;
    float m = this->toothSpacing / PI;
    this->externalRadius = this->primitiveRadius + m / 2;
}
}
