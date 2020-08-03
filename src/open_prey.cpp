#include <open_prey.h>

Open_prey::Open_prey(cell_world::Cell &start, cell_world::Coordinates goal) :
    start_cell(start), goal(goal){
}

cell_world::Agent_status_code Open_prey::update_state(const cell_world::Model_public_state &) {
    return cell_world::Finished;
}

const cell_world::Cell &Open_prey::start_episode() {
    return start_cell;
}

cell_world::Move Open_prey::get_move(const cell_world::Model_public_state &) {
    return move;
}

void Open_prey::end_episode(const cell_world::Model_public_state &) {

}

const cell_world::Cell &Open_prey::cell() const {
    return public_state().cell;
}
