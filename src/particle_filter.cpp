#include <particle_filter.h>

using namespace cell_world;

Particle_filter::Particle_filter(
        const Cell_group &cells,
        const Graph &world_graph,
        const Graph &visibility,
        const Paths &paths) :
        cells(cells),
        world_graph(world_graph),
        visibility(visibility),
        paths(paths){

}

bool Particle_filter::create_particles(int, int) {
    return true;
}
