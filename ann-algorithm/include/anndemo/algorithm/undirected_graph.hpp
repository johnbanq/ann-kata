/**
 * graph ADT for ease of use
 */
#ifndef ANNDEMO_ALGORITHM_GRAPH_H_
#define ANNDEMO_ALGORITHM_GRAPH_H_

#include<vector>
#include<list>
#include<algorithm>
#include<stdexcept>

#include<fmt/format.h>
#include <iostream>

namespace ann {

    struct vertex { 

        unsigned int id;

        bool operator==(const vertex& v) const {
            return this->id == v.id;
        }

    };

    struct edge { 
        vertex a; 
        vertex b;

        bool operator==(const edge& e) const {
            return this->a == e.a && this->b == e.b;
        }

    };

    struct undirected_graph {

        undirected_graph() = default;

        //graph literal, helpful in terms of data desc.
        undirected_graph(std::initializer_list<std::pair<unsigned int, unsigned int>> edges) {
            //prepare all the vertexes
            unsigned int maxID = 0;
            for(const auto& e: edges) {
                maxID = std::max(maxID, std::max(e.first, e.second));
            }
            for(unsigned int i=0;i<=maxID;i++) {
                this->add_vertex();
            }

            for(const auto& e: edges) {
                this->add_edge({{e.first}, {e.second}});
            }
        }


        vertex add_vertex() {
            adjlist.emplace_back();
            return vertex{ (unsigned int)adjlist.size()-1 };
        }

        bool has_vertex(vertex v) const {
            return v.id < adjlist.size(); 
        }

        size_t vertex_size() const {
            return adjlist.size();
        }


        //add edge, performs duplication check and would tell you is this edge newly added
        bool add_edge(edge e) {
            if(has_edge(e)) {
                return false;
            } else {
                add_edge_unchecked(e);
                return true;
            }
        }

        //add edge, but do not perform duplication check
        void add_edge_unchecked(edge e) {
            if(!has_vertex(e.a)||!has_vertex(e.b)) {
                throw std::invalid_argument("error.edge_vertex_not_in_graph");
            }
            //dual link
            adjlist[e.a.id].push_back(e.b);
            adjlist[e.b.id].push_back(e.a);
        }

        bool has_edge(edge e) const {
            if(!has_vertex(e.a)||!has_vertex(e.b)) {
                throw std::invalid_argument("error.edge_vertex_not_in_graph");
            }
            auto& adj = adjlist[e.a.id];
            return std::any_of(adj.begin(), adj.end(), [=](vertex v){ return v.id == e.b.id; });
        }

        //a little sugar so you can ranged-for on this
        const std::list<vertex>& adjcent_to(vertex v) const {
            if(!has_vertex(v)) {
                throw std::invalid_argument("error.vertex_not_in_graph");
            }
            return adjlist[v.id];
        }

        std::vector<std::list<vertex>> adjlist;

    };

}

// hashing logic for vertex

namespace std {
    
    template<> struct hash<ann::vertex> {
        std::size_t operator()(const ann::vertex& v) const noexcept {
           return std::hash<unsigned int>{}(v.id);
        }
    };

}

#endif