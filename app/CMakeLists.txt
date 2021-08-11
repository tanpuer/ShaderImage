cmake_minimum_required(VERSION 3.4.1)

aux_source_directory(src/main/cpp srcs)
aux_source_directory(src/main/cpp/base srcs)
aux_source_directory(src/main/cpp/egl srcs)
aux_source_directory(src/main/cpp/image srcs)
aux_source_directory(src/main/cpp/image/filter srcs)

add_library(
        shaderImage
        SHARED
        ${srcs}
)

find_library(log-lib log)

include_directories(
        src/main/cpp
        src/main/cpp/base
        src/main/cpp/egl
        src/main/cpp/image
        src/main/cpp/image/filter
)

target_link_libraries(
        shaderImage
        android EGL GLESv3
        jnigraphics
        ${log-lib})