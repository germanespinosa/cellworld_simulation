cmake_minimum_required( VERSION 3.10 )

set(CMAKE_CXX_STANDARD 20)

project( cellworld_simulation
        VERSION 2019.1.0
        DESCRIPTION "Cellworld simulation"
        LANGUAGES CXX)

find_package(Cellworld_tools REQUIRED)

set (cellworld_simulation_files
        src/predator.cpp
        src/planner.cpp
        src/poi_prey.cpp
        src/simulation.cpp
        src/particle_filter.cpp
        src/reward.cpp
        src/static_data.cpp
        src/search_tree.cpp
        src/training_prey.cpp
        src/training.cpp
        src/off_line_knowledge.cpp
        src/knowledge_simulation.cpp
        src/knowledge_prey.cpp)

add_library( cellworld_simulation ${cellworld_simulation_files})

target_link_libraries ( cellworld_simulation ${CELLWORLD_TOOLS_LIBRARIES})

target_include_directories(cellworld_simulation
        PUBLIC
        include )

add_executable(cellworld_run src/run.cpp)
target_link_libraries( cellworld_run  cellworld_simulation)

add_executable(cellworld_train src/train.cpp)
target_link_libraries( cellworld_train  cellworld_simulation)

add_executable(cellworld_krun src/knowledge_run.cpp)
target_link_libraries( cellworld_krun  cellworld_simulation)


###
### TESTS
###
find_package(CatchTests CONFIG QUIET)


###
### SIMULATION INSTALLATION
###

if (CMAKE_BUILD_TYPE MATCHES Release)

    include(GNUInstallDirs)

    install(TARGETS cellworld_run
            RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})

    install(TARGETS cellworld_krun
            RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})

    install(TARGETS cellworld_train
            RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})


endif()