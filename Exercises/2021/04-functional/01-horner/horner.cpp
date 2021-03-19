#include "horner.hpp"
#include <iostream>
#include <execution>
#include <cmath>
#include <algorithm>

double
eval(const std::vector<double> &a, const double &x)
{
  double res{0.0};
  for(unsigned int i = 0; i < a.size(); ++i)
    res += a[i] * std::pow(x, i);
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

/*
std::vector<double>
evaluate_poly(const std::vector<double> &points,
              const std::vector<double> &a,
              eval_method                method)
{
  std::vector<double> res(points.size(), 0.0);
  // first version
  // for(unsigned int i = 0; i < res.size(); ++i) res[i] = method(a, points[i]);

  // second version
  // std::transform(std::execution::par, points.begin(), points.end(),
  //		 res.begin(), [&a, &method](const double & point)
  //		 {return method(a, point);} );

  // third version
  std::function<double(double)> evalpoint =
    [&a, &method](const double & point){ return method(a, point); };
  std::transform(std::execution::par, points.begin(),
		 points.end(), res.begin(), evalpoint);
   
  return res;
}
*/

/*
std::vector<double>
evaluate_poly(const std::vector<double> &points,
              const std::vector<double> &a,
              const std::string &        method)
{
  std::vector<double> res(points.size(), 0.0);
  std::function<double(const double &x)> eval =
    [&a](const double & point) -> double {
      double res{0.0};
      for(unsigned int i = 0; i < a.size(); ++i)
	res += a[i] * std::pow(point, i);
      return res;  };

  std::function<double(const double &x)> eval_horner =
    [&a](const double & point) -> double{
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
*/

std::vector<double>                                                   
evaluate_poly(const std::vector<double> &points,                       
              const std::vector<double> &a,                            
              const std::string &        method,
	      const std::string &        mode)                        
{                                                                         std::vector<double> res(points.size(), 0.0);
   auto eval = [&a](const double & point) -> double {                  
      double res{0.0};                                                 
      for(unsigned int i = 0; i < a.size(); ++i)                       
        res += a[i] * std::pow(point, i);                              
      return res;  };                                                   
   auto eval_horner = [&a](const double & point) -> double{            
      double res{0.0};                                                 
      for(auto it = a.crbegin(); it != a.crend(); ++it)                
        res = (res + *it) * point;                                     
      return res;  };
   
   if (mode == "parallel")
     {
       if (method == "Standard")
	 std::transform(std::execution::par, points.begin(),          
			points.end(), res.begin(), eval);
	 else if (method == "Horner")
	   std::transform(std::execution::par, points.begin(),         
                 points.end(), res.begin(), eval_horner);
      }
   else if (mode == "sequential")
     {
       if (method == "Standard")
         std::transform(points.begin(), points.end(), res.begin(),
			eval);
         else if (method == "Horner")
           std::transform(points.begin(),points.end(), res.begin(),
			  eval_horner);
     }
  return res;                                                           
} 
