#include "Point2D.h"
using namespace std;

Point2D::Point2D() {
    x = 0;
    y = 0;
}

Point2D::Point2D(double in_x, double in_y)
{
    x = in_x;
    y = in_y;
}

double Point2D::GetDistanceBetweenPoints(Point2D p1, Point2D p2)
{
    double distance;
    double delX = p2.x - p1.x;
    double delY = p2.y - p1.y;

    distance = sqrt(delX * delX + delY * delY);

    return distance;
}

ostream &operator<<(ostream &out, const Point2D &p)
{
    out << "(" << p.x << ", " << p.y << ")";
    return out;
}

Point2D Point2D::operator+(Vector2D v)
{
    return Point2D(x + v.x, y + v.y);
}

Vector2D Point2D::operator-(Point2D p2)
{
    return Vector2D(x - p2.x, y - p2.y);
}
