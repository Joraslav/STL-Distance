#pragma once

#include "Vector.hpp"

#include <stdexcept>
#include <vector>
#include <utility>

namespace math
{
    template <class T>
    class Matrix
    {
    private:
        std::vector<std::vector<T>> mat_ = {{0, 0, 0},
                                            {0, 0, 0},
                                            {0, 0, 0}};

        std::vector<T> &operator[](size_t index) { return mat_[index]; }
        std::vector<T> const &operator[](size_t index) const { return mat_[index]; };

    public:
        Matrix() = default; // Конструктор

        Matrix(const Matrix<T> &other);            // Конструктор копирования
        Matrix &operator=(const Matrix<T> &other); // Оператор присваивания копированием

        Matrix(Matrix<T> &&other) noexcept;               // Конструктор перемещения
        Matrix<T> &operator=(Matrix<T> &&other) noexcept; // Оператор присваивания перемещением

        T &operator()(const size_t i_index, const size_t j_index);
        T const &operator()(const size_t i_index, const size_t j_index) const;

        void PushCol(const size_t num_col, const Vector &vec);
        void t();
        Matrix<T> erase_i_j(const size_t i_index, const size_t j_index);
        T det();
        Matrix<T> Inverse();
    };

    template <class T>
    Matrix<T>::Matrix(const Matrix<T> &other)
    {
        mat_ = other.mat_;
    }

    template <class T>
    Matrix<T> &Matrix<T>::operator=(const Matrix<T> &other)
    {
        if (this != &other)
        {
            Matrix<T> temp(other);
            std::swap(mat_, temp.mat_);
        }
        return *this;
    }

    template <class T>
    Matrix<T>::Matrix(Matrix<T> &&other) noexcept
    {
        mat_ = other.mat_;
        other.mat_ = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    }

    template <class T>
    Matrix<T> &Matrix<T>::operator=(Matrix<T> &&other) noexcept
    {
        if (this != &other)
        {
            Matrix<T> temp(std::move(other));
            std::swap(mat_, temp.mat_);
        }
        return *this;
    }

    template <class T>
    T &Matrix<T>::operator()(const size_t i_index, const size_t j_index) { return mat_[i_index][j_index]; }

    template <class T>
    T const &Matrix<T>::operator()(const size_t i_index, const size_t j_index) const { return mat_[i_index][j_index]; }

    template <class T>
    void Matrix<T>::PushCol(const size_t num_col, const Vector &vec)
    {
        if (mat_.front().size() <= num_col)
        {
            std::invalid_argument("Incorrect num of col");
        }
        if (mat_.size() > 3)
        {
            std::invalid_argument("Incorrect size matrix and vector");
        }
        for (size_t i = 0; i != 3; ++i)
        {
            mat_[i][num_col] = vec[i];
        }
    }

    template <class T>
    void Matrix<T>::t()
    {
        // Для квадратной матрицы можно сделать оптимизированную in-place версию
        if (mat_.size() == mat_.front().size())
        {
            // In-place транспонирование для квадратной матрицы
            for (size_t i = 0; i < mat_.size(); ++i)
            {
                for (size_t j = i + 1; j < mat_.size(); ++j)
                {
                    std::swap(mat_[i][j], mat_[j][i]);
                }
            }
        }
        else
        {
            // Для прямоугольной матрицы - создание новой матрицы
            std::vector<std::vector<T>> temp(mat_[0].size(), std::vector<T>(mat_.size()));

            for (size_t i = 0; i < mat_.size(); ++i)
            {
                for (size_t j = 0; j < mat_[0].size(); ++j)
                {
                    temp[j][i] = std::move(mat_[i][j]);
                }
            }

            mat_ = std::move(temp);
        }
    }

    template <class T>
    Matrix<T> Matrix<T>::erase_i_j(const size_t i_index, const size_t j_index)
    {
        Matrix<T> rez = *this;
        for (size_t i = 0; i < rez.mat_.size(); ++i)
        {
            rez[i].erase(rez[i].begin() + j_index);
        }
        rez.mat_.erase(rez.mat_.begin() + i_index);
        return rez;
    }

    template <class T>
    T Matrix<T>::det()
    {
        T rez{0};
        T sign = 1;
        auto n_rows{mat_.size()}, n_cols{mat_.front().size()};
        if (mat_.front().size() == 1)
        {
            return mat_[0][0];
        }
        else if ((n_rows == 2) && (n_cols == 2))
        {
            return mat_[0][0] * mat_[1][1] - (mat_[0][1] * mat_[1][0]);
        }
        else
        {
            for (size_t i = 0; i < n_rows; ++i)
            {
                Matrix<T> sub_m = erase_i_j(i, 0);
                rez += sign * mat_[i][0] * sub_m.det();
                sign = (-1) * sign;
            }
        }
        return rez;
    }

    template <class T>
    Matrix<T> Matrix<T>::Inverse()
    {
        const T det = this->det();
        Matrix<T> rez = *this;
        if (det == 0)
        {
            throw std::invalid_argument("Determinant of Matrix is equal 0!!!");
        }
        for (size_t i = 0; i < 3; ++i)
        {
            for (size_t j = 0; j < 3; ++j)
            {
                Matrix<T> sub_m = this->erase_i_j(i, j);
                rez(i, j) = sub_m.det();
                if ((i + j) % 2 == 1)
                {
                    rez(i, j) = -rez(i, j);
                }
                rez(i, j) = rez(i, j) / det;
            }
        }
        rez.t();
        return rez;
    }

} // namespace math
