#include <knowledge_simulation.h>
#include <params_cpp.h>

using namespace std;

int main(int argc, char **argv) {
    params_cpp::Parser p(argc, argv);
    Knowledge_simulation_parameters parameters;
    parameters.goal = cell_world::Coordinates(20,0);
    parameters.knowledge_file = "knowledge.dat";
    parameters.particle_filter.attempts = 1000;
    parameters.particle_filter.particle_count = 100;
    parameters.path_type = "astar";
    parameters.result_file = "result.dat";
    parameters.seeds = {0, 1,2,3,4,5,6,7,8,9};
    parameters.steps = 200;
    parameters.world_info.world_configuration = "hexagonal";
    parameters.world_info.world_implementation = "canonical";
    parameters.world_info.occlusions = "21_05";
    Knowledge_simulation simulation(parameters);
    simulation.run();
}