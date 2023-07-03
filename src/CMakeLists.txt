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
include_directories(Shader)
include_directories(utils)

find_package(OpenMP)
if (OPENMP_FOUND)

    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()


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
        main.cpp Image/ImagePPM.cpp
        Light/AmbientLight.hpp
        Shader/shader.hpp
        Shader/AmbientShader.cpp
        Renderer/StandardRenderer.cpp
        Light/PointLight.hpp
        Shader/WhittedShader.cpp
        Light/AreaLight.hpp
        Primitive/Geometry/Triangle.hpp
        Shader/DistributedShader.hpp
        Shader/DistributedShader.cpp
        Shader/PathTracerShader.cpp
        Shader/PathTracerShader.hpp
        Camera/Environment.cpp
        Camera/Environment.h
        )

# libjpeg library
find_package(JPEG REQUIRED)
include_directories(${JPEG_INCLUDE_DIR})
target_link_libraries(VI_RT PRIVATE ${JPEG_LIBRARY})

# Set the path to the OpenEXR libraries and include directories
set(OpenEXR_INCLUDE_DIRS /usr/include/OpenEXR)
set(OpenEXR_LIBRARIES /usr/lib/x86_64-linux-gnu/libIlmImf.so /usr/lib/x86_64-linux-gnu/libHalf.so)

# imageEXR library

target_include_directories(VI_RT PRIVATE ${OpenEXR_INCLUDE_DIRS})
target_link_libraries(VI_RT PRIVATE ${OpenEXR_LIBRARIES})
