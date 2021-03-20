#include "newton.hpp"
#include "muparser_fun.hpp"
#include "GetPot"
#include <iostream>


int
main(int argc, char **argv)
{
  GetPot commandline("data.txt");
  double tol = commandline("tol", 1e-9);
  unsigned int iter = commandline("iter", 100);
  double x0 = commandline("x0", 0.0);
  std::string fun_expr = commandline("fun", "x^3+5*x+3");
  std::string dfun_expr = commandline("dfun", "3*x^2+5");
  muparser_fun fun(fun_expr);
  muparser_fun dfun(dfun_expr);

  newton_solver solver(fun, dfun, tol, iter);
  std::pair<double, unsigned int> res = solver.solve(x0);
  std::cout << "the result is : " << res.first << std::endl;
  std::cout << "settings:\n "
	    << res.second << " iterations over " << iter << "\n" 
	    << tol << " tolerance\n"
	    <<  x0 << " starting point\n"
	    << fun_expr << " input function\n"
	    << dfun_expr << " derivative"
	    << std::endl;
  return 0;
}
