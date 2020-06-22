#ifndef ANNDEMO_FORMAT_GRAPH_H_
#define ANNDEMO_FORMAT_GRAPH_H_

#include<fmt/format.h>
#include <iostream>

#include "anndemo/algorithm/undirected_graph.hpp"

// formatting logic for undirected_graph

template <>
struct fmt::formatter<ann::vertex> {

  constexpr auto parse(format_parse_context& ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const ann::vertex& v, FormatContext& ctx) {
    return format_to(ctx.out(), "{}", v.id);
  }

};

inline std::ostream& operator<<(std::ostream& os, const ann::vertex& v) {
    os<<fmt::format("{}", v);
    return os;
}


template <>
struct fmt::formatter<ann::edge> {

  constexpr auto parse(format_parse_context& ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const ann::edge& e, FormatContext& ctx) {
    return format_to(ctx.out(), "[{},{}]", e.a, e.b);
  }

};

inline std::ostream& operator<<(std::ostream& os, const ann::edge& e) {
    os<<fmt::format("{}", e);
    return os;
}


template <>
struct fmt::formatter<ann::undirected_graph> {

  constexpr auto parse(format_parse_context& ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const ann::undirected_graph& g, FormatContext& ctx) {
    auto it = format_to(
        ctx.out(), "{{\"vertex\":{},\"edges\":[", g.vertex_size()
    );
    for(unsigned int i=0;i<(unsigned int)g.adjlist.size();i++) {
        unsigned int size = (unsigned int)g.adjlist[i].size();
        unsigned int cur = 0;
        for(const vertex& v: g.adjlist[i]) {
            it = format_to(it, "{}", edge{ i, v.id });
            if(!(i==g.adjlist.size()-1 && cur==size-1)) {
                it = format_to(it, ",");
            }
            cur++;
        }
    }
    it = format_to(it, "]}}");
    return it;
  }

};

inline std::ostream& operator<<(std::ostream& os, const ann::undirected_graph& e) {
    os<<fmt::format("{}", e);
    return os;
}

#endif