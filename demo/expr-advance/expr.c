#include <stdio.h>
#include <string.h>
#include "expr.h"

ABS_CTOR(Expr_node)
cthis->use = 1;             // 构造函数中，将引用计数初始化为1
END_ABS_CTOR

// Expr_node的析构函数（DTOR/END_DTOR用于实现析构函数语义）
DTOR(Expr_node)
if (--cthis->use == 0) {      // 递减引用计数，如果计数为0，释放自己
  cthis->finalize(cthis); // 释放内存之前先清理资源(其他需要释放的对象）
  return lw_oopc_true;
}
return lw_oopc_false;
END_DTOR

// 构建整数表达式（包含一个整数值，无子表达式），n为整数值
void Expr_initInt(Expr* expr, int n) {
  Int_node* intNode = Int_node_new(lw_oopc_file_line);
  intNode->init(intNode, n);

  expr->p = SUPER_PTR(intNode, Expr_node);
}

// 构建一元表达式（包含一个操作符，一个子表达式），op为操作符，opnd为子表达式
void Expr_initUnary(Expr* expr, const char* op, Expr* opnd) {
  Unary_node* unaryNode = Unary_node_new(lw_oopc_file_line);
  unaryNode->init(unaryNode, op, opnd);
  expr->p = SUPER_PTR(unaryNode, Expr_node);
}

// 构建一元表达式的重载形式(通过传入一个整型值参数，构造一个子表达式为整数表达式的一元表达式）
// op为操作符，a为子表达式的整型值
void Expr_initUnaryX(Expr* expr, const char* op, int a) {
  Expr* intExpr = Expr_new(lw_oopc_file_line);
  Unary_node* unaryNode = Unary_node_new(lw_oopc_file_line);

  intExpr->initInt(intExpr, a);
  unaryNode->init(unaryNode, op, intExpr);
  expr->p = SUPER_PTR(unaryNode, Expr_node);

  Expr_delete(intExpr);
}

// 构建二元表达式（包含一个操作符，二个子表达式）
// op为操作符，left为左子表达式，right为右子表达式
void Expr_initBinary(Expr* expr, const char* op, Expr* left, Expr* right) {
  Binary_node* binaryNode = Binary_node_new(lw_oopc_file_line);
  binaryNode->init(binaryNode, op, left, right);
  expr->p = SUPER_PTR(binaryNode, Expr_node);
}

// 构建二元表达式的重载形式(通过传入两个整型值参数，构造两个子表达式均为整数表达式的二元表达式）
// op为操作符，a为左子表达式的整型值，b为右子表达式的整型值
void Expr_initBinaryX(Expr* expr, const char* op, int a, int b) {
  Expr* left = Expr_new(lw_oopc_file_line);
  Expr* right = Expr_new(lw_oopc_file_line);
  Binary_node* binaryNode = Binary_node_new(lw_oopc_file_line);

  left->initInt(left, a);
  right->initInt(right, b);

  binaryNode->init(binaryNode, op, left, right);
  expr->p = SUPER_PTR(binaryNode, Expr_node);

  Expr_delete(left);
  Expr_delete(right);
}

// 构建三元表达式（包含一个操作符，三个子表达式）
void Expr_initTernary(Expr* expr, const char* op, Expr* left, Expr* middle, Expr* right) {
  Ternary_node* ternaryNode = Ternary_node_new(lw_oopc_file_line);
  ternaryNode->init(ternaryNode, op, left, middle, right);
  expr->p = SUPER_PTR(ternaryNode, Expr_node);
}

// 构建三元表达式的重载形式(通过传入三个整型值参数，构造三个子表达式均为整数表达式的三元表达式）
void Expr_initTernaryX(Expr* expr, const char* op, int a, int b, int c) {
  Expr* left = Expr_new(lw_oopc_file_line);
  Expr* middle = Expr_new(lw_oopc_file_line);
  Expr* right = Expr_new(lw_oopc_file_line);
  Ternary_node* ternaryNode = Ternary_node_new(lw_oopc_file_line);

  left->initInt(left, a);
  middle->initInt(middle, b);
  right->initInt(right, c);

  ternaryNode->init(ternaryNode, op, left, middle, right);
  expr->p = SUPER_PTR(ternaryNode, Expr_node);

  Expr_delete(left);
  Expr_delete(middle);
  Expr_delete(right);
}

// 打印表达式（子树）
void Expr_print(Expr* t) {
  Expr_node* p = t->p;
  p->print(p);
}

CTOR(Expr)
FUNCTION_SETTING(initInt, Expr_initInt);
FUNCTION_SETTING(initUnary, Expr_initUnary);
FUNCTION_SETTING(initUnaryX, Expr_initUnaryX);
FUNCTION_SETTING(initBinary, Expr_initBinary);
FUNCTION_SETTING(initBinaryX, Expr_initBinaryX);
FUNCTION_SETTING(initTernary, Expr_initTernary);
FUNCTION_SETTING(initTernaryX, Expr_initTernaryX);
FUNCTION_SETTING(print, Expr_print);
cthis->use = 1;             // 构造函数中，将引用计数初始化为1
END_CTOR

