#include "RKF.hpp"

#include <exception>
#include <fstream>
#include <iostream>

int
main(int argc, char **argv)
{
  try
    {
      const auto f = [](const double &t, const Eigen::VectorXd &y) {
        const double rho   = 28;
        const double sigma = 10;
        const double beta  = 8.0 / 3;

        Eigen::VectorXd out(3);
        out(0) = sigma * (y(1) - y(0));
        out(1) = y(0) * (rho - y(2)) - y(1);
        out(2) = y(0) * y(1) - beta * y(2);

        return out;
      };

      RKFOptions<RKFType::Vector> options("data_Lorenz.pot", 3);

      RKF<RKFType::Vector> solver(f, options);

      /*
       *this can't be done!
      auto table = RKFScheme::make_Butcher_array("RK45");
      solver.set_table(table);
      */
      const auto solution = solver();

      std::cout << std::boolalpha;
      std::cout << "*** Lorenz system ***" << std::endl
                << "  Failed: " << solution.failed << std::endl
                << "  Error estimate: " << solution.error_estimate << std::endl;
      std::cout << std::endl;

      std::ofstream file("results_Lorenz.out");
      file << solution;
    }
  catch (const std::exception &exc)
    {
      std::cout << exc.what() << std::endl;

      return 1;
    }

  return 0;
}
