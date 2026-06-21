#include<iostream>
#include<fstream>
#include<vector>
#include "UtilityClasses.hpp"

#include <random>

double randomDouble(double min, double max) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<double> dist(min, max);

	return dist(gen);
}



//----Vector3 class definations------------------------------------------------//

	Vector3::Vector3() : x(0), y(0), z(0) {}
	Vector3::Vector3(double x, double y, double z) : x(x), y(y), z(z) {}

	void Vector3::print() {
		std::cout << "Vector3(" << x << ", " << y << ", " << z << ")\n";
	}

	Vector3 Vector3::operator+(const Vector3& other) const {
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	Vector3 Vector3:: operator+(const double n) const {
		return Vector3(x + n, y + n, z + n);
	}

	Vector3 Vector3::operator-(const Vector3& other) const {
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	Vector3 Vector3::operator*(const double n) const {
		return Vector3(x * n, y * n, z * n);
	}

	Vector3 Vector3::operator*(const Vector3& other) const {
		return Vector3(x * other.x, y * other.y, z * other.z);
	}

	Vector3 Vector3::operator/(const double n) const {
		return Vector3(x / n, y / n, z / n);
	}

	double Vector3::dot(const Vector3& other) const {
		return ((x * other.x) + (y * other.y) + (z * other.z));
	}

	Vector3 Vector3::cross(const Vector3& other) const
	{
		return Vector3(
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
		);
	}

	Vector3 Vector3::getNormalized() const {
		double mag = sqrt((x * x) + (y * y) + (z * z));
		return Vector3((x / mag), (y / mag), (z / mag));
	}

	double Vector3::getMagnitude() const {
		return sqrt((x * x) + (y * y) + (z * z));
	}




	//------------Ray class definitions---------------------------////

	Ray::Ray(const Point3& origin, const Vector3& direction) : origin(origin), direction(direction) {}

	Point3 Ray::pointAt(double t) const {
		Point3 point(origin + (direction * t));
		return point;
	}


	

