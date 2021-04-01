#ifndef CONTAGION_HPP
#define CONTAGION_HPP

#include "Parameters.hpp"
#include "Person.hpp"

class Contagion
{
public:
  Contagion(const std::string &filename);

  void run();
  void simulate();
  void output_results() const;

private:
  ContagionParameters contagion_parameters;

  std::vector<Person> people;

  std::vector<double>       time;
  std::vector<unsigned int> n_susceptible;
  std::vector<unsigned int> n_infected;
  std::vector<unsigned int> n_recovered;
};

#endif
