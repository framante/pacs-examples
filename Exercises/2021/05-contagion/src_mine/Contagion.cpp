#include "Contagion.hpp"
#include "gnuplot-iostream.hpp"
#include <algorithm>


Contagion::Contagion(const std::string & filename):
  contagionparams(filename),
  n_infected(contagionparams.n_timesteps + 1, 0),
  n_recovered(contagionparams.n_timesteps + 1, 0),
  n_susceptible(contagionparams.n_timesteps + 1, 0),
  time(contagionparams.n_timesteps + 1, 0.)
{
  people.reserve(contagionparams.n_people);
  
  for(std::size_t i = 0; i < contagionparams.n_people; ++i)
    {
      if (i < contagionparams.n_init_infected)
	people.emplace_back(filename, State::Infected);
      else
	people.emplace_back(filename, State::Susceptible);
    }
}


void Contagion::simulate()
{
  for (unsigned int step = 0; \
       step <= contagionparams.n_timesteps; ++step)
    {
      time[step] = static_cast<double>(step) / \
	contagionparams.n_timesteps_per_day;
      if (step >=1 )
	std::for_each(people.begin(), people.end(),
		      [this](Person & person){
			// update the position of people
			person.move();
			// update the infected state of people
			person.update_infection(people);
		      });
      n_susceptible[step] = std::count_if(people.begin(), people.end(),
					  [](const Person & p){
					    return p.is_susceptible();
					  });
      n_infected[step] = std::count_if(people.begin(), people.end(),
					  [](const Person & p){
					    return p.is_infected();
					  });
      n_recovered[step] = std::count_if(people.begin(), people.end(),
					  [](const Person & p){
					    return p.is_recovered();
					  });
      
    }
}

void
Contagion::run()
{
  simulate();
  output_results();
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
       << std::endl;

  for (unsigned int step = 0; step <= contagionparams.n_timesteps; \
       ++step)
    {
      file << std::setw(15) << std::left << time[step]
	   << std::setw(15) << std::left << n_susceptible[step]
           << std::setw(15) << std::left << n_infected[step]
	   << std::setw(15) << std::left  << n_recovered[step]
	   << std::endl;
    }
  file.close();

  // Print a resume of what happened
  std::cout << "final results: " << std::endl;
  std::cout << "Initial susceptible: " << n_susceptible[0] << std::endl;
  std::cout << "Final susceptible: " << n_susceptible[contagionparams.n_timesteps] << std::endl;
  std::cout << "Initial infected: " << n_infected[0] << std::endl;
  std::cout << "Final infected: " << n_infected[contagionparams.n_timesteps] << std::endl;
  std::cout << "Initial recovered: "<< n_recovered[0] << std::endl;
  std::cout << "Final recovered: " << n_recovered[contagionparams.n_timesteps] << std::endl;
  
  // Plot results.                                                                                                                                 
  Gnuplot gp;
  gp << "set xlabel 'Time [days]'; set ylabel 'No. of people'; set key center ""right; plot "
     << gp.file1d(std::tie(time, n_susceptible))
     << "with line linewidth 2 title 'Susceptible',"
     << gp.file1d(std::tie(time, n_infected))
     << "with line linewidth 2 title 'Infected',"
     << gp.file1d(std::tie(time, n_recovered))
     << "with line linewidth 2 title 'Recovered'" << std::endl;
}
