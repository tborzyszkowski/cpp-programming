#pragma once
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cmath>

class Matrix2x2 {
    double data_[2][2]{};

public:
    Matrix2x2() = default;

    Matrix2x2(double a, double b, double c, double d)
        : data_{{a, b}, {c, d}} {}

    // --- indeksowanie ---
    double* operator[](int row) { return data_[row]; }
    const double* operator[](int row) const { return data_[row]; }

    double& operator()(int row, int col) {
        if (row < 0 || row > 1 || col < 0 || col > 1)
            throw std::out_of_range("Matrix2x2::operator(): indeks poza zakresem");
        return data_[row][col];
    }
    const double& operator()(int row, int col) const {
        if (row < 0 || row > 1 || col < 0 || col > 1)
            throw std::out_of_range("Matrix2x2::operator(): indeks poza zakresem");
        return data_[row][col];
    }

    // --- arytmetyka ---
    Matrix2x2 operator+(const Matrix2x2& rhs) const {
        return {data_[0][0] + rhs.data_[0][0], data_[0][1] + rhs.data_[0][1],
                data_[1][0] + rhs.data_[1][0], data_[1][1] + rhs.data_[1][1]};
    }

    Matrix2x2 operator-(const Matrix2x2& rhs) const {
        return {data_[0][0] - rhs.data_[0][0], data_[0][1] - rhs.data_[0][1],
                data_[1][0] - rhs.data_[1][0], data_[1][1] - rhs.data_[1][1]};
    }

    Matrix2x2 operator*(const Matrix2x2& rhs) const {
        return {data_[0][0]*rhs.data_[0][0] + data_[0][1]*rhs.data_[1][0],
                data_[0][0]*rhs.data_[0][1] + data_[0][1]*rhs.data_[1][1],
                data_[1][0]*rhs.data_[0][0] + data_[1][1]*rhs.data_[1][0],
                data_[1][0]*rhs.data_[0][1] + data_[1][1]*rhs.data_[1][1]};
    }

    Matrix2x2 operator*(double scalar) const {
        return {data_[0][0]*scalar, data_[0][1]*scalar,
                data_[1][0]*scalar, data_[1][1]*scalar};
    }

    friend Matrix2x2 operator*(double scalar, const Matrix2x2& m) {
        return m * scalar;
    }

    // --- porównanie ---
    bool operator==(const Matrix2x2& rhs) const {
        constexpr double eps = 1e-9;
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                if (std::abs(data_[i][j] - rhs.data_[i][j]) > eps)
                    return false;
        return true;
    }

    bool operator!=(const Matrix2x2& rhs) const { return !(*this == rhs); }

    // --- metody ---
    double determinant() const {
        return data_[0][0]*data_[1][1] - data_[0][1]*data_[1][0];
    }

    Matrix2x2 transposed() const {
        return {data_[0][0], data_[1][0], data_[0][1], data_[1][1]};
    }

    // --- strumień ---
    friend std::ostream& operator<<(std::ostream& os, const Matrix2x2& m) {
        os << std::fixed << std::setprecision(0)
           << "[" << m.data_[0][0] << " " << m.data_[0][1]
           << "; " << m.data_[1][0] << " " << m.data_[1][1] << "]";
        return os;
    }
};
