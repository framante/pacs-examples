#include "Parameters.hpp"
#include "GetPot"


Domain::Domain(const std::string & filename)
{
  GetPot parser(filename);
  domain_size = parser("domain/domain_size", 1.0);
  pub_size = parser("domain/pub_size", 0.1);
  pub_x = parser("domain/pub_x", 0.5);
  pub_y = parser("domain/pub_y", 0.5);
  pub_z = parser("domain/pub_z", 0.5);
}

ContagionParameters::ContagionParameters(const std::string & filename)
{
  GetPot parser(filename);
  n_people = parser("contagion/n_people", 200);
  n_timesteps_per_day = parser("contagion/n_timesteps_per_day", 24);
  n_timesteps = parser("contagion/n_timesteps", 1440);
  n_init_infected = parser("contagion/n_infected", 1);
  frac_sd = parser("contagion/frac_sd", 0.75);
  r_infection = parser("contagion/r_infection", 0.04);
}

PersonParameters::PersonParameters(const std::string & filename)
{
  GetPot parser(filename);
  n_timesteps_go_to_pub = parser("person/n_timesteps_go_to_pub", 168);
  n_timesteps_at_pub = parser("person/n_timesteps_at_pub", 3);
  n_timesteps_recover = parser("person/n_timesteps_recover", 240);
  n_timesteps_incubation = parser("person/n_timesteps_incubation", 96);
  dr = parser("person/dr", 0.05);
  birth = parser("person/birth", 0.3);
  death = parser("person/death", 0.01);
  disease_death = parser("person/disease_death", 0.1);
}
