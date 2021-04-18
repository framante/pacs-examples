#ifndef NEWTON_HPP
#define NEWTON_HPP

#include "NewtonTraits.hpp"
#include "NewtonMethodsSupport.hpp"
#include "JacobianFactory.hpp"

#include <utility>
#include <memory>
#include <iostream>
#include <iomanip>


template<ProblemType Type>
class Newton
{
public:
  using T = NewtonTraits<Type>;
  Newton(const Newton &) = delete;
  Newton(const typename T::NonLinearSystemType system_,
	 std::unique_ptr<JacobianBase<Type>> jac_,
	 NewtonOptions options_ = NewtonOptions())
    : system(system_),
      jac(std::move(jac_)),
      options(options_)
  {};
  
  NewtonResult<Type>
  solve(const typename T::VariableType & x0);
private:
  
  typename T::NonLinearSystemType system;
  std::unique_ptr<JacobianBase<Type>> jac;
  NewtonOptions options;
  NewtonResult<Type> result;
};

template<ProblemType Type>
NewtonResult<Type>
Newton<Type>::solve(const typename T::VariableType & x0)
{
  const auto &[tol_res, tol_incr, max_n_iter] = options;
  auto &[solution, norm_res, norm_incr, iteration, converged] = result;

  solution = x0;

  for(iteration = 0; iteration < max_n_iter; ++iteration)
    {
      // J * delta = res;
      // x = x - delta;

      auto res = system(solution);
      auto delta = (*jac).solve(solution, res);

      solution -= delta;
      norm_res = T::compute_norm(res);
      norm_incr =  T::compute_norm(delta);

      converged = (norm_res <= tol_res && norm_incr <= tol_incr);

      std::cout << std::setw(15) << std::left << "iteration: "
		<< std::setw(5) << std::left << iteration ;
      std::cout << std::setw(15) << std::left << "residual: " 
		<< std::setw(15) << std::left << norm_res;
      std::cout << std::setw(15) << std::left << "increment: "
		<< std::setw(15) << std::left << norm_incr << std::endl;

      if(converged) break;
    }
  return result;
}


#endif /* NEWTON_HPP_ */
