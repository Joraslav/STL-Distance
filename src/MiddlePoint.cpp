#include "MiddlePoint.hpp"

namespace math
{
    size_t MiddlePoint::GetNum() const { return num_; }

    const Vector &MiddlePoint::GetPoint() const { return point_; }
} // namespace math
