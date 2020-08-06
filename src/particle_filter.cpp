#include <particle_filter.h>

using namespace cell_world;

Particle_filter::Particle_filter(
        const Cell_group &cells,
        const Graph &world_graph,
        const Graph &visibility,
        const Paths &paths,
        const Cell &start_cell,
        const Cell &goal,
        unsigned int particle_count,
        unsigned int attempts_limit) :
        cells(cells),
        world_graph(world_graph),
        visibility(visibility),
        paths(paths),
        start_cell(start_cell),
        goal(goal),
        prey(start_cell),
        predator(world_graph,visibility,paths),
        model(Model(cells).add_agent(prey).add_agent(predator)),
        predator_start_locations(visibility.invert()[start_cell]),
        particle_count(particle_count),
        attempts_limit(attempts_limit),
        _prey_cell (prey.public_state().cell),
        _predator_state(predator.public_state()),
        _predator_cell(_predator_state.cell),
        _predator_internal_state(predator.internal_state()){}

unsigned int Particle_filter::create_particles() {
    particles.clear();
    if (!trajectory.empty()) {
        if (last_observation.agents_state.empty()) {
            _from_no_observation(); // random start
        } else {
            _from_last_observation(); // from specific observation
        }
    }
    return particles.size();
}

void Particle_filter::_from_no_observation() {
    if (predator_start_locations.empty()) return; // all cells are visible, no predator on site
    for (int attempt = 0; particles.size() < particle_count && attempt < attempts_limit; attempt++) {
        predator.start_cell = predator_start_locations.random_cell();
        model.start_episode();
        bool is_good = true;
        for (auto move : trajectory) {
            prey.move = move;
            model.update(); // prey move
            if (visibility[_prey_cell].contains(_predator_cell)) {
                is_good = false;
                break;
            }
            model.update(); // predator move
            if (visibility[_prey_cell].contains(_predator_cell)) {
                is_good = false;
                break;
            }
        }
        if (is_good) particles.emplace_back(_predator_state, _predator_internal_state);
    }
}

void Particle_filter::record_observation(const Model_public_state &state) {
    model.set_public_state(state);
    trajectory.clear(); // clears the trajectory.
}

void Particle_filter::_from_last_observation() {
    for (int attempt = 0; particles.size() < particle_count && attempt < attempts_limit; attempt++) {
        model.set_public_state(last_observation);
        bool is_good = true;
        if (last_observation.current_turn == 1) { // it is a post move observation
            model.update(); // predator move
            if (visibility[_prey_cell].contains(_predator_cell)) {
                is_good = false;
                break;
            }
        }
        for (auto move : trajectory) {
            prey.move = move;
            model.update(); // prey move
            if (visibility[_prey_cell].contains(_predator_cell)) {
                is_good = false;
                break;
            }
            model.update(); // predator move
            if (visibility[_prey_cell].contains(_predator_cell)) {
                is_good = false;
                break;
            }
        }
        if (is_good) particles.emplace_back(_predator_state,_predator_internal_state);
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

Particle::Particle(const cell_world::Agent_public_state &public_state, const Predator_state &internal_state):
    public_state(public_state),
    internal_state(internal_state){
}
