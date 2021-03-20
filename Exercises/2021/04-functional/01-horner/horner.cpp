#include "horner.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

// Parallel policies are only available in GCC since version 9.
#ifdef PARALLELEXEC
#if defined(__GNUC__) && (__GNUC__ >= 9)
#include <execution>
#else
#undef PARALLELEXEC
#endif
#endif

double pow_integer(const double &x, const unsigned int &n)
{
  double res{x};
  for (unsigned int i = 2; i != n + 1; ++i)
    res *= x;
  return res;
}

double
eval(const std::vector<double> &a, const double &x)
{
  double res{0.0};
  for(unsigned int i = 0; i < a.size(); ++i)
    res += a[i] * pow_integer(x, i);
  return res;
}

double
eval_horner(const std::vector<double> &a, const double &x)
{
  double res{0.0};
  for(auto it = a.crbegin(); it != a.crend(); ++it)
    res = (res + *it) * x;
  return res;
}


std::vector<double>
evaluate_poly_old(const std::vector<double> &points,
                  const std::vector<double> &a,
                  eval_method                method)
{
  std::vector<double> res(points.size(), 0.0);
  // first version
  // for(unsigned int i = 0; i < res.size(); ++i)
  //     res[i] = method(a, points[i]);

  // second version
  // std::transform(std::execution::par, points.begin(),
  //                points.end(), res.begin(),
  //                [&a, &method](const double & point)
  //		    {return method(a, point);}        );

  // third version
  std::function<double(double)> eval_point =
    [&a, &method](const double & point){ return method(a, point); };
  std::transform(std::execution::par, points.begin(),
		 points.end(), res.begin(), eval_point);
   
  return res;
}


#ifdef PARALLELEXEC
#warning "Using parallel implementation of std::transform"
//! Evaluates polynomial in a set of points (parallel version)
std::vector<double>
evaluate_poly(const std::vector<double> &points,
              const std::vector<double> &a,
              const std::string &        method)
{
  std::vector<double> res(points.size(), 0.0);

  // it's a std::function<double(const double &x)>
  auto eval = [&a](const double & point) -> double {
      double res{0.0};
      for(unsigned int i = 0; i < a.size(); ++i)
	res += a[i] * pow_integer(point, i);
      return res;  };

  auto eval_horner = [&a](const double & point) -> double{
      double res{0.0};
      for(auto it = a.crbegin(); it != a.crend(); ++it)
        res = (res + *it) * point;
      return res;  };

  if (method == "Standard")
    std::transform(std::execution::par, points.begin(),
                 points.end(), res.begin(),
                 eval);

  else if (method == "Horner")
    std::transform(std::execution::par, points.begin(),
                 points.end(), res.begin(),
                 eval_horner);
  return res;
}

#else
#warning "Using sequential implementation of std::transform"
//! Evaluates polynomial in a set of points
std::vector<double>
evaluate_poly(const std::vector<double> &points,
              const std::vector<double> &a,
              const std::string &        method)
{
  std::vector<double> res(points.size(), 0.0);

  // both eval and eval_horner are
  // std::function<double(const double &x)>                            
  auto eval = [&a](const double & point) -> double {
      double res{0.0};
      for(unsigned int i = 0; i < a.size(); ++i)
        res += a[i] * pow_integer(point, i);
      return res;
	             };

  auto eval_horner = [&a](const double & point) -> double{
      double res{0.0};
      for(auto it = a.crbegin(); it != a.crend(); ++it)
	res = (res + *it) * point;
      return res;
		     };

  if (method == "Standard")
    std::transform(points.begin(), points.end(), res.begin(),
                   eval);

  else if (method == "Horner")
    std::transform(points.begin(), points.end(), res.begin(),
                   eval_horner);
  return res;
}                 
#endif
