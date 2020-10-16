#include <predator.h>

using namespace cell_world;
using namespace std;

Predator::Predator(
        const Predator_parameters &parameters,
        const cell_world::Coordinates start_cell,
        const Static_data &data):
        parameters(parameters),
        data(data),
        start_cell(data.map[start_cell]){
    _speed_base = (unsigned int)parameters.speed;
    _speed_overflow = parameters.speed - (double)_speed_base;
}

Predator::Predator(
        const Predator_parameters &parameters,
        const Static_data &data):
        parameters(parameters),
        data(data){
    start_cell = data.cells.free_cells()[0];
    _speed_base = (unsigned int)parameters.speed;
    _speed_overflow = parameters.speed - (double)_speed_base;
}


const Cell &Predator::start_episode() {
    internal_state().goal = start_cell;
    return start_cell;
}

Move Predator::get_move(const Model_public_state &state) {
    _process_state(state);
    auto &prey_cell = state.agents_state[0].cell;
    auto &predator_cell = state.agents_state[1].cell;
    auto steps = _steps();
    if (steps > 1) {
        internal_state().move = {0, 0};
    }
    internal_state().move = {0,0};
    for (unsigned int step=0; step<steps; step++){
        auto current_coord = predator_cell.coordinates + internal_state().move;
        auto &current_cell = data.map[current_coord];
        if (Chance::coin_toss(parameters.randomness)) {
            internal_state().move = data.world_graph[current_cell].random_cell().coordinates - current_coord;
        } else {
            internal_state().move += data.paths.get_move(current_cell, internal_state().goal);
        }
    }
    return internal_state().move;
}

Agent_status_code Predator::update_state(const Model_public_state &state) {
    _process_state(state);
    return cell_world::Running;
}

void Predator::_process_state(const Model_public_state &state) {
    auto &prey_cell = state.agents_state[0].cell;
    auto &predator_cell = state.agents_state[1].cell;
    if (data.visibility[predator_cell].contains(prey_cell)) {
        internal_state().goal = prey_cell;
        return;
    }
    if (predator_cell == internal_state().goal) {
        internal_state().goal = data.inverted_visibility[predator_cell].random_cell();
    }
}

unsigned int Predator::_steps() const {
    unsigned int steps = _speed_base;
    if (Chance::coin_toss(_speed_overflow)) steps++;
    return steps;
}
