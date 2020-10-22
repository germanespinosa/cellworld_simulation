#include <training.h>

using namespace cell_world;
using namespace json_cpp;
using namespace std;

Training::Training(Training_parameters &sp) :
        parameters(sp),
        data (
                sp.world,
                sp.path_type),
        model (data.cells),
        prey (data,
                data.map[sp.goal],
                sp.input_file,
                sp.output_file),
        predator (sp.predator,data)
{
    model.add_agent(prey);
    model.add_agent(predator);
}

unsigned int Training::run() {
    auto predator_start_locations = data.cells.free_cells();
    for (unsigned int episode = 0; episode < parameters.episodes; episode ++) {
        if (episode % 1000 == 0) {
            cout << "\r" << "episode " << episode  << " of " << parameters.episodes << " - " << parameters.episodes - episode << " left   ";
        }
        predator.start_cell = predator_start_locations.random_cell();
        model.run();
    }
    cout << "\r" << "done!                                                                                   " << endl;
    return 1;
}