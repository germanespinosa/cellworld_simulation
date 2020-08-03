//
// Created by german on 7/31/20.
//

#include <agents/predator.h>

Predator::Predator(
        const cell_world::Graph &world_graph,
        const cell_world::Graph &visibility,
        const cell_world::Paths &paths,
        const cell_world::Cell &start_cell):
        world_graph(world_graph),
        visibility(visibility),
        paths(paths),
        start_cell(start_cell){}

const cell_world::Cell &Predator::start_episode() {
    return start_cell;
}

cell_world::Move Predator::get_move(const cell_world::Model_public_state &) {
    return cell_world::Move();
}

cell_world::Agent_status_code Predator::update_state(const cell_world::Model_public_state &) {
    return cell_world::Finished;
}

void Predator::end_episode(const cell_world::Model_public_state &) {

}
