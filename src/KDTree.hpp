#pragma once

#include "Vector.hpp"

#include <algorithm>
#include <memory>
#include <vector>

namespace math
{
    class KDTree
    {
    private:
        struct Node
        {
            Vector point;
            std::unique_ptr<Node> left;
            std::unique_ptr<Node> right;

            Node(const Vector &p) : point(p), left(nullptr), right(nullptr) {}
        };

        std::unique_ptr<Node> root_;
        size_t k_; // Размерность пространства (в данном случае 3)

        std::unique_ptr<Node> BuildTree(std::vector<Vector>::iterator begin, std::vector<Vector>::iterator end, size_t depth);

        void NearestNeighborSearch(const Node *node, const Vector &target, size_t depth, Vector &best, double &best_dist) const;

    public:
        KDTree(std::vector<Vector> &points)
            : root_(nullptr), k_(3)
        {
            root_ = BuildTree(points.begin(), points.end(), 0);
        }

        Vector NearestNeighbor(const Vector &target) const;
    };
} // namespace math
