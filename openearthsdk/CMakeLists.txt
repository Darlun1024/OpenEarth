# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.



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

             # Provides a relative path to your source file(s).
              src/main/cpp/util/image.h
              src/main/cpp/util/image.c
              src/main/cpp/util/assets_file_reader.cpp
              src/main/cpp/util/util.cpp
             src/main/cpp/native_earth_view.cpp
             src/main/cpp/earth/sphere.cpp
             src/main/cpp/earth/earth_renderer.cpp
             src/main/cpp/main.cpp
             src/main/cpp/jni.cpp
             src/main/cpp/logging.hpp


             )
INCLUDE_DIRECTORIES (src/main/cpp/deps/glm/
                    src/main/cpp/deps/libpng/
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
                       EGL    #必须要添加这两行，否则会报link错误，找不到glXXXX方法
                       GLESv2 #
                       GLESv3 #
                       android
                       z    #zlib
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib}
                        )