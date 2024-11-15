#include <simulation.h>

using namespace std;

int main(int argc, char **argv)
{
    Simulation_parameters p;
    p.particle_filter.attempts = 1000;
    p.particle_filter.particle_count = 100;
    p.path_type = "astar";
    p.planner.reward.discount = .1;
    p.planner.reward.fail = -100;
    p.planner.roll_outs = 100;
    p.planner.use_pois = true;
    p.planner.tree_mode = "mcts";
    p.predator.randomness = .25;
    p.predator.speed = 1.5;
    p.predator_location = cell_world::Coordinates(10,10);
    p.predator_present = true;
    p.prey.goal = cell_world::Coordinates(20,0);
    p.seeds = {0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
    p.world_info.world_configuration = "hexagonal";
    p.world_info.world_implementation = "canonical";
    p.world_info.occlusions = "21_05";
    Simulation simulation(p);
    simulation.run();
}