#include <cmath>
#include <cstdlib>
#include <string>
#include <iostream>

#ifndef VECTOR_H
#define VECTOR_H

class Vector {

private:
    int size;
    double* vals;

public:
    Vector() : size(0), vals(NULL) {}

    Vector(int s) : size(s) {
        allocateMemory();
        fillVector(0);
    }
    //2d case
    Vector(double a, double b) : size(2) {
        allocateMemory();
        vals[0] = a; vals[1] = b;
    }
    //3d case
    Vector(double a, double b, double c) : size(3) {
        allocateMemory();
        vals[0] = a; vals[1] = b; vals[2] = c;
    }
    //I assume you're giving the correct size of the
    //array along with the array. It will break if you don't.
    Vector(int s, double* arr) : size(s) {
        allocateMemory();
        for(int i = 0; i < s; i++) {
            vals[i] = arr[i];
        }
    }

    //copy constructor
    Vector(const Vector& v) {
        size = v.size;
        allocateMemory();

        for(int i = 0; i < size; i++)
            vals[i] = v.vals[i];
    }

    Vector(const Vector* v) {
        size = v->size;
        allocateMemory();

        for(int i = 0; i < size; i++)
            vals[i] = v->vals[i];
    }

    //destructor
    ~Vector() {
        destroy();
    }

    //callable destructor
    void destroy() {
        if(vals != NULL) {
            delete [] vals;
        }
        vals = NULL;
        size = 0;
    }

    void allocateMemory() {
        if(size <= 0) {
            size = 0;
            vals = NULL;
        } else {
            vals = new double[size];
        }
    }

    int getSize() {
        return size;
    }

    //sets the value at pos
    bool setVal(int pos, double val) {
        if(pos < 0 || pos >= size)
            return false;
        vals[pos] = val;
        return true;
    }

    //overwrites the values in the vector with the values from the supplied vector
    void setVals(Vector v) {
        if(size != v.size) {
            destroy();
            size = v.size;
            allocateMemory();
        }
        for(int i = 0; i < v.size; i++) {
            vals[i] = v[i];
        }
    }

    //overwrites the values in the vector using the values from the supplied array
    void setVals(int size, double* arr) {
        setVals(Vector(size, arr));
    }

    bool is2d() {
        if(size == 2)
            return true;
        return false;
    }

    bool is3d() {
        if(size == 3)
            return true;
        return false;
    }

    double getVal(int index) {
        if(index < 0 || index >= size) {
            return -12345;
        }
        return vals[index];
    }

    double operator[](int index) {
        return getVal(index);
    }

    //scalar multiplication
    //Note: I included functionality for both Vector * scalar
    //		   and scalar * Vector. they may or may not make
    //		   sense mathematically, but intuitively, it makes
    //		   sense to me, so....I'm leaving it.
    friend Vector operator*(Vector v, double scale) {
        Vector result = new Vector(v.size, v.vals);
        for(int i = 0; i < v.size; i++) {
            result.vals[i] *= scale;
        }
        return result;
    }

    friend Vector operator*(double scale, Vector v) {
        return v*scale;
    }

    Vector operator*=(double scale) {
        for(int i = 0; i < size; i++) {
            vals[i] *= scale;
        }
        return *this;
    }

    //scalar division
    //Note: I only included Vector/scalar because it doesn't
    //		  make much sense to divide a number by a vector
    friend Vector operator/(Vector v, double scale) {
        return v*(1.0/scale);
    }

    Vector operator/=(double scale) {
        for(int i = 0; i < size; i++) {
            vals[i] /= scale;
        }
        return *this;
    }

    //dot product
    friend double operator*(Vector u, Vector v) {
        if(u.size != v.size)
            return -12345;
        double sum = 0;
        for(int i = 0; i < u.size; i++) {
            sum += u.vals[i] * v.vals[i];
        }
        return sum;
    }

    friend double operator*(Vector u, Vector* v) {
        return u*(*v);
    }

    friend double operator*(Vector* u, Vector v) {
        return (*u)*v;
    }

