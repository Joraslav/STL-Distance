#include "AABBTree.hpp"
#include "MathOperations.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

namespace math
{
    AABBTree::AABBTree(const std::vector<Triangle> &triangles)
    {
        std::vector<Triangle> triangles_copy = triangles;
        root_ = BuildTree(triangles_copy, 0, triangles_copy.size());
    }

    std::unique_ptr<AABBTreeNode> AABBTree::BuildTree(std::vector<Triangle> &triangles,
                                                      size_t start, size_t end)
    {
        if (start >= end)
        {
            return nullptr;
        }

        auto node = std::make_unique<AABBTreeNode>();

        // Если это листовой узел
        if (end - start == 1)
        {
            node->triangle = triangles[start];
            ComputeBounds(triangles[start], node->min_bounds, node->max_bounds);
            return node;
        }

        // Вычисляем общие границы для текущего набора треугольников
        std::array<double, 3> min_bounds = {std::numeric_limits<double>::max(),
                                            std::numeric_limits<double>::max(),
                                            std::numeric_limits<double>::max()};
        std::array<double, 3> max_bounds = {std::numeric_limits<double>::lowest(),
                                            std::numeric_limits<double>::lowest(),
                                            std::numeric_limits<double>::lowest()};

        for (size_t i = start; i < end; ++i)
        {
            std::array<double, 3> tri_min, tri_max;
            ComputeBounds(triangles[i], tri_min, tri_max);

            for (size_t j = 0; j < 3; ++j)
            {
                min_bounds[j] = std::min(min_bounds[j], tri_min[j]);
                max_bounds[j] = std::max(max_bounds[j], tri_max[j]);
            }
        }

        node->min_bounds = min_bounds;
        node->max_bounds = max_bounds;

        // Сортируем треугольники по средней точке вдоль одной из осей (например, X)
        size_t axis = 1; // Можно выбрать ось с наибольшим размером
        size_t mid = start + (end - start) / 2;

        std::nth_element(triangles.begin() + start, triangles.begin() + mid, triangles.begin() + end,
                         [axis](const Triangle &a, const Triangle &b)
                         {
                             return a.GetMidlePoint()[axis] < b.GetMidlePoint()[axis];
                         });

        // Рекурсивно строим дочерние узлы
        node->left = BuildTree(triangles, start, mid);
        node->right = BuildTree(triangles, mid, end);

        return node;
    }

    void AABBTree::ComputeBounds(const Triangle &triangle,
                                 std::array<double, 3> &min_bounds,
                                 std::array<double, 3> &max_bounds)
    {
        min_bounds = {std::numeric_limits<double>::max(),
                      std::numeric_limits<double>::max(),
                      std::numeric_limits<double>::max()};
        max_bounds = {std::numeric_limits<double>::lowest(),
                      std::numeric_limits<double>::lowest(),
                      std::numeric_limits<double>::lowest()};

        for (size_t i = 0; i < 3; ++i)
        {
            const Vector &point = triangle.GetPoint(i);
            for (size_t j = 0; j < 3; ++j)
            {
                min_bounds[j] = std::min(min_bounds[j], point[j]);
                max_bounds[j] = std::max(max_bounds[j], point[j]);
            }
        }
    }

    double AABBTree::AABBToAABB(const AABBTreeNode *node1, const AABBTreeNode *node2) const
    {
        double distance = 0.0;

        for (size_t i = 0; i < 3; ++i)
        {
            if (node1->max_bounds[i] < node2->min_bounds[i])
            {
                distance += std::pow(node2->min_bounds[i] - node1->max_bounds[i], 2);
            }
            else if (node2->max_bounds[i] < node1->min_bounds[i])
            {
                distance += std::pow(node1->min_bounds[i] - node2->max_bounds[i], 2);
            }
        }

        return std::sqrt(distance);
    }

    void AABBTree::FindClosestRecursive(const AABBTreeNode *node1, const AABBTreeNode *node2,
                                        const Triangle *&closest1, const Triangle *&closest2,
                                        double &min_distance) const
    {
        if (!node1 || !node2)
        {
            return;
        }

        // Вычисляем минимальное расстояние между AABB узлами
        double distance = AABBToAABB(node1, node2);

        // Если расстояние больше текущего минимального, пропускаем узлы
        if (distance >= min_distance)
        {
            return;
        }

        // Если оба узла листовые, вычисляем расстояние между треугольниками
        if (node1->IsLeaf() && node2->IsLeaf())
        {
            Triangle tr_1 = node1->triangle;
            Triangle tr_2 = node2->triangle;
            double gjk = dist::GJK::Distance(tr_1, tr_2);
            double vert = MinVertexDistance(tr_1, tr_2);
            double segments = MinSegmentDistance(tr_1, tr_2);
            std::vector<double> dist_tr = {gjk, vert, segments};
            double triangle_distance = *std::min_element(dist_tr.begin(), dist_tr.end());

            if (triangle_distance < min_distance)
            {
                min_distance = triangle_distance;
                closest1 = &node1->triangle;
                closest2 = &node2->triangle;
            }
            return;
        }

        // Рекурсивно проверяем дочерние узлы
        if (node1->IsLeaf())
        {
            FindClosestRecursive(node1, node2->left.get(), closest1, closest2, min_distance);
            FindClosestRecursive(node1, node2->right.get(), closest1, closest2, min_distance);
        }
        else if (node2->IsLeaf())
        {
            FindClosestRecursive(node1->left.get(), node2, closest1, closest2, min_distance);
            FindClosestRecursive(node1->right.get(), node2, closest1, closest2, min_distance);
        }
        else
        {
            FindClosestRecursive(node1->left.get(), node2->left.get(), closest1, closest2, min_distance);
            FindClosestRecursive(node1->left.get(), node2->right.get(), closest1, closest2, min_distance);
            FindClosestRecursive(node1->right.get(), node2->left.get(), closest1, closest2, min_distance);
            FindClosestRecursive(node1->right.get(), node2->right.get(), closest1, closest2, min_distance);
        }
    }

    void AABBTree::FindClosestTriangles(const AABBTree &other, const Triangle *&closest1,
                                        const Triangle *&closest2, double &min_distance) const
    {
        closest1 = nullptr;
        closest2 = nullptr;
        min_distance = std::numeric_limits<double>::max();

        FindClosestRecursive(root_.get(), other.root_.get(), closest1, closest2, min_distance);
    }

} // namespace math
