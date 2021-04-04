#include "Person.hpp"
#include <iostream>
#include <chrono>
#include <cmath>
#include <limits>
#include <algorithm>

Person::Person(const std::string & filename, \
	       const State & init_state) :
  personparams(filename),
  domainparams(filename),
  contagionparams(filename),
  engine(std::chrono::system_clock::now().time_since_epoch().count()),
  rand(0., 1.),
  randi(1, personparams.n_timesteps_go_to_pub),
  state(init_state),
  x(rand(engine) * domainparams.domain_size),
  y(rand(engine) * domainparams.domain_size),
  z(rand(engine) * domainparams.domain_size),
  does_sd(false),
  t_infection(0),
  is_at_pub(false),
  t_go_to_pub(randi(engine)), // to be modified later
  t_spent_at_pub(0)
{
  if (rand(engine) < contagionparams.frac_sd)
    does_sd = true;
}

void
Person::move()
{
  t_go_to_pub -= 1;

  if (is_at_pub)
    t_spent_at_pub += 1;
  else
    {
      x_old = x;
      y_old = y;
      z_old = z;
    }

  Move next_move;
  
  if(t_go_to_pub > 0)
    {
      if (does_sd)
	next_move = Move::Stay;
      else
	next_move = Move::Walk;
    }
  else if(is_at_pub)
    {
      if (t_spent_at_pub == personparams.n_timesteps_at_pub)
	next_move = Move::Return_From_Pub;
      else
	next_move = Move::Stay;
    }
  else // if t_go_to_pub == 0
    next_move = Move::Go_To_Pub;


  if (next_move == Move::Walk)
    {
      bool out_of_bounds = true;
      double x_new = 0.;
      double y_new = 0.;
      double z_new = 0.;
      do
	{
	  // Generate random step and update x, y
	  const double phi = 2 * M_PI * rand(engine);
	  const double theta = M_PI * rand(engine);

	  const double dx = personparams.dr * std::cos(phi);
	  const double dy = personparams.dr * std::sin(phi);
	  const double dz = personparams.dr * std::cos(theta);
	  x_new = x + dx;
	  y_new = y + dy;
	  z_new = z + dz;
          // check that x and y are not outside of the domain
	  out_of_bounds = (x_new < 0) or
	    (x_new > domainparams.domain_size)
	    or (y_new < 0) or (y_new > domainparams.domain_size)
	    or (z_new < 0) or (z_new > domainparams.domain_size);
	}
      while(out_of_bounds);
      x = x_new;
      y = y_new;
      z = z_new;
    }
  else if (next_move == Move::Go_To_Pub)
    {
      // generate random position inside the pub domain
      // [pub_x - pub_size/2, pub_x + pub_size/2, pub_z + pub_size/2]
      const double phi = 2 * M_PI * rand(engine);
      const double theta = M_PI * rand(engine);
      
      x = domainparams.pub_x + std::cos(phi) * \
	domainparams.pub_size;
      y = domainparams.pub_x + std::sin(phi) *	\
	domainparams.pub_size;
      z = domainparams.pub_x + std::cos(theta) * \
	domainparams.pub_size;
      
      is_at_pub = true;
      t_go_to_pub = -1;
    }
  else if (next_move == Move::Return_From_Pub)
    {
      // restore old positions
      x = x_old;
      y = y_old;
      z = z_old;
      
      // reset counters
      is_at_pub = false;
      t_go_to_pub = personparams.n_timesteps_go_to_pub;
      t_spent_at_pub = 0;
    }
  else // if (next_move == Move::Stay)
    {
      // do nothing
    }
}
	       
	       
void
Person::update_infection(std::vector<Person> & people)
{
  if (state == State::Infected)
    {
      t_infection += 1;
      
      if (t_infection > personparams.n_timesteps_recover)
	state = State::Recovered;	
    }
  if (state == State::Infected)
    {
     std::for_each(people.begin(), people.end(),
		      [this](Person & person){
	  // compute distance
	  const double x_distance = x - person.x;
	  const double y_distance = y - person.y;
	  const double z_distance = z - person.z;
	  const double distance = std::sqrt(x_distance * x_distance +
					    y_distance * y_distance +
					    z_distance * z_distance);
	  // if it's not me
	  if (distance <= std::numeric_limits<double>::epsilon())
	    return;

	  // if "other is too close and has not previously recovered
	  if (distance < contagionparams.r_infection &&
	      person.state == State::Susceptible)
	    person.state = State::Infected;
		      });
    }
}

bool Person::give_birth(const std::string & filename,
			std::vector<Person> & people)
{
  if (rand(engine) < personparams.birth)
    {
      people.emplace_back(filename, State::Susceptible);
      return true;
    }
  else
    return false;
}

bool Person::die(std::vector<Person> & people)
{
  double probability = rand(engine);
  if (state == State::Infected and
      probability < personparams.disease_death)
    {
      auto it = std::find_if(people.cbegin(), people.cend(),
			     [this](const Person & person)
	    {
	      return (state == person.state and
		      x == person.x and y == person.y and
		      z == person.z and x_old == person.x_old
		      and y_old == person.y_old and
		      z_old == person.z_old and does_sd == person.does_sd
		      and t_infection == person.t_infection and
		      is_at_pub == person.is_at_pub and
		      t_go_to_pub == person.t_go_to_pub and
		      t_spent_at_pub == person.t_spent_at_pub);
			     });
      people.erase(it);
      return true;
    }
  else
    return false;
}
