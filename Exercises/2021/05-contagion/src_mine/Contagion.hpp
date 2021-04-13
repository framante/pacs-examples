#ifndef CONTAGION_HPP
#define CONTAGION_HPP

#include "Person.hpp"


class Contagion
{
public:
  Contagion(const std::string & filename);
  void run();
  
private:
  bool mortality();
  bool natality();

  void output_results() const;
  void simulate();
  
  const std::string file_name;
  PersonParameters personparams;
  ContagionParameters contagionparams;
  container_type people;
  std::vector<unsigned int> n_infected;
  std::vector<unsigned int> n_recovered;
  std::vector<unsigned int> n_susceptible;
  std::vector<unsigned int> n_exposed;
  std::vector<double> time;
};
#endif /* CONTAGION_HPP */