// Expr的析构函数（DTOR/END_DTOR用于实现析构函数语义）
DTOR(Expr)
if (--cthis->use == 0) {      // 递减引用计数，如果计数为0，释放自己
  Expr_node_delete(cthis->p);
  return lw_oopc_true;
}
return lw_oopc_false;
END_DTOR

// 整数表达式节点的初始化
void Int_node_init(Int_node* t, int k) {
  t->n = k;
}

// 整数表达式节点的打印
void Int_node_print(Expr_node* t) {
  Int_node* cthis = SUB_PTR(t, Expr_node, Int_node);
  printf("%d", cthis->n);
}

// 整数表达式节点的资源清理
void Int_node_finalize(Expr_node* t) {
  // 什么都不需要做
}

CTOR(Int_node)
SUPER_CTOR(Expr_node);
FUNCTION_SETTING(init, Int_node_init);
FUNCTION_SETTING(Expr_node.print, Int_node_print);
FUNCTION_SETTING(Expr_node.finalize, Int_node_finalize);
END_CTOR

// 设置节点的操作符
void setOp(char* opAddr, const char* opValue) {
  memset(opAddr, 0, 3);
  strncpy(opAddr, opValue, 2);
}

// 一元表达式节点的初始化
void Unary_node_init(Unary_node* t, const char* opValue, Expr* b) {
  setOp(t->op, opValue);
  t->opnd = b;
  ++b->use;   // 指针赋值时，将指针所指对象的引用计数自增
}

// 一元表达式节点的打印
void Unary_node_print(Expr_node* t) {
  Unary_node* cthis = SUB_PTR(t, Expr_node, Unary_node);
  Expr* opnd = cthis->opnd;

  printf("(");
  printf("%s", cthis->op);
  opnd->print(opnd);
  printf(")");
}

// 一元表达式节点的资源清理
void Unary_node_finalize(Expr_node* t) {
  Unary_node* cthis = SUB_PTR(t, Expr_node, Unary_node);

  Expr_delete(cthis->opnd);
}

CTOR(Unary_node)
SUPER_CTOR(Expr_node);
FUNCTION_SETTING(init, Unary_node_init);
FUNCTION_SETTING(Expr_node.print, Unary_node_print);
FUNCTION_SETTING(Expr_node.finalize, Unary_node_finalize);
END_CTOR

// 二元表达式节点的初始化
void Binary_node_init(Binary_node* t, const char* opValue, Expr* left, Expr* right) {
  setOp(t->op, opValue);
  t->left = left;
  t->right = right;
  ++left->use;    // 指针赋值时，将指针所指对象的引用计数自增
  ++right->use;   // 指针赋值时，将指针所指对象的引用计数自增
}

// 二元表达式节点的打印
void Binary_node_print(Expr_node* t) {
  Binary_node* cthis = SUB_PTR(t, Expr_node, Binary_node);

  Expr* left = cthis->left;
  Expr* right = cthis->right;

  printf("(");
  left->print(left);
  printf("%s", cthis->op);
  right->print(right);
  printf(")");
}

// 二元表达式节点的资源清理
void Binary_node_finalize(Expr_node* t) {
  Binary_node* cthis = SUB_PTR(t, Expr_node, Binary_node);

  Expr_delete(cthis->left);
  Expr_delete(cthis->right);
}

CTOR(Binary_node)
SUPER_CTOR(Expr_node);
FUNCTION_SETTING(init, Binary_node_init);
FUNCTION_SETTING(Expr_node.print, Binary_node_print);
FUNCTION_SETTING(Expr_node.finalize, Binary_node_finalize);
END_CTOR

// 三元表达式节点的初始化
void Ternary_node_init(Ternary_node* t, const char* opValue,
                       Expr* left, Expr* middle, Expr* right) {
  setOp(t->op, opValue);      //(a);

  t->left = left;
  t->middle = middle;
  t->right = right;
  ++left->use;
  ++middle->use;
  ++right->use;
}

// 三元表达式节点的打印
void Ternary_node_print(Expr_node* t) {
  Ternary_node* cthis = SUB_PTR(t, Expr_node, Ternary_node);
  Expr* left = cthis->left;
  Expr* middle = cthis->middle;
  Expr* right = cthis->right;

  printf("(");
  left->print(left);
  printf(" ? ");
  middle->print(middle);
  printf(" : ");
  right->print(right);
  printf(")");
}

// 三元表达式节点的资源清理
void Ternary_node_finalize(Expr_node* t) {
  Ternary_node* cthis = SUB_PTR(t, Expr_node, Ternary_node);

  Expr_delete(cthis->left);
  Expr_delete(cthis->middle);
  Expr_delete(cthis->right);
}

CTOR(Ternary_node)
SUPER_CTOR(Expr_node);
FUNCTION_SETTING(init, Ternary_node_init);
FUNCTION_SETTING(Expr_node.print, Ternary_node_print);
FUNCTION_SETTING(Expr_node.finalize, Ternary_node_finalize);
END_CTOR
