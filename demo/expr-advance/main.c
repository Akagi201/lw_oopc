#include <stdio.h>
#include "expr.h"

int main() {
  Expr* expr1 = Expr_new(lw_oopc_file_line);
  Expr* expr2 = Expr_new(lw_oopc_file_line);
  Expr* expr3 = Expr_new(lw_oopc_file_line);
  Expr* expr = Expr_new(lw_oopc_file_line);

  expr1->initUnaryX(expr1, "-", 0);
  expr2->initUnaryX(expr2, "-", 5);
  expr3->initBinaryX(expr3, "+", 3, 4);
  expr->initTernary(expr, "?:", expr1, expr2, expr3);

  expr->print(expr);
  printf("\n");

  Expr_delete(expr);
  Expr_delete(expr3);
  Expr_delete(expr2);
  //Expr_delete(expr1);

  lw_oopc_report();

  return 0;
}
