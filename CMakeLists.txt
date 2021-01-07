project(CS143 C)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG_INIT} -Wall -g")

set(CMAKE_C_COMPILER "/usr/bin/gcc")

add_executable(Projet1 Projet1/Projet1.c)
add_executable(Projet2 Projet2/Projet2.c)
add_executable(Projet3 Projet3/Projet3.c)
add_executable(Projet4 Projet4/Projet4.c)
add_executable(Projet5 Projet5/Projet5.c)
