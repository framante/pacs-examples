#include "Contagion.hpp"
#include <iostream>
int
main(int argc, char **argv)
{
  Contagion contagion("params.pot");
  contagion.run();
  Domain d("params.pot");
  //std::cout << "domain size: " <<  d.domain_size << std::endl;
  //std::cout << "pub_size:  " << d.pub_size << std::endl;
  ContagionParameters cp("params.pot");
  //std::cout << "initial number of infected: " << cp.n_init_infected << std::endl;
  PersonParameters pp("params.pot");
  
  
  return 0;
}
