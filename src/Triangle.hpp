#pragma once

#include "Vector.hpp"

#include <array>
#include <vector>

namespace math
{
    class Triangle
    {
    private:
        size_t num_ = 0;
        Vector norm_;
        std::array<Vector, 3> points_;

    public:
        Triangle() = default;
        Triangle(const size_t num, const Vector &norm,
                 const Vector &p1, const Vector &p2, const Vector &p3);
        Triangle(const size_t num, const Vector &norm, const std::array<Vector, 3> &points);
        Triangle(const size_t num, const Vector &norm, const std::vector<Vector> &vec_points);
        Triangle(const std::vector<Vector> &vec_points);

        Triangle(const Triangle &other) = default;            // Конструктор копирования
        Triangle &operator=(const Triangle &other) = default; // Оператор присваивания копированием

        Triangle(Triangle &&other) noexcept = default;            // Конструктор перемещения
        Triangle &operator=(Triangle &&other) noexcept = default; // Оператор присваивания перемещением

        size_t GetNum() const;
        Vector GetNorm() const;
        const Vector &GetPoint(const size_t num) const;
        Vector GetEdge(const size_t num) const;
        Vector GetMidlePoint() const;

        bool CheckingExistencePoint(const size_t num) const;
        bool IsContainPoint(const size_t num) const;

        bool operator==(const Triangle &other) const;
        bool operator!=(const Triangle &other) const;
    };

    /**
     * Структура для хеширования треугольников
     */
    struct TriangleHash
    {
        std::size_t operator()(const Triangle &triangle) const;
    };

} // namespace math
