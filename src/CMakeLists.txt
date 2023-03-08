cmake_minimum_required(VERSION 3.24)
project(VI_RT)

set(CMAKE_CXX_STANDARD 14)

include_directories(3DSortingStruct)
include_directories(Camera)
include_directories(Image)
include_directories(Light)
include_directories(Primitive)
include_directories(Primitive/BRDF)
include_directories(Primitive/Geometry)
include_directories(Rays)
include_directories(Renderer)
include_directories(Scene)
include_directories(Scene/tinyobjloader)
include_directories(Scene/tinyobjloader/examples/obj_sticher)
include_directories(Scene/tinyobjloader/examples/viewer)
include_directories(Scene/tinyobjloader/examples/voxelize)
include_directories(Scene/tinyobjloader/experimental)
include_directories(Scene/tinyobjloader/experimental/lfpAlloc)
include_directories(Scene/tinyobjloader/mapbox)
include_directories(Scene/tinyobjloader/tests)
include_directories(utils)

add_executable(VI_RT
        3DSortingStruct/AccelStruct.hpp
        Camera/camera.hpp
        Camera/perspective.cpp
        Camera/perspective.hpp
        Image/image.hpp
        Light/light.hpp
        Primitive/BRDF/BRDF.hpp
        Primitive/BRDF/Phong.cpp
        Primitive/BRDF/Phong.hpp
        Primitive/Geometry/geometry.hpp
        Primitive/Geometry/mesh.cpp
        Primitive/Geometry/mesh.hpp
        Primitive/BB.hpp
        Primitive/primitive.hpp
        Rays/intersection.hpp
        Rays/ray.hpp
        Renderer/renderer.hpp
        Scene/tinyobjloader/tiny_obj_loader.h
        #Scene/loader_example.cc
        Scene/scene.cpp
        Scene/scene.hpp
        #utils/AABB.cpp
        utils/RGB.hpp
        utils/vector.hpp
        main.cpp)