    //more dot product in case people don't know that * is the dot product
    static double dot(Vector u, Vector v) {
        return u*v;
    }

    static double dot(Vector* u, Vector v) {
        return u*v;
    }

    static double dot(Vector u, Vector* v) {
        return u*v;
    }

    static double dot(Vector* u, Vector* v) {
        return (*u)*(*v);
    }

    //so much dot product...
    double dot(Vector v) {
        return (*this)*v;
    }

    double dot(Vector* v) {
        return (*this)*(*v);
    }

    //cross product. Vectors must be 3 dimensional to get
    //a meaningful result.
    static Vector cross(Vector u, Vector v) {
        if(!(u.is3d() && v.is3d()))
            return Vector();
        Vector result(3);
        //formula for cross product of two vectors;
        result.vals[0] = u[1]*v[2] - u[2]*v[1];
        result.vals[1] = u[2]*v[0] - u[0]*v[2];
        result.vals[2] = u[0]*v[1] - u[1]*v[0];
        return result;
    }

    static Vector cross(Vector* u, Vector v) {
        return cross(*u, v);
    }

    static Vector cross(Vector* u, Vector* v) {
        return cross(*u, *v);
    }

    static Vector cross(Vector u, Vector* v) {
        return cross(u, *v);
    }

    Vector cross(Vector v) {
        return cross(*this, v);
    }

    Vector cross(Vector* v) {
        return cross(*this, *v);
    }

    //vector addition
    friend Vector operator+(Vector u, Vector v) {
        if(u.size != v.size)
            return Vector();
        Vector result(u.size);
        for(int i = 0; i < u.size; i++) {
            result.vals[i] = u.vals[i] + v.vals[i];
        }
        return result;
    }

    friend Vector operator+(Vector u, Vector* v) {
        return u+(*v);
    }

    friend Vector operator+(Vector* u, Vector v) {
        return (*u)+v;
    }

    Vector operator+=(Vector v) {
        *this = *this + v;
        return *this;
    }

    //vector subtraction. You'll note this is just adding
    //the negative version of the vector on the right.
    friend Vector operator-(Vector u, Vector v) {
        return (u + (v*-1));
    }

    friend Vector operator-(Vector u, Vector* v) {
        return (u + ((*v)*-1));
    }

    friend Vector operator-(Vector* u, Vector v) {
        return ((*u) + (v*-1));
    }

    Vector operator-=(Vector v) {
        *this = *this - v;
        return *this;
    }

    //equals operator
    Vector& operator=(Vector v) {
        if(this == &v) {
            return *this;
        }
        destroy();
        size = v.size;
        allocateMemory();
        for(int i = 0; i < size; i++) {
            vals[i] = v[i];
        }
        return *this;
    }

    Vector& operator=(Vector* v) {
        if(this == v) {
            return *this;
        }
        destroy();
        size = v->size;
        allocateMemory();
        for(int i = 0; i < size; i++) {
            vals[i] = v->vals[i];
        }
        return *this;
    }

