#pragma once

#include "Vector.hpp"

#include <deque>
#include <unordered_map>
#include <utility>
#include <vector>

namespace math
{
    class AltMDM
    {
    public:
        // Структура для хранения состояния алгоритма
        struct State
        {
            std::vector<double> steps;
            std::vector<double> alpha;
            std::vector<double> beta;
            Vector u;
            Vector v;
        };

        using CacheKey = std::pair<Vector, Vector>;
        using InnerProductCache = std::unordered_map<CacheKey, double, PairVectorHash>;
        using CacheOrder = std::deque<CacheKey>;

        struct LRUCache
        {
            InnerProductCache cache;
            CacheOrder order;
        };

        AltMDM();

        void RemoveLeastRecentlyUsed();
        double GetCachedInnerProduct(const Vector &a, const Vector &b);

        void Initialize(const std::vector<Vector> &X, const std::vector<Vector> &Y);
        Vector ComputeConvexCombination(const std::vector<Vector> &points, const std::vector<double> &coeffs);
        void UpdateU(const std::vector<Vector> &X);
        void UpdateV(const std::vector<Vector> &Y);
        size_t FindMaxProjectionIndex(const std::vector<Vector> &points, const Vector &direction,
                                      const std::vector<double> &coeffs);
        size_t FindMinProjectionIndex(const std::vector<Vector> &points, const Vector &direction);
        double ComputeStep(const Vector &x_max, const Vector &x_min, double coeff, const Vector &direction);
        double ComputeDelta(const std::vector<Vector> &X, const std::vector<Vector> &Y);

        std::pair<Vector, Vector> FindMinDistance(const std::vector<Vector> &X,
                                                  const std::vector<Vector> &Y,
                                                  double epsilon = 1e-10,
                                                  size_t max_iterations = 1000);

        std::vector<double> GetSteps() const;

    private:
        State state_;
        LRUCache lru_cache_;
    };
} // namespace math