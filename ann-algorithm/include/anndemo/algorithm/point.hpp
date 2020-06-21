#ifndef ANNDEMO_ALGORITHM_POINT_H_
#define ANNDEMO_ALGORITHM_POINT_H_

#include <tuple>
#include <fmt/format.h>

namespace ann {

    using point = std::tuple<int, int>;

    inline int getX(const point& p) {
        return std::get<0>(p);
    }

    inline int getY(const point& p) {
        return std::get<1>(p);
    }

}

// formatting logic for ann::point

template <>
struct fmt::formatter<ann::point> {

  constexpr auto parse(format_parse_context& ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const ann::point& p, FormatContext& ctx) {
    return format_to(
        ctx.out(),
        "({}, {})",
        ann::getX(p), ann::getY(p)
    );
  }
};

#endif