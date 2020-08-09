#include <static_data.h>
#include <cell_world_tools.h>

using namespace cell_world;
using namespace json_cpp;
using namespace std;

Static_data::Static_data(
        World &world,
        string path_type):
        world_graph(world.create_graph()),
        cells(world_graph.cells),
        map(cells),
        paths(world.create_paths(
            Json_create<Path_builder>(
                    Web_resource::from("paths").key(world.name).key(path_type).get()
                )
            )
        ),
        visibility(
            world.create_graph(
                Json_create<Graph_builder>(
                    Web_resource::from("graph").key(world.name).key("visibility").get()
                    )
                )
            ),
        inverted_visibility(visibility.invert()),
        pois(
            world.create_cell_group(
                Json_create<Cell_group_builder>(
                    Web_resource::from("cell_group").key(world.name).key("pois").get()
                    )
                )
            ),
        pois_graph(
            world.create_graph(
                Json_create<Graph_builder>(
                        Web_resource::from("graph").key(world.name).key("pois").key(path_type).get())
                )
            ){}
