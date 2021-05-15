#include <omp.h>

#include <cmath>
#include <iostream>
#include <iomanip>

int
main(int argc, char ** argv)
{
  const unsigned int n_points = 1000;
  const double h = 1./n_points;
  
  double sum = 0.;
#pragma omp parallel for reduction(+:sum) 
  for (unsigned i = 0; i < n_points ; ++i)
    {
      double x = h * (i + 0.5);
      sum += 4./(1 + x * x);
    }

  double pi = h * sum;
  std::cout << std::setprecision(16) << "pi = " << pi << std::endl;
  std::cout << std::setprecision(16) << "pi(exact) = " << M_PI << std::endl;
  return 0;
}
