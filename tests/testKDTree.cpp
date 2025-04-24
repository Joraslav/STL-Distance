// filepath: d:\PROGRAMS\C++\STL-Distance\tests\test_testKDTree.cpp
#include "KDTree.hpp"
#include "Vector.hpp"

#include <gtest/gtest.h>

#include <vector>

using namespace math;

class KDTreeTest : public ::testing::Test
{
protected:
    std::vector<Vector> points;

    void SetUp() override
    {
        points = {
            Vector({1.0, 2.0, 3.0}),
            Vector({4.0, 5.0, 6.0}),
            Vector({7.0, 8.0, 9.0}),
            Vector({2.0, 3.0, 4.0}),
            Vector({6.0, 7.0, 8.0}),
        };
    }
};

TEST_F(KDTreeTest, BuildTree)
{
    // Проверяем, что дерево строится без исключений
    ASSERT_NO_THROW({ KDTree tree(points); });
}

TEST_F(KDTreeTest, NearestNeighbor)
{
    KDTree tree(points);
    Vector target({5.0, 5.0, 5.0});
    Vector nearest = tree.NearestNeighbor(target);

    // Проверяем, что ближайшая точка корректна
    EXPECT_EQ(nearest, Vector({4.0, 5.0, 6.0}));
}

TEST_F(KDTreeTest, NearestNeighborSinglePoint)
{
    std::vector<Vector> singlePoint = {Vector({1.0, 1.0, 1.0})};
    KDTree tree(singlePoint);
    Vector target({2.0, 2.0, 2.0});
    Vector nearest = tree.NearestNeighbor(target);

    // Проверяем, что единственная точка возвращается
    EXPECT_EQ(nearest, Vector({1.0, 1.0, 1.0}));
}

TEST_F(KDTreeTest, NearestNeighborEmptyTree)
{
    std::vector<Vector> emptyPoints;
    KDTree tree(emptyPoints);
    Vector target({1.0, 1.0, 1.0});

    // Проверяем, что выбрасывается исключение для пустого дерева
    EXPECT_THROW(tree.NearestNeighbor(target), std::runtime_error);
}

TEST_F(KDTreeTest, NearestNeighborFarPoint)
{
    KDTree tree(points);
    Vector target({100.0, 100.0, 100.0});
    Vector nearest = tree.NearestNeighbor(target);

    // Проверяем, что ближайшая точка корректна для удаленной цели
    EXPECT_EQ(nearest, Vector({7.0, 8.0, 9.0}));
}

// Main function for running all tests
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}