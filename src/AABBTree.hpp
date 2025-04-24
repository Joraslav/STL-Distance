#pragma once

#include "Triangle.hpp"
#include "GJK.hpp"

#include <array>
#include <memory>
#include <vector>

namespace math
{
    struct AABBTreeNode
    {
        std::unique_ptr<AABBTreeNode> left;
        std::unique_ptr<AABBTreeNode> right;

        Triangle triangle;

        std::array<double, 3> min_bounds;
        std::array<double, 3> max_bounds;

        AABBTreeNode() = default;

        bool IsLeaf() const { return !left && !right; }
    };

    class AABBTree
    {
    private:
        std::unique_ptr<AABBTreeNode> root_;

        std::unique_ptr<AABBTreeNode> BuildTree(std::vector<Triangle> &triangles,
                                                size_t start, size_t end);
        void ComputeBounds(const Triangle &triangle,
                           std::array<double, 3> &min_bounds, std::array<double, 3> &max_bounds);

        double AABBToAABB(const AABBTreeNode *node1, const AABBTreeNode *node2) const;

        void FindClosestRecursive(const AABBTreeNode *node1, const AABBTreeNode *node2,
                                  const Triangle *&closest1, const Triangle *&closest2,
                                  double &min_distance) const;

    public:
        AABBTree(const std::vector<Triangle> &triangles);
        ~AABBTree() = default;

        void FindClosestTriangles(const AABBTree &other, const Triangle *&closest1,
                                  const Triangle *&closest2, double &min_distance) const;
    };

} // namespace math
