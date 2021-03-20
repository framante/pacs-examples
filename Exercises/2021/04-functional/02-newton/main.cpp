#include "newton.hpp"
#include <iostream>
#include <cmath>
#include "GetPot"

double func(double x){
  return x*x*x + 5*x + 3;
}
double func_prime(double x){
  return 3*x*x + 5;
}


int
main(int argc, char **argv)
{
  GetPot commandline(argc, argv);
  double tol = commandline("tol", 1e-9);
  unsigned int iter = commandline("iter", 100);
  double x0 = commandline("x0", 0.0);
  
  auto f = [](double x){
	     return x*x*x + 5*x + 3; };
  auto df = [](double x){
	        return 3*x*x + 5; };

  newton_solver solver(func, func_prime, tol, iter);
  std::pair<double, unsigned int> res = solver.solve(x0);
  std::cout << "the result is : " << res.first << std::endl;
  std::cout << "settings:\n "
	    << res.second << " iterations over " << iter << "\n" 
	    << tol << " tolerance\n"
	    <<  x0 << " starting point"
	    << std::endl;
  return 0;
}