    //comparators.
    bool operator==(Vector v) {
        if(v.size != size) {
            return false;
        }
        for(int i = 0; i < size; i++) {
            if(v[i] != vals[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(Vector v) {
        return !(*this == v);
    }

    //this is where a less than and greater than comparator would go,
    //but I don't think that makes a lot of sense for vectors. you could
    //be comparing magnitude, position, angle, or any number of things,
    //so if you want to compare those, you'll have to do it yourself

    friend std::ostream& operator<<(std::ostream& out, Vector v) {
        if(v.size > 0) {
            out << "[ " << v[0];
            for(int i = 1; i < v.size; i++) {
                out << ", " << v[i];
            }
            out << " ]";
        } else {
            out << "[ ]";
        }
        return out;
    }

    //expected format: [ 1.0, 2, 4.3 ]
    //also, this overwrites the vector with the vector being entered
    friend std::istream& operator>>(std::istream& in, Vector& v) {
        v.destroy();
        char garbage;
        double val;
        in >> garbage;
        while(garbage != ']') {
            in >> val >> garbage;
            v.pushBack(val);
        }
        return in;
    }

    //returns the length of the vector
    double length() {
        double sum = 0;
        for(int i = 0; i < size; i++) {
            sum += (vals[i] * vals[i]);
        }
        return sqrt(sum);
    }

    static double length(Vector v) {
        return v.length();
    }

    //same thing as length...I've heard it called magnitude before though too.
    double magnitude() {
        return length();
    }

    static double magnitude(Vector v) {
        return v.length();
    }

    //normalize the vector this is called on
    void normalize() {
        *this = (*this)/length();
    }

    //returns a normalized version of the vector this was called on
    Vector getNormalizedVector() {
        Vector result(size, vals);
        result.normalize();
        return result;
    }

    //I've heard it called the unit vector more than normalized vector, so this is for that...
    void toUnitVector() {
        normalize();
    }

    Vector getUnitVector() {
        return getNormalizedVector();
    }

    //returns the largest number in the vector
    double max() {
        if(size == 0)
            return 0;
        double max = vals[0];
        for(int i = 1; i < size; i++) {
            if(vals[i] > max)
                max = vals[i];
        }
        return max;
    }

    //returns the smallest number in the vector
    double min() {
        if(size == 0)
            return 0;
        double min = vals[0];
        for(int i = 1; i < size; i++) {
            if(vals[i] < min)
                min = vals[i];
        }
        return min;
    }

    bool isUnit() {
        if(length() == 1) {
            return true;
        }
        return false;
    }

    void clear() {
        destroy();
    }

    //changes the size of the vector to the newSize, keeping as many of the values as
    //possible in the case of a smaller size, and adding zeroes in the case of a
    //larger size
    void changeSize(int newSize) {
        if(newSize <= 0) {
            destroy();
            return;
        }
        double* temp = vals;
        vals = new double[newSize];
        if(newSize < size) {
            for(int i = 0; i < newSize; i++) {
                vals[i] = temp[i];
            }
        } else {
            for(int i = 0; i < size; i++) {
                vals[i] = temp[i];
            }
            for(int i = size; i < newSize; i++) {
                vals[i] = 0;
            }
        }
        size = newSize;
        delete [] temp;
    }

    void pushBack(double newVal) {
        changeSize(size+1);
        setVal(size-1, newVal);
    }

    static Vector zeroVector(int size) {
        return Vector(size);
    }

    static void fillVector(Vector v, double fillNumber) {
        v.fillVector(fillNumber);
    }

    //fills the vector with the given fill number
    void fillVector(double fillNumber) {
        for(int i = 0; i < size; i++) {
            vals[i] = fillNumber;
        }
    }

    //returns true if there are any values that are not zero in the vector
    bool isNonZero() {
        for(int i = 0; i < size; i++) {
            if(vals[i] != 0) {
                return true;
            }
        }
        return false;
    }

    //returns true if all values in the vector are zero, or if the size is zero
    bool isZero() {
        return !isNonZero();
    }

    //gives the index of the first value that isn't zero, or in the case
    //of all values being zero, returns the size. mainly added this to
    //make reducing matrices to row-echelon form easier
    int getFirstNonZeroElementIndex() {
        for(int i = 0; i < size; i++) {
            if(vals[i] != 0) {
                return i;
            }
        }
        return size;
    }

    //returns an array with the same values as the vector
    double* toArray() {
        double* result = new double[size];
        for(int i = 0; i < size; i++) {
            result[i] = vals[i];
        }
        return result;
    }

    //this is meant for applying a rotation matrix to a vector,
    //so I'm assuming it's a vector 3 when this is called, and
    //that the last value should be a 1. it will work for any
    //size vector, but it won't be particularly useful.
    Vector toVector4() {
        changeSize(4);
        setVal(3, 1.0);
        return *this;
    }

    Vector toVector3() {
        changeSize(3);
        return *this;
    }

    bool isEmpty() {
        if(size == 0) {
            return true;
        }
        return false;
    }

    friend Vector abs(Vector v) {
        Vector result;
        for(int i = 0; i < v.size; i++)  {
            result.pushBack(std::abs(v[i]));
        }
        return result;
    }
};

#endif //VECTOR_H
