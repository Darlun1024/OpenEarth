# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# CACHE
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(C_LAUNCHER   "${CCACHE_PROGRAM}")
    set(CXX_LAUNCHER "${CCACHE_PROGRAM}")
        # Support Unix Makefiles and Ninja
        configure_file(scripts/launch-c.in   launch-c   @ONLY)
        configure_file(scripts/launch-cxx.in launch-cxx @ONLY)

        set(CMAKE_C_COMPILER_LAUNCHER        "${CMAKE_BINARY_DIR}/launch-c")
        set(CMAKE_CXX_COMPILER_LAUNCHER      "${CMAKE_BINARY_DIR}/launch-cxx")

    execute_process(COMMAND chmod a+rx "${CMAKE_BINARY_DIR}/launch-c" "${CMAKE_BINARY_DIR}/launch-cxx")

    if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments -fcolor-diagnostics")
    endif()
else()
    message(STATUS "Can't find ccache — consider installing ccache to improve recompilation performance")
endif()

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

#用于编译文件下下所有文件
#aux_source_directory(src/main/cpp/deps/libpng/  PNG_SRC_LIST)
#aux_source_directory(src/main/cpp/deps/libjpeg/ JPEG_SRC_LIST)

INCLUDE_DIRECTORIES (src/main/cpp/deps/)

add_library(libjpeg SHARED IMPORTED)
add_library(libsqlite SHARED IMPORTED)
#这里千万要注意路径的写法
set_target_properties(libjpeg PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libjpeg.so)
set_target_properties(libsqlite PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libsqliteX.so)

add_library( # Sets the name of the library.
             lib-earth

             # Sets the library as a shared library.
             SHARED

                 #libpng
              src/main/cpp/deps/libpng/png.c
              src/main/cpp/deps/libpng/pngerror.c
              src/main/cpp/deps/libpng/pngget.c
              src/main/cpp/deps/libpng/pngmem.c
              src/main/cpp/deps/libpng/pngpread.c
              src/main/cpp/deps/libpng/pngread.c
              src/main/cpp/deps/libpng/pngrio.c
              src/main/cpp/deps/libpng/pngrtran.c
              src/main/cpp/deps/libpng/pngrutil.c
              src/main/cpp/deps/libpng/pngtrans.c
              src/main/cpp/deps/libpng/pngset.c
              src/main/cpp/deps/libpng/pngtrans.c
              src/main/cpp/deps/libpng/pngwio.c
              src/main/cpp/deps/libpng/pngwrite.c
              src/main/cpp/deps/libpng/pngwtran.c
              src/main/cpp/deps/libpng/pngwutil.c


             # util
              src/main/cpp/util/image.c
              src/main/cpp/util/png_reader.c
              src/main/cpp/util/jpeg_reader.c
              src/main/cpp/util/assets_file_reader.cpp
              src/main/cpp/util/util.cpp
              src/main/cpp/util/looper.cpp
              src/main/cpp/util/thread_local.cpp
              src/main/cpp/util/thread_local.cpp
              src/main/cpp/util/url.cpp
              # gl
              src/main/cpp/gl/gl.cpp;
              src/main/cpp/shaders/shader.cpp;
              src/main/cpp/shaders/point_shader.cpp;
              src/main/cpp/shaders/line_shader.cpp;
              src/main/cpp/shaders/fill_shader.cpp;
              src/main/cpp/shaders/raster_shader.cpp;
              src/main/cpp/shaders/light_raster_shader.cpp;
              src/main/cpp/programs/program.cpp;
              src/main/cpp/texture/texture.cpp;
              # data source
              src/main/cpp/storage/data_source.cpp;
              src/main/cpp/storage/http_data_source.cpp;
              src/main/cpp/storage/mbtile_info.cpp;
              src/main/cpp/storage/database_data_source.cpp;
              src/main/cpp/storage/mbtile_data_source.cpp;

              src/main/cpp/earth/opengl_project.cpp

              # geometry
              src/main/cpp/earth/geometry/Ray.cpp
              src/main/cpp/earth/geometry/geometry_util.cpp
              src/main/cpp/earth/geometry/latlng.cpp
              src/main/cpp/earth/geometry/bounds.cpp
              src/main/cpp/earth/geometry/geometry.cpp
              src/main/cpp/earth/geometry/point.cpp
              src/main/cpp/earth/geometry/line.cpp
              src/main/cpp/earth/geometry/polygon.cpp

              # geometry::util
              src/main/cpp/earth/geometry/util/math_util.cpp
              src/main/cpp/earth/geometry/util/sphere_util.cpp
              src/main/cpp/earth/geometry/util/envelope.cpp

              # style
              src/main/cpp/earth/style/style.cpp
              src/main/cpp/earth/style/line_style.cpp
              src/main/cpp/earth/style/point_style.cpp
              src/main/cpp/earth/style/polygon_style.cpp

              # earth
              src/main/cpp/native_earth_view.cpp
              src/main/cpp/earth/earth.cpp
              src/main/cpp/earth/transform.cpp
              src/main/cpp/earth/sphere.cpp

              # tile
              src/main/cpp/earth/tile.cpp
              src/main/cpp/earth/tile/tile_id.cpp
              src/main/cpp/earth/tile/tile_cache.cpp
              src/main/cpp/earth/tile/tile_data.cpp
              src/main/cpp/earth/tile_management.cpp
              src/main/cpp/earth/source/source.cpp

              # feature
              src/main/cpp/earth/feature/feature.cpp
              src/main/cpp/earth/feature/point_feature.cpp
              src/main/cpp/earth/feature/line_feature.cpp
              src/main/cpp/earth/feature/polygon_feature.cpp

              # layer
              src/main/cpp/earth/layer/layer.cpp
              src/main/cpp/earth/layer/vector_layer.cpp
              src/main/cpp/earth/layer/point_layer.cpp
              src/main/cpp/earth/layer/line_layer.cpp
              src/main/cpp/earth/layer/polygon_layer.cpp
              src/main/cpp/earth/layer/raster_layer.cpp
              src/main/cpp/earth/earth_renderer.cpp

              src/main/cpp/main.cpp
              src/main/cpp/jni.cpp
              src/main/cpp/logging.hpp
             )


# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib
              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       lib-earth
                       libjpeg
                       libsqlite
                       EGL    #必须要添加这两行，否则会报link错误，找不到glXXXX方法
                       GLESv2 #
                       GLESv3 #
                       android
                       z    #zlib
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib}
                        )