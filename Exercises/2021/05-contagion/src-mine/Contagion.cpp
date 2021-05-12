#include "Contagion.hpp"
#include "gnuplot-iostream.hpp"
#include <algorithm>


unsigned Contagion::n_people = 0;

Contagion::Contagion(const std::string & filename):
  file_name(filename),
  personparams(filename),
  contagionparams(filename),
  n_infected(contagionparams.n_timesteps + 1, 0),
  n_recovered(contagionparams.n_timesteps + 1, 0),
  n_susceptible(contagionparams.n_timesteps + 1, 0),
  n_exposed(contagionparams.n_timesteps + 1, 0),
  n_dead(contagionparams.n_timesteps + 1, 0),
  time(contagionparams.n_timesteps + 1, 0.)
{
  people.reserve(contagionparams.n_people);
  
  for(std::size_t i = 0; i < contagionparams.n_people; ++i)
    {
      if (i < contagionparams.n_init_infected)
	people.emplace_back(filename, State::Infected);
      else
	people.emplace_back(filename, State::Susceptible);
      ++n_people;
    }
}


void Contagion::simulate()
{
  for (unsigned int step = 0; \
       step <= contagionparams.n_timesteps; ++step)
    {
      time[step] = static_cast<double>(step) / \
	contagionparams.n_timesteps_per_day;

      unsigned int new_borns = 0;

      if (step >= 1)
	std::for_each(people.begin(), people.end(),
		      [this, & new_borns](auto & p){
			// update the position of people
			p.move();
			// update the infected state of people
			p.update_infection(people);
			// natality
			bool born = p.give_birth();
			if (born)
			  ++new_borns;
		      });

      n_susceptible[step] = std::count_if(people.begin(), people.end(),
					  [](const auto & p){
					    return p.is_susceptible();
					  });
      n_infected[step] = std::count_if(people.begin(), people.end(),
					  [](const auto & p){
					    return p.is_infected();
					  });
      n_recovered[step] = std::count_if(people.begin(), people.end(),
					  [](const auto & p){
					    return p.is_recovered();
					  });
      n_exposed[step] = std::count_if(people.begin(), people.end(),
					  [](const auto & p){
					    return p.is_exposed();
					  });
      n_dead[step] = std::count_if(people.begin(), people.end(),
					  [](const auto & p){
					    return p.is_dead();
					  });

      people.reserve(people.size() + new_borns);
      for(unsigned i = 0; i < new_borns; ++i)
	{
	  people.emplace_back(file_name, State::Susceptible);
	  ++n_people;
	  std::cout << "at time step " << step << " " << new_borns
		    << " people were born" << std::endl;
	}

       people.erase(std::remove_if(people.begin(), people.end(),
		     [](auto & p){
		       return p.is_dead();
		     }), people.end());
      if (step >= 1)
	n_dead[step] += n_dead[step-1];

    }
}

void
Contagion::run()
{
  simulate();
  output_results();
}

void
Contagion::add_person()
{
  people.emplace_back(file_name, State::Susceptible);
  ++n_people;
}

void
Contagion::remove_person(container_type::size_type key)
{
  people.erase(people.begin() + key);
  --n_people;
}

void
Contagion::output_results() const
{
  // Output results to CSV file.

  std::ofstream file("output.csv", std::ofstream::out);
  file << std::setw(15) << std::left << "time"
       << std::setw(15) << std::left << "n_susceptible"
       << std::setw(15) << std::left << "n_infected"
       << std::setw(15) << std::left << "n_recovered"
       << std::setw(15) << std::left << "n_exposed"
       << std::setw(15) << std::left << "n_dead"
       << std::endl;

  for (unsigned int step = 0; step <= contagionparams.n_timesteps; \
       ++step)
    {
      file << std::setw(15) << std::left << time[step]
	   << std::setw(15) << std::left << n_susceptible[step]
           << std::setw(15) << std::left << n_infected[step]
	   << std::setw(15) << std::left << n_recovered[step]
	   << std::setw(15) << std::left << n_exposed[step]
	   << std::setw(15) << std::left << n_dead[step]
	   << std::endl;
    }
  file.close();

  // Print a resume of what happened
  std::cout << "final results: " << std::endl;
  std::cout << "Initial susceptible: " << n_susceptible[0] << std::endl;
  std::cout << "Final susceptible: " << n_susceptible[contagionparams.n_timesteps] << std::endl;
  std::cout << "Initial infected: " << n_infected[0] << std::endl;
  std::cout << "Final infected: " << n_infected[contagionparams.n_timesteps] << std::endl;
  std::cout << "Initial recovered: " << n_recovered[0] << std::endl;
  std::cout << "Final recovered: " << n_recovered[contagionparams.n_timesteps] << std::endl;
  std::cout << "Initial exposed: "<< n_exposed[0] << std::endl;
  std::cout << "Final exposed: " << n_exposed[contagionparams.n_timesteps] << std::endl;
  std::cout << "Initial dead: "<< n_dead[0] << std::endl;
  std::cout << "Final dead: " << n_dead[contagionparams.n_timesteps] << std::endl;
  
  // Plot results.                                                                                                                                 
  Gnuplot gp;
  gp << "set xlabel 'Time [days]'; set ylabel 'No. of people'; set key center ""right; plot "
     << gp.file1d(std::tie(time, n_susceptible))
     << "with line linewidth 2 title 'Susceptible',"
     << gp.file1d(std::tie(time, n_infected))
     << "with line linewidth 2 title 'Infected',"
     << gp.file1d(std::tie(time, n_exposed))
     << "with line linewidth 2 title 'Exposed',"
     << gp.file1d(std::tie(time, n_recovered))
     << "with line linewidth 2 title 'Recovered',"
     << gp.file1d(std::tie(time, n_dead))
     << "with line linewidth 2 title 'Dead'" << std::endl;
}

