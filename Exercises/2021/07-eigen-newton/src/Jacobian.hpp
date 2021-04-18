#ifndef JACOBIAN_HPP
#define JACOBIAN_HPP

#include "NewtonTraits.hpp"


template<ProblemType Type>
class JacobianBase
{
public:
  using T = NewtonTraits<Type>;
  JacobianBase() = default;
  virtual ~JacobianBase() = default;

  virtual typename T::VariableType
  solve(const typename T::VariableType & x,
	const typename T::VariableType & res) const = 0;
};


template<ProblemType Type>
class FullJacobian : public JacobianBase<Type>
{
public:
  using T = NewtonTraits<Type>;
  FullJacobian(const typename T::JacobianFunctionType & jac_) : jac(jac_) {};

  virtual typename T::VariableType
  solve(const typename T::VariableType & x,
	const typename T::VariableType & res) const override;
private:
  typename T::JacobianFunctionType jac;
};


template<ProblemType Type>
class DiscreteJacobian : public JacobianBase<Type>
{
public:
  using T = NewtonTraits<Type>;
  DiscreteJacobian(const typename T::NonLinearSystemType & system_,
		   const double & h_) :
    system(system_),
    h(h_) {};
  
  virtual typename T::VariableType
  solve(const typename T::VariableType & x,
	const typename T::VariableType & res) const override;
private:
  typename T::NonLinearSystemType system;
  double h;
  
};


#endif /* JACOBIAN_HPP */
