#include "Person.hpp"
#include <iostream>
#include <chrono>

Person::Person(cons std::string & filename) :
  personparams(PersonParameters(filename)),
  domainparams(Domain(filename)),
  contagionparams(ContagionParameters(filename)),
  engine(std::chrono::system_clock::now().time_since_epoch().count()),
  distr(0.,1.),
  state(Susceptible),
  x(distr(engine) * domainparams.domain_size),
  y(distr(engine) * domainparams.domain_size),
  does_sd(false),
  t_infection(0),
  is_at_pub(false),
  t_to_go_to_pub(0),
  t_spent_at_pub(0)
{
  if (distr(engine) < contagionparams.frac_sd)
    does_sd = true;
}

void
Person::move()
	       
	       
