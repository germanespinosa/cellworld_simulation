#include <simulation.h>

using namespace cell_world;
using namespace cell_world_tools;
using namespace json_cpp;

Simulation::Simulation(Simulation_parameters &sp) :
    world ( sp.world ),
    pois ( sp.world.create_cell_group(
                    Json_create<Cell_group_builder>(
                            Web_resource::from("cell_group")
                            .key(world.name)
                            .key("pois").get()
                    )
            )
    ),
    world_graph(sp.world.create_graph()),
    poi_graph ( sp.world.create_graph(
                    Json_create<Graph_builder>(
                            Web_resource::from("graph")
                            .key(sp.world.name)
                            .key("pois")
                            .key(sp.cmd_parameters["path_type"]).get()
                    )
                )
    ),
    cells (sp.world.create_cell_group()),
    map (cells),
    paths (sp.world.create_paths(sp.path_type)),
    visibility ( sp.world.create_graph(
                    Json_create<Graph_builder>(
                            Web_resource::from("graph")
                            .key(sp.world.name).key("visibility").get()
                    )
                )
    ),
    model (cells),
    prey (cells, world_graph, poi_graph, visibility, paths, map[sp.prey_start], map[sp.goal]),
    predator (world_graph, visibility, paths, map[sp.predator_start])
    {
        model.add_agent(prey);
        model.add_agent(predator);
    }

unsigned int Simulation::run() {
    model.run();
    return 1;
}
