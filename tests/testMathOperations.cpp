#include "Matrix.hpp"
#include "MathOperations.hpp"
#include "Vector.hpp"

#include <gtest/gtest.h>

#include <cmath>
#include <stdexcept>

using namespace math;

class MathOperationsTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        mat(0, 0) = 1.0;
        mat(0, 1) = 0.0;
        mat(0, 2) = 0.0;
        mat(1, 0) = 0.0;
        mat(1, 1) = 2.0;
        mat(1, 2) = 0.0;
        mat(2, 0) = 0.0;
        mat(2, 1) = 0.0;
        mat(2, 2) = 3.0;

        vec = Vector{1.0, 2.0, 3.0};
    }

    Matrix<double> mat;
    Vector vec;
};

TEST_F(MathOperationsTest, MatrixVectorMultiplication)
{
    Vector result = mat * vec;
    EXPECT_DOUBLE_EQ(result[0], (1.0));
    EXPECT_DOUBLE_EQ(result[1], 4.0);
    EXPECT_DOUBLE_EQ(result[2], 9.0);
}

TEST_F(MathOperationsTest, Norm2)
{
    EXPECT_DOUBLE_EQ(Norm2(vec), std::sqrt(14.0));
    EXPECT_DOUBLE_EQ(Norm2(Vector{0.0, 0.0, 0.0}), 0.0);
}

TEST_F(MathOperationsTest, Normalize)
{
    Vector normalized = Normalize(vec);
    double length = std::sqrt(14.0);
    EXPECT_DOUBLE_EQ(normalized[0], 1.0 / length);
    EXPECT_DOUBLE_EQ(normalized[1], 2.0 / length);
    EXPECT_DOUBLE_EQ(normalized[2], 3.0 / length);

    EXPECT_THROW(Normalize(Vector{0.0, 0.0, 0.0}), std::invalid_argument);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}