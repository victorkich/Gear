cmake_minimum_required(VERSION 3.17)
project(Gear)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(include/GL)
include_directories(src)

add_executable(Gear
        include/GL/freeglut.h
        include/GL/freeglut_ext.h
        include/GL/freeglut_std.h
        include/GL/glut.h
        src/Bola.h
        src/Botao.h
        src/gl_canvas2d.cpp
        src/gl_canvas2d.h
        src/main.cpp
        src/Relogio.cpp
        src/Relogio.h
        src/Vector2.h)
