#include <poi_prey.h>

Poi_prey::Poi_prey(const cell_world::Cell_group &cells,
                   const cell_world::Graph &world_graph,
                   const cell_world::Graph &pois_graph,
                   const cell_world::Graph &visibility,
                   const cell_world::Paths &paths,
                   const cell_world::Cell &start_cell,
                   const cell_world::Cell &goal):
                   cells(cells),
                   pois_graph(pois_graph),
                   world_graph(world_graph),
                   visibility(visibility),
                   paths(paths),
                   start_cell(start_cell),
                   goal(goal),
                   model(cells),
                   prey(start_cell, goal),
                   predator(world_graph, visibility, paths),
                   particle_filter(cells,world_graph,visibility,paths, start_cell, goal){ }

const cell_world::Cell &Poi_prey::start_episode() {
    return start_cell;
}

cell_world::Move Poi_prey::get_move(const cell_world::Model_public_state &) {
    return cell_world::Move();
}

cell_world::Agent_status_code Poi_prey::update_state(const cell_world::Model_public_state &) {
    return cell_world::Finished;
}


Panning_prey::Panning_prey(const cell_world::Cell &start, const cell_world::Cell &goal) :
        start_cell(start), goal(goal){

}

const cell_world::Cell &Panning_prey::start_episode() {
    return start_cell;
}

cell_world::Move Panning_prey::get_move(const cell_world::Model_public_state &) {
    return move;
}

cell_world::Agent_status_code Panning_prey::update_state(const cell_world::Model_public_state &) {
    return cell_world::Running;
}

void Panning_prey::end_episode(const cell_world::Model_public_state &) {

}
