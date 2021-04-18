#ifndef NEWTONTRAITS_HPP
#define NEWTONTRAITS_HPP

#include "Eigen/Dense"
#include <functional>

enum class ProblemType
  {
   Vector,
   Scalar
  };

template<ProblemType Type>
class NewtonTraits
{};

template<>
class NewtonTraits<ProblemType::Scalar>
{
  public:
  using VariableType = double;
  using JacobianMatrixType = double;

  using NonLinearSystemType = std::function<VariableType(const VariableType &)>;
  using JacobianFunctionType = std::function<JacobianMatrixType(const VariableType &)>;

  static double 
  compute_norm(const VariableType & x)
  {
    return std::abs(x);
  }
};

template<>
class NewtonTraits<ProblemType::Vector>
{
public:
  using VariableType = Eigen::VectorXd; // dynamic vector of double
  using JacobianMatrixType = Eigen::MatrixXd;

  using NonLinearSystemType = std::function<VariableType(const VariableType &)>;
  using JacobianFunctionType = std::function<JacobianMatrixType(const VariableType &)>;

  static double
  compute_norm(const VariableType & x)
  {
    return x.norm();
  }
};



#endif /* NEWTONTRAITS_HPP */
