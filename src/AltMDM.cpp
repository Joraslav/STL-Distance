#include "AltMDM.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <iostream>
#include "MathOperations.hpp"

namespace math
{
    constexpr size_t MAX_CACHE_SIZE = 2000;

    AltMDM::AltMDM() : state_(), lru_cache_() {}

    void AltMDM::RemoveLeastRecentlyUsed()
    {
        if (!lru_cache_.order.empty())
        {
            const auto &oldest_key = lru_cache_.order.front();
            lru_cache_.cache.erase(oldest_key);
            lru_cache_.order.pop_front();
        }
    }

    double AltMDM::GetCachedInnerProduct(const Vector &a, const Vector &b)
    {
        CacheKey key = std::make_pair(a, b);
        auto it = lru_cache_.cache.find(key);

        if (it != lru_cache_.cache.end())
        {
            auto order_it = std::find(lru_cache_.order.begin(), lru_cache_.order.end(), key);
            if (order_it != lru_cache_.order.end())
            {
                lru_cache_.order.erase(order_it);
            }
            lru_cache_.order.push_back(key);
            return it->second;
        }

        double result = a * b;
        lru_cache_.cache[key] = result;
        lru_cache_.order.push_back(key);

        if (lru_cache_.cache.size() > MAX_CACHE_SIZE)
        {
            RemoveLeastRecentlyUsed();
        }

        return result;
    }

    void AltMDM::Initialize(const std::vector<Vector> &X, const std::vector<Vector> &Y)
    {
        if (X.empty() || Y.empty())
        {
            throw std::invalid_argument("Input point sets cannot be empty");
        }

        state_.alpha.assign(X.size(), 1.0 / static_cast<double>(X.size()));
        state_.beta.assign(Y.size(), 1.0 / static_cast<double>(Y.size()));
        state_.u = ComputeConvexCombination(X, state_.alpha);
        state_.v = ComputeConvexCombination(Y, state_.beta);
    }

    Vector AltMDM::ComputeConvexCombination(const std::vector<Vector> &points, const std::vector<double> &coeffs)
    {
        Vector result;
        for (size_t i = 0; i < points.size(); ++i)
        {
            result = result + (coeffs[i] * points[i]);
        }
        return result;
    }

    void AltMDM::UpdateU(const std::vector<Vector> &X)
    {
        const Vector diff = state_.u - state_.v;
        const size_t i_max = FindMaxProjectionIndex(X, diff, state_.alpha);
        const size_t i_min = FindMinProjectionIndex(X, diff);

        const double step = ComputeStep(X[i_max], X[i_min], state_.alpha[i_max], diff);

        const double alpha_update = step * state_.alpha[i_max];
        state_.alpha[i_max] -= alpha_update;
        state_.alpha[i_min] += alpha_update;
        state_.u = state_.u + alpha_update * (X[i_min] - X[i_max]);
    }

    void AltMDM::UpdateV(const std::vector<Vector> &Y)
    {
        const Vector diff = state_.v - state_.u;
        const size_t j_max = FindMaxProjectionIndex(Y, diff, state_.beta);
        const size_t j_min = FindMinProjectionIndex(Y, diff);

        const double step = ComputeStep(Y[j_max], Y[j_min], state_.beta[j_max], diff);

        const double beta_update = step * state_.beta[j_max];
        state_.beta[j_max] -= beta_update;
        state_.beta[j_min] += beta_update;
        state_.v = state_.v + beta_update * (Y[j_min] - Y[j_max]);
    }

    size_t AltMDM::FindMaxProjectionIndex(const std::vector<Vector> &points, const Vector &direction,
                                          const std::vector<double> &coeffs)
    {
        size_t max_index = 0;
        double max_proj = -std::numeric_limits<double>::max();

        for (size_t i = 0; i < points.size(); ++i)
        {
            if (coeffs[i] > 0)
            {
                const double proj = GetCachedInnerProduct(points[i], direction);
                if (proj > max_proj)
                {
                    max_proj = proj;
                    max_index = i;
                }
            }
        }
        return max_index;
    }

    size_t AltMDM::FindMinProjectionIndex(const std::vector<Vector> &points, const Vector &direction)
    {
        size_t min_index = 0;
        double min_proj = std::numeric_limits<double>::max();

        for (size_t i = 0; i < points.size(); ++i)
        {
            const double proj = GetCachedInnerProduct(points[i], direction);
            if (proj < min_proj)
            {
                min_proj = proj;
                min_index = i;
            }
        }
        return min_index;
    }

    double AltMDM::ComputeStep(const Vector &x_max, const Vector &x_min, double coeff, const Vector &direction)
    {
        const Vector diff = x_min - x_max;
        const double denominator = diff * diff;
        const double numerator = direction * (x_max - x_min);
        const double step = numerator / (coeff * denominator);
        state_.steps.push_back(std::clamp(step, 0.0, 1.0));
        return std::clamp(step, 0.0, 1.0);
    }

    double AltMDM::ComputeDelta(const std::vector<Vector> &X, const std::vector<Vector> &Y)
    {
        const Vector diff = state_.u - state_.v;
        double max_proj_x = -std::numeric_limits<double>::max();
        double min_proj_x = std::numeric_limits<double>::max();
        double max_proj_y = -std::numeric_limits<double>::max();
        double min_proj_y = std::numeric_limits<double>::max();

        for (size_t i = 0; i < X.size(); ++i)
        {
            const double proj_x = GetCachedInnerProduct(X[i], diff);
            if (state_.alpha[i] > 0)
            {
                max_proj_x = std::max(max_proj_x, proj_x);
            }
            min_proj_x = std::min(min_proj_x, proj_x);
        }

        for (size_t j = 0; j < Y.size(); ++j)
        {
            const double proj_y = GetCachedInnerProduct(Y[j], -diff);
            if (state_.beta[j] > 0)
            {
                max_proj_y = std::max(max_proj_y, proj_y);
            }
            min_proj_y = std::min(min_proj_y, proj_y);
        }

        return (max_proj_x - min_proj_x) + (max_proj_y - min_proj_y);
    }

    std::pair<Vector, Vector> AltMDM::FindMinDistance(const std::vector<Vector> &X,
                                                      const std::vector<Vector> &Y,
                                                      double epsilon,
                                                      size_t max_iterations)
    {
        if (X.empty() || Y.empty())
        {
            throw std::invalid_argument("Input point sets cannot be empty");
        }

        Initialize(X, Y);

        double delta = std::numeric_limits<double>::max();
        size_t iteration = 0;

        while (delta > epsilon && iteration < max_iterations)
        {
            UpdateU(X);
            UpdateV(Y);
            delta = ComputeDelta(X, Y);

            if (iteration % 1000 == 0)
            {
                std::cout << "Iteration: " << iteration
                          << ", Delta: " << delta
                          << ", Cache size: " << lru_cache_.cache.size() << std::endl;
            }

            ++iteration;
        }

        std::cout << "AltMDM:\n";

        std::cout << "Finale u - (" << state_.u[0] << ", "
                  << state_.u[1] << ", "
                  << state_.u[2] << ")" << std::endl;
        std::cout << "Finale v - (" << state_.v[0] << ", "
                  << state_.v[1] << ", "
                  << state_.v[2] << ")" << std::endl;
        std::cout << "Dist AltMDM: " << Norm2(state_.u - state_.v) << std::endl;

        return {state_.u, state_.v};
    }

    std::vector<double> AltMDM::GetSteps() const
    {
        return state_.steps;
    }
} // namespace math
