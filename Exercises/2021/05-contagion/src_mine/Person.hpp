#ifndef PERSON_HPP
#define PERSON_HPP

#include "Parameters.hpp"
#include <random>
#include <vector>

enum class State
  {
   Infected,
   Susceptible,
   Recovered
  };

enum class Move
  {
   Stay,
   Walk,
   Go_To_Pub,
   Return_From_Pub
  };


class Person
{
public:
  
  Person(const std::string & filename, const State& init_state);
  void move();
  void update_infection(std::vector<Person> & people);

  bool is_infected() const;
  bool is_susceptible() const;
  bool is_recovered() const;
  
private:
  PersonParameters personparams;
  Domain domainparams;
  ContagionParameters contagionparams;

  std::default_random_engine engine;
  std::uniform_real_distribution<double> rand;
  std::uniform_int_distribution<unsigned int> randi;

  State state;

  double x;
  double y;
  
  double x_old; // to be set???
  double y_old;

  

  bool does_sd;
  unsigned int t_infection;

  bool is_at_pub;
  int t_go_to_pub;
  unsigned int t_spent_at_pub;  

};

#endif /* PERSON_HPP */
