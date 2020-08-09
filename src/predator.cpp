#include <predator.h>

using namespace cell_world;
using namespace std;

Predator::Predator(
        const Predator_parameters &parameters,
        const Static_data &data):
        parameters(parameters),
        data(data),
        start_cell(data.map[parameters.start]){
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
    Move move {0,0};
    for (unsigned int step=0; step<steps; step++){
        auto current = predator_cell.coordinates + move;
        move += data.paths.get_move(data.map[current],prey_cell);
    }
    return move;
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
    unsigned int steps = Chance::dice(_speed_base);
    if (Chance::coin_toss(_speed_overflow)) steps++;
    return steps;
}
