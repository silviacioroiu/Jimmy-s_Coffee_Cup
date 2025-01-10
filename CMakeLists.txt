cmake_minimum_required(VERSION 3.27)
project(JimmysCup)

set(CMAKE_CXX_STANDARD 17)

add_executable(JimmysCup main.cpp
        Angajati.cpp
        Angajati.h
        Produse.cpp
        Produse.h
        Comenzi.cpp
        Comenzi.h
        Evenimente.cpp
        Evenimente.h
        Rapoarte.cpp
        Rapoarte.h)
