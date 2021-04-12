#ifndef DERIVATIVES_HPP
#define DERIVATIVES_HPP

#include <functional>

class BACKWARD;

class FORWARD
{
public:
  using Other = BACKWARD;
};

class BACKWARD
{
public:
  using Other = FORWARD;
};

template<unsigned int N, typename functype, class Differentiation>
class NthDerivative
{
public:
  NthDerivative(const functype & f, const double & step) :
    previous_derivative(f, step), h(step) {};
  double
  operator()(const double & x) const
  {
    if constexpr(std::is_same_v<Differentiation, FORWARD>)
		  return (previous_derivative(x + h) - previous_derivative(x)) / h;
    else
      return (previous_derivative(x) - previous_derivative(x - h)) / h;
  };
  
private:
  // notice that you need typename because it's a type
  // that depends on a template parameter
  NthDerivative<N-1, functype, typename Differentiation::Other> previous_derivative;
  double h;
};

template<typename functype, class Differentiation>
class NthDerivative<1, functype, Differentiation>
{
public:
  NthDerivative(const functype & func, const double & step) :
    f(func), h(step) {};
  double
  operator()(const double & x) const
  {
    if constexpr(std::is_same_v<Differentiation, FORWARD>)
		  return (f(x + h) - f(x)) / h;
    else
      return (f(x) - f(x - h)) / h;
  };
private:
  functype f;
  double h;
};
	     
template<unsigned int N, class functype>
auto
make_forward_derivative(const functype & f, const double & h)
{
  return NthDerivative<N, functype, FORWARD>(f, h);
}

template<unsigned int N, class functype>
auto
make_backward_derivative(const functype & f, const double & h)
{
  return NthDerivative<N, functype, BACKWARD>(f, h);
}



#endif /* DERIVATIVES_HPP */
