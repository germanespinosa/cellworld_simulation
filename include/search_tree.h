#pragma once
#include <cell_world.h>

struct Search_tree_node{
    Search_tree_node(Search_tree_node &parent,
                     const cell_world::Cell &,
                     unsigned int);
    cell_world::Cell_group options_cells;
    std::vector<Search_tree_node> options;
    std::vector<double> options_reward;
    std::vector<unsigned int> options_visits;
    Search_tree_node &parent;

    const cell_world::Cell &cell;
    unsigned int remaining_steps;
private:
    bool _loaded;
    Search_tree_node(const cell_world::Cell &,
                     unsigned int);
    friend class Search_tree;
};

struct Search_tree {
    enum Mode{
        mcts,
        ucb1
    };
    Search_tree(const cell_world::Graph &,
                     const cell_world::Paths &,
                     const cell_world::Cell &,
                     unsigned int,
                     Mode );

    void record_reward(double);
    const cell_world::Cell &get_best_option();
    void rewind();
    bool finished();
    const cell_world::Cell &get_option();
    const cell_world::Cell &current_option() const;
    const cell_world::Graph &graph;
    const cell_world::Paths &paths;
    unsigned int remaining_steps;
    Search_tree_node root;
    Search_tree_node *_current;
    double estimated_reward;
    Mode mode;

private:
    void _load_current();
};

