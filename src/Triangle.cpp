#include "Triangle.hpp"

#include <stdexcept>

namespace math
{
    Triangle::Triangle(size_t num, const Vector &norm,
                       const Vector &p1, const Vector &p2, const Vector &p3)
        : num_(num), norm_(norm), points_({p1, p2, p3}) {}

    Triangle::Triangle(size_t num, const Vector &norm, const std::array<Vector, 3> &points)
        : num_(num), norm_(norm), points_(points) {}

    Triangle::Triangle(size_t num, const Vector &norm, const std::vector<Vector> &vec_points)
        : num_(num), norm_(norm)
    {
        if (vec_points.size() < 3)
        {
            throw std::invalid_argument("Vector must contain at least 3 points");
        }
        points_ = {vec_points[0], vec_points[1], vec_points[2]};
    }

    Triangle::Triangle(const std::vector<Vector> &vec_points)
        : num_(0), norm_({0.0, 0.0, 0.0})
    {
        if (vec_points.size() < 3)
        {
            throw std::invalid_argument("Vector must contain at least 3 points");
        }
        points_ = {vec_points[0], vec_points[1], vec_points[2]};
    }

    size_t Triangle::GetNum() const { return num_; }

    Vector Triangle::GetNorm() const { return norm_; }

    const Vector &Triangle::GetPoint(const size_t num) const
    {
        if (num > 2)
        {
            static const Vector zero_vec{0., 0., 0.};
            return zero_vec;
        }
        return points_[num];
    }

    Vector Triangle::GetEdge(size_t num) const
    {
        switch (num)
        {
        case 0:
            return points_[1] - points_[0];
        case 1:
            return points_[2] - points_[1];
        case 2:
            return points_[0] - points_[2];
        default:
            return Vector{0., 0., 0.};
        }
    }

    Vector Triangle::GetMidlePoint() const
    {
        std::array<double, 3> midle_point = {0., 0., 0.};

        for (const Vector &point : points_)
        {
            midle_point[0] += point[0];
            midle_point[1] += point[1];
            midle_point[2] += point[2];
        }

        midle_point[0] /= 3.0;
        midle_point[1] /= 3.0;
        midle_point[2] /= 3.0;
        
        return Vector(midle_point);
    }

    bool Triangle::CheckingExistencePoint(const size_t num) const
    {
        for (const Vector &point : points_)
        {
            if (point.GetNum() == num)
            {
                return true;
            }
        }
        return false;
    }

    bool Triangle::IsContainPoint(const size_t num) const
    {
        if (CheckingExistencePoint(num))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool Triangle::operator==(const Triangle &other) const
    {
        return num_ == other.num_ &&
               norm_ == other.norm_ &&
               points_ == other.points_;
    }

    bool Triangle::operator!=(const Triangle &other) const
    {
        return !(*this == other);
    }

    size_t TriangleHash::operator()(const Triangle &triangle) const
    {
        size_t seed = 0;

        // Лямбда-функция для комбинирования хэшей
        auto hash_combine = [&seed](size_t hash)
        {
            // Используется стандартная техника комбинирования хэшей
            seed ^= hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        };

        // Хэшируем номер треугольника
        size_t num_hash = std::hash<size_t>{}(triangle.GetNum());
        hash_combine(num_hash);

        // Хэшируем нормаль треугольника
        size_t norm_hash = VectorHash{}(triangle.GetNorm());
        hash_combine(norm_hash);

        // Хэшируем первую точку треугольника
        size_t point0_hash = VectorHash{}(triangle.GetPoint(0));
        hash_combine(point0_hash);

        // Хэшируем вторую точку треугольника
        size_t point1_hash = VectorHash{}(triangle.GetPoint(1));
        hash_combine(point1_hash);

        // Хэшируем третью точку треугольника
        size_t point2_hash = VectorHash{}(triangle.GetPoint(2));
        hash_combine(point2_hash);

        // Возвращаем итоговый хэш
        return seed;
    }

} // namespace math
