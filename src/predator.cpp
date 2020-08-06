#include <predator.h>

using namespace cell_world;
using namespace std;

Predator::Predator(
        const Graph &world_graph,
        const Graph &visibility,
        const Paths &paths,
        const Cell &start_cell):
        world_graph(world_graph),
        visibility(visibility),
        inverted_visibility(visibility.invert()),
        paths(paths),
        start_cell(start_cell){}

Predator::Predator(
        const Graph &world_graph,
        const Graph &visibility,
        const Paths &paths):
        world_graph(world_graph),
        visibility(visibility),
        inverted_visibility(visibility.invert()),
        paths(paths){}

const Cell &Predator::start_episode() {
    internal_state().goal = start_cell;
    return start_cell;
}

Move Predator::get_move(const Model_public_state &state) {
    _process_state(state);
    auto &prey_cell = state.agents_state[0].cell;
    auto &predator_cell = state.agents_state[1].cell;
    return paths.get_move(predator_cell,prey_cell);
}

Agent_status_code Predator::update_state(const Model_public_state &state) {
    _process_state(state);
    return cell_world::Running;
}

void Predator::_process_state(const Model_public_state &state) {
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
