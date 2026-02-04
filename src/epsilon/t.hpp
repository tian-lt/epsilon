#ifndef EPSILON_INC_T_HPP
#define EPSILON_INC_T_HPP

// std
#include <concepts>
#include <cstdint>
#include <ranges>

namespace epx {

template <class T>
concept container = std::ranges::contiguous_range<T> &&  //
                    std::ranges::sized_range<T> &&       //
                    requires {
                      typename T::value_type;
                      std::is_unsigned_v<typename T::value_type>;
                    } && requires(T c) { c.reserve(size_t{}); };

enum class sign : uint8_t { positive, negative };

template <container C>
struct z {
  using container_type = C;
  using digit_type = typename C::value_type;
  std::strong_ordering operator<=>(const z&) const = default;

  C digits;  // // least significant digits (LSD)
  sign sgn = sign::positive;
};

}  // namespace epx

#endif
