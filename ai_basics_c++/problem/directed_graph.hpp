#pragma once

#include <vector>
#include <type_traits>
#include <iostream>

#include "../interface/state.hpp"

class DirectedGraph{
private:

    int _n_nodes;
    std::vector<int> _edge_end;
    std::vector<double> _weight;
    std::vector<int> _first;
    std::vector<int> _next;

public:
    
    DirectedGraph(int n_nodes) : _n_nodes(n_nodes), _first(n_nodes, -1) {}
    DirectedGraph() = default;

    int n_nodes() const {return _n_nodes;}
    std::vector<int> edge_end() const {return _edge_end;}
    std::vector<double> weight() const {return _weight;}
    std::vector<int> first() const {return _first;}
    std::vector<int> next() const {return _next;}

    void add_edge(int start_node, int end_node, double weight){
        
        _edge_end.push_back(end_node);
        _weight.push_back(weight);
        _next.push_back(_first[start_node]);
        _first[start_node] = _edge_end.size() - 1;
    }

    std::vector<int> edge_indexes_starting_from(int node) const {
        
        std::vector<int> edge_indexes;
        
        for (int index = _first[node]; ~index; index = _next[index]){
            edge_indexes.push_back(index);
        }
        
        return edge_indexes;
    }
};

typedef int DirectedGraphAction;

class DirectedGraphState : public StateBase<DirectedGraphAction> {
private:
    
    int _current_node, _target_node;
    DirectedGraphAction _last_edge_index;
    DirectedGraph const* _graph;
    double _cumulative_cost;

public:
    
    DirectedGraphState() = default;
    
    DirectedGraphState(const DirectedGraph& graph, int current_node, int target_node) :
        _graph(&graph), _current_node(current_node), _target_node(target_node), _last_edge_index(-1), _cumulative_cost(0) {}

    int current_node() const {return _current_node;}
    int target_node() const {return _target_node;}
    int last_edge_index() const {return _last_edge_index;}
    DirectedGraph graph() const {return *_graph;}

    double cumulative_cost() const override {return _cumulative_cost;}

    std::vector<DirectedGraphAction> action_space() const override {
        return _graph->edge_indexes_starting_from(_current_node);
    }
    
    bool success() const override {
        return _current_node == _target_node;
    }
    
    bool fail() const override {
        return not success() and _graph->first()[_current_node] == -1;
    }
    
    // weight of edges
    double cost() const override {
        return _last_edge_index == -1 ? 0 : _graph->weight()[_last_edge_index];
    }

    const DirectedGraphState& next(const DirectedGraphAction& action) const override {
        
        static DirectedGraphState next_state;
        
        next_state = *this;

        next_state._last_edge_index = action;
        next_state._current_node = next_state._graph->edge_end()[action];
        next_state._cumulative_cost += next_state.cost();

        return next_state;
    }

    void show() const override {
        std::cout << "At: " << _current_node << ", from edge: " << _last_edge_index << ", distance: " << _cumulative_cost << std::endl;
    }

    friend struct std::hash<DirectedGraphState>;
    
    friend bool operator== (const DirectedGraphState& ds1, const DirectedGraphState& ds2){
        
        return (ds1._current_node == ds2._current_node 
            and ds1._target_node == ds2._target_node
            and ds1._graph == ds2._graph);
    }
};

template<>
struct std::hash<DirectedGraphState>{
    
    size_t operator() (const DirectedGraphState& ds) const {
        return ds._current_node;
    } 
};
