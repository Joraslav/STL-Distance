#include "ReadSTL.hpp"
#include "Vector.hpp"
#include "Triangle.hpp"

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

using namespace math;
using namespace read_stl;

// Вспомогательная функция для создания тестового STL файла
void CreateTestSTLFile(const std::string &filename)
{
    std::ofstream out(filename, std::ios::binary);

    // Заголовок (80 байт)
    char header[80] = "Test STL File";
    out.write(header, 80);

    // Количество треугольников (4 байта)
    uint32_t num_triangles = 2;
    out.write(reinterpret_cast<char *>(&num_triangles), 4);

    // Первый треугольник
    float normal[3] = {0, 0, 1};
    float vertices[3][3] = {{0, 0, 0}, {1, 0, 0}, {0, 1, 0}};
    uint16_t attribute = 0;

    out.write(reinterpret_cast<char *>(normal), 12);
    out.write(reinterpret_cast<char *>(vertices), 36);
    out.write(reinterpret_cast<char *>(&attribute), 2);

    // Второй треугольник
    float normal2[3] = {0, 1, 0};
    float vertices2[3][3] = {{0, 0, 0}, {0, 0, 1}, {1, 0, 0}};

    out.write(reinterpret_cast<char *>(normal2), 12);
    out.write(reinterpret_cast<char *>(vertices2), 36);
    out.write(reinterpret_cast<char *>(&attribute), 2);
}

class ReadSTLTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Создаем тестовый STL файл
        test_filename = "test_file.stl";
        CreateTestSTLFile(test_filename);

        // Создаем несуществующий файл для тестирования ошибок
        non_existent_filename = "non_existent_file.stl";
    }

    void TearDown() override
    {
        // Удаляем тестовый файл после завершения тестов
        if (std::filesystem::exists(test_filename))
        {
            std::filesystem::remove(test_filename);
        }
    }

    std::string test_filename;
    std::string non_existent_filename;
};

TEST_F(ReadSTLTest, ReadValidSTLFile)
{
    std::vector<Triangle> triangles = GetTriangles(test_filename);

    // Проверяем количество треугольников
    ASSERT_EQ(triangles.size(), 2);

    // Проверяем первый треугольник
    EXPECT_EQ(triangles[0].GetNum(), 0);
    EXPECT_EQ(triangles[0].GetNorm(), Vector({0, 0, 1}));
    EXPECT_EQ(triangles[0].GetPoint(0), Vector({0, 0, 0}));
    EXPECT_EQ(triangles[0].GetPoint(1), Vector({1, 0, 0}));
    EXPECT_EQ(triangles[0].GetPoint(2), Vector({0, 1, 0}));

    // Проверяем второй треугольник
    EXPECT_EQ(triangles[1].GetNum(), 1);
    EXPECT_EQ(triangles[1].GetNorm(), Vector({0, 1, 0}));
    EXPECT_EQ(triangles[1].GetPoint(0), Vector({0, 0, 0}));
    EXPECT_EQ(triangles[1].GetPoint(1), Vector({0, 0, 1}));
    EXPECT_EQ(triangles[1].GetPoint(2), Vector({1, 0, 0}));

    // Проверяем, что точки получили правильные номера
    // Ожидается, что точки (0,0,0) будут иметь одинаковый номер в обоих треугольниках
    EXPECT_EQ(triangles[0].GetPoint(0).GetNum(), triangles[1].GetPoint(0).GetNum());
}

TEST_F(ReadSTLTest, ReadNonExistentFile)
{
    std::vector<Triangle> triangles = GetTriangles(non_existent_filename);
    EXPECT_TRUE(triangles.empty());
}

TEST_F(ReadSTLTest, PointNumbering)
{
    std::vector<Triangle> triangles = GetTriangles(test_filename);
    ASSERT_EQ(triangles.size(), 2);

    // Проверяем уникальность номеров для уникальных точек
    EXPECT_NE(triangles[0].GetPoint(0).GetNum(), triangles[0].GetPoint(1).GetNum());
    EXPECT_NE(triangles[0].GetPoint(0).GetNum(), triangles[0].GetPoint(2).GetNum());
    EXPECT_NE(triangles[0].GetPoint(1).GetNum(), triangles[0].GetPoint(2).GetNum());

    // Проверяем, что одинаковые точки имеют одинаковые номера
    EXPECT_EQ(triangles[0].GetPoint(0).GetNum(), triangles[1].GetPoint(0).GetNum());
}

TEST_F(ReadSTLTest, TriangleProperties)
{
    std::vector<Triangle> triangles = GetTriangles(test_filename);
    ASSERT_EQ(triangles.size(), 2);

    // Проверяем нормали
    EXPECT_DOUBLE_EQ(triangles[0].GetNorm()[0], 0.0);
    EXPECT_DOUBLE_EQ(triangles[0].GetNorm()[1], 0.0);
    EXPECT_DOUBLE_EQ(triangles[0].GetNorm()[2], 1.0);

    EXPECT_DOUBLE_EQ(triangles[1].GetNorm()[0], 0.0);
    EXPECT_DOUBLE_EQ(triangles[1].GetNorm()[1], 1.0);
    EXPECT_DOUBLE_EQ(triangles[1].GetNorm()[2], 0.0);

    // Проверяем вершины первого ольника
    EXPECT_DOUBLE_EQ(triangles[0].GetPoint(0)[0], 0.0);
    EXPECT_DOUBLE_EQ(triangles[0].GetPoint(0)[1], 0.0);
    EXPECT_DOUBLE_EQ(triangles[0].GetPoint(0)[2], 0.0);

    EXPECT_DOUBLE_EQ(triangles[0].GetPoint(1)[0], 1.0);
    EXPECT_DOUBLE_EQ(triangles[0].GetPoint(1)[1], 0.0);
    EXPECT_DOUBLE_EQ(triangles[0].GetPoint(1)[2], 0.0);

    EXPECT_DOUBLE_EQ(triangles[0].GetPoint(2)[0], 0.0);
    EXPECT_DOUBLE_EQ(triangles[0].GetPoint(2)[1], 1.0);
    EXPECT_DOUBLE_EQ(triangles[0].GetPoint(2)[2], 0.0);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}