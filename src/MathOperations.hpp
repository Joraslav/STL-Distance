#pragma once

#include "Vector.hpp"
#include "Matrix.hpp"
#include "Triangle.hpp"

namespace math
{
    struct Segment
    {
        Vector begin, end;
    };

    Vector operator*(const Matrix<double> &mat, const Vector &vec);

    double Norm2(const Vector &vec);

    Vector Normalize(const Vector &vec);

    double DistancePointToTriangle(const Vector &point, const Triangle &triangle);

    double MinVertexDistance(const Triangle &tr_a, const Triangle &tr_b);

    double PointToSegment(const Vector &point, const Segment &segment);

    double SegmentToSegment(const Segment &seg_a, const Segment &seg_b);

    double MinSegmentDistance(const Triangle &tr_a, const Triangle &tr_b);
} // namespace math
