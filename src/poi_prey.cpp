#include <poi_prey.h>

using namespace cell_world;

Poi_prey::Poi_prey(const Cell_group &cells,
                   const Graph &world_graph,
                   const Graph &pois_graph,
                   const Graph &visibility,
                   const Paths &paths,
                   const Cell &start_cell,
                   const Cell &goal):
                   cells(cells),
                   pois_graph(pois_graph),
                   world_graph(world_graph),
                   visibility(visibility),
                   paths(paths),
                   start_cell(start_cell),
                   goal(goal),
                   model(cells),
                   prey(start_cell, goal),
                   predator(world_graph, visibility, paths),
                   particle_filter(cells,world_graph,visibility,paths, start_cell, goal){ }

const Cell &Poi_prey::start_episode() {
    return start_cell;
}

Move Poi_prey::get_move(const Model_public_state &state) {
    bool contact = process_state (state);
    return plan(contact, state);
}

Agent_status_code Poi_prey::update_state(const Model_public_state &state) {
    if (!process_state (state)) particle_filter.trajectory.push_back(move);
    return cell_world::Finished;
}

bool Poi_prey::process_state(const Model_public_state &state) {
    // if predator is visible records the
    // observation and returns true
    auto &prey_cell = state.agents_state[0].cell;
    auto &predator_cell = state.agents_state[1].cell;
    bool contact = visibility[prey_cell].contains(predator_cell);
    if (contact) particle_filter.record_observation(state);
    return contact;
}

Move Poi_prey::plan(bool use_full_state, const cell_world::Model_public_state &prey_state) {
    Model_public_state mps = prey_state;
    if (!use_full_state) particle_filter.create_particles(500,5000);
    for (unsigned int t = 0; t < 1000; t++){
        if (!use_full_state) mps.agents_state[1] = particle_filter.get_particle();
        model.set_public_state(mps);
        
    }
    return Move();
}
