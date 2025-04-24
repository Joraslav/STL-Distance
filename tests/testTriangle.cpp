#include "Triangle.hpp"
#include "Vector.hpp"

#include <gtest/gtest.h>

#include <cmath>
#include <optional>
#include <stdexcept>

using namespace math;

class TriangleTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        v1 = Vector{1.0, 0.0, 0.0};
        v2 = Vector{0.0, 1.0, 0.0};
        v3 = Vector{0.0, 0.0, 1.0};
        normal = Vector{1.0, 1.0, 1.0};
        normal.Normalize();

        triangle1.emplace(1, normal, v1, v2, v3);
        triangle2.emplace(2, normal, std::array<Vector, 3>{v3, v2, v1});
        triangle_empty.emplace(3, Vector{0, 0, 0}, Vector{0, 0, 0}, Vector{0, 0, 0}, Vector{0, 0, 0});
    }

    Vector v1, v2, v3, normal;
    std::optional<Triangle> triangle1;
    std::optional<Triangle> triangle2;
    std::optional<Triangle> triangle_empty;
};

TEST_F(TriangleTest, Constructors)
{
    // Проверка конструктора с тремя векторами
    EXPECT_EQ(triangle1->GetNum(), 1); // Используем -> для доступа к optional
    EXPECT_EQ((*triangle1).GetNorm()[0], normal[0]);
    EXPECT_EQ(triangle1->GetPoint(0)[0], 1.0);

    // Проверка конструктора с массивом
    std::array<Vector, 3> points = {v1, v2, v3};
    Triangle t(4, normal, points);
    EXPECT_EQ(t.GetNum(), 4);
    EXPECT_EQ(t.GetPoint(1)[1], 1.0);

    // Проверка конструктора с вектором
    std::vector<Vector> vec_points = {v1, v2, v3};
    Triangle t2(5, normal, vec_points);
    EXPECT_EQ(t2.GetNum(), 5);
    EXPECT_EQ(t2.GetPoint(2)[2], 1.0);

    // Проверка исключения при недостаточном количестве точек
    std::vector<Vector> short_points = {v1, v2};
    EXPECT_THROW(Triangle(6, normal, short_points), std::invalid_argument);
}

TEST_F(TriangleTest, GetMethods)
{
    // Проверка GetNum()
    EXPECT_EQ(triangle1->GetNum(), 1);
    EXPECT_EQ(triangle2->GetNum(), 2);

    // Проверка GetNorm()
    Vector norm = triangle1->GetNorm();
    EXPECT_NEAR(norm[0], 1.0 / sqrt(3), 1e-10);
    EXPECT_NEAR(norm[1], 1.0 / sqrt(3), 1e-10);
    EXPECT_NEAR(norm[2], 1.0 / sqrt(3), 1e-10);

    // Проверка GetPoint()
    EXPECT_EQ(triangle1->GetPoint(0), v1);
    EXPECT_EQ(triangle1->GetPoint(1), v2);
    EXPECT_EQ(triangle1->GetPoint(2), v3);

    // Проверка недопустимого индекса в GetPoint()
    const Vector &invalid_point = triangle1->GetPoint(5);
    EXPECT_EQ(invalid_point[0], 0.0);
    EXPECT_EQ(invalid_point[1], 0.0);
    EXPECT_EQ(invalid_point[2], 0.0);
}

TEST_F(TriangleTest, EdgeCalculation)
{
    // Проверка GetEdge()
    Vector edge0 = triangle1->GetEdge(0);
    EXPECT_DOUBLE_EQ(edge0[0], -1.0);
    EXPECT_DOUBLE_EQ(edge0[1], 1.0);
    EXPECT_DOUBLE_EQ(edge0[2], 0.0);

    Vector edge1 = triangle1->GetEdge(1);
    EXPECT_DOUBLE_EQ(edge1[0], 0.0);
    EXPECT_DOUBLE_EQ(edge1[1], -1.0);
    EXPECT_DOUBLE_EQ(edge1[2], 1.0);

    Vector edge2 = triangle1->GetEdge(2);
    EXPECT_DOUBLE_EQ(edge2[0], 1.0);
    EXPECT_DOUBLE_EQ(edge2[1], 0.0);
    EXPECT_DOUBLE_EQ(edge2[2], -1.0);

    // Проверка недопустимого индекса ребра
    Vector invalid_edge = triangle1->GetEdge(5);
    EXPECT_DOUBLE_EQ(invalid_edge[0], 0.0);
    EXPECT_DOUBLE_EQ(invalid_edge[1], 0.0);
    EXPECT_DOUBLE_EQ(invalid_edge[2], 0.0);
}

TEST_F(TriangleTest, ComparisonOperators)
{
    Triangle t1 = *triangle1;
    Triangle t2(1, normal, v3, v2, v1);

    // Проверка оператора ==
    EXPECT_TRUE(*triangle1 == t1);
    EXPECT_FALSE(*triangle1 == t2);

    // Проверка оператора !=
    EXPECT_FALSE(*triangle1 != t1);
    EXPECT_TRUE(*triangle1 != t2);
}

TEST_F(TriangleTest, CopyAndMoveSemantics)
{
    // Проверка конструктора копирования
    Triangle t_copy(*triangle1);
    EXPECT_EQ(t_copy, *triangle1);

    // Проверка оператора присваивания копированием
    Triangle t_assign = *triangle1;
    EXPECT_EQ(t_assign, *triangle1);

    // Проверка конструктора перемещения (без std::move для временного объекта)
    Triangle t_move(Triangle(10, normal, v1, v2, v3));
    EXPECT_EQ(t_move.GetNum(), 10);

    // Проверка оператора присваивания перемещением
    Triangle temp(11, normal, v1, v2, v3);
    Triangle t_assign_move = std::move(temp);
    EXPECT_EQ(t_assign_move.GetNum(), 11);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}