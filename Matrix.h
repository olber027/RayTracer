#include "Vector.h"
#include "Point.h"
#include <cstdlib>

#ifndef MATRIX_H
#define MATRIX_H

using namespace std;

class Matrix {

public:

    Matrix() : rows(0), cols(0), mat(NULL) {}
    Matrix(int r, int c) : rows(r), cols(c) {
        allocateMemory();
        zeroMatrix();
    }

    //fills the matrix of the given dimensions with the given value
    Matrix(int r, int c, double init) : rows(r), cols(c) {
        allocateMemory();
        fillMatrix(init);
    }

    //copy contstructor
    Matrix(const Matrix& m) {
        rows = m.rows;
        cols = m.cols;
        allocateMemory();
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                mat[i][j] = m.mat[i][j];
            }
        }
    }

    //copy constructor
    Matrix(const Matrix* m) {
        rows = m->rows;
        cols = m->cols;
        allocateMemory();
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                mat[i][j] = m->mat[i][j];
            }
        }
    }

    //destructor
    ~Matrix() { deallocateMemory(); }

    Matrix& operator=(Matrix m) {
        if(this == &m) {
            return *this;
        }
        deallocateMemory();
        rows = m.rows;
        cols = m.cols;
        allocateMemory();
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                mat[i][j] = m.mat[i][j];
            }
        }
    }

    Matrix& operator=(Matrix* m) {
        if(this == m) {
            return *this;
        }
        deallocateMemory();
        rows = m->rows;
        cols = m->cols;
        allocateMemory();
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                mat[i][j] = m->mat[i][j];
            }
        }
    }

    void allocateMemory() {
        if(rows == 0 || cols == 0) {
            mat = NULL;
            rows = 0;
            cols = 0;
            return;
        }
        mat = new double*[rows];
        for(int i = 0; i < rows; i++) {
            mat[i] = new double[cols];
        }
    }

    void deallocateMemory() {
        if(mat != NULL) {
            for(int i = 0; i < rows; i++) {
                delete [] mat[i];
            }
            delete [] mat;
        }
    }

    void clear() {
        deallocateMemory();
    }

    double operator()(int r, int c) {
        if(!isInBounds(r,c)) {
            return -12345;
        } else {
            return mat[r][c];
        }
    }

    //if I return a vector, it lets you access items in a m[i][j] fashion...loopholes!
    Vector operator[](int r) {
        return Vector(cols, mat[r]);
    }

    int numRows() { return rows; }
    int numcols() { return cols; }
    int rowLength() { return cols; }
    int colLength() { return rows; }

    //sets all values in the matrix to 0
    void zeroMatrix() {
        fillMatrix(0);
    }

    //fills all entries in the matrix with the given value.
    //overwrites any values that were previously there.
    void fillMatrix(double fill) {
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                mat[i][j] = fill;
            }
        }
    }

    //returns true if the given location is in the bounds of the matrix.
    bool isInBounds(int r, int c) {
        if(r >= rows || c >= cols || r < 0 || c < 0) {
            return false;
        } else {
            return true;
        }
    }

    //sets the value at the given location to the given value
    bool setVal(int r, int c, double val) {
        if(isInBounds(r,c)) {
            mat[r][c] = val;
            return true;
        }
        return false;
    }

    //sets all entries in the given row to the given value.
    bool setRow(int rowNum, double val) {
        if(isInBounds(rowNum, 0)) {
            for(int i = 0; i < cols; i++) {
                mat[rowNum][i] = val;
            }
            return true;
        }
        return false;
    }

    //sets the given row to the new vector. If the vector is smaller
    //than the number of columns in the matrix, the empty entries will be
    //filled in with 0. If it is larger, only the entries that fit will be filled in.
    bool setRow(int rowNum, Vector v) {
        if(isInBounds(rowNum, 0)) {
            if(v.getSize() < cols) {
                for(int i = 0; i < v.getSize(); i++) {
                    mat[rowNum][i] = v[i];
                }
                for(int i = v.getSize(); i < cols; i++) {
                    mat[rowNum][i] = 0;
                }
            } else {
                for(int i = 0; i < cols; i++) {
                    mat[rowNum][i] = v[i];
                }
            }
            return true;
        }
        return false;
    }

    //sets all entries in the given column to the given value.
    bool setCol(int colNum, double val) {
        if(isInBounds(0, colNum)) {
            for(int i = 0; i < rows; i++) {
                mat[i][colNum] = val;
            }
            return true;
        }
        return false;
    }

    //sets the given column to the new vector. If the vector is smaller
    //than the number of rows in the matrix, the empty entries will be
    //filled in with 0. If it is larger, only the entries that fit will be filled in.
    bool setCol(int colNum, Vector v) {
        if(isInBounds(0, colNum)) {
            if(v.getSize() < rows) {
                for(int i = 0; i < v.getSize(); i++) {
                    mat[i][colNum] = v[i];
                }
                for(int i = v.getSize(); i < rows; i++) {
                    mat[i][colNum] = 0;
                }
            } else {
                for(int i = 0; i < rows; i++) {
                    mat[i][colNum] = v[i];
                }
            }
            return true;
        }
        return false;
    }

    //returns true if the number of rows is equal to the number of columns
    //false otherwise.
    bool isSquare() {
        if(rows == cols)
            return true;
        return false;
    }

    //this adds a row to the matrix. it assumes that if the matrix has
    //no values in it, that the matrix was meant to be empty, and thus makes
    //a 1xN matrix with the given vector as the only row. if you'd like to keep
    //all the rows of zeroes, see addRowKeepingZeroes()
    bool addRow(Vector v) {
        if(!isZeroMatrix()) {
            if(v.getSize() != cols) {
                return false;
            }
            Matrix temp = *this;
            deallocateMemory();
            rows++;
            allocateMemory();
            for(int i = 0; i < rows-1; i++) {
                for(int j = 0; j < cols; j++) {
                    mat[i][j] = temp.mat[i][j];
                }
            }
        } else {
            deallocateMemory();
            cols = v.getSize();
            rows = 1;
            allocateMemory();
        }
        for(int i = 0; i < cols; i++) {
            mat[rows-1][i] = v[i];
        }
        return true;
    }

    bool addRow(int size, double* arr) {
        return addRow(Vector(size, arr));
    }

    //adds a row to the matrix keeping all rows of zeroes
    bool addRowKeepingZeroes(Vector v) {
        if(!isZeroMatrix()) {
            return addRow(v);
        } else {
            if(v.getSize() != rowLength()) {
                return false;
            }
            int tempRows = rows;
            int tempCols = cols;
            deallocateMemory();
            rows = tempRows+1;
            cols = tempCols;
            allocateMemory();
            zeroMatrix();
            setRow(rows-1, v);
            return true;
        }
    }

    bool addRowKeepingZeroes(int s, double* arr) {
        return addRowKeepingZeroes(Vector(s, arr));
    }

    //adds a column to the matrix. if the matrix was a zero matrix, it will delete the
    //previously existing columns of zeroes and replace them with the new column.
    bool addCol(Vector v) {
        if(!isZeroMatrix()) {
            if(v.getSize() != rows) {
                return false;
            }
            Matrix temp = this;
            deallocateMemory();
            cols++;
            allocateMemory();
            for(int i = 0; i < rows; i++) {
                for(int j = 0; j < cols-1; j++) {
                    mat[i][j] = temp.mat[i][j];
                }
                mat[i][cols-1] = v[i];
            }
        } else {
            deallocateMemory();
            rows = v.getSize();
            cols = 1;
            allocateMemory();
            for(int i = 0; i < rows; i++) {
                mat[i][0] = v[i];
            }
        }
        return true;
    }

    bool addCol(int size, double* arr) {
        return addCol(Vector(size, arr));
    }

    //adds a column to the matrix keeping all columns of zeroes
    bool addColKeepingZeroes(Vector v) {
        if(!isZeroMatrix()) {
            return addCol(v);
        } else {
            if(v.getSize() != colLength()) {
                return false;
            }
            int tempRows = rows;
            int tempCols = cols;
            deallocateMemory();
            rows = tempRows;
            cols = tempCols+1;
            allocateMemory();
            zeroMatrix();
            setCol(cols-1, v);
            return true;
        }
    }

    bool addColKeepingZeroes(int s, double* arr) {
        return addColKeepingZeroes(Vector(s, arr));
    }

    //returns true if the dimensions of the input matrix are the same as the
    //dimensions of the calling object
    bool compareDimensions(Matrix m) {
        return (m.rows == rows && m.cols == cols);
    }

    bool compareDimensions(Matrix* m) {
        return (m->rows == rows && m->cols == cols);
    }

    static bool compareDimensions(Matrix m, Matrix l) {
        return (m.rows == l.rows && m.cols == l.cols);
    }

    //deletes the specified row. returns false if the matrix does not have that row.
    bool deleteRow(int rowNum) {
        if(!isInBounds(rowNum, 0)) {
            return false;
        }
        Matrix temp = this;
        deallocateMemory();
        rows--;
        allocateMemory();
        bool skippedRow = false;
        for(int i = 0; i < temp.rows; i++) {
            if(i == rowNum) {
                skippedRow = true;
                continue;
            }
            for(int j = 0; j < cols; j++) {
                if(skippedRow)
                    mat[i-1][j] = temp.mat[i][j];
                else
                    mat[i][j] = temp.mat[i][j];
            }
        }
        return true;
    }

    //deletes the specified column. returns false if the matrix does not have that column.
    bool deleteCol(int colNum) {
        if(!isInBounds(0, colNum)) {
            return false;
        }
        Matrix temp = this;
        deallocateMemory();
        cols--;
        allocateMemory();
        for(int i = 0; i < rows; i++) {
            bool skippedCol = false;
            for(int j = 0; j < temp.cols; j++) {
                if(j == colNum) {
                    skippedCol = true;
                    continue;
                }
                if(skippedCol)
                    mat[i][j-1] = temp.mat[i][j];
                else
                    mat[i][j] = temp.mat[i][j];
            }
        }
        return true;
    }

    friend ostream& operator<<(ostream& out, Matrix m) {
        out.setf(ios::fixed);
        out.precision(3);
        out << "[ ";
        for(int i = 0; i < m.colLength(); i++) {
            if(i != 0)
                out << "  ";
            out << m.getRowAsVector(i) << endl;
        }
        out << "  ]" << endl;
        return out;
    }

    friend istream& operator>>(istream& in, Matrix& m) {
        m.clear();
        char garbage;
        Vector v;
        in >> garbage;
        while(garbage != ']') {
            in >> v;
            m.addRow(v);
            garbage = in.peek();
        }
        return in;
    }

    //NOTE: all operators are pretty self explanatory. I will not be commenting on them
    //			 except where I deem appropriate.

    friend Matrix operator+(Matrix m, Matrix l) {
        if(!m.compareDimensions(l))
            return Matrix();
        Matrix result(m.rows, m.cols);
        for(int i = 0; i < result.rows; i++) {
            for(int j = 0; j < result.cols; j++) {
                result.mat[i][j] = m.mat[i][j] + l.mat[i][j];
            }
        }
        return result;
    }

    friend Matrix operator+(Matrix* m, Matrix l) {
        return (*m)+l;
    }

    friend Matrix operator+(Matrix m, Matrix* l) {
        return m+(*l);
    }

    friend Matrix operator-(Matrix m, Matrix l) {
        if(!m.compareDimensions(l))
            return Matrix();
        Matrix result(m.rows, m.cols);
        for(int i = 0; i < result.rows; i++) {
            for(int j = 0; j < result.cols; j++) {
                result.mat[i][j] = m.mat[i][j] - l.mat[i][j];
            }
        }
        return result;
    }

    friend Matrix operator-(Matrix* m, Matrix l) {
        return (*m)-l;
    }

    friend Matrix operator-(Matrix m, Matrix* l) {
        return m-(*l);
    }

    friend Matrix operator*(Matrix m, Matrix l) {
        if(m.rowLength() != l.colLength())
            return Matrix();
        Matrix result(m.colLength(), l.rowLength(), 0);
        for(int i = 0; i < result.rows; i++) {
            for(int j = 0; j < result.cols; j++) {
                for(int k = 0; k < m.cols; k++) {
                    result.mat[i][j] += m.mat[i][k]*l.mat[k][j];
                }
            }
        }
        return result;
    }

    friend Matrix operator*(Matrix* m, Matrix l) {
        return (*m)*l;
    }

    friend Matrix operator*(Matrix m, Matrix* l) {
        return m*(*l);
    }

    //Treating the vector as a row vector
    friend Vector operator*(Vector v, Matrix m) {
        if(v.getSize() != m.colLength()) {
            return Vector();
        }
        Vector result(m.rowLength());
        for(int i = 0; i < m.rowLength(); i++) {
            double sum = 0.0;
            for(int j = 0; j < m.colLength(); j++) {
                sum += v[j]*m[j][i];
            }
            result.setVal(i, sum);
        }
        return result;
    }

    friend Vector operator*(Vector* v, Matrix m) {
        return (*v)*m;
    }


    //Treating the vector as a column Vector
    friend Vector operator*(Matrix m, Vector v) {
        if(v.getSize() != m.rowLength()) {
            return Vector();
        }
        Vector result(m.colLength());
        for(int i = 0; i < m.colLength(); i++) {
            double sum = 0.0;
            for(int j = 0; j < m.rowLength(); j++) {
                sum += v[j]*m[i][j];
            }
            result.setVal(i, sum);
        }
        return result;
    }

    friend Vector operator*(Matrix m, Vector* v) {
        return m*(*v);
    }

    friend Point operator*(Point p, Matrix m) {
        if(p.getSize() != m.colLength()) {
            return Point();
        }
        Point result(m.rowLength());
        for(int i = 0; i < m.rowLength(); i++) {
            double sum = 0.0;
            for(int j = 0; j < m.colLength(); j++) {
                sum += p[j]*m[j][i];
            }
            result.setVal(i, sum);
        }
        return result;
    }

    friend Point operator*(Point* p, Matrix m) {
        return (*p)*m;
    }

    friend Point operator*(Matrix m, Point p) {
        if(p.getSize() != m.rowLength()) {
            return Point();
        }
        Point result(m.colLength());
        for(int i = 0; i < m.colLength(); i++) {
            double sum = 0.0;
            for(int j = 0; j < m.rowLength(); j++) {
                sum += p[j]*m[i][j];
            }
            result.setVal(i, sum);
        }
        return result;
    }


    friend Point operator*(Matrix m, Point* p) {
        return m*(*p);
    }

    friend Matrix operator*(Matrix m, double s) {
        Matrix result(m.rows, m.cols);
        for(int i = 0; i < result.rows; i++) {
            for(int j = 0; j < result.cols; j++) {
                result.mat[i][j] = m.mat[i][j]*s;
            }
        }
        return result;
    }

    friend Matrix operator/(Matrix m, double s) {
        return m*(1.0/s);
    }

    //returns true if all entries are the same in both matrices
    bool operator==(Matrix m) {
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                if(mat[i][j] != m[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }

    //returns true if not all entries are the same in both matrices
    bool operator!=(Matrix m) {
        return !(*this == m);
    }

    //returns the row echelon form of the matrix this is called on.
    Matrix getRowEchelonForm() {
        Matrix result;
        result = *this;
        if(result.colLength() <= 1) {
            return result;
        }
        result.orderRows();

        for(int i = 0; i < result.colLength(); i++) {
            Vector currRow = result.getRowAsVector(i);
            int counter = currRow.getFirstNonZeroElementIndex();
            if(counter >= result.rowLength()) {
                continue;
            }
            for(int j = i+1; j < result.colLength(); j++) {
                Vector temp = result.getRowAsVector(j);
                double numer = temp.getVal(counter);
                if(numer == 0) {
                    continue;
                } else {
                    double denom = currRow[counter];
                    double mult = numer/denom;
                    temp = temp - currRow*mult;
                    result.setRow(j, temp);
                }
            }
            result.orderRows();
        }

        return result;
    }

    //returns the reduced row echelon form of the matrix this is called on
    Matrix getReducedRowEchelonForm() {
        Matrix result = getRowEchelonForm();

        for(int i = 0; i < result.colLength(); i++) {
            Vector temp = result.getRowAsVector(i);
            int ind = temp.getFirstNonZeroElementIndex();
            temp /= temp[ind];
            result.setRow(i, temp);
        }

        for(int i = 1; i < result.colLength(); i++) {
            Vector currRow = result.getRowAsVector(i);
            int ind = currRow.getFirstNonZeroElementIndex();
            for(int j = i-1; j >= 0; j--) {
                Vector temp = result.getRowAsVector(j);
                temp = temp - currRow*temp[ind];
                result.setRow(j, temp);
            }
        }
        return result;
    }

    //this function orders the rows:
    //                              all zero rows go to the bottom
    //                              rows with leading zeroes go below rows without
    void orderRows() {
        if(colLength() <= 1 || isOrdered()) {
            return;
        }
        //get all the zero rows to the bottom
        int numZeroRows = 0;
        for(int i = 0; i < colLength() - numZeroRows; i++) {
            Vector temp = getRowAsVector(i);
            while(temp.isZero()) {
                swapRows(i, colLength() - 1 - numZeroRows);
                numZeroRows++;
                temp = getRowAsVector(i);
            }
        }
        //order the remaining rows...this is terribly inefficient
        //should probably figure out a good way to do this instead of just throwing it
        //in a while !ordered loop...yay bubble sort.
        while(!isOrdered()) {
            for(int i = 0; i < colLength() - numZeroRows; i++) {
                Vector temp = getRowAsVector(i);
                int ind = temp.getFirstNonZeroElementIndex();
                for(int j = i+1; j < colLength() - numZeroRows; j++) {
                    Vector temp2 = getRowAsVector(j);
                    int ind2 = temp2.getFirstNonZeroElementIndex();
                    if(ind2 < ind) {
                        swapRows(i, j);
                        break;
                    }
                }
            }
        }

    }

    //reduces the matrix this is called on to row echelon form.
    Matrix reduceToRowEchelonForm() {
        *this = getRowEchelonForm();
        return *this;
    }

    //reduces the matrix this is called on to reduced row echelon form.
    Matrix reduceToReducedRowEchelonForm() {
        *this = getReducedRowEchelonForm();
        return *this;
    }

    //returns true if the matrix this is called on is in row echelon form.
    bool isInRowEchelonForm() {
        if(colLength() <= 1) {
            return false;
        }
        //checking all non-zero rows are above any zero rows
        int firstZeroRow = -1;
        for(int i = 0; i < colLength(); i++) {
            Vector temp = getRowAsVector(i);
            if(temp.isZero()) {
                firstZeroRow = i;
                break;
            }
        }
        int lastNonZeroRow = -1;
        for(int i = 0; i < colLength(); i++) {
            Vector temp = getRowAsVector(i);
            if(temp.isNonZero()) {
                lastNonZeroRow = i;
            }
        }
        if(firstZeroRow != -1 && firstZeroRow < lastNonZeroRow) {
            return false;
        }
        //checking all leading coefficients are to the right of the leading coefficient of the row above it
        int prevCol = -1;
        int currCol = 0;
        for(int i = 0; i < colLength(); i++) {
            Vector temp = getRowAsVector(i);
            if(temp.isNonZero()) {
                for(int j = 0; j < temp.getSize(); j++) {
                    if(temp[j] != 0) {
                        currCol = j;
                        if(currCol <= prevCol) {
                            return false;
                        }
                        prevCol = currCol;
                        break;
                    }
                }
            }
        }
        return true;
    }

    //returns true if the matrix this is called on is in reduced row echelon form....doesn't actually work right now though.
    bool isInReducedRowEchelonForm() {
        if(!isInRowEchelonForm()) {
            return false;
        }
        //checking if the leading coefficient is one and that the leading coefficient is the only nonzero entry in the column
        for(int i = 0; i < colLength(); i++) {
            Vector tempRow = getRowAsVector(i);
            if(tempRow.isZero()) {
                continue;
            }
            int leadingCoefficient = -1;
            for(int j = 0; j < tempRow.getSize(); j++) {
                if(tempRow[j] != 0) {
                    if(tempRow[j] != 1) {
                        return false;
                    }
                    leadingCoefficient = j;
                    break;
                }
            }
            Vector tempCol = getColAsVector(leadingCoefficient);
            for(int j = 0; j < tempCol.getSize(); j++) {
                if(tempCol.getVal(j) != 0 && j != i) {
                    return false;
                }
            }
        }
        return true;
    }

    //TRUE       FALSE
    //[1 2 3]    [0 1 2]
    //|0 1 2|    |1 2 3|
    //[0 0 1]    [0 0 1]
    bool isOrdered() {
        for(int i = 0; i < colLength(); i++) {
            Vector temp = getRowAsVector(i);
            int ind = temp.getFirstNonZeroElementIndex();
            for(int j = i+1; j < colLength(); j++) {
                Vector temp2 = getRowAsVector(j);
                int ind2 = temp2.getFirstNonZeroElementIndex();
                if(ind2 < ind) {
                    return false;
                }
            }
        }
        return true;
    }

    //returns the inverse of the matrix this is called on, if it exists.
    Matrix inverse() {
        if(!isSquare() || determinant() == 0) {
            return Matrix();
        }
        Matrix result(rows, cols);
        result = *this;
        //add identity matrix to the right side of the matrix
        for(int i = 0; i < rowLength(); i++) {
            Vector v;
            for(int j = 0; j < colLength(); j++) {
                if(i == j) {
                    v.pushBack(1);
                } else {
                    v.pushBack(0);
                }
            }
            result.addCol(v);
        }

        //the reduced row echelon form of the matrix if the determinant is not zero
        //and the matrix is square will be the identity matrix, leaving the inverse on
        //the right side of the new matrix we made
        result.reduceToReducedRowEchelonForm();

        //get rid of the identity matrix on the left side
        for(int i = rowLength() - 1; i >= 0; i--) {
            result.deleteCol(i);
        }

        return result;
    }

    //makes the matrix this is called on equal to the inverse of that matrix and returns the result.
    void inversify() {
        *this = inverse();
    }

    //returns the determinant of the matrix this is called on, if it exists. returns -12345 if it does not.
    //NOTE: run time for this is n!/2...only use if you have to and store the value when you can as to not re-run it.
    //TODO: make this not shitty run time.
    double determinant() {
        if(!isSquare()) {
            return -12345;
        }
        if(rows == 2 && cols == 2) {
            return mat[0][0]*mat[1][1] - mat[0][1]*mat[1][0];
        }
        double sum = 0;
        double sign = 1;
        for(int i = 0; i < rowLength(); i++) {
            Matrix temp = createSubMatrix((*this), i);
            sum += sign*mat[0][i]*temp.determinant();
            sign*=-1;
        }
        return sum;
    }

    //returns a vector consisting of all the values in the given row.
    Vector getRowAsVector(int rowNum) {
        if(!isInBounds(rowNum, 0)) {
            return Vector();
        }
        Vector result;
        for(int i = 0; i < rowLength(); i++) {
            result.pushBack(mat[rowNum][i]);
        }
        return result;
    }

    //returns an array consisting of all the values in the given row
    double* getRowAsArray(int rowNum) {
        return getRowAsVector(rowNum).toArray();
    }

    //returns a vector consisting of the values in the given column.
    Vector getColAsVector(int colNum) {
        if(!isInBounds(0, colNum)) {
            return Vector();
        }
        Vector result;
        for(int i = 0; i < colLength(); i++) {
            result.pushBack(mat[i][colNum]);
        }
        return result;
    }

    //returns an array consisting of the values in the given column.
    double* getColAsArray(int colNum) {
        return getColAsVector(colNum).toArray();
    }

    //swap the given rows of the matrix.
    bool swapRows(int row1, int row2) {
        if(!isInBounds(row1, 0) || !isInBounds(row2, 0)) {
            return false;
        }

        Vector temp1 = getRowAsVector(row1);
        Vector temp2 = getRowAsVector(row2);

        setRow(row1, temp2);
        setRow(row2, temp1);

        return true;
    }

    //swap the given columns of the matrix.
    bool swapCols(int col1, int col2) {
        if(!isInBounds(0, col1) || !isInBounds(0, col2)) {
            return false;
        }

        Vector temp1 = getColAsVector(col1);
        Vector temp2 = getColAsVector(col2);

        setCol(col1, temp2);
        setCol(col2, temp1);

        return true;
    }

    //get the matrix starting at startRow, and ending at endCol. both parameters are inclusive
    Matrix getPartialMatrixByCols(int startCol, int endCol) {
        if(!isInBounds(0, startCol) || !isInBounds(0, endCol)) {
            return Matrix();
        }
        Matrix result;

        for(int i = startCol; i <= endCol; i++) {
            Vector temp = getColAsVector(i);
            result.addCol(temp);
        }

        return result;
    }

    //returns a matrix consisting of all rows between startRow and endRow, inclusive.
    Matrix getPartialMatrixByRows(int startRow, int endRow) {
        if(!isInBounds(startRow, 0) || !isInBounds(endRow, 0)) {
            return Matrix();
        }
        Matrix result;

        for(int i = startRow; i <= endRow; i++) {
            Vector temp = getRowAsVector(i);
            result.addRow(temp);
        }

        return result;
    }

    //delete all between "startRow" and "endRow", inclusive.
    bool deleteRows(int startRow, int endRow) {
        if(!isInBounds(startRow, 0) || !isInBounds(endRow, 0)) {
            return false;
        }
        //could optimize this to copy the rows left over if we're
        //deleting more than half the matrix.
        for(int i = startRow; i <= endRow; i++) {
            deleteRow(i);
        }
    }

    bool deleteCols(int startCol, int endCol) {
        if(!isInBounds(0, startCol) || !isInBounds(0, endCol)) {
            return false;
        }
        //could optimize this to copy the columns left over if we're
        //deleting more than half the matrix.
        for(int i = startCol; i <= endCol; i++) {
            deleteCol(i);
        }
    }

    //returns true if all entries in the matrix are zero. false otherwise.
    bool isZeroMatrix() {
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                if(mat[i][j] != 0) {
                    return false;
                }
            }
        }
        return true;
    }

    //solves the Ax=b equation, assuming the calling object is A, and b is provided.
    //returns x;
    Vector solveAxb(Vector b) {
        if(!isSquare() || determinant() == 0) {
            return Vector();
        }
        Vector x(rowLength());
        x = inverse() * b;

        return x;
    }

    //returns the transpose of the matrix that this
    //function is called on.
    Matrix getTranspose() {
        Matrix result(cols, rows);
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                result.mat[j][i] = mat[i][j];
            }
        }
        return result;
    }

    //tranposes the matrix that this function is called on.
    Matrix transpose() {
        *this = getTranspose();
        return *this;
    }

    //returns an identity Size x Size identity matrix
    static Matrix getIdentityMatrix(int size) {
        Matrix result(size, size);
        for(int i = 0; i < size; i++) {
            result.mat[i][i] = 1;
        }
        return result;
    }

    //convert a 1xn matrix or a nx1 matrix to a vector
    //returns an empty vector if the matrix has dimensions
    //other than 1xn or nx1
    Vector toVector() {
        if(rows != 1 || cols != 1) {
            return Vector();
        }
        if(rows == 1) {
            return getColAsVector(0);
        } else {
            return getRowAsVector(0);
        }
    }

    //returns true if the matrix is singular. false otherwise.
    bool isSingular() {
        if(determinant() == 0) {
            return true;
        } else {
            return false;
        }
    }

    //applies a rotation of rads radians to the matrix that this
    //function is called on. Only works for 2x2 and 3x3 matrices.
    bool applyRotationInRadians(double rads, string axis) {
        if(rows == 2 && cols == 2) {
            Matrix rot;
            rot.addRow(Vector(cos(rads), -1*sin(rads)));
            rot.addRow(Vector(sin(rads), cos(rads)));
            *this = rot*(*this);
            return true;
        } else if(rows == 3 && cols == 3) {
            Matrix rot;
            if(axis == "x") {
                rot.addRow(Vector(1,0,0));
                rot.addRow(Vector(0,cos(rads), -1*sin(rads)));
                rot.addRow(Vector(0,sin(rads), cos(rads)));
            } else if (axis == "y") {
                rot.addRow(Vector(cos(rads), 0, sin(rads)));
                rot.addRow(Vector(0, 1, 0));
                rot.addRow(Vector(-1*sin(rads), 0, cos(rads)));
            } else if (axis == "z") {
                rot.addRow(Vector(cos(rads), -1*sin(rads), 0));
                rot.addRow(Vector(sin(rads), cos(rads), 0));
                rot.addRow(Vector(0,0,1));
            }
            *this = rot*(*this);
            return true;
        }
        return false;
    }

    //applies a rotation of "degrees" degrees to the matrix that this
    //function is called on. Only works for 2x2 and 3x3 matrices.
    bool applyRotationInDegrees(double degrees, string axis) {
        double radians = degrees*3.14159/180.0;
        return applyRotationInRadians(radians, axis);
    }

    //trans should be in the form of [ x, y ] for translating a 2x2
    //and in the form of [ x, y, z ] for translating a 3x3
    bool applyTranslation(Vector trans) {
        if(rows == 2 && cols == 2) {
            trans.pushBack(1);
            Matrix ident = getIdentityMatrix(3);
            ident.setCol(2, trans);
            changeSize(rows+1, cols+1);
            setRow(rows, Vector(1,1,1));
            *this = ident*(*this);
            deleteRow(rows);
            deleteCol(cols);
            return true;
        } else if (rows == 3 && cols == 3) {
            trans.pushBack(1);
            Matrix ident = getIdentityMatrix(4);
            ident.setCol(3, trans);
            changeSize(rows+1, cols+1);
            Vector v(4);
            v.fillVector(1);
            setRow(rows, v);
            *this = ident*(*this);
            deleteRow(rows-1);
            deleteCol(cols-1);
            return true;
        }
        return false;
    }

    //changes the size of the matrix to the given values.
    //deletes rows or columns if the new size is smaller,
    //and adds them if it is larger.
    void changeSize(int numRows, int numCols) {
        while(rows > numRows) {
            deleteRow(rows-1);
        }
        while(cols > numCols) {
            deleteCol(cols-1);
        }
        while(rows < numRows) {
            addRow(Vector(cols));
        }
        while(cols < numCols) {
            addCol(Vector(rows));
        }
    }

    Matrix get2dRotationMatrix(double rads) {
        Matrix rot;
        rot.addRow(Vector(cos(rads), -1*sin(rads)));
        rot.addRow(Vector(sin(rads), cos(rads)));
        return rot;
    }

    //returns a matrix that will rotate another matrix 'rads' redians about the 'axis' axis.
    //this is designed purely for rotation, however, and does not provide the capability of
    //adding a translation component without some work on your part (read: adding another row
    //and column to the result of this function call)
    Matrix get3dRotationMatrix(double rads, string axis) {
        Matrix rot;
        if(axis == "x") {
            rot.addRow(Vector(1,0,0));
            rot.addRow(Vector(0,cos(rads), -1*sin(rads)));
            rot.addRow(Vector(0,sin(rads), cos(rads)));
        } else if (axis == "y") {
            rot.addRow(Vector(cos(rads), 0, sin(rads)));
            rot.addRow(Vector(0, 1, 0));
            rot.addRow(Vector(-1*sin(rads), 0, cos(rads)));
        } else if (axis == "z") {
            rot.addRow(Vector(cos(rads), -1*sin(rads), 0));
            rot.addRow(Vector(sin(rads), cos(rads), 0));
            rot.addRow(Vector(0,0,1));
        }
        return rot;
    }

    //returns true if the matrix has no entries, false otherwise.
    bool isEmpty() {
        if(rows == 0 && cols == 0) {
            return true;
        }
        return false;
    }

    //returns a matrix with the absolute value of all values
    //in the given matrix.
    friend Matrix abs(Matrix m) {
        Matrix result = m;
        for(int i = 0; i < result.rows; i++) {
            for(int j = 0; j < result.cols; j++) {
                result.mat[i][j] = std::abs(result.mat[i][j]);
            }
        }
        return result;
    }

private:
    int rows, cols;
    double** mat;

    //helper function for determinant calculations
    Matrix createSubMatrix(Matrix mat, int pos) {
        Matrix temp(mat.colLength()-1, mat.rowLength()-1);
        bool skipped = false;
        for(int j = 0; j < mat.rowLength(); j++) {
            Vector v;
            if(j != pos) {
                for(int k = 1; k < mat.colLength(); k++) {
                    v.pushBack(mat[k][j]);
                }
            } else {
                skipped = true;
            }
            if(v.getSize() != 0) {
                if(skipped) {
                    temp.setCol(j-1, v);
                } else {
                    temp.setCol(j, v);
                }
            }
        }
        return temp;
    }
};

#endif //MATRIX_H
