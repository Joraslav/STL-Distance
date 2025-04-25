cmake_minimum_required(VERSION 3.28.0)
project(STL-Distance
    VERSION 0.5.0
    LANGUAGES CXX
    DESCRIPTION "A project for STL distance calculations"
)

# Настройка политик CMake
if(POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW) # Для правильной обработки опций
endif()

# Настройка версии и стандартов
message(STATUS "Version of project: ${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Для кросс-платформенной совместимости

# Настройка предупреждений (платформонезависимая версия)
if(MSVC)
    add_compile_options(
        /W4
        /WX
        /wd4201
        /wd4251
        /wd4275)
else()
    add_compile_options(
        -Wall
        -Wextra
        -Werror
        -Wpedantic
        -Wconversion)
endif()

# Включение современных практик CMake
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Для инструментов анализа кода

# Поиск GTest
find_package(GTest REQUIRED)
if(NOT GTest_FOUND)
    message(FATAL_ERROR "GTest не найден!")
else()
    message(STATUS "GTest найден ^_^. Version: ${GTest_VERSION}")
endif()

include_directories(src lib/stl_reader)

set(MATH
    src/Vector.hpp
    src/Vector.cpp
    src/Triangle.hpp
    src/Triangle.cpp
    src/MiddlePoint.hpp
    src/MiddlePoint.cpp
    src/Matrix.hpp
    src/MathOperations.hpp
    src/MathOperations.cpp)

set(ALT_MDM
    src/AltMDM.hpp
    src/AltMDM.cpp)

set(KDTREE
    src/KDTree.hpp
    src/KDTree.cpp)

set(AABBTREE
    src/AABBTree.hpp
    src/AABBTree.cpp)

set(GJK_SOURCE
    src/GJK.hpp
    src/GJK.cpp)

set(DISTANCE
    src/Distance.hpp
    src/Distance.cpp)

set(READSTL src/ReadSTL.hpp src/ReadSTL.cpp)

add_library(Math STATIC ${MATH})
add_library(ReadSTL STATIC ${READSTL})
add_library(AltMDM STATIC ${ALT_MDM})
add_library(KDTree STATIC ${KDTREE})
add_library(GJK STATIC ${GJK_SOURCE})
add_library(Distance STATIC ${DISTANCE})
add_library(AABBTree STATIC ${AABBTREE})

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(ReadSTL Math)
target_link_libraries(Distance GJK KDTree AABBTree Math)
target_link_libraries(${PROJECT_NAME} Math ReadSTL AltMDM KDTree GJK Distance AABBTree)

# Тесты
include(CTest)
enable_testing()
# Vector
add_executable(testVector tests/testVector.cpp)
target_link_libraries(testVector PRIVATE Math GTest::GTest GTest::Main)
add_test(NAME VectorTest COMMAND testVector)
# Triangle
add_executable(testTriangle tests/testTriangle.cpp)
target_link_libraries(testTriangle PRIVATE Math GTest::GTest GTest::Main)
add_test(NAME TriangleTest COMMAND testTriangle)
# Matrix
add_executable(testMatrix tests/testMatrix.cpp)
target_link_libraries(testMatrix PRIVATE Math GTest::GTest GTest::Main)
add_test(NAME MatrixTest COMMAND testMatrix)
# MathOperations
add_executable(testMathOperations tests/testMathOperations.cpp)
target_link_libraries(testMathOperations PRIVATE Math GTest::GTest GTest::Main)
add_test(NAME MathOperationsTest COMMAND testMathOperations)
# ReadSTL
add_executable(testReadSTL tests/testReadSTL.cpp)
target_link_libraries(testReadSTL PRIVATE Math ReadSTL GTest::GTest GTest::Main)
add_test(NAME ReadSTLTest COMMAND testReadSTL)
# KDTree
add_executable(testKDTree tests/testKDTree.cpp)
target_link_libraries(testKDTree PRIVATE Math KDTree GTest::GTest GTest::Main)
add_test(NAME KDTreeTest COMMAND testKDTree)


# Опционально: установка выходных файлов
install(TARGETS ${PROJECT_NAME} Math
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)


