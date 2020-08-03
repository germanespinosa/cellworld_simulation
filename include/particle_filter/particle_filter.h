#pragma once
#include <cell_world.h>
#include <agents/predator.h>

struct Particle_filter {
    Particle_filter(const cell_world::Cell_group &, const cell_world::Graph &, const cell_world::Graph &, const cell_world::Paths&);

    bool create_particles (int, int);

    const cell_world::Cell_group &cells;
    const cell_world::Graph &world_graph;
    const cell_world::Graph &visibility;
    const cell_world::Paths &paths;

    std::vector<cell_world::Agent_public_state> public_particles;
    std::vector<Predator_state> internal_particles;
};