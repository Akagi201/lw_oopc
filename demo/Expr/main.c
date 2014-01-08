#include <stdio.h>
#include "expr.h"

int main() {
  Expr* expr1 = Expr_new();
  Expr* expr2 = Expr_new();
  Expr* expr3 = Expr_new();
  Expr* expr = Expr_new();
  //Expr exprx;
  //Expr_ctor(&exprx);

  expr1->initUnaryX(expr1, "-", 5);
  expr2->initBinaryX(expr2, "+", 3, 4);
  expr3->initBinary(expr3, "*", expr1, expr2);
  expr->initBinary(expr, "*", expr3, expr3);

  expr3->print(expr3);
  printf("\n");
  expr->print(expr);
  printf("\n");

  Expr_delete(expr);
  Expr_delete(expr3);
  Expr_delete(expr2);
  Expr_delete(expr1);

  //Expr_dtor(&exprx);

  return 0;
}
