#include "newton.hpp"

newton_solver::newton_solver(std::function<double(double)> f,
		std::function<double(double)> f_prime,
		double tolerance, unsigned int max_iter):
  fun(f), fun_prime(f_prime), tol(tolerance), iterations(max_iter)
{};


std::pair<double, unsigned int>
newton_solver::solve(double x0)
{
  double delta = - fun(x0) / fun_prime(x0);
  double x1 = x0 + delta;
  unsigned int i = 0;
  
  for (i = 1; i < iterations &&	 std::abs(delta) > tol; ++i)
    {
      x0 = x1;
      delta = -fun(x0) / fun_prime(x0);
      x1 = x0 + delta;
    }

  return std::make_pair(x1, i);
}
