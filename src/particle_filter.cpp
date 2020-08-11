#include <particle_filter.h>

using namespace cell_world;

Particle_filter::Particle_filter(
        const Particle_filter_parameters &parameters,
        const Predator_parameters &predator_parameters,
        const Static_data &data,
        const Cell &start,
        const Cell &goal) :
        parameters(parameters),
        data(data),
        start(start),
        goal(goal),
        predator_start_locations(data.inverted_visibility[start]),
        prey(start),
        predator(predator_parameters,data),
        model(Model(data.cells)),
        observation_counter(0){
    model.add_agent(prey).add_agent(predator);
    model.start_episode();
}

int Particle_filter::create_particles() {
    particles.clear();
    if (observation_counter) { // there was at least one observation
        if (trajectory.empty()) return 0; // predator is currently visible
        _from_last_observation(); // from specific observation
    } else {
        _from_no_observation(); // random start
        if (particles.empty()) return -1; // there is no predator
    }
    return (int)particles.size();
}

void Particle_filter::_from_no_observation() {
    const cell_world::Cell &_prey_cell = prey.public_state().cell;
    const cell_world::Agent_public_state &_predator_state = predator.public_state();
    const cell_world::Cell & _predator_cell= _predator_state.cell;
    Predator_state &_predator_internal_state = predator.internal_state();

    if (predator_start_locations.empty()) return; // all cells are visible, no predator on site
    for (int attempt = 0; particles.size() < parameters.particle_count && attempt < parameters.attempts; attempt++) {
        predator.start_cell = predator_start_locations.random_cell();
        model.restart_episode();
        bool is_good = true;
        for (auto &move : trajectory) {
            prey.move = move;
            model.update(); // prey move
            if (data.visibility[_prey_cell].contains(_predator_cell)) {
                is_good = false;
                break;
            }
            model.update(); // predator move
            if (data.visibility[_prey_cell].contains(_predator_cell)) {
                is_good = false;
                break;
            }
        }
        if (is_good) particles.emplace_back(_predator_state, _predator_internal_state);
    }
}

void Particle_filter::record_observation(const Model_public_state &state) {
    observation_counter++;
   last_observation = state;
    trajectory.clear(); // clears the trajectory.
}

void Particle_filter::_from_last_observation() {
    const cell_world::Cell &_prey_cell = prey.public_state().cell;
    const cell_world::Agent_public_state &_predator_state = predator.public_state();
    const cell_world::Cell & _predator_cell= _predator_state.cell;
    Predator_state &_predator_internal_state = predator.internal_state();

    for (int attempt = 0; particles.size() < parameters.particle_count && attempt < parameters.attempts; attempt++) {
        model.set_public_state(last_observation);
        predator.update_state(last_observation);
        bool is_good = true;
        if (last_observation.current_turn == 1) { // it is a post move observation
            model.update(); // predator move
            if (data.visibility[_prey_cell].contains(_predator_cell)) {
                is_good = false;
                break;
            }
        }
        for (auto &move : trajectory) {
            prey.move = move;
            model.update(); // prey move
            if (data.visibility[_prey_cell].contains(_predator_cell)) {
                is_good = false;
                break;
            }
            model.update(); // predator move
            if (data.visibility[_prey_cell].contains(_predator_cell)) {
                is_good = false;
                break;
            }
        }
        if (is_good) particles.emplace_back(_predator_state,_predator_internal_state);
    }
}

Belief_state Particle_filter::get_belief_state() {
    Belief_state bs;
    Cell_group cells;
    for (auto &p:particles) {
        if (cells.add(p.public_state.cell)){
            bs.particles_coordinates.push_back(p.public_state.cell.coordinates);
            bs.hits.push_back(1);
        } else {
            bs.hits[cells.find(p.public_state.cell)]++;
        }
    }
    return bs;
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
