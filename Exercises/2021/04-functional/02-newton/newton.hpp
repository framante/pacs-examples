#ifndef NEWTON_HPP
#define NEWTON_HPP
#include <functional>
#include <utility>


class newton_solver
{
private:
  double tol = 1e-7;
  unsigned int iterations = 100;
  std::function<double(double)> fun;
  std::function<double(double)> fun_prime;
public:
  // constructor
  newton_solver(std::function<double(double)> f,
		std::function<double(double)> f_prime,
		double tolerance, unsigned int max_iter);
  // solver
  std::pair<double, unsigned int> solve(double x0);
  // destructor
  ~newton_solver() = default;
};


#endif  
