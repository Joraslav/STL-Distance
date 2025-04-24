#include "Matrix.hpp"
#include "Vector.hpp"

#include <gtest/gtest.h>

using namespace math;

class MatrixTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Инициализация тестовых матриц
        mat1 = Matrix<double>();
        mat1(0, 0) = 1.0;
        mat1(0, 1) = 2.0;
        mat1(0, 2) = 3.0;
        mat1(1, 0) = 4.0;
        mat1(1, 1) = 5.0;
        mat1(1, 2) = 6.0;
        mat1(2, 0) = 7.0;
        mat1(2, 1) = 8.0;
        mat1(2, 2) = 9.0;

        mat2 = Matrix<double>();
        mat2(0, 0) = 2.0;
        mat2(0, 1) = 0.0;
        mat2(0, 2) = 0.0;
        mat2(1, 0) = 0.0;
        mat2(1, 1) = 2.0;
        mat2(1, 2) = 0.0;
        mat2(2, 0) = 0.0;
        mat2(2, 1) = 0.0;
        mat2(2, 2) = 2.0;

        identity = Matrix<double>();
        identity(0, 0) = 1.0;
        identity(0, 1) = 0.0;
        identity(0, 2) = 0.0;
        identity(1, 0) = 0.0;
        identity(1, 1) = 1.0;
        identity(1, 2) = 0.0;
        identity(2, 0) = 0.0;
        identity(2, 1) = 0.0;
        identity(2, 2) = 1.0;
    }

    Matrix<double> mat1;
    Matrix<double> mat2;
    Matrix<double> identity;
};

TEST_F(MatrixTest, DefaultConstructor)
{
    Matrix<double> m;
    for (size_t i = 0; i < 3; ++i)
    {
        for (size_t j = 0; j < 3; ++j)
        {
            EXPECT_EQ(m(i, j), 0.0);
        }
    }
}

TEST_F(MatrixTest, CopyConstructor)
{
    Matrix<double> copy(mat1);
    for (size_t i = 0; i < 3; ++i)
    {
        for (size_t j = 0; j < 3; ++j)
        {
            EXPECT_EQ(copy(i, j), mat1(i, j));
        }
    }
}

TEST_F(MatrixTest, MoveConstructor)
{
    Matrix<double> temp(mat1);
    Matrix<double> moved(std::move(temp));

    for (size_t i = 0; i < 3; ++i)
    {
        for (size_t j = 0; j < 3; ++j)
        {
            EXPECT_EQ(moved(i, j), mat1(i, j));
        }
    }
}

TEST_F(MatrixTest, AssignmentOperators)
{
    Matrix<double> copy;
    copy = mat1;
    for (size_t i = 0; i < 3; ++i)
    {
        for (size_t j = 0; j < 3; ++j)
        {
            EXPECT_EQ(copy(i, j), mat1(i, j));
        }
    }

    Matrix<double> moved;
    moved = std::move(copy);
    for (size_t i = 0; i < 3; ++i)
    {
        for (size_t j = 0; j < 3; ++j)
        {
            EXPECT_EQ(moved(i, j), mat1(i, j));
        }
    }
}

TEST_F(MatrixTest, ElementAccess)
{
    EXPECT_EQ(mat1(0, 0), 1.0);
    EXPECT_EQ(mat1(1, 1), 5.0);
    EXPECT_EQ(mat1(2, 2), 9.0);

    mat1(1, 1) = 10.0;
    EXPECT_EQ(mat1(1, 1), 10.0);
}

TEST_F(MatrixTest, PushCol)
{
    Vector v{1.0, 2.0, 3.0};
    mat2.PushCol(1, v);
    EXPECT_EQ(mat2(0, 1), 1.0);
    EXPECT_EQ(mat2(1, 1), 2.0);
    EXPECT_EQ(mat2(2, 1), 3.0);
}

TEST_F(MatrixTest, Transpose)
{
    mat1.t();
    EXPECT_EQ(mat1(0, 0), 1.0);
    EXPECT_EQ(mat1(0, 1), 4.0);
    EXPECT_EQ(mat1(0, 2), 7.0);
    EXPECT_EQ(mat1(1, 0), 2.0);
    EXPECT_EQ(mat1(1, 1), 5.0);
    EXPECT_EQ(mat1(1, 2), 8.0);
}

TEST_F(MatrixTest, Determinant)
{
    EXPECT_DOUBLE_EQ(mat2.det(), 8.0);
    EXPECT_DOUBLE_EQ(identity.det(), 1.0);

    Matrix<double> singular;
    singular(0, 0) = 1.0;
    singular(0, 1) = 2.0;
    singular(0, 2) = 3.0;
    singular(1, 0) = 4.0;
    singular(1, 1) = 5.0;
    singular(1, 2) = 6.0;
    singular(2, 0) = 7.0;
    singular(2, 1) = 8.0;
    singular(2, 2) = 9.0;
    EXPECT_DOUBLE_EQ(singular.det(), 0.0);
}

TEST_F(MatrixTest, Inverse)
{
    Matrix<double> inv = mat2.Inverse();
    EXPECT_DOUBLE_EQ(inv(0, 0), 0.5);
    EXPECT_DOUBLE_EQ(inv(1, 1), 0.5);
    EXPECT_DOUBLE_EQ(inv(2, 2), 0.5);

    Matrix<double> singular;
    singular(0, 0) = 1.0;
    singular(0, 1) = 2.0;
    singular(0, 2) = 3.0;
    singular(1, 0) = 4.0;
    singular(1, 1) = 5.0;
    singular(1, 2) = 6.0;
    singular(2, 0) = 7.0;
    singular(2, 1) = 8.0;
    singular(2, 2) = 9.0;
    EXPECT_THROW(singular.Inverse(), std::invalid_argument);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}