#ifndef RKFTRAITS_HPP
#define RKFTRAITS_HPP

#include <functional>
#include <vector>
#include "Eigen/Dense"
#include <type_traits>


/*

//this is just like the implementation of NewtonSolver

enum class ProblemType : unsigned int
  {
   Scalar = 0,
   Vector = 1
  };

template<ProblemType Type>
class RKFTraits
{};

template<>
class RKFTraits<ProblemType::Scalar>
{
  using VariableType = double;
  using FunctionType = std::function<VariableType(const double &, const VariableType &)>;
};

template<>
class RKFTraits<ProblemType::Vector>
{
  using VariableType = Eigen::VectorXd;
  using FunctionType = std::function<VariableType(const double &, const VariableType &)>;
};
*/

class ProblemType
{
public:
  using Scalar = double;
  using Vector = Eigen::VectorXd;
};

/// this is more general since it can be used for any Type
/// not only double and Eigen::VectorXd
template<class Type>
class RKFTraits
{
public:
  using VariableType = Type;
  using FunctionType = std::function<Type(const double &, const Type &)>;
};


template<class T>
double
norm(const T & var)
{
  if constexpr (std::is_same_v<T, double>)
		 return std::abs(var);
  else
    return var.norm();
}

/*
enum class RKFScheme : unsigned int
{
  RKF23 = 0,
  RKF45 = 1
};
*/

#endif /* RKFTRAITS_HPP */
