#ifndef POINT2D_H
#define POINT2D_H
#include <cmath>
#include <iostream>
#include "Vector2D.h"
using namespace std;
class Point2D
{
public:
    double x;
    double y;

    Point2D();
    Point2D(double in_x, double in_y);

    static double GetDistanceBetweenPoints(Point2D p1, Point2D p2);

    // Overloaded operators
    friend ostream &operator<<(ostream &out, const Point2D &p);
    Point2D operator+(Vector2D);
    Vector2D operator-(const Point2D& p2) const;
};
#endif