#include "Contagion.hpp"
#include <iostream>
int
main(int argc, char **argv)
{
  Contagion contagion("params.pot");
  contagion.run();
  Domain d("params.pot");
  std::cout << d.domain_size << " " << d.pub_size << std::endl;
  ContagionParameters cp("params.pot");
  std::cout << cp.n_init_infected << std::endl;
  PersonParameters pp("params.pot");
  std::cout << pp.n_timesteps_recover << std::endl;
  
  return 0;
}
