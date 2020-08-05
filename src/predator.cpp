//
// Created by german on 7/31/20.
//

#include <predator.h>

Predator::Predator(
        const cell_world::Graph &world_graph,
        const cell_world::Graph &visibility,
        const cell_world::Paths &paths,
        const cell_world::Cell &start_cell):
        world_graph(world_graph),
        visibility(visibility),
        inverted_visibility(visibility.invert()),
        paths(paths),
        start_cell(start_cell){}

Predator::Predator(
        const cell_world::Graph &world_graph,
        const cell_world::Graph &visibility,
        const cell_world::Paths &paths):
        world_graph(world_graph),
        visibility(visibility),
        inverted_visibility(visibility.invert()),
        paths(paths){}

const cell_world::Cell &Predator::start_episode() {
    internal_state().goal = start_cell;
    return start_cell;
}

cell_world::Move Predator::get_move(const cell_world::Model_public_state &state) {
    _process_state(state);
    auto &prey_cell = state.agents_state[0].cell;
    auto &predator_cell = state.agents_state[1].cell;
    return paths.get_move(prey_cell, predator_cell);
}

cell_world::Agent_status_code Predator::update_state(const cell_world::Model_public_state &state) {
    _process_state(state);
    return cell_world::Running;
}

void Predator::_process_state(const cell_world::Model_public_state &state) {
    auto &prey_cell = state.agents_state[0].cell;
    auto &predator_cell = state.agents_state[1].cell;
    if (visibility[predator_cell].contains(prey_cell)) {
        internal_state().goal = prey_cell;
        return;
    }
    if (predator_cell == internal_state().goal) {
        internal_state().goal = inverted_visibility[predator_cell].random_cell();
    }
}
