#ifndef MATHLIB_H
#define MATHLIB_H

class Point2D {
public:
    Point2D();
    Point2D(double inX, double inY);

    double distanceTo(Point2D other);
    double fastDistanceTo(Point2D other);
    double getX();
    double getY();
    static double dot(Point2D a, Point2D b);
    void setPos(double inX, double inY);

private:
    double mX;
    double mY;
};

class Point3D {
public:
    Point3D();
    Point3D(double inX, double inY, double inZ);

    double distanceTo(Point3D other);
    double fastDistanceTo(Point3D other);
    double getX();
    double getY();
    double getZ();
    Point3D normalize();
    Point3D across(Point3D other);
    void setPos(double inX, double inY, double inZ);
    double mX;
    double mY;
    double mZ;
};

class Porabola {
public:
    Porabola(Point3D startingPoint, Point3D endingPoint);

    double getVolocity(double x, double y, double z);

private:
    double const a;
    double const b;//b is coefficient
    Point3D startingPoint;
    Point3D endingPoint;
};
#endif

