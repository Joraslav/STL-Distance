#pragma once

#include "Vector.hpp"
#include "Triangle.hpp"
#include "MathOperations.hpp"
#include "KDTree.hpp"
#include "GJK.hpp"
#include "MiddlePoint.hpp"

#include <vector>
#include <stdexcept>
#include <string>
#include <iostream>

namespace dist
{
    using namespace std::string_literals;

    enum class Body
    {
        Body_1,
        Body_2
    };

    class Distance
    {
    private:
        std::vector<math::Triangle> triangles_1_;
        std::vector<math::Triangle> triangles_2_;

        std::vector<math::Vector> points_body_1_;
        std::vector<math::Vector> points_body_2_;

        math::Triangle closest_triangle_1_;
        math::Triangle closest_triangle_2_;

        std::vector<math::Vector> CollectPoints(const Body &body) const;
        std::vector<math::MiddlePoint> CalculationMiddlePoints(const Body &body) const;
        std::vector<math::Triangle> FindIncidentTriangles(const Body &body, const math::Vector &target) const;

    public:
        Distance(const std::vector<math::Triangle> &triangles_1, const std::vector<math::Triangle> &triangles_2) : triangles_1_(triangles_1),
                                                                                                                   triangles_2_(triangles_2)
        {
            if (triangles_1_.empty() || triangles_2_.empty())
            {
                throw std::invalid_argument("Bodys cannot be empty!"s);
            }
        }

        void CollectPointsFromBodys();
        const std::vector<math::Vector> &GetPointsBody(const Body &body) const;

        std::pair<math::Vector, math::Vector> ClosestPointsKDTree() const;

        double FindDistanceBetweenBody();
    };

} // namespace dist
