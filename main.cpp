#include <iostream>

#include "Vector.hpp"
#include "ReadSTL.hpp"
#include "Distance.hpp"

#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace math;
using namespace dist;
using namespace read_stl;
using namespace std::literals;
using namespace std::chrono;

int main()
{
    std::string body_1 = "../data/Body.stl"s;
    std::string body_2 = "../data/Gun.stl"s;

    // std::string fout = "OutPut.txt"s;

    // std::string file_1, file_2;

    // std::cout << "Write file name like this - <name-file>.stl" << std::endl;
    // std::cout << "Input first file name - ";
    // std::getline(std::cin, file_1);
    // std::cout << "Input second file name - ";
    // std::getline(std::cin, file_2);

    const auto start_read = steady_clock::now();
    std::vector<Triangle> triangles_1 = GetTriangles(body_1);
    std::vector<Triangle> triangles_2 = GetTriangles(body_2);
    const auto end_read = steady_clock::now();
    const auto read_time = duration<double>(end_read - start_read);
    std::cout << "Elemements in 1 body "s << body_1 << ": "s << triangles_1.size() << std::endl;
    std::cout << "Elemements in 2 body "s << body_2 << ": "s << triangles_2.size() << std::endl;
    std::cout << "Read time: "s << read_time.count() << " seconds" << std::endl;

    Distance container(triangles_1, triangles_2);

    const auto start_calculate = steady_clock::now();
    // AABBTree tree_1(triangles_1);
    // AABBTree tree_2(triangles_2);
    // double distance = 0.0;
    // const Triangle *tr_1 = nullptr, *tr_2 = nullptr;
    // tree_1.FindClosestTriangles(tree_2, tr_1, tr_2, distance);
    const double distance = container.FindDistanceBetweenBody();
    const auto end_calculate = steady_clock::now();
    const auto calculate_time = duration<double>(end_calculate - start_calculate);
    std::cout << "Calculate time: "s << calculate_time.count() << " seconds"s << std::endl;
    std::cout << "Distance: "s << distance << std::endl;

    // std::ofstream out_file(fout);
    // if (!out_file.is_open())
    // {
    //     std::cerr << "Failed to open output file: " << fout << std::endl;
    //     return 1;
    // }
    // out_file << "Elemements in 1 body "s << file_1 << ": "s << triangles_1.size() << std::endl;
    // out_file << "Elemements in 2 body "s << file_2 << ": "s << triangles_2.size() << std::endl;
    // out_file << "Read time: "s << read_time.count() << " seconds"s << std::endl;
    // out_file << "Calculate time: "s << calculate_time.count() << " seconds"s << std::endl;
    // out_file << "Distance: "s << distance << std::endl;

    return 0;
}