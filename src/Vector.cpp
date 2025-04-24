#include "Vector.hpp"

#include <cmath>
#include <functional>
#include <stdexcept>

namespace math
{
    Vector::Vector(const size_t num, const std::array<double, 3> &arr)
        : num_(num), coords_(arr) {}

    Vector::Vector(const std::array<double, 3> &arr)
        : num_(0), coords_(arr) {}

    Vector::Vector(std::initializer_list<double> list)
    {
        auto it = list.begin();
        for (size_t i = 0; i < 3 && it != list.end(); ++i, ++it)
        {
            coords_[i] = *it;
        }
    }

    double &Vector::operator[](const size_t index) { return coords_[index]; }

    const double &Vector::operator[](const size_t index) const { return coords_[index]; }

    bool Vector::operator==(const Vector &other) const
    {
        return coords_[0] == other.coords_[0] &&
               coords_[1] == other.coords_[1] &&
               coords_[2] == other.coords_[2];
    }

    bool Vector::operator!=(const Vector &other) const
    {
        return !(*this == other);
    }

    void Vector::SetNum(const size_t num) { num_ = num; }

    size_t Vector::GetNum() const { return num_; }

    void Vector::Normalize()
    {
        const double length = std::sqrt((*this) * (*this));
        if (length == 0.0)
        {
            throw std::invalid_argument("Cannot normalize zero vector");
        }
        for (auto &coord : coords_)
        {
            coord /= length;
        }
    }

    Vector operator-(const Vector &vec)
    {
        return Vector({-vec[0], -vec[1], -vec[2]});
    }

    Vector operator+(const Vector &lhs, const Vector &rhs)
    {
        return {lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2]};
    }

    Vector operator-(const Vector &lhs, const Vector &rhs)
    {
        return {lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2]};
    }

    double operator*(const Vector &lhs, const Vector &rhs)
    {
        return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2];
    }

    Vector operator%(const Vector &lhs, const Vector &rhs)
    {
        return {
            lhs[1] * rhs[2] - lhs[2] * rhs[1],
            lhs[2] * rhs[0] - lhs[0] * rhs[2],
            lhs[0] * rhs[1] - lhs[1] * rhs[0]};
    }

    size_t VectorReadHash::operator()(const Vector &vec) const
    {
        const size_t h1 = std::hash<double>{}(vec[0]);
        const size_t h2 = std::hash<double>{}(vec[1]);
        const size_t h3 = std::hash<double>{}(vec[2]);

        // Combine the hashes using a more robust method
        size_t seed = 0;
        seed ^= h1 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= h2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= h3 + 0x9e3779b9 + (seed << 6) + (seed >> 2);

        return seed;
    }

    size_t VectorHash::operator()(const Vector &vec) const
    {
        const size_t h_num = std::hash<size_t>{}(vec.GetNum());

        const size_t h1 = std::hash<double>{}(vec[0]);
        const size_t h2 = std::hash<double>{}(vec[1]);
        const size_t h3 = std::hash<double>{}(vec[2]);

        // Combine the hashes using a more efficient and robust method
        size_t hash = h_num;
        hash ^= h1 + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        hash ^= h2 + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        hash ^= h3 + 0x9e3779b9 + (hash << 6) + (hash >> 2);

        return hash;
    }

    size_t PairVectorHash::operator()(const std::pair<Vector, Vector> &pair) const
    {
        VectorHash vector_hash;
        size_t hash1 = vector_hash(pair.first);
        size_t hash2 = vector_hash(pair.second);

        // Комбинируем хэши двух векторов
        return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2));
    }

} // namespace math
