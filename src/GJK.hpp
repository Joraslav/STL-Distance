#pragma once

#include "Triangle.hpp"
#include "Vector.hpp"

namespace dist
{
    class GJK
    {
    private:
        static math::Vector Support(const math::Triangle &a, const math::Triangle &b, const math::Vector &direction);

    public:
        GJK();
        ~GJK();

        static double Distance(const math::Triangle &a, const math::Triangle &b);
    };
} // namespace dist
