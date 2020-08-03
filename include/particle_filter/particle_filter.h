#pragma once
#include <cell_world.h>
#include <agents/predator.h>

struct Particle_filter {
    Particle_filter(const cell_world::World &, const cell_world::Graph &, const cell_world::Paths&);
    bool create_particles (int, int);
    std::vector<cell_world::Agent_public_state> public_particles;
    std::vector<Predator_state> internal_particles;
};