#ifndef EXPR_H_INCLUDED_
#define EXPR_H_INCLUDED_

#include "lw_oopc.h"

// 表达式节点
ABS_CLASS(Expr_node) {
  int use;        // 引用计数

  void (* print)(Expr_node* t);        // 打印表达式节点
                                       //int (*eval)(Expr_node* t, int* value);
  void (* finalize)(Expr_node* t);     // 子类通过覆写finalize方法，实现对资源清理行为的定制
};

// 表达式（子树的概念），其中，init*方法族提供了构建子树的高层API，方便用户使用
CLASS(Expr) {
  int use;        // 引用计数
  Expr_node* p;   // 子树的根节点

  // 构建整数表达式（包含一个整数值，无子表达式）
  void (* initInt)(Expr* t, int);

  // 构建一元表达式（包含一个操作符，一个子表达式）
  void (* initUnary)(Expr* t, const char*, Expr*);
  // 构建一元表达式的重载形式(通过传入一个整型值参数，构造一个子表达式为整数表达式的一元表达式）
  void (* initUnaryX)(Expr* t, const char*, int);

  // 构建二元表达式（包含一个操作符，二个子表达式）
  void (* initBinary)(Expr* t, const char*, Expr*, Expr*);
  // 构建二元表达式的重载形式(通过传入两个整型值参数，构造两个子表达式均为整数表达式的二元表达式）
  void (* initBinaryX)(Expr* t, const char*, int, int);

  // 构建三元表达式（包含一个操作符，三个子表达式）
  void (* initTernary)(Expr* t, const char*, Expr*, Expr*, Expr*);
  // 构建三元表达式的重载形式(通过传入三个整型值参数，构造三个子表达式均为整数表达式的三元表达式）
  void (* initTernaryX)(Expr* t, const char*, int, int, int);

  void (* print)(Expr* t);     // 打印子树
};

// 整数表达式节点
CLASS(Int_node) {
  EXTENDS(Expr_node);     // 继承Expr_node

  int n;                  // 整数值

  // 初始化整数表达式节点（传入整数值）
  void (* init)(Int_node* t, int k);
};

// 一元表达式节点
CLASS(Unary_node) {
  EXTENDS(Expr_node);     // 继承Expr_node

  char op[3];             // 假设操作符最长不超过2个字符
  Expr* opnd;             // 子表达式

  // 初始化一元表达式节点（传入操作符和1个子表达式）
  void (* init)(Unary_node* t, const char* a, Expr* b);
};

// 二元表达式节点
CLASS(Binary_node) {
  EXTENDS(Expr_node);     // 继承Expr_node

  char op[3];             // 假设操作符最长不超过2个字符
  Expr* left;             // 左子表达式
  Expr* right;            // 右子表达式

  // 初始化二元表达式节点（传入操作符和2个子表达式）
  void (* init)(Binary_node* t, const char* a, Expr* b, Expr* c);
};

// 三元表达式节点
CLASS(Ternary_node) {
  EXTENDS(Expr_node);

  char op[3];             // 假设操作符最长不超过2个字符
  Expr* left;
  Expr* middle;
  Expr* right;

  // 初始化三元表达式节点（传入操作符和3个子表达式）
  void (* init)(Ternary_node* t, const char* op, Expr* left, Expr* middle, Expr* right);
};

#endif
