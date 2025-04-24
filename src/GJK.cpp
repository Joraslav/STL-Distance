#include "GJK.hpp"
#include <limits>
#include <cmath>

namespace dist
{
    using namespace math;

    Vector GJK::Support(const Triangle &a, const Triangle &b, const Vector &direction)
    {
        // Найти самую дальнюю точку на треугольнике A в направлении direction
        Vector farthest_A = a.GetPoint(0);
        double max_dot_A = farthest_A * direction;

        for (size_t i = 1; i < 3; ++i)
        {
            Vector point = a.GetPoint(i);
            double dot = point * direction;
            if (dot > max_dot_A)
            {
                max_dot_A = dot;
                farthest_A = point;
            }
        }

        // Найти самую дальнюю точку на треугольнике B в противоположном направлении
        Vector farthest_B = b.GetPoint(0);
        double max_dot_B = farthest_B * (-direction);

        for (size_t i = 1; i < 3; ++i)
        {
            Vector point = b.GetPoint(i);
            double dot = point * (-direction);
            if (dot > max_dot_B)
            {
                max_dot_B = dot;
                farthest_B = point;
            }
        }

        // Возвращаем разность между двумя точками
        return farthest_A - farthest_B;
    }

    bool HandleSimplex(std::vector<Vector> &simplex, Vector &direction)
    {
        // Обработка симплекса в зависимости от его размера
        if (simplex.size() == 2)
        {
            // Симплекс — отрезок
            Vector a = simplex[1];
            Vector b = simplex[0];
            Vector ab = b - a;
            Vector ao = -a;

            // Направление перпендикулярно отрезку AB в сторону начала координат
            direction = ab % (ao % ab);
            return false;
        }
        else if (simplex.size() == 3)
        {
            // Симплекс — треугольник
            Vector a = simplex[2];
            Vector b = simplex[1];
            Vector c = simplex[0];
            Vector ab = b - a;
            Vector ac = c - a;
            Vector ao = -a;

            // Нормаль к плоскости треугольника
            Vector abc = ab % ac;

            // Проверяем, в какую сторону направлено направление
            if ((abc % ac) * ao > 0)
            {
                // Точка находится за ребром AC
                simplex.erase(simplex.begin() + 1); // Удаляем B
                direction = ac % (ao % ac);
            }
            else if ((ab % abc) * ao > 0)
            {
                // Точка находится за ребром AB
                simplex.erase(simplex.begin()); // Удаляем C
                direction = ab % (ao % ab);
            }
            else
            {
                // Точка находится внутри треугольника
                direction = abc;
                if (abc * ao < 0)
                {
                    direction = -abc;
                }
            }
            return false;
        }
        else if (simplex.size() == 4)
        {
            // Симплекс — тетраэдр
            return true; // Пересечение найдено
        }

        return false;
    }

    double GJK::Distance(const Triangle &a, const Triangle &b)
    {
        // Начальное направление
        Vector direction = a.GetPoint(0) - b.GetPoint(0);

        // Если начальное направление равно нулю, задаём произвольное
        if (direction * direction == 0)
        {
            direction = Vector{1.0, 0.0, 0.0};
        }

        // Построение симплекса
        std::vector<Vector> simplex;
        simplex.push_back(Support(a, b, direction));

        // Инвертируем направление
        direction = -simplex[0];

        while (true)
        {
            // Добавляем новую точку в симплекс
            Vector new_point = Support(a, b, direction);

            // Если новая точка не проходит проверку, возвращаем расстояние
            if (new_point * direction <= 0)
            {
                return std::sqrt(direction * direction);
            }

            simplex.push_back(new_point);

            // Обновляем симплекс и направление
            if (HandleSimplex(simplex, direction))
            {
                return 0.0; // Треугольники пересекаются
            }
        }
    }
} // namespace dist