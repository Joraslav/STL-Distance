#include "ReadSTL.hpp"
#include "Vector.hpp"
#include "Triangle.hpp"

#include <array>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace read_stl
{
    using namespace math;
    using namespace stl_reader;

    std::vector<Triangle> GetTriangles(const std::string &filename)
    {
        // Хеш-таблица для хранения уникальных точек и их номеров
        std::unordered_map<Vector, size_t, VectorReadHash> point_map;
        size_t point_counter = 0;

        // Функция для получения или создания номера точки
        auto get_point_number = [&](const Vector &point) -> size_t
        {
            auto [it, inserted] = point_map.emplace(point, point_counter);
            if (inserted)
            {
                return point_counter++;
            }
            return it->second;
        };

        try
        {
            StlMesh<double, size_t> mesh(filename);

            std::vector<Triangle> triangles;

            for (size_t itri = 0; itri < mesh.num_tris(); itri++)
            {
                const double *ptr_for_normal = mesh.tri_normal(itri);
                Vector normal({ptr_for_normal[0], ptr_for_normal[1], ptr_for_normal[2]});

                std::array<Vector, 3> points;
                for (size_t icorner = 0; icorner < 3; ++icorner)
                {
                    const double *ptr_for_one_point = mesh.tri_corner_coords(itri, icorner);
                    points[icorner] = Vector({ptr_for_one_point[0], ptr_for_one_point[1], ptr_for_one_point[2]});
                }

                for (auto &point : points)
                {
                    size_t num = get_point_number(point);
                    point.SetNum(num);
                }

                triangles.emplace_back(itri, normal, points);
            }
            return triangles;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Ошибка при чтении STL-файла: " << e.what() << '\n';
            return {};
        }
        catch (...)
        {
            std::cerr << "Неизвестная ошибка при чтении STL-файла: " << filename << '\n';
            return {};
        }
    }

} // namespace read_stl
