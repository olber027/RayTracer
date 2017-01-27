#include "Vector.h"
#include <cstdlib>
#include <iostream>
#include <cmath>

#ifndef POINT_H
#define POINT_H
class Point {

private:
    int size;
    double* vals;

public:

    Point() : size(0), vals(NULL) {}
    Point(int s) : size(s) { allocateMemory(); zeroPoint(); }
    Point(double a, double b) : size(2) { allocateMemory(); vals[0] = a; vals[1] =b; }
    Point(double a, double b,  double c) : size(3) {
        allocateMemory();
        vals[0] = a;
        vals[1] = b;
        vals[2] = c;
    }
    Point(int s, double* arr) : size(s) {
        allocateMemory();
        for(int i = 0; i < size; i++) {
            vals[i] = arr[i];
        }
    }

    Point(const Point& p) {
        size = p.size;
        vals = new double[size];
        for(int i = 0; i < size; i++) {
            vals[i] = p.vals[i];
        }
    }

    Point(const Point* p) {
        size = p->size;
        vals = new double[size];
        for(int i = 0; i < size; i++) {
            vals[i] = p->vals[i];
        }
    }

    ~Point() {
        deallocateMemory();
    }

    void destroy() {
        deallocateMemory();
    }

    void allocateMemory() {
        if(size <= 0) {
            size = 0;
            vals = NULL;
        } else {
            vals = new double[size];
        }
    }

    void deallocateMemory() {
        if(vals != NULL) {
            delete [] vals;
        }
        vals = NULL;
        size = 0;
    }

    int getSize() {
        return size;
    }

    double operator[](int index) {
        return getVal(index);
    }

    double getVal(int index) {
        if(index < 0 || index > size) {
            return -12345;
        }
        return vals[index];
    }

    void setVal(int index, double val) {
        if(index >= 0 && index < size) {
            vals[index] = val;
        }
    }

    void setVals(Point p) {
        if(size != p.size) {
            destroy();
            size = p.size;
            allocateMemory();
        }
        for(int i = 0; i < size; i++) {
            vals[i] = p[i];
        }
    }

    void setVals(int size, double* arr) {
        setVals(Point(size, arr));
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

    Point& operator=(Point p) {
        if(this == &p) {
            return *this;
        }
        destroy();
        size = p.size;
        allocateMemory();
        for(int i = 0; i < size; i++) {
            vals[i] = p.vals[i];
        }
        return *this;
    }

    Point& operator=(Point* p) {
        if(this == p) {
            return *this;
        }
        destroy();
        size = p->size;
        allocateMemory();
        for(int i = 0; i < size; i++) {
            vals[i] = p->vals[i];
        }
        return *this;
    }

    bool operator==(Point p) {
        if(p.size != size) {
            return false;
        }
        for(int i = 0; i < size; i++) {
            if(p[i] != vals[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(Point p) {
        return !(*this == p);
    }

    friend Point operator*(Point p, double s) {
        Point result(p.size, p.vals);
        for(int i = 0; i < p.size; i++) {
            result.vals[i] *= s;
        }
        return result;
    }

    friend Point operator*(double s, Point p) {
        return p*s;
    }

    Point operator*=(double s) {
        for(int i = 0; i < size; i++) {
            vals[i] *= s;
        }
        return *this;
    }

    friend Point operator/(double s, Point p) {
        return p*(1.0/s);
    }

    friend Point operator/(Point p, double s) {
        return p*(1.0/s);
    }

    Point operator/=(double s) {
        for(int i = 0; i < size; i++) {
            vals[i] /= s;
        }
        return *this;
    }

    friend Point operator+(Point p, Vector v) {
        if(p.size != v.getSize()) {
            return Point();
        }
        Point result(p.size);
        for(int i = 0; i < p.size; i++) {
            result.vals[i] = p.vals[i] + v.getVal(i);
        }
        return result;
    }

    friend Point operator+(Point p, Vector* v) {
        return p+(*v);
    }

    Point operator+=(Vector v) {
        *this = *this + v;
        return *this;
    }

    friend Point operator-(Point p, Vector v) {
        return p + (v*-1);
    }

    friend Point operator-(Point p, Vector* v) {
        return p + ((*v)*-1);
    }

    Point operator-=(Vector v) {
        *this = *this - v;
        return *this;
    }

    friend Vector operator-(Point p, Point q) {
        if(p.size != q.size) {
            return Vector();
        }
        Vector result(p.size);
        for(int i = 0; i < p.size; i++) {
            result.setVal(i, p[i] - q[i]);
        }
        return result;
    }

    friend Vector operator-(Point* p, Point q) {
        return (*p)-q;
    }

    friend Vector operator-(Point p, Point* q) {
        return p-(*q);
    }

    friend std::ostream& operator<<(std::ostream& out, Point p) {
        if(p.size > 0) {
            out << "[ " << p[0];
            for(int i = 1; i < p.size; i++) {
                out << ", " << p[i];
            }
            out << " ]";
        } else {
            out << " [ ] ";
        }
        return out;
    }

    friend std::istream& operator>>(std::istream& in, Point& p) {
        p.destroy();
        char garbage;
        double val;
        in >> garbage;
        while(garbage != ']') {
            in >> val >> garbage;
            p.pushBack(val);
        }
        return in;
    }

    void zeroPoint() {
        for(int i = 0; i < size; i++) {
            vals[i] = 0;
        }
    }

    void fillPoint(double fillNumber) {
        for(int i = 0; i < size; i++) {
            vals[i] = fillNumber;
        }
    }

    void clear() {
        zeroPoint();
    }

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

    void changeSize(int newSize) {
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

    double* toArray() {
        double* result = new double[size];
        for(int i = 0; i < size; i++) {
            result[i] = vals[i];
        }
        return result;
    }

    Vector toVector() {
        return Vector(size, vals);
    }

    bool isEmpty() {
        if(size == 0) {
            return true;
        }
        return false;
    }

    friend Point abs(Point p) {
        Point result;
        for(int i = 0; i < p.size; i++) {
            result.pushBack(std::abs(p[i]));
        }
        return result;
    }
};
#endif //POINT_H
