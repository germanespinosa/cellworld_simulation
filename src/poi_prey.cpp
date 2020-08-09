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
    return start;
}

Move Poi_prey::get_move(const Model_public_state &state) {
    bool contact = process_state (state);
    if (internal_state().status == Running) {
        if (contact) {
            internal_state().move = planner.get_best_move (state,internal_state().estimated_reward);
        } else {
            auto particle_count = planner.filter.create_particles();
            if (particle_count) {
                internal_state().move = planner.get_best_move (state,internal_state().estimated_reward, planner.filter.particles);
            } else { // means no predator, no need to plan
                internal_state().move = data.paths.get_move(public_state().cell,goal);
            }
        }
    } else {
        internal_state().move = {0,0};
    }
    return internal_state().move;
}

Agent_status_code Poi_prey::update_state(const Model_public_state &state) {
    if (internal_state().status == Running) {
        if (!process_state(state)) planner.filter.trajectory.push_back(internal_state().move);
        internal_state().coordinates = state.agents_state[0].cell.coordinates;
    }
    return internal_state().status;
}

bool Poi_prey::process_state(const Model_public_state &state) {
    // if predator is visible records the
    // observation and returns true
    auto &prey_cell = state.agents_state[0].cell;
    auto &predator_cell = state.agents_state[1].cell;
    bool contact = data.visibility[prey_cell].contains(predator_cell);
    if (contact) planner.filter.record_observation(state);
    if (prey_cell == goal || predator_cell == prey_cell) internal_state().status = Finished;
    return contact;
}