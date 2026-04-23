// SPDX-License-Identifier: MIT
// Copyright (c) 2026-present Tian Liao

// std
#include <algorithm>
#include <format>
#include <optional>
#include <ranges>
#include <string_view>

// epx
#include "tmp.hpp"
#include "utils.hpp"

namespace epx::script {

namespace details {
namespace {

class dump_session {
 public:
  explicit dump_session(const mathscript& script) noexcept : script_(script) {}
  std::string run() && {
    for (auto i = 0uz; i < script_.statements.size(); ++i) {
      result_ += std::format("stmt[{}]=", i);
      dump_expr(std::get<expr*>(script_.statements[i]));
    }
    return result_;
  }

 private:
  void fill_margin() { result_.append(margin_, ' '); }
  [[nodiscard]] unsigned new_margin() {
    auto old_margin_ = margin_;
    margin_ = 2u + (unsigned)std::ranges::distance(result_ | std::views::reverse | std::views::take_while([](char ch) {
                                                     return ch != '\n' && ch != '\r';
                                                   }));
    return old_margin_;
  }
  void restore_margin(unsigned old_margin) { margin_ = old_margin; }
  void feed_line() {
    if (result_.empty()) return;
    if (result_.back() == '\r' || result_.back() == '\n') return;
    result_ += '\n';
  }
  void dump_val(const val_term* val) {
    std::visit(tmp::overloads{
                   [&](token_integer_literal literal) { result_ += std::format("[val|integer={}]", literal.raw); },
                   [&](token_real_literal literal) { result_ += std::format("[val|real={}]", literal.raw); },
                   [&](token_id id) { result_ += std::format("[val|id={}]", id.raw); },
                   [&](auto) { result_ = "[val|unknown]"; },
               },
               val->val);
  }
  void dump_binop_expr(const binop_expr* e) {
    char opch;
    switch (e->kind()) {
      case node_kind::add_expr:
        opch = '+';
        break;
      case node_kind::sub_expr:
        opch = '-';
        break;
      case node_kind::mul_expr:
        opch = '*';
        break;
      case node_kind::div_expr:
        opch = '/';
        break;
      default:
        opch = '?';
    }
    auto om = new_margin();
    result_ += std::format("[bin_op_expr|op={}]\n", opch);
    fill_margin();
    result_ += "left=";
    dump_expr(e->left);
    feed_line();
    fill_margin();
    result_ += "right=";
    dump_expr(e->right);
    feed_line();
    restore_margin(om);
  }
  void dump_paren_expr(const paren_expr* e) {
    auto om = new_margin();
    result_ += "[paren_expr]\n";
    fill_margin();
    result_ += "inner=";
    dump_expr(e->inner);
    feed_line();
    restore_margin(om);
  }
  void dump_func_call(const func_call* f) {
    auto om = new_margin();
    result_ += std::format("[func_call|id={}, param_count={}]\n", f->id.raw, f->params.size());
    for (auto i = 0uz; i < f->params.size(); ++i) {
      fill_margin();
      result_ += std::format("param[{}]=", i);
      dump_expr(f->params[i]);
      feed_line();
    }
    restore_margin(om);
  }
  void dump_expr(const expr* e) {
    switch (e->kind()) {
      case node_kind::add_expr:
      case node_kind::sub_expr:
      case node_kind::mul_expr:
      case node_kind::div_expr:
        dump_binop_expr(static_cast<const binop_expr*>(e));
        break;
      case node_kind::val:
        dump_val(static_cast<const val_term*>(e));
        break;
      case node_kind::paren_expr:
        dump_paren_expr(static_cast<const paren_expr*>(e));
        break;
      case node_kind::func_call:
        dump_func_call(static_cast<const func_call*>(e));
        break;
      default:
        result_ += "??? unknown expr ???";
    }
  }

 private:
  std::string result_;
  const mathscript& script_;
  unsigned margin_ = 0;
};

}  // namespace
}  // namespace details

std::string dump(const mathscript& script) noexcept { return details::dump_session{script}.run(); }

}  // namespace epx::script
