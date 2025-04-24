#include "MathOperations.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>

namespace math
{
    Vector operator*(const Matrix<double> &mat, const Vector &vec)
    {
        Vector result;
        for (size_t i = 0; i < 3; ++i)
        {
            double sum = 0.0;
            for (size_t j = 0; j < 3; ++j)
            {
                sum += mat(i, j) * vec[j];
            }
            result[i] = sum;
        }
        return result;
    }

    double Norm2(const Vector &vec) { return std::sqrt(vec * vec); }

    Vector Normalize(const Vector &vec)
    {
        const double length = Norm2(vec);
        if (length == 0.0)
        {
            throw std::invalid_argument("Cannot normalize zero vector");
        }

        return Vector{
            vec[0] / length,
            vec[1] / length,
            vec[2] / length};
    }

    double DistancePointToTriangle(const Vector &point, const Triangle &triangle)
    {
        const Vector &A = triangle.GetPoint(0);
        const Vector &B = triangle.GetPoint(1);
        const Vector &C = triangle.GetPoint(2);

        // Векторы треугольника
        Vector AB = B - A;
        Vector AC = C - A;
        Vector AP = point - A;

        // Нормаль к плоскости треугольника
        Vector normal = AB % AC;
        normal.Normalize();

        // Проекция точки на плоскость треугольника
        double distance_to_plane = AP * normal;
        Vector projection = point - distance_to_plane * normal;

        // Проверяем, находится ли проекция внутри треугольника
        Vector AB_proj = B - projection;
        Vector AC_proj = C - projection;
        Vector BC_proj = C - B;

        Vector cross1 = AB % AC_proj;
        Vector cross2 = AC % AB_proj;
        Vector cross3 = BC_proj % (projection - B);

        if ((cross1 * normal >= 0) && (cross2 * normal >= 0) && (cross3 * normal >= 0))
        {
            // Проекция внутри треугольника
            return std::abs(distance_to_plane);
        }

        // Если проекция вне треугольника, вычисляем расстояние до ближайшего ребра
        double distance_to_AB = std::sqrt(std::pow((AP * AB) / (AB * AB), 2));
        double distance_to_AC = std::sqrt(std::pow((AP * AC) / (AC * AC), 2));
        double distance_to_BC = std::sqrt(std::pow(((point - B) * BC_proj) / (BC_proj * BC_proj), 2));

        return std::min({distance_to_AB, distance_to_AC, distance_to_BC});
    }

    double MinVertexDistance(const Triangle &tr_a, const Triangle &tr_b)
    {
        std::vector<Vector> vert_a = {
            tr_a.GetPoint(0),
            tr_a.GetPoint(1),
            tr_a.GetPoint(2)};
        std::vector<Vector> vert_b = {
            tr_b.GetPoint(0),
            tr_b.GetPoint(1),
            tr_b.GetPoint(2)};

        std::vector<double> distance_vertexes;
        for (const Vector &vert_1 : vert_a)
        {
            for (const Vector &vert_2 : vert_b)
            {
                distance_vertexes.push_back(Norm2(vert_1 - vert_2));
            }
        }

        return *std::min_element(distance_vertexes.begin(), distance_vertexes.end());
    }

    double PointToSegment(const Vector &point, const Segment &segment)
    {
        const Vector ab = segment.end - segment.begin;
        const Vector ap = point - segment.begin;
        double t = (ap * ab) / (ab * ab);
        t = std::max(0.0, std::min(1.0, t));

        const Vector proj = segment.begin + t * ab;
        return Norm2(point - proj);
    }

    double SegmentToSegment(const Segment &seg_a, const Segment &seg_b)
    {
        // Проверка между внутренними точками
        Vector u = seg_a.end - seg_a.begin;
        Vector v = seg_b.end - seg_b.begin;
        Vector w = seg_b.begin - seg_a.begin;

        double a = u * u; // u · u
        double b = u * v; // u · v
        double c = v * v; // v · v
        double d = u * w; // u · w
        double e = v * w; // v · w

        double det = a * c - b * b; // Определитель матрицы системы

        double s, t;
        if (det != 0.0) // Если отрезки не параллельны
        {
            s = (b * e - c * d) / det;
            t = (a * e - b * d) / det;
        }
        else // Если отрезки параллельны
        {
            s = 0.0;
            t = std::clamp(d / a, 0.0, 1.0);
        }

        // Ограничиваем s и t отрезком [0, 1]
        s = std::clamp(s, 0.0, 1.0);
        t = std::clamp(t, 0.0, 1.0);

        // Вычисляем ближайшие точки
        Vector p = seg_a.begin + t * u;
        Vector q = seg_b.begin + s * v;

        const double dist = Norm2(p - q); // Расстояние между ближайшими точками
        return dist;

        // const std::vector<double> t_vals = {0.0, 0.5, 1.0};
        // double dist = std::numeric_limits<double>::max();

        // for (const double &t1 : t_vals)
        // {
        //     for (const double &t2 : t_vals)
        //     {
        //         Vector p1 = seg_a.begin + t1 * (seg_a.end - seg_a.begin);
        //         Vector p2 = seg_b.begin + t2 * (seg_b.end - seg_b.begin);
        //         dist = std::min(dist, Norm2(p1 - p2));
        //     }
        // }

        // return dist;
    }

    double MinSegmentDistance(const Triangle &tr_a, const Triangle &tr_b)
    {
        const std::vector<Segment> segments_a = {
            {tr_a.GetPoint(0), tr_a.GetPoint(1)},
            {tr_a.GetPoint(1), tr_a.GetPoint(2)},
            {tr_a.GetPoint(2), tr_a.GetPoint(0)}};
        const std::vector<Segment> segments_b = {
            {tr_b.GetPoint(0), tr_b.GetPoint(1)},
            {tr_b.GetPoint(1), tr_b.GetPoint(2)},
            {tr_b.GetPoint(2), tr_b.GetPoint(0)}};

        std::vector<double> segments_dist;
        for (size_t i = 0; i != 3; ++i)
        {
            for (size_t j = 0; j != 3; ++j)
            {
                segments_dist.push_back(PointToSegment(tr_a.GetPoint(i), segments_b[j]));
                segments_dist.push_back(PointToSegment(tr_b.GetPoint(i), segments_a[j]));
                segments_dist.push_back(SegmentToSegment(segments_a[i], segments_b[j]));
            }
        }

        return *std::min_element(segments_dist.begin(), segments_dist.end());
    }
} // namespace math
