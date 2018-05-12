#include <iostream>
#include "GLBall.h"
#include "NBodyScene.h"
#include <math.h>
#define dt 0.0001
#define cor0(x) ((((x) < 1e-50) && ((x) > -1e-50)) ? 0 : (x))
#include <fstream>

using namespace std;

class Vector
{
public:
Vector(){}

virtual ~Vector(){}

Vector(double a, double b)
{
    x = a;
    y = b;
}

double get_X() const
{
    return x;
}

double get_Y() const
{
    return y;
}

void setX(double x)
{
    this->x = x;
}

void setY(double y)
{
    this->y = y;
}

Vector operator+ (const Vector& v)
{
    double a = this->get_X() + v.x;
    double b = this->get_Y() + v.y;
    return Vector(a, b);
}

Vector operator- (const Vector& v)
{
    double a = this->get_X() - v.x;
    double b = this->get_Y() - v.y;
    return Vector(a, b);
}

Vector operator* (const double f)
{
    double a = (this->get_X()) * f;
    double b = (this->get_Y()) * f;
    return Vector(a, b);
}

Vector operator/ (const double f)
{
    double a = (this->get_X()) / f;
    double b = (this->get_Y()) / f;
    return Vector(a, b);
}

private:
    double x, y;
};

Vector operator* (double f, Vector& v)
{
    double a = f * v.get_X();
    double b = f * v.get_Y();
    return Vector(a, b);
}

Vector operator/ (double f, Vector& v)
{
    double a = f / v.get_X();
    double b = f / v.get_Y();
    return Vector(a, b);
}

istream& operator>>(istream &is, Vector &v)
{
    int a, b;
    is >> a >> b;
    v.setX(a);
    v.setY(b);
    return is;
}

ostream& operator<< (ostream &os, Vector &v)
{
    os << v.get_X() << " " <<v.get_Y();
    return os;
}

class Body : public GLBall
{
public:
Body() {}

~Body() {}

Body(double a, double b, Vector c, double d)
{
    x = a;
    y = b;
    speed = c;
    m = d;
}

void Step (Body* Array, int n)
{
    Vector accel = this->ResultantForce(Array, n) / this->m;
    this->speed = this->speed + accel * dt;
    this->x = this->x + this->speed.get_X() * dt;
    this->y = this->y + this->speed.get_Y() * dt;
}

double getX() const
{
    return this->x;
}

double getY() const
{
    return this->y;
}

Vector getspeed() const
{
    return this->speed;
}

double getmass() const
{
    return this->m;
}

double getR () const
{
    return pow(this->m, 1/3);
}

void setx (double X)
{
    x = X;
}

void sety(double Y)
{
    y = Y;
}

void setspeed(Vector s)
{
    speed = s;
}

void setmass(double M)
{
    m = M;
}

private:

double distance (Body* B)
{
    return sqrt((this->x - B->x)*(this->x - B->x) + (this->y - B->y)*(this->y - B->y));
}

Vector Force (Body* B)
{
    Vector a;
    if ((this->distance(B)) > 0.00001)
    {
        a.setX(B->x - this->x);
        a.setY(B->y - this->y);
        a = this->m * a;
        a = B->m * a;
        a = a / ((this->distance(B))*(this->distance(B)));
    }
    else
    {
        a.setX(0);
        a.setY(0);
    }
    return a;
}

Vector ResultantForce (Body* Array, int n)
{
    Vector a;
    a.setX(0);
    a.setY(0);
    for (int i = 0; i < n; i++)
    {
        a = a + this->Force(&Array[i]);
    }
    return a;
}

double x;
double y;
Vector speed;
double m;
};

istream& operator>>(istream &is, Body &B)
{
    double x, y, m;
    Vector s;
    is >> x >> y >> s >> m;
    B.setx(x);
    B.sety(y);
    B.setspeed(s);
    B.setmass(m);
    return is;
}

ostream& operator<<(ostream &os,const Body &B)
{
    os << cor0(B.getX()) << "\t" << cor0(B.getY()) << "\t" << cor0(B.getspeed().get_X()) << "\t" << cor0(B.getspeed().get_Y()) << "\t" << cor0(B.getmass()) << "\t";
    return os;
}

class SampleScene : public NBodyScene
{
protected:
    int n;
    Body *Array;

public:
    // Реализация методов интерфейса на базе *ваших* переменных

    unsigned int getNumberOfBodies() const override
    {
        return n;
    }

    const GLBall& getBody(unsigned int number) const override
    {
        return Array[number];
    }

    void doTimeStep() override
    {
            for (int i = 0; i < n; i++)
            {
                cout << Array [i];
                Array[i].Step(Array, n);
            }
            cout << endl;
    }

    void initScene()
    {
        cin >> n;
        Array = new Body[n];
        for (int i = 0; i < n; i++)
        {
            cin >> Array[i];
        }
    }
};

    NBodyScene* getScene()
    {
        SampleScene* s = new SampleScene();
        s->initScene();
        return s;
    }

int main()
{
    SampleScene* scene = (SampleScene*)getScene();
    for(int i = 0; i < 100; i++)
    {
        scene->doTimeStep();
    }

    delete scene;
    return 0;
}
