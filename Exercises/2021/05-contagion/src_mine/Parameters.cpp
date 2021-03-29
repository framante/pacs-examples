#include "Parameters.hpp"
#include "GetPot"


Domain::Domain(const std::string & filename)
{
  GetPot parser(filename);
  double  domain_size = parser("domain/domain_size", 1.0);
  double  pub_size = parser("domain/pub_size", 0.1);
  double pub_x = parser("domain/pub_x", 0.5);
  double pub_y = parser("domain/pub_y", 0.5);
}

ContagionParameters::ContagionParameters(const std::string & filename)
{
  GetPot parser(filename);
  double n_people = parser("contagion\n_people", 200);
  unsigned int n_timesteps_per_day = parser("contagion\n_timesteps_per_day", 24);
  unsigned int  n_timesteps = parser("contagion\n_timesteps", 1440);
  unsigned int n_init_infected = parser("contagion\n_infected", 1);
  float frac_sd = parser("contagion\frac_sd", 0.75);
  float r_infection = parser("contagion\r_infection", 0.04);
}

PersonParameters::PersonParameters(const std::string & filename)
{
  GetPot parser(filename);
  unsigned int n_timesteps_go_to_pub = parser("person\n_timesteps_go_to_pub", 168);
  unsigned int n_timesteps_at_pub = parser("person\n_timesteps_at_pub", 3);
  unsigned int n_timesteps_recover = parser("person\n_timesteps_recover", 240);
  float dr = parser("person\dr", 0.05);
}
