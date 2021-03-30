#include "Person.hpp"
#include <iostream>
#include <chrono>

Person::Person(const std::string & filename, \
	       const State & init_state) :
  personparams(PersonParameters(filename)),
  domainparams(Domain(filename)),
  contagionparams(ContagionParameters(filename)),
  engine(std::chrono::system_clock::now().time_since_epoch().count()),
  rand(0., 1.),
  randi(1, personparams.n_timesteps_go_to_pub),
  state(init_state),
  x(rand(engine) * domainparams.domain_size),
  y(rand(engine) * domainparams.domain_size),
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
  t_go_to_pub += 1;

  if (is_at_pub)
    t_spent_at_pub += 1;
  else
    {
      x_old = x;
      y_old = y;
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
      // generate random step and update x, y
      const double alpha = 2 * M_PI * rand(engine);
      const double dx = personparams.dr * std::cos(alpha);
      const double dy = personparams.dr * std::sin(alpha);
      x += dx;
      y += dy;
      
      // check that x and y are not outside of the domain
      if (x < 0)
	x = 0;
      else if (x > domainparams.domain_size)
	x = domainparams.domain_size;
      if (y < 0)
	y = 0;
      else if (y < domainparams.domain_size)
	y = domainparams.domain_size;
    }
  else if (next_move == Move::Stay)
    {
      // do nothing
    }
  
  else if (next_move == Move::Go_To_Pub)
    {
      // generate random position inside the pub domain
      // [pub_x - pub_size/2, pub_x + pub_size/2]
      const double alpha = 2 * M_PI * rand(engine);
      x = domainparams.pub_x + std::cos(alpha) * \
	domainparams.pub_size;
      y = domainparams.pub_x + std::sin(alpha) * \
	domainparams.pub_size;

      is_at_pub = true;
      t_go_to_pub = -1;
      //t_spent_at_pub = 0;
    }
  else if (next_move == Move::Return_From_Pub)
    {
      // restore old positions
      x = x_old;
      y = y_old;
      
      // reset counters
      is_at_pub = false;
      t_go_to_pub = personparams.n_timesteps_go_to_pub;
      t_spent_at_pub = 0;
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
      for(auto & person : people)
	{
	  // compute distance
	  const double x_distance = x - person.x;
	  const double y_distance = y - person.y;
	  const double distance = std::sqrt(x_distance * x_distance +
					    y_distance * y_distance);
	  // if it's not me
	  if (distance < 1e-12)
	    continue;
	  else
	    break;

	  // if "other is too close and has not previously recovered
	  if (distance < contagionparams.r_infection &&
	      person.state == State::Susceptible)
	    person.state = State::Infected;
	}
    }
}


bool
Person::is_infected() const
{
  return ( state == State::Infected);
}

bool
Person::is_susceptible() const
{
  return ( state == State::Susceptible);
}

bool
Person::is_recovered() const
{
  return ( state == State::Recovered);
}
