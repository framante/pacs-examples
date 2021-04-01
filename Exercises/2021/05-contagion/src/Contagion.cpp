#include "Contagion.hpp"
#include "gnuplot-iostream.hpp"


Contagion::Contagion(const std::string &filename)
  : contagion_parameters(filename)
  , time(contagion_parameters.n_timesteps + 1, 0.)
  , n_susceptible(contagion_parameters.n_timesteps + 1, 0)
  , n_infected(contagion_parameters.n_timesteps + 1, 0)
  , n_recovered(contagion_parameters.n_timesteps + 1, 0)
{
  people.reserve(contagion_parameters.n_people);

  
  for (size_t n = 0; n < contagion_parameters.n_people; ++n)
    {
      if (n < contagion_parameters.n_init_infected)
        people.emplace_back(filename, State::Infected);
      else
        people.emplace_back(filename, State::Susceptible);
    }
}

void
Contagion::simulate()
{
  for (unsigned int step = 0; step <= contagion_parameters.n_timesteps; ++step)
    {
      time[step] =
        static_cast<double>(step) / contagion_parameters.n_timesteps_per_day;

      for (size_t n = 0; n < people.size(); ++n)
        {
          // update position
          people[n].move();

          // update the infection state of people
          people[n].update_infection(people);

          if (people[n].is_susceptible())
            ++n_susceptible[n];
          else if (people[n].is_infected())
            ++n_infected[n];
          else // if (people[n].is_recovered())
            ++n_recovered[n];
        }
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
  file << "time, n_susceptible, n_infected, n_recovered" << std::endl;                                                                                   for (unsigned int step = 0; step <= contagion_parameters.n_timesteps;                                                                                       ++step)                                                                                                                                              {                                                                                                                                                         file << time[step] << ", " << n_susceptible[step] << ", "                                                                                                   << n_infected[step] << ", " << n_recovered[step]
                   << std::endl;
     }  
  file.close();

  // Plot results.                                                                                                                                      
  Gnuplot gp;
  gp << "set xlabel 'Time [days]'; set ylabel 'No. of people'; \                                                                                        
set key center "
    "right; plot "
     << gp.file1d(std::tie(time, n_susceptible))
     << "with line linewidth 2 title 'Susceptible',"
     << gp.file1d(std::tie(time, n_infected))
     << "with line linewidth 2 title 'Infected',"
     << gp.file1d(std::tie(time, n_recovered))
     << "with line linewidth 2 title 'Recovered'" << std::endl;
}


