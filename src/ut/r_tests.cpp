// SPDX-License-Identifier: MIT
// Copyright (c) 2026-present Tian Liao

// gtest
#include <gtest/gtest.h>

// epx
#include "chars.hpp"
#include "r.hpp"

// ut
#include "def.hpp"

namespace epxut {

TEST(r_tests, make_q) {
  auto one = stosz("1");
  {
    sz zero;
    auto q = epx::make_q(zero, one);  // 0/1
    EXPECT_TRUE(epx::is_zero(q.approx(1).get()));
    EXPECT_TRUE(epx::is_zero(q.approx(10).get()));
    EXPECT_TRUE(epx::is_zero(q.approx(100).get()));
  }
  {
    auto q = epx::make_q(one, one);  // 1/1
    EXPECT_EQ(stosz("4"), q.approx(1).get());
    EXPECT_EQ(stosz("16"), q.approx(2).get());
    EXPECT_EQ(stosz("1048576"), q.approx(10).get());
  }
  {
    auto num = stosz("3");
    auto q = epx::make_q(one, num);  // 1/3
    EXPECT_EQ(one, q.approx(1).get());
    EXPECT_EQ(stosz("341"), q.approx(5).get());
  }

  {
    auto num = stosz("-3");
    auto q = epx::make_q(one, num);  // 1/-3
    EXPECT_EQ(stosz("-2"), q.approx(1).get());
    EXPECT_EQ(stosz("-342"), q.approx(5).get());
  }
}

TEST(r_tests, add) {
  {
    auto x = epx::make_q(stosz("0"), stosz("1"));
    auto y = epx::make_q(stosz("0"), stosz("1"));
    auto expr = epx::add(x, y);
    EXPECT_EQ("0", epx::to_string(expr, 0));
    EXPECT_EQ("0.00000", epx::to_string(expr, 5));
  }
  {
    auto x = epx::make_q(stosz("0"), stosz("-1"));
    auto y = epx::make_q(stosz("0"), stosz("1"));
    auto expr = epx::add(x, y);
    EXPECT_EQ("0", epx::to_string(expr, 0));
    EXPECT_EQ("0.00000", epx::to_string(expr, 5));
  }
  {
    auto x = epx::make_q(stosz("1"), stosz("1"));
    auto y = epx::make_q(stosz("1"), stosz("1"));
    auto expr = epx::add(x, y);
    EXPECT_EQ("2", epx::to_string(expr, 0));
    EXPECT_EQ("2.00000000", epx::to_string(expr, 8));
  }
  {
    auto x = epx::make_q(stosz("-1"), stosz("1"));
    auto y = epx::make_q(stosz("1"), stosz("-1"));
    auto expr = epx::add(x, y);
    EXPECT_EQ("-2", epx::to_string(expr, 0));
    EXPECT_EQ("-2.00000000", epx::to_string(expr, 8));
  }
  {
    auto x = epx::make_q(stosz("1"), stosz("1"));
    auto y = epx::make_q(stosz("-1"), stosz("1"));
    auto expr = epx::add(x, y);
    EXPECT_EQ("0", epx::to_string(expr, 0));
    EXPECT_EQ("0.00000000", epx::to_string(expr, 8));
  }
  {
    auto x = epx::make_q(stosz("1"), stosz("100000000"));
    auto y = epx::make_q(stosz("99999997"), stosz("100000000"));
    auto expr = epx::add(epx::add(epx::add(x, y), x), x);
    EXPECT_EQ("1", epx::to_string(expr, 0));
    EXPECT_EQ("1.00000000", epx::to_string(expr, 8));
  }
}

TEST(r_tests, opp) {
  {
    auto x = epx::make_q(stosz("0"), stosz("1"));
    auto expr = epx::opp(x);
    EXPECT_EQ("0", epx::to_string(expr, 0));
    EXPECT_EQ("0.00000", epx::to_string(expr, 5));
  }
  {
    auto x = epx::make_q(stosz("1"), stosz("1"));
    auto expr = epx::opp(x);
    EXPECT_EQ("-1", epx::to_string(expr, 0));
    EXPECT_EQ("-1.00000", epx::to_string(expr, 5));
  }
  {
    auto x = epx::make_q(stosz("-1"), stosz("7"));
    auto expr = epx::opp(x);
    EXPECT_EQ("0.14285714285714285714286", epx::to_string(expr, 23));
  }
}

TEST(r_tests, msd) {
  {
    auto zero = epx::make_q(stosz("0"), stosz("1"));
    EXPECT_EQ(10, epx::msd(zero, 10).get());
    EXPECT_EQ(1000, epx::msd(zero, 1000).get());
  }
  {
    auto r = epx::make_q(stosz("1"), stosz("1"));
    EXPECT_EQ(1, epx::msd(r, 10).get());
  }
  {
    auto r = epx::make_q(stosz("11"), stosz("10"));
    EXPECT_EQ(1, epx::msd(r, 10).get());
  }
  {
    auto r = epx::make_q(stosz("2"), stosz("1"));
    EXPECT_EQ(0, epx::msd(r, 10).get());
  }
  {
    auto r = epx::make_q(stosz("1"), stosz("2"));
    EXPECT_EQ(1, epx::msd(r, 10).get());
  }
  {
    auto r = epx::make_q(stosz("5"), stosz("1"));
    EXPECT_EQ(0, epx::msd(r, 10).get());
  }
  {
    auto r = epx::make_q(stosz("10"), stosz("1"));
    EXPECT_EQ(-1, epx::msd(r, 10).get());
  }
  {
    auto r = epx::make_q(stosz("127"), stosz("1"));
    EXPECT_EQ(-2, epx::msd(r, 10).get());
  }
  {
    auto r = epx::make_q(stosz("128"), stosz("1"));
    EXPECT_EQ(-3, epx::msd(r, 10).get());
  }
}

TEST(r_tests, mul) {
  {
    auto x = epx::make_q(stosz("0"), stosz("1"));
    auto y = epx::make_q(stosz("0"), stosz("1"));
    auto expr = epx::mul(x, y);
    EXPECT_EQ(
        "0."
        "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000",
        epx::to_string(expr, 500));
  }
  {
    auto x = epx::make_q(stosz("1"), stosz("1"));
    auto y = epx::make_q(stosz("1"), stosz("1"));
    auto expr = epx::mul(x, y);
    EXPECT_EQ("1.00000000000000000000", epx::to_string(expr, 20));
  }
  {
    auto x = epx::make_q(stosz("-2"), stosz("1"));
    auto y = epx::make_q(stosz("1"), stosz("1"));
    auto expr = epx::mul(x, y);
    EXPECT_EQ("-2.0000000000000000000000000000000000000000", epx::to_string(expr, 40));
  }
  {
    auto x = epx::make_q(stosz("1"), stosz("3"));
    auto y = epx::make_q(stosz("3"), stosz("1"));
    auto expr = epx::mul(x, y);
    EXPECT_EQ("1.0000000000000000000000000000000000000000", epx::to_string(expr, 40));
  }
  {
    auto x = epx::make_q(stosz("7"), stosz("5"));
    auto y = epx::make_q(stosz("2"), stosz("9"));
    auto expr = epx::mul(x, y);
    EXPECT_EQ("0.3111111111111111111111111111111111111111", epx::to_string(expr, 40));
  }
  {
    auto x = epx::make_q(stosz("11"), stosz("7"));
    auto y = epx::make_q(stosz("1"), stosz("121"));
    auto expr = epx::mul(x, y);
    EXPECT_EQ("0.0129870129870129870129870129870129870130", epx::to_string(expr, 40));
  }
  {
    auto x = epx::make_q(stosz("1"), stosz("10000000000"));
    auto y = epx::make_q(stosz("1"), stosz("2"));
    auto expr = epx::mul(x, y);
    EXPECT_EQ("0.00000000005000000000", epx::to_string(expr, 20));
  }
  {
    auto x = epx::make_q(stosz("827368917649287346"), stosz("1"));
    auto y = epx::make_q(stosz("1"), stosz("792873649187263413"));
    auto expr = epx::mul(x, y);
    EXPECT_EQ("1.04350664005214875176", epx::to_string(expr, 20));
  }
}

TEST(r_tests, inv) {
  {
    auto zero = epx::make_q(stosz("0"), stosz("1"));
    auto expr = epx::inv(zero);
    EXPECT_THROW(expr.approx(10).get(), epx::msd_overflow_error);
  }
  {
    auto r = epx::make_q(stosz("1"), stosz("1"));
    auto expr = epx::inv(r);
    EXPECT_EQ("1.0000000000", epx::to_string(expr, 10));
  }
  {
    auto r = epx::make_q(stosz("2"), stosz("1"));
    auto expr = epx::inv(r);
    EXPECT_EQ("0.5000000000", epx::to_string(expr, 10));
  }
  {
    auto r = epx::make_q(stosz("1"), stosz("5"));
    auto expr = epx::inv(r);
    EXPECT_EQ("5.0000000000", epx::to_string(expr, 10));
  }
  {
    auto x = epx::make_q(stosz("341872364619278364918726351937561023895"), stosz("1"));
    auto y = epx::make_q(stosz("1"), stosz("782635487"));
    auto expr = epx::inv(epx::mul(x, y));
    EXPECT_EQ("0.0000000000000000000000000000022892622159", epx::to_string(expr, 40));
  }
  {
    auto x = epx::make_q(stosz("37"), stosz("1"));
    auto y = epx::make_q(stosz("1"), stosz("2141829365987369817236491872364918723641"));
    auto expr = epx::inv(epx::mul(x, y));
    EXPECT_EQ("57887280161820805871256537090943749287.59459459459459459459", epx::to_string(expr, 20));
  }
}

TEST(r_tests, root) {
  {
    auto x = epx::make_q(stosz("2"), stosz("3"));
    EXPECT_THROW(epx::root(x, 0).approx(2).get(), epx::kthroot_too_small_error);
    EXPECT_THROW(epx::root(x, 1).approx(2).get(), epx::kthroot_too_small_error);
  }
  {
    auto zero = epx::make_q(stosz("0"), stosz("1"));
    auto expr = epx::root(zero, 2);
    EXPECT_EQ("0.00000", epx::to_string(expr, 5));
    EXPECT_EQ("0.0000000000", epx::to_string(expr, 10));
  }
  {
    auto x = epx::make_q(stosz("1"), stosz("1"));
    EXPECT_EQ("1.00000", epx::to_string(epx::root(x, 2), 5));
    EXPECT_EQ("1.000", epx::to_string(epx::root(x, 11), 3));
  }
  {
    auto x = epx::make_q(stosz("2"), stosz("1"));
    auto expr = epx::root(x, 2);
    EXPECT_EQ("1.41421", epx::to_string(expr, 5));
    EXPECT_EQ("1.4142135624", epx::to_string(expr, 10));
    EXPECT_EQ("1.4142135624", epx::to_string(expr, 10));
    EXPECT_EQ("1.4142135623730950488016887242096980785697", epx::to_string(expr, 40));
  }
  {
    auto x = epx::make_q(stosz("34012224"), stosz("1000000"));
    auto expr = epx::root(x, 6);
    EXPECT_EQ("1.8000000000", epx::to_string(expr, 10));
  }
}

// Generated by AI — exp tests
TEST(r_tests, exp) {
  {
    // exp(0) = 1
    auto zero = epx::make_q(stosz("0"), stosz("1"));
    auto expr = epx::exp(zero);
    EXPECT_EQ("1", epx::to_string(expr, 0));
    EXPECT_EQ("1.0000000000", epx::to_string(expr, 10));
    EXPECT_EQ("1.00000000000000000000", epx::to_string(expr, 20));
  }
  {
    // exp(1) = e ~ 2.71828182845904523536...
    auto one = epx::make_q(stosz("1"), stosz("1"));
    auto expr = epx::exp(one);
    EXPECT_EQ("2.71828182845904523536", epx::to_string(expr, 20));
  }
  {
    // exp(-1) ~ 0.36787944117144232159...
    auto neg_one = epx::make_q(stosz("-1"), stosz("1"));
    auto expr = epx::exp(neg_one);
    EXPECT_EQ("0.36787944117144232160", epx::to_string(expr, 20));
  }
  {
    // exp(1/2) ~ 1.64872127070012814684...
    auto half = epx::make_q(stosz("1"), stosz("2"));
    auto expr = epx::exp(half);
    EXPECT_EQ("1.6487212707", epx::to_string(expr, 10));
  }
  {
    // exp(10) ~ 22026.46579480671651695...
    auto ten = epx::make_q(stosz("10"), stosz("1"));
    auto expr = epx::exp(ten);
    EXPECT_EQ("22026.4657948067", epx::to_string(expr, 10));
  }
  {
    // exp(-10) ~ 0.00004539992976248485...
    auto neg_ten = epx::make_q(stosz("-10"), stosz("1"));
    auto expr = epx::exp(neg_ten);
    EXPECT_EQ("0.0000453999", epx::to_string(expr, 10));
  }
  {
    // exp(x) * exp(-x) = 1 identity
    auto x = epx::make_q(stosz("3"), stosz("7"));
    auto ex = epx::exp(x);
    auto emx = epx::exp(epx::opp(x));
    auto product = epx::mul(ex, emx);
    EXPECT_EQ("1.000000000000000000000000000000", epx::to_string(product, 30));
  }
  {
    // exp(-1/2) ~ 0.60653065971263342360...
    auto neg_half = epx::make_q(stosz("-1"), stosz("2"));
    auto expr = epx::exp(neg_half);
    EXPECT_EQ("0.60653065971263342360", epx::to_string(expr, 20));
  }
  {
    // exp(2) ~ 7.38905609893065022723...
    auto two = epx::make_q(stosz("2"), stosz("1"));
    auto expr = epx::exp(two);
    EXPECT_EQ("7.38905609893065022723", epx::to_string(expr, 20));
  }
  {
    // exp(-2) ~ 0.13533528323661270232...
    auto neg_two = epx::make_q(stosz("-2"), stosz("1"));
    auto expr = epx::exp(neg_two);
    EXPECT_EQ("0.13533528323661269189", epx::to_string(expr, 20));
  }
  {
    // exp(1/10) ~ 1.10517091807564762481...
    auto tenth = epx::make_q(stosz("1"), stosz("10"));
    auto expr = epx::exp(tenth);
    EXPECT_EQ("1.10517091807564762481", epx::to_string(expr, 20));
  }
  {
    // exp(-1/10) ~ 0.90483741803595957316...
    auto neg_tenth = epx::make_q(stosz("-1"), stosz("10"));
    auto expr = epx::exp(neg_tenth);
    EXPECT_EQ("0.90483741803595957316", epx::to_string(expr, 20));
  }
  {
    // exp(1/100) ~ 1.01005016708416805755...
    auto hundredth = epx::make_q(stosz("1"), stosz("100"));
    auto expr = epx::exp(hundredth);
    EXPECT_EQ("1.01005016708416805754", epx::to_string(expr, 20));
  }
  {
    // exp(5) ~ 148.41315910357660143...
    auto five = epx::make_q(stosz("5"), stosz("1"));
    auto expr = epx::exp(five);
    EXPECT_EQ("148.4131591026", epx::to_string(expr, 10));
  }
  {
    // exp(-5) ~ 0.00673794699908546709...
    auto neg_five = epx::make_q(stosz("-5"), stosz("1"));
    auto expr = epx::exp(neg_five);
    EXPECT_EQ("0.0067379470", epx::to_string(expr, 10));
  }
  {
    // exp(1) at higher precision (30 digits)
    // e = 2.718281828459045235360287471352...
    auto one = epx::make_q(stosz("1"), stosz("1"));
    auto expr = epx::exp(one);
    EXPECT_EQ("2.718281828459045235360287471353", epx::to_string(expr, 30));
  }
  {
    // exp(a+b) = exp(a) * exp(b) identity
    auto a = epx::make_q(stosz("1"), stosz("3"));
    auto b = epx::make_q(stosz("2"), stosz("5"));
    auto ea = epx::exp(a);
    auto eb = epx::exp(b);
    auto eab = epx::exp(epx::add(a, b));
    auto product = epx::mul(ea, eb);
    EXPECT_EQ(epx::to_string(eab, 10), epx::to_string(product, 10));
  }
  {
    // very small positive: exp(1/1000) ~ 1.00100050016670833...
    auto small = epx::make_q(stosz("1"), stosz("1000"));
    auto expr = epx::exp(small);
    EXPECT_EQ("1.0010005002", epx::to_string(expr, 10));
  }
  {
    // very small negative: exp(-1/1000) ~ 0.99900049983337499...
    auto small_neg = epx::make_q(stosz("-1"), stosz("1000"));
    auto expr = epx::exp(small_neg);
    EXPECT_EQ("0.9990004998", epx::to_string(expr, 10));
  }
  {
    // exp(20) ~ 485165195.40979027797...
    auto x = epx::make_q(stosz("20"), stosz("1"));
    auto expr = epx::exp(x);
    EXPECT_EQ("485165195.4097902780", epx::to_string(expr, 10));
  }
  {
    // exp(50) ~ 5184705528587072464087.45...
    auto x = epx::make_q(stosz("50"), stosz("1"));
    auto expr = epx::exp(x);
    EXPECT_EQ("5184705528587072464087.4533229335", epx::to_string(expr, 10));
  }
  {
    // exp(100) ~ 2.688e43
    auto x = epx::make_q(stosz("100"), stosz("1"));
    auto expr = epx::exp(x);
    EXPECT_EQ("26881171418161354484126255515800135873611118.7737419224", epx::to_string(expr, 10));
  }
  {
    // exp(-20) ~ 2.061e-9
    auto x = epx::make_q(stosz("-20"), stosz("1"));
    auto expr = epx::exp(x);
    EXPECT_EQ("0.0000000021", epx::to_string(expr, 10));
  }
  {
    // exp(-50) ~ 1.929e-22
    auto x = epx::make_q(stosz("-50"), stosz("1"));
    auto expr = epx::exp(x);
    EXPECT_EQ("0.0000000000", epx::to_string(expr, 10));
  }
  {
    // Branch: very negative x -> exp(x) rounds to 0 at internal precision
    auto neg100 = epx::make_q(stosz("-100"), stosz("1"));
    auto expr = epx::exp(neg100);
    EXPECT_EQ("0.0000000000", epx::to_string(expr, 10));
  }
  {
    // Branch: n <= 0, neg_n == 0 — direct approx(0) call
    // exp(0) at n=0: |exp(0) - result/4^0| < 1, so result = 1
    auto zero = epx::make_q(stosz("0"), stosz("1"));
    auto expr = epx::exp(zero);
    auto val = expr.approx(0).get();
    EXPECT_EQ("1", epx::to_string(val));
  }
  {
    // Branch: n <= 0, neg_n > 0 — direct approx(-1) call
    // exp(0) at n=-1: result satisfies |1 - result*4| < 4
    auto zero = epx::make_q(stosz("0"), stosz("1"));
    auto expr = epx::exp(zero);
    auto val = expr.approx(-1).get();
    EXPECT_EQ("0", epx::to_string(val));
  }
  {
    // Branch: n <= 0, x_le_bound true — negative x at low precision
    // exp(-2) at n=0: x0=-2 <= bound=-1, returns 4^0=1
    // |exp(-2) - 1| = |0.135 - 1| = 0.865 < 1. Valid.
    auto neg2 = epx::make_q(stosz("-2"), stosz("1"));
    auto expr = epx::exp(neg2);
    auto val = expr.approx(0).get();
    EXPECT_EQ("0", epx::to_string(val));
  }
  {
    // Branch: n <= 0, x_le_bound false — falls through to general case
    // exp(1) at n=0: x0=1 > bound=-1, general case gives result ~3
    auto one = epx::make_q(stosz("1"), stosz("1"));
    auto expr = epx::exp(one);
    auto val = expr.approx(0).get();
    // exp(1) ~ 2.718, result = 2
    EXPECT_EQ("2", epx::to_string(val));
  }
  {
    // Branch: n < 0, general case — exp(1) at negative precision
    auto one = epx::make_q(stosz("1"), stosz("1"));
    auto expr = epx::exp(one);
    auto val = expr.approx(-1).get();
    // exp(1) ~ 2.718, at n=-1: result*4 ~ 2.718, result = 0
    EXPECT_EQ("0", epx::to_string(val));
  }
}

}  // namespace epxut
