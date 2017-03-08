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

#include "wheelcreator.h"
namespace wheel {

WheelCreator::WheelCreator() : positionOffset(0,0)
{
    needToRecomputeHoles = true;
    needToRecomputeTooth = true;
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

float WheelCreator::getBaseRadius() const
{
    return this->primitiveRadius * cos(this->contactAngle);
}

std::vector<Point> WheelCreator::toothProfile() const
{
    std::vector<Point> result(0);
    float m = this->toothSpacing / PI;
    float r_base = this->primitiveRadius * cos(this->contactAngle);
    float r_foot = this->primitiveRadius - m;
    float tooth_domain_max = std::acos(r_base/this->externalRadius);
    float tooth_domain_min = (r_foot<r_base)?0:std::acos(r_base/r_foot);
    for(float u=0; u<this->pointResolution; u++)
    {
        float t = (1-u/this->pointResolution) * (-tooth_domain_max) + u/this->pointResolution * (-tooth_domain_min);
        result.push_back(Point(std::tan(t)-t,r_base / cos(t)));
    }
    return result;
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

std::vector<Point> WheelCreator::getPoints()
{
    std::vector<Point> result(0);
    if(needToRecomputeTooth)
    {
        tooth = WheelCreator::computeATooth(0);
        needToRecomputeTooth = false;
    }
    for(int begin=0; begin<this->numberOfTeeth; begin++)
    {
        for(Point p : tooth)
        {
            float x = p.y * cos(p.x + begin*2*PI/this->numberOfTeeth + this->rotationOffset) + this->positionOffset.x;
            float y = p.y * sin(p.x + begin*2*PI/this->numberOfTeeth + this->rotationOffset) + this->positionOffset.y;
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
    needToRecomputeHoles = true;
    needToRecomputeTooth = true;
}

void WheelCreator::setExternalRadius(float r)
{
    this->externalRadius = r;
    float m = this->toothSpacing / PI;
    this->primitiveRadius = r - m/2;
    this->numberOfTeeth = this->primitiveRadius * 2 * PI / this->toothSpacing;
    needToRecomputeHoles = true;
    needToRecomputeTooth = true;
}

void WheelCreator::setContactAngle(float alpha)
{
    this->contactAngle = alpha;
    WheelCreator::computeValues();
    needToRecomputeTooth = true;
}

void WheelCreator::setNumberOfTeeth(int z)
{
    this->numberOfTeeth = z;
    WheelCreator::computeValues();
    needToRecomputeHoles = true;
    needToRecomputeTooth = true;
}

void WheelCreator::setToothSpacing(float p)
{
    this->toothSpacing = p;
    WheelCreator::computeValues();
    needToRecomputeHoles = true;
    needToRecomputeTooth = true;
}

void WheelCreator::setHoleRadius(float r)
{
    this->holeRadius = r;
    needToRecomputeHoles = true;
}

void WheelCreator::setNumberOfLighteningHole(int n)
{
    this->numberOfLighteningHole = n;
    needToRecomputeHoles = true;
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
    needToRecomputeHoles = true;
    needToRecomputeTooth = true;
}

void WheelCreator::setRotationOffset(float alpha)
{
    this->rotationOffset = alpha;
}

void WheelCreator::syncWith(WheelCreator &wheel, bool stacked) const
{
    if (!stacked)
    {
        float d = this->primitiveRadius + wheel.primitiveRadius;
        float r = this->primitiveRadius/wheel.primitiveRadius;
        wheel.setPositionOffset(this->positionOffset.x+d, this->positionOffset.y);
        wheel.setRotationOffset((1+((this->numberOfTeeth)%2)/2+wheel.numberOfTeeth%2)*PI/wheel.numberOfTeeth - this->rotationOffset * r);
    }
    else
    {
        wheel.setPositionOffset(this->positionOffset);
        wheel.setRotationOffset(this->rotationOffset);
    }
}

void WheelCreator::setClearance(float c)
{
    this->clearance = c;
    needToRecomputeTooth = true;
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

std::vector<std::vector<Point>> WheelCreator::getLighteningHoles()
{
    if(needToRecomputeHoles)
    {
        hole = WheelCreator::computeAHole(0);
        needToRecomputeHoles = false;
    }
    std::vector<std::vector<Point>> result(0);
    for(int i=0; i<this->numberOfLighteningHole; i++)
    {
        std::vector<Point> thisHole(0);
        for(Point p : hole)
        {
            float x = p.y * cos(p.x + i*2*PI/this->numberOfLighteningHole + this->rotationOffset) + this->positionOffset.x;
            float y = p.y * sin(p.x + i*2*PI/this->numberOfLighteningHole + this->rotationOffset) + this->positionOffset.y;
            thisHole.push_back(Point(x,y));
        }
        result.push_back(thisHole);

    }
    return result;
}

void WheelCreator::setArmWidth(float w)
{
    this->armWidth = w;
    needToRecomputeHoles = true;
}

float WheelCreator::getArmWidth() const
{
    return this->armWidth;
}

QString WheelCreator::svg(Point offset, int i)
{
    float m = this->toothSpacing / PI;
    float r_base = this->primitiveRadius * cos(this->contactAngle);
    float r_foot = this->primitiveRadius - m;
    float tooth_domain_max = std::acos(r_base/this->externalRadius);
    float deport = (1- this->clearance)*(PI / this->numberOfTeeth / 2 + std::tan(this->contactAngle) - this->contactAngle);
    float top_max = deport - std::tan(tooth_domain_max) + tooth_domain_max;
    float r_ext = externalRadius;

    std::vector<Point> toothSide = WheelCreator::toothProfile();

    QString result = "";
    result.append("<g stroke=\"black\" stroke-width=\"0.5\" fill=\"none\" class=\"wheel\" id=\"wheel");
    result.append(QString::number(i+1));
    result.append("\" transform=\"translate(");
    result.append(QString::number(offset.x)).append(",");
    result.append(QString::number(offset.y)).append(")\">\n");

    // Tooth
    result.append("<g class=\"frame\">\n");
    float alpha = -top_max;
    result.append("<path d=\"M");
    result.append(QString::number(r_ext*std::cos(alpha))).append(",").append(QString::number(r_ext*std::sin(alpha))).append(" ");
    for(int n=0; n<numberOfTeeth; n++)
    {
        float alpha0 = alpha;
        alpha += 2*top_max;
        result.append("A").append(QString::number(r_ext)).append(",").append(QString::number(r_ext)).append(" ");
        result.append("0 0,1 ").append(QString::number(r_ext*cos(alpha))).append(",");
        result.append(QString::number(r_ext*sin(alpha))).append(" ");

        alpha += std::tan(tooth_domain_max) - tooth_domain_max;

        for(Point p : toothSide)
        {
            result.append("L");
            result.append(QString::number(p.y * std::cos(alpha + p.x))).append(",");
            result.append(QString::number(p.y * std::sin(alpha + p.x))).append(" ");
        }

        result.append("L").append(QString::number(r_foot*std::cos(alpha)));
        result.append(",").append(QString::number(r_foot*std::sin(alpha))).append(" ");
        alpha = alpha0 + 2*PI/numberOfTeeth - deport+top_max;
        result.append("A").append(QString::number(r_foot)).append(",").append(QString::number(r_foot)).append(" ");
        result.append("0 0,1").append(QString::number(r_foot*std::cos(alpha))).append(",");
        result.append(QString::number(r_foot*std::sin(alpha))).append(" ");

        for(int i=toothSide.size()-1; i >= 0; i--)
        {
            Point p = toothSide[i];
            result.append("L");
            result.append(QString::number(p.y * std::cos(alpha - p.x))).append(", ");
            result.append(QString::number(p.y * std::sin(alpha - p.x))).append(" ");
        }
        alpha = alpha0 + 2*PI/numberOfTeeth;
    }
    result.append("Z\"/>\n</g>\n");

    // Lightening holes
    r_ext = this->primitiveRadius - m - std::max(this->holeRadius*2.0, (double)this->armWidth);
    float r_int = std::max(this->holeRadius*2.0, this->armWidth * 1.25);
    float begin_ext = std::asin(this->armWidth/2/r_ext);
    float end_ext = 2*PI/this->numberOfLighteningHole - std::asin(this->armWidth/2/r_ext);
    float begin_int = std::asin(this->armWidth/2/r_int);
    float end_int = 2*PI/this->numberOfLighteningHole - std::asin(this->armWidth/2/r_int);
    result.append("<g class=\"lighteningHoles\">\n");
    alpha = 0;
    for(int i=0; i<numberOfLighteningHole; i++)
    {
        result.append("<path d=\"M");
        result.append(QString::number(r_int*std::cos(alpha+begin_int))).append(",");
        result.append(QString::number(r_int*std::sin(alpha+begin_int))).append(" ");
        result.append("A").append(QString::number(r_int)).append(",").append(QString::number(r_int)).append(" ");
        result.append("0 0,1 ").append(QString::number(r_int*std::cos(alpha+end_int))).append(",");
        result.append(QString::number(r_int*std::sin(alpha+end_int))).append(" ");

        result.append("L").append(QString::number(r_ext*std::cos(alpha+end_ext))).append(",");
        result.append(QString::number(r_ext*std::sin(alpha+end_ext))).append(" ");

        result.append("A").append(QString::number(r_ext)).append(",").append(QString::number(r_ext)).append(" ");
        result.append("0 0,0 ").append(QString::number(r_ext*std::cos(alpha+begin_ext))).append(",");
        result.append(QString::number(r_ext*std::sin(alpha+begin_ext))).append(" Z\"/>\n");

        alpha += 2*PI/numberOfLighteningHole;
    }
    result.append("</g>\n");

    // Center hole
    result.append("<circle cx=\"0\" cy=\"0\" r=\"").append(QString::number(holeRadius)).append("\"/>\n");

    result.append("<path d=\"M");
    result.append(QString::number(-1.5*holeRadius)).append(",0 H");
    result.append(QString::number(+1.5*holeRadius)).append(" M0,");
    result.append(QString::number(-1.5*holeRadius)).append(" V");
    result.append(QString::number(+1.5*holeRadius)).append("\" />\n");

    result.append("</g>\n");
    return result;
}

}
