#include "Vector2D.h"
using namespace std;

//Constructors

Vector2D::Vector2D(){
    x = 0;
    y = 0;
}

Vector2D::Vector2D(double in_x, double in_y){
    x = in_x;
    y = in_y;
}

// Overloaded Operators

Vector2D Vector2D::operator*(double d){
    return Vector2D(x*d, y*d);
}
Vector2D Vector2D::operator/(double d){
    return Vector2D(x/d, y/d);
}

ostream &operator<<(ostream &out, const Vector2D &v) {
    out << "<" << v.x << ", " << v.y << ">";
    return out;
}