#include "Jacobian.hpp"


/*
// this should be moved in the .hpp file since they're methods of template classes
template<ProblemType Type>
NewtonTraits<Type>::VariableType
FullJacobian<Type>::solve(const T::VariableType & x,
				const T::VariableType & res) const
{
  const T::JacobianMatrixType J = jac(x);
  return J.fullPivLu().solve(res);
}


template<ProblemType Type>
NewtonTraits<Type>::VariableType
DiscreteJacobian<Type>::solve(const T::VariableType & x,
				const T::VariableType & res) const
{
  const size_t n = x.size();
  T::JacobianMatrixType J (n, n);

  for(unsigned int i = 0; i < n; ++i)
    {
      T::VariableType x_n(x);
      T::VariableType x_p(x);

      x_n[i] += h;
      x_p[i] -= h;

      T::VariableType f_n = system(x_n);
      T::VariableType f_p = system(x_p);

      J.col(i) = (f_n - f_p) / (2*h);
    }
  return J.fullPivLu().solve(res);
}
*/


// these are the specialization of methods for a template class
// so they can say in the .cpp file !!

template<>
NewtonTraits<ProblemType::Vector>::VariableType
FullJacobian<ProblemType::Vector>::solve(const typename T::VariableType & x,
					 const typename T::VariableType & res) const
{
  const typename T::JacobianMatrixType J = jac(x);
  return J.fullPivLu().solve(res);
}

template<>
NewtonTraits<ProblemType::Scalar>::VariableType
FullJacobian<ProblemType::Scalar>::solve(const typename T::VariableType & x,
					 const typename T::VariableType & res) const
{
  return res / jac(x);
}


template<>
NewtonTraits<ProblemType::Vector>::VariableType
DiscreteJacobian<ProblemType::Vector>::solve(const typename T::VariableType & x,
					     const typename T::VariableType & res) const
{
  const size_t n = x.size();
  typename T::JacobianMatrixType J (n, n);

  for(unsigned int i = 0; i < n; ++i)
    {
      typename T::VariableType x_n(x);
      typename T::VariableType x_p(x);

      x_n[i] += h;
      x_p[i] -= h;

      typename T::VariableType f_n = system(x_n);
      typename T::VariableType f_p = system(x_p);

      J.col(i) = (f_n - f_p) / (2*h);
    }
  return J.fullPivLu().solve(res);
}

template<>
NewtonTraits<ProblemType::Scalar>::VariableType
DiscreteJacobian<ProblemType::Scalar>::solve(const typename T::VariableType & x,
					     const typename T::VariableType & res) const
{
  typename T::VariableType x_n(x);
  typename T::VariableType x_p(x);

  x_n += h;
  x_p -= h;

  typename T::VariableType f_n = system(x_n);
  typename T::VariableType f_p = system(x_p);

  auto J = (f_n - f_p) / (2*h);

  return res / J;
}
