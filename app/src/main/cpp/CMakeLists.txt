# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

add_library( # Sets the name of the library.
        native-lib

        SHARED

        egl/egl_core.cpp
        egl/base_surface.cpp
        egl/offscreen_surface.cpp

        base/gl_utils.cpp
        base/Looper.cpp
        base/utils.cpp

        image/ImageLooper.cpp
        image/ImageRenderer.cpp
        image/filter/ImageBaseFilter.cpp
        image/filter/ImageGrayFilter.cpp
        image/ImageData.cpp

        native-lib.cpp)

find_library(log-lib log)

target_link_libraries( # Specifies the target library.
        native-lib
        android EGL GLESv3
        jnigraphics

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})