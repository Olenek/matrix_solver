#include "matrix.cpp"

int main(){
    RowVector v = {{1, -2, 2, -1, 8},
                   {2, -4, 1,  1, 7},
                   {1, -2, 0,  1, 2}};
    Matrix m = Matrix(v);
    m.getSolutions();
}
