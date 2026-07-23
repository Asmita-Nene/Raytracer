#ifndef UTILITYCLASSES_HPP
#define UTILITYCLASSES_HPP

#include<fstream>
#include<vector>

double randomDouble(double min, double max);

double degreeToRadian(double angle);


class Vector3 {
public:
	double x, y, z;

	Vector3();

	Vector3(double x, double y, double z);

	void print();

	void display()const;

	Vector3 operator+(const Vector3& other) const;

	Vector3 operator+(const double n) const;

	Vector3 operator-(const Vector3& other) const;

	Vector3 operator*(const double n) const;

	Vector3 operator*(const Vector3& other) const;

	Vector3 operator / (const double n) const;

	double dot(const Vector3& other)const;

	Vector3 cross(const Vector3& other)const;

	Vector3 getNormalized() const;

	double getMagnitude() const;

};


//Defining the class alias for better readability of the code and the logic
using Color = Vector3;
using Point3 = Vector3;



class Ray {
public:
	Point3 origin;
	Vector3 direction;

	Ray(const Point3& origin, const Vector3& direction);

	Point3 pointAt(double t) const;
};

#endif // !UTILITYCLASSES_HPP
