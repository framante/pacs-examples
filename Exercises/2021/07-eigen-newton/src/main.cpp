#include "NewtonTraits.hpp"
#include "JacobianFactory.hpp"
#include "Newton.hpp"
#include <utility>


int
main(int argc, char **argv)
{
  constexpr ProblemType Type = ProblemType::Vector;

  using VariableType       = NewtonTraits<Type>::VariableType;
  using JacobianMatrixType = NewtonTraits<Type>::JacobianMatrixType;

  auto system = [](const VariableType &x) -> VariableType {
    VariableType y(2);

    y(0) = (x[0] - 1) * (x[0] - 1) + 0.1 * (x[1] - 5) * (x[1] - 5);
    y(1) = 1.5 - x[0] - 0.1 * x[1];

    return y;
  };

  auto jacobian_fun = [](const VariableType &x) -> JacobianMatrixType {
    JacobianMatrixType J(2, 2);

    J(0, 0) = 2 * (x[0] - 1);
    J(0, 1) = 0.2 * (x[1] - 5);
    J(1, 0) = -1;
    J(1, 1) = -0.1;

    return J;
  };

  FullJacobian<Type> jac_full(jacobian_fun);

  const double h = 0.1;
  DiscreteJacobian<Type> discrete_jac(system, h);

  auto jac = make_jacobian<Type, JacobianType::Full>(jacobian_fun);
  Newton<Type> newton(system, std::move(jac));
  // now jac is invalid since it has been moved

  NewtonTraits<Type>::VariableType x0(2);
  auto result = newton.solve(x0);
  // or auto [sol, norm_res, norm_incr, iteration, converged] = newton.solve(x0);
  
  Newton<Type> quasi_newton(system, make_jacobian<Type, JacobianType::Discrete>(system, h));

  
  return 0;
}
