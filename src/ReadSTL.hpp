#pragma once

#include "stl_reader.h"

#include "Vector.hpp"
#include "Triangle.hpp"

#include <string>
#include <vector>

namespace read_stl
{
    std::vector<math::Triangle> GetTriangles(const std::string &filename);

} // namespace read_stl
