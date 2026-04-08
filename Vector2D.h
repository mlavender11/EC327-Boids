#ifndef VECTOR2D_H
#define VECTOR2D_H
#include <iostream>
using namespace std;

class Vector2D
{
public:
    // Public Members
    double x;
    double y;

    // Constructors
    Vector2D();
    Vector2D(double in_x, double in_y);

    // Overloaded Operators
    Vector2D operator*(double d);
    Vector2D operator/(double d);
    friend ostream &operator<<(ostream &out, const Vector2D &v);
};

#endif