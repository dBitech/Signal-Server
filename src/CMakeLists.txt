cmake_minimum_required(VERSION 3.15)
project(SignalServer)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CXXFLAGS "-Wall -O3 -s -ffast-math")

find_package(Threads REQUIRED)

include_directories(.)
include_directories(models)

add_executable(signalserver
        models/cost.cc
        models/cost.hh
        models/ecc33.cc
        models/ecc33.hh
        models/egli.cc
        models/egli.hh
        models/ericsson.cc
        models/ericsson.hh
        models/fspl.cc
        models/fspl.hh
        models/hata.cc
        models/hata.hh
        models/itwom3.0.cc
        models/itwom3.0.hh
        models/los.cc
        models/los.hh
        models/pel.cc
        models/pel.hh
        models/soil.cc
        models/soil.hh
        models/sui.cc
        models/sui.hh
        common.h
        image-ppm.cc
        image-ppm.hh
        image.cc
        image.hh
        inputs.cc
        inputs.hh
        main.cc
        main.hh
        outputs.cc
        outputs.hh
        tiles.cc
        tiles.hh)
target_link_libraries(signalserver Threads::Threads bz2 dl m z)

add_custom_target(symlinks ALL
        COMMAND ${CMAKE_COMMAND} -E create_symlink signalserver signalserverHD
        COMMAND ${CMAKE_COMMAND} -E create_symlink signalserver signalserverLIDAR
        BYPRODUCTS signalserverHD signalserverLIDAR)
add_dependencies(symlinks signalserver)