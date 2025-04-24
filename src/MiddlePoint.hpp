#pragma once

#include "Vector.hpp"

namespace math
{

    class MiddlePoint
    {
    private:
        size_t num_ = 0;
        Vector point_;

    public:
        MiddlePoint() = default;
        MiddlePoint(const size_t num, const Vector &point) : num_(num), point_(point) {}
        MiddlePoint(const Vector &point) : point_(point) {};

        size_t GetNum() const;
        const Vector &GetPoint() const;
    };

} // namespace math
