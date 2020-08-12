#include <poi_prey.h>

using namespace cell_world;
using namespace std;

Poi_prey::Poi_prey(const Poi_prey_parameters &parameters,
                   const Planner_parameters &planner_parameters,
                   const Particle_filter_parameters &particle_filter_parameters,
                   const Predator_parameters &predator_parameters,
                   const Static_data &data):
        parameters(parameters),
        data(data),
        start(data.map[parameters.start]),
        goal(data.map[parameters.goal]),
        planner( planner_parameters,
                 particle_filter_parameters,
                 predator_parameters,
                 data,
                 start,
                 goal ){
}

const Cell &Poi_prey::start_episode() {
    internal_state().status = cell_world::Running;
    internal_state().coordinates = start.coordinates;
    planner.reset();
    return start;
}

Move Poi_prey::get_move(const Model_public_state &state) {
    internal_state().move = planner.get_best_move(state,internal_state().estimated_reward);
    internal_state().option = planner.option;
    return internal_state().move;
}

Agent_status_code Poi_prey::update_state(const Model_public_state &state) {
    internal_state().status = planner.update_state(state);
    return internal_state().status;
}
