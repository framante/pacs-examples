#ifndef JACOBIANFACTORY_HPP
#define JACOBIANFACTORY_HPP

#include "Jacobian.hpp"
#include <memory>

enum class JacobianType
  {
   Full,
   Discrete
  };

template<ProblemType ProbType, JacobianType Type, class... Args>
std::unique_ptr<JacobianBase<ProbType>>
make_jacobian(Args &&... args)
{
  if constexpr (Type == JacobianType::Discrete)
		 return std::make_unique<DiscreteJacobian<ProbType>>
		 (std::forward<Args>(args)...);
  else
    return std::make_unique<FullJacobian<ProbType>>
      (std::forward<Args>(args)...);
}


#endif /* JACOBIANFACTORY_HPP */
