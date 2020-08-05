#include <poi_prey.h>

using namespace cell_world;
using namespace std;

Poi_prey::Poi_prey(const Cell_group &cells,
                   const Graph &world_graph,
                   const Cell_group &pois,
                   const Graph &pois_graph,
                   const Graph &visibility,
                   const Paths &paths,
                   const Cell &start_cell,
                   const Cell &goal,
                   const Planning_parameters &planning):
        cells(cells),
        pois(pois),
        pois_graph(pois_graph),
        world_graph(world_graph),
        visibility(visibility),
        paths(paths),
        start_cell(start_cell),
        goal(goal),
        planning(planning),
        particle_filter(cells,world_graph,visibility,paths, start_cell, goal){
}

const Cell &Poi_prey::start_episode() {
    internal_state().status = cell_world::Running;
    internal_state().coordinates = start_cell.coordinates;
    return start_cell;
}

Move Poi_prey::get_move(const Model_public_state &state) {
    bool contact = process_state (state);
    if (internal_state().status == Running) {
        internal_state().move = plan(contact, state);
    } else {
        internal_state().move = {0,0};
    }
    return internal_state().move;
}

Agent_status_code Poi_prey::update_state(const Model_public_state &state) {
    if (internal_state().status == Running) {
        if (!process_state(state)) particle_filter.trajectory.push_back(internal_state().move);
        internal_state().coordinates = state.agents_state[0].cell.coordinates;
    }
    return internal_state().status;
}

bool Poi_prey::process_state(const Model_public_state &state) {
    // if predator is visible records the
    // observation and returns true
    auto &prey_cell = state.agents_state[0].cell;
    auto &predator_cell = state.agents_state[1].cell;
    bool contact = visibility[prey_cell].contains(predator_cell);
    if (contact) particle_filter.record_observation(state);
    if (prey_cell == goal || predator_cell == prey_cell) internal_state().status = Finished;
    return contact;
}