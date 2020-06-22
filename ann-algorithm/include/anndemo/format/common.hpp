#ifndef ANNDEMO_FORMAT_COMMON_H_
#define ANNDEMO_FORMAT_COMMON_H_

#include <utility>
#include <vector>
#include <list>
#include <queue>

#include <fmt/format.h>
#include <iostream>


// formatting logic for common containers

template <typename K, typename V>
struct fmt::formatter<std::pair<K, V>> {

  constexpr auto parse(format_parse_context& ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const std::pair<K, V>& v, FormatContext& ctx) {
    auto it = ctx.out();
    return fmt::format_to(ctx.out(), "[{},{}]", v.first, v.second);
  }

};

template<typename K, typename V>
inline std::ostream& operator<<(std::ostream& os, const std::pair<K, V>& v) {
    os<<fmt::format("{}", v);
    return os;
}

template <typename T>
struct fmt::formatter<std::vector<T>> {

  constexpr auto parse(format_parse_context& ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const std::vector<T>& v, FormatContext& ctx) {
    auto it = ctx.out();
    size_t size = v.size();
    size_t cur = 0;
    it = format_to(it, "[");
    for(const auto& t: v) {
        it = format_to(it, "{}", t);
        if(cur!=size-1) {
            it = format_to(it, ",");
        }
        cur++;
    }
    it = format_to(it, "]");
    return it;
  }

};

template<typename T>
inline std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    os<<fmt::format("{}", v);
    return os;
}


template <typename T>
struct fmt::formatter<std::list<T>> {

  constexpr auto parse(format_parse_context& ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const std::list<T>& v, FormatContext& ctx) {
    auto it = ctx.out();
    unsigned int size = v.size();
    unsigned int cur = 0;
    it = format_to(it, "[");
    for(const auto& t: v) {
        it = format_to(it, "{}", t);
        if(cur!=size-1) {
            it = format_to(it, ",");
        }
        cur++;
    }
    it = format_to(it, "]");
    return it;
  }

};

template<typename T>
inline std::ostream& operator<<(std::ostream& os, const std::list<T>& v) {
    os<<fmt::format("{}", v);
    return os;
}

//useful for printing priority_queue
template<typename Elem, typename Cont, typename Comp>
inline std::vector<Elem> project(const std::priority_queue<Elem, Cont, Comp>& q) {
    auto q2 = q;
    std::vector<Elem> result;
    while(!q2.empty()) {
        result.push_back(q2.top());
        q2.pop();
    }
    return result;
}

#endif