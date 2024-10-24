#include <static_data.h>

using namespace cell_world;
using namespace json_cpp;
using namespace std;

Static_data::Static_data(
        World_info world_info,
        string path_type):
        world(World::get_from_world_info(world_info)),
        world_graph(world.create_graph()),
        cells(world_graph.cells),
        map(cells),
        paths(world.create_paths(
                Path_builder::get_from_parameters_name(world_info.world_configuration, world_info.occlusions, path_type)
            )
        ),
        visibility(
            world.create_graph(
                Json_create<Graph_builder>(
                    Resources::from("graph").key(world_info.world_configuration).key(world_info.occlusions).key("cell_visibility").get()
                    )
                )
            ),
        inverted_visibility(Coordinates_visibility::invert(visibility)),
        pois(
            world.create_cell_group(
                Json_create<Cell_group_builder>(
                    Resources::from("cell_group").key(world_info.world_configuration).key(world_info.occlusions).key("lppo").get()
                    )
                )
            ),
        pois_graph(
            world.create_graph(
                Json_create<Graph_builder>(
                Resources::from("graph").key(world_info.world_configuration).key(world_info.occlusions).key("options").get()
                )
            )
        ){}

