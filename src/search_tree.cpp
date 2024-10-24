#include <search_tree.h>

using namespace std;
using namespace cell_world;

Search_tree_node::Search_tree_node(
        const Cell &cell,
        unsigned int remaining_steps):
        Search_tree_node(*this, cell, remaining_steps)
        {
        }

Search_tree_node::Search_tree_node(
        Search_tree_node &parent,
        const Cell &cell,
        unsigned int remaining_steps):
        options_cells(),
        parent(parent),
        cell(cell),
        remaining_steps(remaining_steps),
        _loaded(false)
        {
        }


Search_tree::Search_tree(
        const Graph &graph,
        const Paths &paths,
        const Cell &cell,
        unsigned int remaining_steps,
        Mode mode) :
        graph(graph),
        paths(paths),
        remaining_steps(remaining_steps),
        root(cell,remaining_steps),
        _current(&root),
        mode(mode){
    _load_current();
}

void Search_tree::_load_current() {
    if (!_current->_loaded) {
        for (const Cell &n:graph[_current->cell]){
            int steps = paths.get_steps(_current->cell,n);
            if (!steps) steps = 1;
            if (steps != Not_found && steps <= (int)_current->remaining_steps) {
                _current->options_cells.add(n);
                _current->options.emplace_back(*_current, n, _current->remaining_steps - steps);
            }
        }
        _current->options_reward = vector<float>(_current->options_cells.size(),0);
        _current->options_visits = vector<unsigned int>(_current->options_cells.size(),0);
        _current->_loaded = true;
    }
}

const Cell &Search_tree::get_option() {
    if (mode == mcts) {
        _current = &pick_random(_current->options);
    } else {
        auto reward_chances = Chance::get_chances(_current->options_reward, -50, 150);
        auto visit_chances = Chance::invert_chances(_current->options_visits);
        auto chances = Chance::combine_chances(reward_chances, visit_chances);
        _current = &(_current->options[Chance::pick(chances)]);
    }
    _load_current();
    return _current->cell;
}

void Search_tree::record_reward(float reward) {
    Search_tree_node *node = _current;
    while (node != &root) {
        auto &cell = node->cell;
        node = &(node->parent);
        int option_index = node->options_cells.find(cell);
        auto &option_counter = node->options_visits[option_index];
        auto &option_reward = node->options_reward[option_index];
        option_reward = (option_reward * option_counter + reward) / (option_counter + 1);
        option_counter ++;
    }
}

void Search_tree::rewind() {
    _current = &root;
}

const Cell &Search_tree::get_best_option() {
    unsigned int max_i = 0;
    for (unsigned int i=1;i<root.options_reward.size();i++)
        if (root.options_reward[max_i]<root.options_reward[i]) max_i=i;
    estimated_reward = root.options_reward[max_i];
    return root.options_cells[max_i];
}

bool Search_tree::finished() {
    return _current->options_cells.empty();
}

const Cell &Search_tree::current_option() const {
    return _current->cell;
}
