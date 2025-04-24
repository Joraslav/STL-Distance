#include "Distance.hpp"
#include "AABBTree.hpp"

#include <algorithm>
#include <limits>
#include <unordered_set>

namespace dist
{

    using namespace math;

    std::vector<Vector> Distance::CollectPoints(const Body &body) const
    {
        std::unordered_set<Vector, VectorHash> set_points;

        if (body == Body::Body_1)
        {
            for (const Triangle &triangle : triangles_1_)
            {
                for (size_t i = 0; i != 3; ++i)
                {
                    set_points.insert(triangle.GetPoint(i));
                }
            }
        }
        else if (body == Body::Body_2)
        {
            for (const Triangle &triangle : triangles_2_)
            {
                for (size_t i = 0; i != 3; ++i)
                {
                    set_points.insert(triangle.GetPoint(i));
                }
            }
        }
        else
        {
            throw std::logic_error("Unknown body type!"s);
        }

        std::vector<Vector> result(set_points.begin(), set_points.end());
        return result;
    }

    std::vector<math::MiddlePoint> Distance::CalculationMiddlePoints(const Body &body) const
    {
        std::vector<math::MiddlePoint> result;

        switch (body)
        {
        case Body::Body_1:
            for (const Triangle &triangle : triangles_1_)
            {
                result.emplace_back(MiddlePoint{triangle.GetNum(), triangle.GetMidlePoint()});
            }
            break;
        case Body::Body_2:
            for (const Triangle &triangle : triangles_2_)
            {
                result.emplace_back(MiddlePoint{triangle.GetNum(), triangle.GetMidlePoint()});
            }
            break;

        default:
            throw std::logic_error("Unknown argument in CalcalationMiddlePoints!"s);
            break;
        }

        return result;
    }

    std::vector<Triangle> Distance::FindIncidentTriangles(const Body &body, const Vector &target) const
    {
        std::unordered_set<Triangle, TriangleHash> set_triangles;

        switch (body)
        {
        case Body::Body_1:
            for (const Triangle &triangle : triangles_1_)
            {
                if (triangle.IsContainPoint(target.GetNum()))
                {
                    set_triangles.insert(triangle);
                }
            }
            break;
        case Body::Body_2:
            for (const Triangle &triangle : triangles_2_)
            {
                if (triangle.IsContainPoint(target.GetNum()))
                {
                    set_triangles.insert(triangle);
                }
            }
            break;

        default:
            throw std::logic_error("Unknown argument in FindIncidentTriangles!"s);
            break;
        }

        std::vector<Triangle> result(set_triangles.begin(), set_triangles.end());
        return result;
    }

    void Distance::CollectPointsFromBodys()
    {
        if (triangles_1_.empty() || triangles_2_.empty())
        {
            throw std::logic_error("Triangles are empty! Cannot collect points."s);
        }

        points_body_1_ = CollectPoints(Body::Body_1);
        points_body_2_ = CollectPoints(Body::Body_2);
    }

    const std::vector<math::Vector> &Distance::GetPointsBody(const Body &body) const
    {
        switch (body)
        {
        case Body::Body_1:
            return points_body_1_;
            break;
        case Body::Body_2:
            return points_body_2_;
            break;

        default:
            throw std::logic_error("Unknown argument in GetPointsBody!"s);
            break;
        }
    }

    std::pair<Vector, Vector> Distance::ClosestPointsKDTree() const
    {
        std::vector<Vector> points_1_copy = points_body_1_;
        std::vector<Vector> points_2_copy = points_body_2_;

        KDTree kd_tree_1(points_1_copy);
        KDTree kd_tree_2(points_2_copy);

        double min_dist = std::numeric_limits<double>::max();
        Vector closest_point_1, closest_point_2;

        if (points_1_copy.size() <= points_2_copy.size())
        {
            for (const Vector &point_1 : points_1_copy)
            {
                Vector closest_2 = kd_tree_2.NearestNeighbor(point_1);
                double dist = Norm2(point_1 - closest_2);
                if (dist < min_dist)
                {
                    min_dist = dist;
                    closest_point_1 = point_1;
                    closest_point_2 = closest_2;
                }
            }
        }
        else
        {
            for (const Vector &point_2 : points_2_copy)
            {
                Vector closest_1 = kd_tree_1.NearestNeighbor(point_2);
                double dist = Norm2(point_2 - closest_1);
                if (dist < min_dist)
                {
                    min_dist = dist;
                    closest_point_1 = closest_1;
                    closest_point_2 = point_2;
                }
            }
        }

        std::cout << "Num Point 1: "s << closest_point_1.GetNum() << std::endl;
        std::cout << "Num Point 2: "s << closest_point_2.GetNum() << std::endl;

        return {closest_point_1, closest_point_2};
    }

    double Distance::FindDistanceBetweenBody()
    {
        AABBTree tree_1(triangles_1_);
        AABBTree tree_2(triangles_2_);

        double distance = 0.0;
        const Triangle *tr_1 = nullptr, *tr_2 = nullptr;
        tree_1.FindClosestTriangles(tree_2, tr_1, tr_2, distance);

        closest_triangle_1_ = *tr_1;
        closest_triangle_2_ = *tr_2;

        return distance;
    }

} // namespace dist
