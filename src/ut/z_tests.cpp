// gtest
#include <gtest/gtest.h>

// epx
#include <z.hpp>

// ut
#include "def.hpp"

namespace epxut {

TEST(z_tests, normalize) {
  {
    sz zero;
    EXPECT_TRUE(epx::is_zero(zero));
    EXPECT_TRUE(epx::is_positive(zero));
    epx::normalize(zero);
    EXPECT_TRUE(epx::is_zero(zero));
    EXPECT_TRUE(epx::is_positive(zero));
  }
  {
    mz zero;
    EXPECT_TRUE(epx::is_zero(zero));
    EXPECT_TRUE(epx::is_positive(zero));

    zero.digits = {0, 0};
    EXPECT_FALSE(epx::is_zero(zero));
    EXPECT_TRUE(epx::is_positive(zero));
    epx::normalize(zero);
    epx::add_n(zero, zero);
    EXPECT_TRUE(epx::is_zero(zero));
    EXPECT_TRUE(epx::is_positive(zero));
  }
  {
    lz zero;
    EXPECT_TRUE(epx::is_zero(zero));
    EXPECT_TRUE(epx::is_positive(zero));

    zero.digits = {0, 0};
    zero.sgn = epx::sign::negative;
    EXPECT_FALSE(epx::is_zero(zero));
    EXPECT_FALSE(epx::is_positive(zero));
    epx::normalize(zero);
    EXPECT_TRUE(epx::is_zero(zero));
    EXPECT_TRUE(epx::is_positive(zero));
  }
  {
    sz num = {.digits = {0, 1, 2, 0, 0}, .sgn = epx::sign::negative};
    EXPECT_FALSE(epx::is_zero(num));
    EXPECT_FALSE(epx::is_positive(num));
    epx::normalize(num);
    auto expected_digits = sz::container_type{0, 1, 2};
    EXPECT_EQ(expected_digits, num.digits);
    EXPECT_FALSE(epx::is_positive(num));
  }
}

TEST(z_tests, add) {
  {
    sz zero, one{.digits = {1}}, minus_one{.digits = {1}, .sgn = epx::sign::negative};
    EXPECT_TRUE(epx::is_zero(epx::add(zero, zero)));
    EXPECT_TRUE(epx::is_zero(epx::add(one, minus_one)));
    EXPECT_EQ(one, epx::add(zero, one));
    EXPECT_EQ(one, epx::add(one, zero));
  }
  {
    sz a = {.digits = {255}};
    sz b = {.digits = {1}};
    sz expected = {.digits = {0, 1}};
    EXPECT_EQ(expected, epx::add(a, b));
    EXPECT_EQ(expected, epx::add(b, a));
  }
  {
    sz a = {.digits = {255}, .sgn = epx::sign::negative};
    sz b = {.digits = {1}, .sgn = epx::sign::negative};
    sz expected = {.digits = {0, 1}, .sgn = epx::sign::negative};
    EXPECT_EQ(expected, epx::add(a, b));
    EXPECT_EQ(expected, epx::add(b, a));
  }
}

TEST(z_tests, sub) {
  sz zero, one{.digits = {1}}, minus_one{.digits = {1}, .sgn = epx::sign::negative}, two{.digits = {2}},
      minus_two{.digits = {2}, .sgn = epx::sign::negative};
  EXPECT_TRUE(epx::is_zero(epx::sub(zero, zero)));
  EXPECT_TRUE(epx::is_zero(epx::sub(one, one)));
  EXPECT_TRUE(epx::is_zero(epx::sub(minus_one, minus_one)));
  EXPECT_EQ(two, epx::sub(one, minus_one));
  EXPECT_EQ(minus_two, epx::sub(minus_one, one));
}

}  // namespace epxut
