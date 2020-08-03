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
        predator(world_graph,visibility,paths){}

bool Particle_filter::create_particles(int count, int limit) {
    
    return true;
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

void Particle_filter::Prey::end_episode(const Model_public_state &) {

}

Particle_filter::Prey::Prey(const Cell &start_cell):
    start_cell(start_cell){}
