#ifndef PERSON_HPP
#define PERSON_HPP

#include "Parameters.hpp"

class Person
{
public:
  enum State{Infected, Susceptible, Recovered};
  
  Person();
  void move();
private:
  PersonParameters personparams;
  Domain domainparams;
  ContagionParameters contagionparams;

  std::default_random_engine engine;
  std::uniform_real_distribution<double> distr;

  State state;

  double x;
  double y;

  bool does_sd;
  unsigned int t_infection;

  bool is_at_pub;
  unsigned int t_to_go_to_pub;
  unsigned int t_spent_at_pub;  

};

#endif /* PERSON_HPP */
