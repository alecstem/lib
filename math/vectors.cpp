#include <bits/stdc++.h>
using namespace std;

template<typename T>
struct Vector
{
	T x,y,z;
	Vector(T x, T y, T z) : x(x), y(y), z(z) {}

	T dot(const Vector &b)
	{
		return (x*b.x)+(y*b.y)+(z*b.z);
	}

	Vector cross(const Vector &b)
	{
		// Compute cross product
		Vector r = Vector(1,1,1);
		r.x = (y*b.z)-(z*b.y);
		r.y = -((x*b.z)-(z*b.x));
       		r.z = (x*b.y)-(y*b.x);
		return r;	
	}
};

template<typename T>
ostream& operator<<(ostream &os, const Vector<T> &v)
{
	// Override << operator for Vector struct
	os << v.x << " " << v.y << " " << v.z;
	return os;
}

int main()
{
	float x1, x2, y1, y2, z1, z2;
	char c;

	// Accept user input
	
	cout << "Vector A: \n";
	cin >> x1 >> y1 >> z1;
	cout << "Vector B: \n";
	cin >> x2 >> y2 >> z2;
	cout << "(d)ot, (c)ross, (q)uit: \n";
	cin >> c;

	// Create Vectors
	
	Vector<float> a(x1,y1,z1), b(x2,y2,z2);
	
	switch(c)
	{
		case 'd':
			cout << a.dot(b);
			break;
		case 'c':
			cout << a.cross(b);
			break;
		case 'q':
			return 0;
			break;
		default:
			break;
	}
	return 0;
}
