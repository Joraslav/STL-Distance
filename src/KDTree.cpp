#include "KDTree.hpp"

#include <limits>
#include <cmath> // Для std::pow и std::sqrt

namespace math
{
    using namespace std::string_literals;

    Vector KDTree::NearestNeighbor(const Vector &target) const
    {
        if (!root_)
        {
            throw std::runtime_error("KDTree is empty. Cannot find nearest neighbor."s);
        }

        Vector best;
        double best_dist = std::numeric_limits<double>::max();
        NearestNeighborSearch(root_.get(), target, 0, best, best_dist);
        return best;
    }

    std::unique_ptr<KDTree::Node> KDTree::BuildTree(std::vector<Vector>::iterator begin, std::vector<Vector>::iterator end, size_t depth)
    {
        if (begin >= end)
            return nullptr;

        size_t axis = depth % k_;
        auto mid = begin + (end - begin) / 2;

        std::nth_element(begin, mid, end, [axis](const Vector &a, const Vector &b)
                         { return a[axis] < b[axis]; });

        auto node = std::make_unique<Node>(*mid);
        node->left = BuildTree(begin, mid, depth + 1);
        node->right = BuildTree(mid + 1, end, depth + 1);

        return node;
    }

    void KDTree::NearestNeighborSearch(const Node *node, const Vector &target, size_t depth, Vector &best, double &best_dist) const
    {
        if (!node)
            return;

        // Вычисление расстояния
        double dist = 0;
        for (size_t i = 0; i < 3; ++i)
        {
            dist += std::pow(node->point[i] - target[i], 2);
        }

        if (dist < best_dist && dist > 0)
        {
            best = node->point;
            best_dist = dist;
        }

        size_t axis = depth % k_;
        double diff = target[axis] - node->point[axis];
        double diff_squared = diff * diff;

        const Node *first = diff < 0 ? node->left.get() : node->right.get();
        const Node *second = diff < 0 ? node->right.get() : node->left.get();

        NearestNeighborSearch(first, target, depth + 1, best, best_dist);

        if (diff_squared < best_dist)
        {
            NearestNeighborSearch(second, target, depth + 1, best, best_dist);
        }
    }
} // namespace math
