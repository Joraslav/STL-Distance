#include "Vector.hpp"

#include <gtest/gtest.h>

#include <cmath>

using namespace math;

class VectorTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        v1 = Vector{1.0, 2.0, 3.0};
        v2 = Vector{4.0, 5.0, 6.0};
        v_zero = Vector{0.0, 0.0, 0.0};
    }

    Vector v1;
    Vector v2;
    Vector v_zero;
};

TEST_F(VectorTest, DefaultConstructor)
{
    Vector v;
    EXPECT_EQ(v[0], 0.0);
    EXPECT_EQ(v[1], 0.0);
    EXPECT_EQ(v[2], 0.0);
    EXPECT_EQ(v.GetNum(), 0);
}

TEST_F(VectorTest, InitializerListConstructor)
{
    Vector v{1.5, 2.5, 3.5};
    EXPECT_EQ(v[0], 1.5);
    EXPECT_EQ(v[1], 2.5);
    EXPECT_EQ(v[2], 3.5);

    Vector v_short{1.5};
    EXPECT_EQ(v_short[0], 1.5);
    EXPECT_EQ(v_short[1], 0.0);
    EXPECT_EQ(v_short[2], 0.0);
}

TEST_F(VectorTest, ArrayConstructor)
{
    std::array<double, 3> arr = {1.1, 2.2, 3.3};
    Vector v(arr);
    EXPECT_EQ(v[0], 1.1);
    EXPECT_EQ(v[1], 2.2);
    EXPECT_EQ(v[2], 3.3);
}

TEST_F(VectorTest, CopyConstructor)
{
    Vector v1_copy(v1);
    EXPECT_EQ(v1_copy[0], 1.0);
    EXPECT_EQ(v1_copy[1], 2.0);
    EXPECT_EQ(v1_copy[2], 3.0);

    v1_copy[0] = 5.0;
    EXPECT_EQ(v1[0], 1.0); // Проверка на глубокое копирование
}

TEST_F(VectorTest, AssignmentOperator)
{
    Vector v = v1;
    EXPECT_EQ(v[0], 1.0);
    EXPECT_EQ(v[1], 2.0);
    EXPECT_EQ(v[2], 3.0);

    v[0] = 5.0;
    EXPECT_EQ(v1[0], 1.0); // Проверка на глубокое копирование
}

TEST_F(VectorTest, ElementAccess)
{
    EXPECT_EQ(v1[0], 1.0);
    EXPECT_EQ(v1[1], 2.0);
    EXPECT_EQ(v1[2], 3.0);

    v1[0] = 5.0;
    EXPECT_EQ(v1[0], 5.0);

    const Vector &v_const = v1;
    EXPECT_EQ(v_const[0], 5.0);
}

TEST_F(VectorTest, EqualityOperators)
{
    Vector v1_copy = v1;
    EXPECT_TRUE(v1 == v1_copy);
    EXPECT_FALSE(v1 != v1_copy);

    EXPECT_FALSE(v1 == v2);
    EXPECT_TRUE(v1 != v2);
}

TEST_F(VectorTest, Addition)
{
    Vector sum = v1 + v2;
    EXPECT_EQ(sum[0], 5.0);
    EXPECT_EQ(sum[1], 7.0);
    EXPECT_EQ(sum[2], 9.0);

    Vector sum_with_zero = v1 + v_zero;
    EXPECT_EQ(sum_with_zero[0], v1[0]);
    EXPECT_EQ(sum_with_zero[1], v1[1]);
    EXPECT_EQ(sum_with_zero[2], v1[2]);
}

TEST_F(VectorTest, Subtraction)
{
    Vector diff = v2 - v1;
    EXPECT_EQ(diff[0], 3.0);
    EXPECT_EQ(diff[1], 3.0);
    EXPECT_EQ(diff[2], 3.0);

    Vector diff_with_zero = v1 - v_zero;
    EXPECT_EQ(diff_with_zero[0], v1[0]);
    EXPECT_EQ(diff_with_zero[1], v1[1]);
    EXPECT_EQ(diff_with_zero[2], v1[2]);
}

TEST_F(VectorTest, DotProduct)
{
    double dot = v1 * v2;
    EXPECT_EQ(dot, 32.0); // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32

    double dot_with_zero = v1 * v_zero;
    EXPECT_EQ(dot_with_zero, 0.0);
}

TEST_F(VectorTest, ScalarMultiplication)
{
    Vector scaled_left = 2.0 * v1;
    EXPECT_EQ(scaled_left[0], 2.0);
    EXPECT_EQ(scaled_left[1], 4.0);
    EXPECT_EQ(scaled_left[2], 6.0);

    Vector scaled_right = v1 * 3.0;
    EXPECT_EQ(scaled_right[0], 3.0);
    EXPECT_EQ(scaled_right[1], 6.0);
    EXPECT_EQ(scaled_right[2], 9.0);
}

TEST_F(VectorTest, CrossProduct)
{
    Vector cross = v1 % v2;
    EXPECT_EQ(cross[0], -3.0); // 2*6 - 3*5 = 12 - 15 = -3
    EXPECT_EQ(cross[1], 6.0);  // 3*4 - 1*6 = 12 - 6 = 6
    EXPECT_EQ(cross[2], -3.0); // 1*5 - 2*4 = 5 - 8 = -3

    Vector cross_with_zero = v1 % v_zero;
    EXPECT_EQ(cross_with_zero[0], 0.0);
    EXPECT_EQ(cross_with_zero[1], 0.0);
    EXPECT_EQ(cross_with_zero[2], 0.0);
}

TEST_F(VectorTest, Normalize)
{
    Vector v{1.0, 2.0, 3.0};
    v.Normalize();
    double length = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    EXPECT_NEAR(length, 1.0, 1e-10);

    // v_zero.Normalize();
    // EXPECT_EQ(v_zero[0], 0.0);
    // EXPECT_EQ(v_zero[1], 0.0);
    // EXPECT_EQ(v_zero[2], 0.0);
}

TEST_F(VectorTest, NumAccessors)
{
    Vector v;
    EXPECT_EQ(v.GetNum(), 0);

    v.SetNum(42);
    EXPECT_EQ(v.GetNum(), 42);
}

TEST_F(VectorTest, VectorHash)
{
    VectorHash hasher;
    Vector v1_copy = v1;

    EXPECT_EQ(hasher(v1), hasher(v1_copy));
    EXPECT_NE(hasher(v1), hasher(v2));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}