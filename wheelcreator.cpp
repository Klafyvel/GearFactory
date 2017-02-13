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

float WheelCreator::getClearance() const
{
    return this->clearance;
}

std::vector<Point> WheelCreator::computeATooth(float begin) const
{
    std::vector<Point>result(0);

    float m = this->toothSpacing / PI;
    float r_base = this->primitiveRadius * cos(this->contactAngle);
    float r_foot = this->primitiveRadius - m;
    float tooth_domain_max = std::acos(r_base/this->externalRadius);
    float tooth_domain_min = (r_foot<r_base)?0:std::acos(r_base/r_foot);
    float deport = (1- this->clearance)*(PI / this->numberOfTeeth / 2 + std::tan(this->contactAngle) - this->contactAngle);
    float top_max = begin+deport - std::tan(tooth_domain_max) + tooth_domain_max;

    for(float u=0; u<this->pointResolution; u++)
    {
        float t = (1-u/this->pointResolution) * begin + u/this->pointResolution * top_max;
        result.push_back(Point(t,this->externalRadius));
    }

    for(float u=0; u<this->pointResolution; u++)
    {
        float t = (1-u/this->pointResolution) * (-tooth_domain_max) + u/this->pointResolution * (-tooth_domain_min);
        result.push_back(Point(begin+deport+std::tan(t)-t,r_base / cos(t)));
    }

    for(float u=0; u<this->pointResolution; u++)
    {
        float t = (1-u/this->pointResolution) * (begin+deport) + u/this->pointResolution * (begin+2*PI/this->numberOfTeeth - deport);
        result.push_back(Point(t,r_foot));
    }

    for(float u=0; u<this->pointResolution; u++)
    {
        float t = u/this->pointResolution * tooth_domain_max + (1-u/this->pointResolution) * tooth_domain_min;
        result.push_back(Point(begin-deport+2*PI/this->numberOfTeeth+std::tan(t)-t,r_base / cos(t)));
    }

    for(float u=0; u<this->pointResolution; u++)
    {
        float t = (1-u/this->pointResolution) * (begin+2*PI/this->numberOfTeeth-deport+std::tan(tooth_domain_max)-tooth_domain_max) + u/this->pointResolution * (begin+2*PI/this->numberOfTeeth);
        result.push_back(Point(t,this->externalRadius));
    }
    return result;
}

std::vector<Point> WheelCreator::getPoints() const
{
    std::vector<Point> result(0);
    std::vector<Point> tooth = WheelCreator::computeATooth(this->rotationOffset);
    for(int begin=0; begin<this->numberOfTeeth; begin++)
    {
        for(Point p : tooth)
        {
            float x = p.y * cos(p.x + begin*2*PI/this->numberOfTeeth) + this->positionOffset.x;
            float y = p.y * sin(p.x + begin*2*PI/this->numberOfTeeth) + this->positionOffset.y;
            result.push_back(Point(x,y));
        }
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
    this->externalRadius = this->primitiveRadius + m/2;
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
    wheel.setRotationOffset((1+((this->numberOfTeeth)%2)/2+wheel.numberOfTeeth%2)*PI/wheel.numberOfTeeth - this->rotationOffset * r);
}

void WheelCreator::setClearance(float c)
{
    this->clearance = c;
}

std::vector<Point> WheelCreator::computeAHole(float begin) const
{
    float m = this->toothSpacing / PI;
    float r_ext = this->primitiveRadius - m - std::max(this->holeRadius*2.0, (double)this->armWidth);
    float r_int = std::max(this->holeRadius*2.0, this->armWidth * 1.25);
    float begin_ext = begin + std::asin(this->armWidth/2/r_ext);
    float end_ext = begin + 2*PI/this->numberOfLighteningHole - std::asin(this->armWidth/2/r_ext);
    float begin_int = begin + std::asin(this->armWidth/2/r_int);
    float end_int = begin + 2*PI/this->numberOfLighteningHole - std::asin(this->armWidth/2/r_int);

    std::vector<Point> result(0);
    float res = 3*this->pointResolution;
    for(float u=0; u<res; u++)
    {
        float t = u/res * end_int + (1-u/res) * begin_int;
        result.push_back(Point(t,r_int));
    }
    for(float u=0; u<res; u++)
    {
        float t = u/res * begin_ext + (1-u/res) * end_ext;
        result.push_back(Point(t,r_ext));
    }

    result.push_back(Point(begin_int,r_int));
    return result;
}

std::vector<std::vector<Point>> WheelCreator::getLighteningHoles() const
{
    std::vector<Point> hole = WheelCreator::computeAHole(this->rotationOffset);
    std::vector<std::vector<Point>> result(0);
    for(int i=0; i<this->numberOfLighteningHole; i++)
    {
        std::vector<Point> thisHole(0);
        for(Point p : hole)
        {
            float x = p.y * cos(p.x + i*2*PI/this->numberOfLighteningHole) + this->positionOffset.x;
            float y = p.y * sin(p.x + i*2*PI/this->numberOfLighteningHole) + this->positionOffset.y;
            thisHole.push_back(Point(x,y));
        }
        result.push_back(thisHole);

    }
    return result;
}

void WheelCreator::setArmWidth(float w)
{
    this->armWidth = w;
}

float WheelCreator::getArmWidth() const
{
    return this->armWidth;
}

}
