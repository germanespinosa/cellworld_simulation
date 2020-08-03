#include <simulation.h>

using namespace cell_world;
using namespace cell_world_tools;
using namespace json_cpp;

Simulation::Simulation(Simulation_parameters &sp) :
    world (sp.world),
    pois ( world.create_cell_group(
                    Json_create<Cell_group_builder>(
                            Web_resource::from("cell_group")
                            .key(world.name)
                            .key("pois").get()
                    )
            )
    ),
    world_graph(sp.world.create_graph()),
    poi_graph ( world.create_graph(
                    Json_create<Graph_builder>(
                            Web_resource::from("graph")
                            .key(world.name)
                            .key("pois")
                            .key(sp.cmd_parameters["path-type"]).get()
                    )
                )
    ),
    cells (world.create_cell_group()),
    map (cells),
    paths (world.create_paths(sp.paths)),
    visibility (world.create_graph(Json_create<Graph_builder>(Web_resource::from("graph").key(world.name).key
    ("visibility").get()))),
    Model (cells),
    prey (cells, world_graph, poi_graph, visibility, paths, map[sp.prey_start], map[sp.goal]),
    predator (world_graph, visibility, paths, map[sp.predator_start])
    {
        add_agent(prey);
        add_agent(predator);
    }