#include <particle_filter.h>

using namespace cell_world;

Particle_filter::Particle_filter(
        const Cell_group &cells,
        const Graph &world_graph,
        const Graph &visibility,
        const Paths &paths,
        const Cell &start_cell,
        const Cell &goal) :
        cells(cells),
        world_graph(world_graph),
        visibility(visibility),
        paths(paths),
        start_cell(start_cell),
        goal(goal),
        prey(start_cell),
        predator(world_graph,visibility,paths),
        model(cells),
        predator_start_locations(visibility.invert()[start_cell]){
    model.add_agent(prey);
    model.add_agent(predator);
}

unsigned int Particle_filter::create_particles(int count, int limit) {
    if (last_observation.agents_state.empty()) {
        _from_no_observation(count, limit); // random start
    } else {
        _from_last_observation(count, limit); // from specific observation
    }
    return public_particles.size();
}

void Particle_filter::_from_no_observation(int count, int limit) {
    public_particles.clear();
    auto &prey_cell = model.state.public_state.agents_state[0].cell;
    auto &predator_cell = model.state.public_state.agents_state[1].cell;
    auto &predator_state = model.state.public_state.agents_state[1];
    for (int attempt = 0; public_particles.size() < count && attempt < limit; attempt++) {
        predator.start_cell = predator_start_locations.random_cell();
        model.start_episode();
        bool is_good = true;
        for (auto move : trajectory) {
            prey.move = move;
            model.update(); // prey move
            if (visibility[prey_cell].contains(predator_cell)) {
                is_good = false;
                break;
            }
            model.update(); // predator move
            if (visibility[prey_cell].contains(predator_cell)) {
                is_good = false;
                break;
            }
        }
        if (is_good) public_particles.push_back(predator_state);
    }
}

cell_world::Agent_public_state Particle_filter::get_particle() {
    if (public_particles.empty()) return last_observation.agents_state[1];
    return pick_random(public_particles);
}

void Particle_filter::record_observation(const Model_public_state &state) {
    model.set_public_state(state);
    trajectory.clear(); // clears the trajectory.
}

void Particle_filter::_from_last_observation(int count, int limit) {
    public_particles.clear();
    auto &prey_cell = model.state.public_state.agents_state[0].cell;
    auto &predator_cell = model.state.public_state.agents_state[1].cell;
    auto &predator_state = model.state.public_state.agents_state[1];
    for (int attempt = 0; public_particles.size() < count && attempt < limit; attempt++) {
        model.set_public_state(last_observation);
        bool is_good = true;
        if (last_observation.current_turn == 1) { // it is a post move observation
            model.update(); // predator move
            if (visibility[prey_cell].contains(predator_cell)) {
                is_good = false;
                break;
            }
        }
        for (auto move : trajectory) {
            prey.move = move;
            model.update(); // prey move
            if (visibility[prey_cell].contains(predator_cell)) {
                is_good = false;
                break;
            }
            model.update(); // predator move
            if (visibility[prey_cell].contains(predator_cell)) {
                is_good = false;
                break;
            }
        }
        if (is_good) public_particles.push_back(predator_state);
    }
}

const cell_world::Cell &Particle_filter::Prey::start_episode() {
    return start_cell;
}

cell_world::Move Particle_filter::Prey::get_move(const Model_public_state &) {
    return move;
}

cell_world::Agent_status_code Particle_filter::Prey::update_state(const Model_public_state &) {
    return Running;
}

Particle_filter::Prey::Prey(const Cell &start_cell):
    start_cell(start_cell){}
