cmake_minimum_required( VERSION 3.10 )

set(CMAKE_CXX_STANDARD 20)

project( cellworld_simulation
        VERSION 2019.1.0
        DESCRIPTION "Cellworld simulation"
        LANGUAGES CXX)

####
#### DEPENDENCIES
####

find_package (Dependencies QUIET)

if (NOT ${Dependencies_FOUND})
    if (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/DependenciesConfig.cmake)
        file(DOWNLOAD https://raw.githubusercontent.com/germanespinosa/dependencies/main/DependenciesConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/DependenciesConfig.cmake)
    endif()
    set(Dependencies_DIR "${CMAKE_CURRENT_BINARY_DIR}")
    find_package (Dependencies REQUIRED)
endif()

####
#### END DEPENDENCIES
####

install_git_dependency( Cellworld
        https://github.com/cellworld/cellworld
        CMAKE_PROJECT
        IMPORT_TARGETS cellworld
        INCLUDE_DIRECTORIES include)


install_git_dependency(Params-cpp https://github.com/cellworld/dependency_params-cpp
        CMAKE_PROJECT
        IMPORT_TARGETS params-cpp)

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
        src/knowledge_prey.cpp
        src/map_symbols.cpp)

add_library( cellworld_simulation ${cellworld_simulation_files})

target_link_libraries ( cellworld_simulation cellworld params-cpp)

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