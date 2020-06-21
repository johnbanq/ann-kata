#include <cstdlib>
#include <stdexcept>
#include "catch.hpp"
#include "anndemo/algorithm/undirected_graph.hpp"

using ann::undirected_graph;
using ann::vertex;
using ann::edge;

TEST_CASE( "graph literal works", "[algorithm]" ) {
    undirected_graph g {
        {0,1},
        {1,2},
        {2,3}
    };

    CHECK(g.vertex_size() == 4);
    CHECK(g.has_edge({{0}, {1}}));
    CHECK(g.has_edge({{1}, {2}}));
    CHECK(g.has_edge({{2}, {3}}));
}

TEST_CASE( "add_vertex works", "[algorithm]" ) {
    undirected_graph g;

    auto v = g.add_vertex();

    CHECK(g.has_vertex(v));
}

TEST_CASE( "has_vertex works", "[algorithm]" ) {
    undirected_graph g;

    CHECK(!g.has_vertex({0}));
    g.add_vertex();
    CHECK(g.has_vertex({0}));
}

TEST_CASE( "vertex_size works", "[algorithm]" ) {
    undirected_graph g;

    CHECK(g.vertex_size() == 0);
    g.add_vertex();
    CHECK(g.vertex_size() == 1);
}


TEST_CASE( "add_edge works", "[algorithm]" ) {
    undirected_graph g;
    auto a = g.add_vertex();
    auto b = g.add_vertex();

    //first time add: returns true, adds the edge
    CHECK(!g.has_edge({a, b}));
    CHECK(g.add_edge({a, b}));
    CHECK(g.has_edge({a, b}));

    //repeated add: returns false
    CHECK(!g.add_edge({a, b}));
}

TEST_CASE( "add_edge rejects invalid edge", "[algorithm]" ) {
    
    undirected_graph g;
    vertex a{0};
    vertex b{1};

    CHECK_THROWS_AS(g.add_edge({a, b}), std::invalid_argument);
}


TEST_CASE( "add_edge_unchecked works", "[algorithm]" ) {
    undirected_graph g;
    auto a = g.add_vertex();
    auto b = g.add_vertex();

    CHECK(!g.has_edge({a, b}));
    g.add_edge_unchecked({a, b});
    CHECK(g.has_edge({a, b}));
}

TEST_CASE( "add_edge_unchecked rejects invalid edge", "[algorithm]" ) {
    undirected_graph g;
    vertex a{0};
    vertex b{1};

    CHECK_THROWS_AS(g.add_edge_unchecked({a, b}), std::invalid_argument);
}


TEST_CASE( "has_edge works", "[algorithm]" ) {
    undirected_graph g;
    auto a = g.add_vertex();
    auto b = g.add_vertex();

    CHECK(!g.has_edge({a, b}));
    g.add_edge_unchecked({a, b});
    CHECK(g.has_edge({a, b}));
}


TEST_CASE( "adjancent_to works", "[algorithm]" ) {
    undirected_graph g {
        {0,1},
        {0,2},
        {0,3}
    };

    CHECK(g.adjcent_to({0}) == std::list<vertex>{vertex{1},vertex{2},vertex{3}});
}


TEST_CASE( "format works", "[algorithm]" ) {
    undirected_graph g {
        {0,1},
        {0,2},
        {0,3}
    };

    CHECK(fmt::format("{}", g) == "{\"vertex\":4,\"edges\":[[0,1],[0,2],[0,3],[1,0],[2,0],[3,0]]}");
}
