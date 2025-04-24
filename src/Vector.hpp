#pragma once

#include <array>
#include <cstddef>
#include <initializer_list>

namespace math
{
    class Vector
    {
    private:
        size_t num_ = 0;
        std::array<double, 3> coords_ = {0, 0, 0};

    public:
        Vector() = default;
        Vector(const size_t num, const std::array<double, 3> &arr);
        Vector(const std::array<double, 3> &arr);
        Vector(std::initializer_list<double> list);

        Vector(const Vector &other) = default;            // Конструктор копирования
        Vector &operator=(const Vector &other) = default; // Оператор присваивания копированием

        Vector(Vector &&other) noexcept = default;            // Конструктор перемещения
        Vector &operator=(Vector &&other) noexcept = default; // Оператор присваивания перемещением

        double &operator[](const size_t index);
        const double &operator[](const size_t index) const;

        bool operator==(const Vector &other) const;
        bool operator!=(const Vector &other) const;

        void SetNum(const size_t num);
        size_t GetNum() const;
        void Normalize();
    };

    /**
     * Перегрузка унарного оператора вычитания
     */
    Vector operator-(const Vector &vec);

    /**
     * Перегрузка сложения векторов
     */
    Vector operator+(const Vector &lhs, const Vector &rhs);

    /**
     * Перегрузка вычитания векторов
     */
    Vector operator-(const Vector &lhs, const Vector &rhs);

    /**
     * Перегрузка скалярного умножения
     */
    double operator*(const Vector &lhs, const Vector &rhs);

    /**
     * Перегрузка умножения на скаляр слева
     */
    template <class ConstType>
    Vector operator*(const ConstType c, const Vector &vec)
    {
        return Vector{{vec[0] * c,
                       vec[1] * c,
                       vec[2] * c}};
    }

    template <class ConstType>
    Vector operator*(const Vector &vec, const ConstType c)
    {
        return c * vec;
    }

    /**
     * Перегрузка векторного умножения
     */
    Vector operator%(const Vector &lhs, const Vector &rhs);

    /**
     * Структура для хеширования вектора при чтении из STL-файла
     */
    struct VectorReadHash
    {
        std::size_t operator()(const Vector &vec) const;
    };

    /**
     * Структура для хеширования вектора при использовании в unordered_map
     * и unordered_set
     */
    struct VectorHash
    {
        std::size_t operator()(const Vector &vec) const;
    };

    /**
     * Структура для хеширования пары векторов
     */
    struct PairVectorHash
    {
        std::size_t operator()(const std::pair<Vector, Vector> &pair) const;
    };

} // namespace math
