#ifndef CONTAGION_HPP
#define CONTAGION_HPP

#include "Person.hpp"


class Contagion
{
public:
  Contagion(const std::string & filename);
  void run();
  void output_results() const;
  void simulate();
  
private:
  ContagionParameters contagionparams;
  std::vector<Person> people;
  std::vector<unsigned int> n_infected;
  std::vector<unsigned int> n_recovered;
  std::vector<unsigned int> n_susceptible;
  std::vector<unsigned int> n_exposed;
  std::vector<double> time;
};
#endif /* CONTAGION_HPP */
