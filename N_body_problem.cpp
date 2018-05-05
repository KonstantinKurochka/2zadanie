#include <iostream>
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

double getX() const
{
    return x;
}

double getY() const
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
    double a = this->getX() + v.x;
    double b = this->getY() + v.y;
    return Vector(a, b);
}

Vector operator- (const Vector& v)
{
    double a = this->getX() - v.x;
    double b = this->getY() - v.y;
    return Vector(a, b);
}

Vector operator* (const double f)
{
    double a = (this->getX()) * f;
    double b = (this->getY()) * f;
    return Vector(a, b);
}

Vector operator/ (const double f)
{
    double a = (this->getX()) / f;
    double b = (this->getY()) / f;
    return Vector(a, b);
}

private:
    double x, y;
};

Vector operator* (double f, Vector& v)
{
    double a = f * v.getX();
    double b = f * v.getY();
    return Vector(a, b);
}

Vector operator/ (double f, Vector& v)
{
    double a = f / v.getX();
    double b = f / v.getY();
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
    os << v.getX() << " " <<v.getY();
    return os;
}

class Body
{
public:
Body() {}

virtual ~Body() {}

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
    this->x = this->x + this->speed.getX() * dt;
    this->y = this->y + this->speed.getY() * dt;
}

double getx() const
{
    return this->x;
}

double gety() const
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
    os << cor0(B.getx()) << "\t" << cor0(B.gety()) << "\t" << cor0(B.getspeed().getX()) << "\t" << cor0(B.getspeed().getY()) << "\t" << cor0(B.getmass()) << "\t";
    return os;
}

int main()
{
    int n;
    cin >> n;
    Body Array[n];
    for (int i = 0; i < n; i++)
    {
        cin >> Array[i];
    }
    double t = 0;

    /*for (t = 0; t < 1.5; t = t + dt) //Вывод через стандартный поток
    {
        for (int i = 0; i < n; i++)
        {
            cout << Array [i];
            Array[i].Step(Array, n);
        }
    }*/

    ofstream fout("data.txt");          // Вывод в файл
    for (t = 0; t < 1.5; t = t + dt)
    {
        fout << t << "\t";

        for (int i = 0; i < n; i++)
        {
            fout << Array [i];
            Array[i].Step(Array, n);
        }
        fout << endl;
    }
    fout.close();
    return 0;
}
