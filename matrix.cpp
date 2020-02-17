#include <vector>
#include <iostream>

typedef std::vector<std::vector<float>> RowVector;
typedef std::vector<float> Row;

class Matrix {
private:
    RowVector matrix;
    unsigned rows;
    unsigned cols;
public:
    Matrix(const RowVector &v) {
        matrix = v;
        rows = v.size();
        cols = v[0].size();
    }

    Matrix operator+(const Matrix& other) {
        for (unsigned i = 0; i < rows; ++i) {
            for (unsigned j = 0; j < cols; ++j) {
                matrix[i][j] += other.matrix[i][j];
            }
        }
        return *this;
    }

    Matrix operator-(const Matrix& other) {
        for (unsigned i = 0; i < rows; ++i) {
            for (unsigned j = 0; j < cols; ++j) {
                matrix[i][j] -= other.matrix[i][j];
            }
        }
        return *this;
    }

    Matrix operator*(const float lambda) {
        for (unsigned i = 0; i < rows; ++i) {
            for (unsigned j = 0; j < cols; ++j) {
                matrix[i][j] *= lambda;
            }
        }
        return *this;
    }

    Matrix d(const unsigned row, const float lambda) {
        for (unsigned j = 0; j < cols; ++j) {
            matrix[row][j] *= lambda;
        }
        return *this;
    }

    Matrix l(const unsigned row1, const unsigned row2, const float lambda) {
        for (unsigned j = 0; j < cols; ++j) {
            matrix[row1][j] += (matrix[row2][j] * lambda);
        }
        return *this;
    }

    Matrix t(const unsigned row1, const unsigned row2) {
        Row tmp = matrix[row1];
        matrix[row1] = matrix[row2];
        matrix[row2] = tmp;
        return *this;
    }

    Matrix to_ref() {
        unsigned lgr = 0; //last_good_row
        unsigned lgc = 0; //last_good_row;
        while (lgr != (rows - 1) || lgc != (cols - 1)) {
            bool has_pivot = false;
            for (unsigned i = lgr; i < rows; ++i) {
                if (matrix[i][lgc] != 0) {
                    has_pivot = true;
                    if (i != lgr) {
                        this->t(lgr, i);
                    }
                    this->d(lgr, 1 / matrix[lgr][lgc]);
                    for (unsigned j = lgr + 1; j < rows; ++j) {
                        this->l(j, lgr, -(matrix[j][lgc] / matrix[lgr][lgc]));
                    }
                    break;
                }
            }
            if (not has_pivot) {
                lgc++;
            } else {
                lgr++;
                lgc++;
            }
        }
        return *this;
    }

    Matrix to_rref() {
        this->to_ref();
        int lgr = rows - 1; //last_good_row
        bool found_non_zero = false;
        for (int i = lgr; i >= 0; --i) { //search of non_zero_row
            for (int j = 0; j < cols; j++) {
                if (matrix[i][j] != 0) {
                    found_non_zero = true;
                    lgr = i;
                    break;
                }
            }
            if (found_non_zero) break;
        }
        for (int i = lgr; i > 0; --i) {
            for (int j = 0; j < cols; j++) {
                if (matrix[i][j] == 1) {
                    for (int k = i - 1; k >= 0; --k) {
                        this->l(k, i, -matrix[k][j]);
                    }
                    break;
                }
            }
        }
        return *this;
    }

    void printMatrix() {
        float tmp;
        for (unsigned i = 0; i < rows; ++i) {
            for (unsigned j = 0; j < cols; ++j) {
                tmp = matrix[i][j];
                if (tmp == -0) std::cout << 0 << " ";
                else {
                    std::cout << tmp << " ";
                }
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }

    std::vector<std::pair<int, int>> get_pivots() {
        std::vector<std::pair<int, int>> pivots;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (matrix[i][j] == 1) {
                    pivots.push_back(std::make_pair(i, j));
                    break;
                }
            }
        }
        return pivots;
    }

    void getSolutions() {
        this->to_rref();
        for (std::pair<int, int> p : this->get_pivots()) {
            std::cout << "x[" << p.second << "] = " << matrix[p.first][cols - 1];
            bool is_pivot = true;
            for (int j = 0; j < cols - 1; j++) {
                if (matrix[p.first][j] == 1 && is_pivot) is_pivot = false;
                else if (matrix[p.first][j] != 0) {
                    if(matrix[p.first][j] > 0)
                        std::cout << " - " <<  matrix[p.first][j] << "x[" << j << "]";
                    else
                        std::cout << " + " << -matrix[p.first][j] << "x[" << j << "]";
                }
            }
            std::cout << "\n";
        }
    }
};
