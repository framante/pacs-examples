#ifndef RKF_HPP
#define RKF_HPP

#include "RKFTraits.hpp"
#include "ButcherRKF.hpp"
#include <type_traits>

template<class Type>
class RKFResult
{
public:
  std::vector<double> time;
  std::vector<Type> y;
  double error_estimate = 0.;
  unsigned n_iter = 0;
  unsigned n_expansions = 0;
  unsigned n_reductions = 0;
  bool failed = false;
};

template<class ButcherType, class Type>
class RKF
{
public:
  RKF() = default;
  RKF(const ButcherType & table_, const typename RKFTraits<Type>::FunctionType & func) :
    table(table_), function(func)
  {
    static_assert(std::is_same_v<Type, ProblemType::Scalar> ||
		  std::is_same_v<Type, ProblemType::Vector>,
		  "wrong ProblemType specified.");
  };
  void
  set_table(const ButcherType & table_)
  {
    table = table_;
  };
  void
  set_function(const typename RKFTraits<Type>::FunctionType & func)
  {
    function = func;
  }
  RKFResult<Type>
  solve(const double & t0, const double & tf, const Type & y0,
	const double & h0, const double & tol, const unsigned int & n_max_steps)
  {
    auto & [time, y, error_estimate, n_iter, n_expansions, n_reductions, failed] = result;

    time.push_back(t0);
    y.push_back(y0);

    double time_curr = t0;
    Type y_curr = y0;
    double h = h0;
    bool rejected = false;
    
    for (n_iter = 0; n_iter < n_max_steps; ++n_iter)
      {
	const auto [y_low, y_high] = step(time_curr, y_curr, h);
	const double error = norm(y_high - y_low);
	if (error < tol * h / (tf - t0))
	  {
	    rejected = false;
	    time_curr += h;
	    y_curr = y_high;

	    time.push_back(time_curr);
	    y.push_back(y_curr);
	    error_estimate += error;

	    h *= 1.2;
	    ++n_expansions;
	  }
	else
	  {
	    rejected = true;
	    h *= 0.8;
    	    ++n_reductions;
	  }
	if (time_curr >= tf)
	  break;
      }
    
    if (time_curr < tf && n_iter == n_max_steps)
      {
	failed = true;
	throw std::runtime_error("Final time not reached");
      }
    else
      failed = false;
    
    return result;
  };
  
  std::pair<Type, Type>
  step(const double & t, const Type & y, const double & h)
  {
    Type y_low(y);
    Type y_high(y);

    const auto & A = table.A;
    const auto & b1 = table.b1;
    const auto & b2 = table.b2;
    const auto & c = table.c;

    std::array<Type, ButcherType::n_stages()> K;

    double time(t);
    K[0] = function(t, y) * h;
    
    for (unsigned int i = 1; i < K.size(); ++i)
      {
	time = t + c[i]*h;
	Type yy(y);
	for (unsigned int j = 0; j < i; ++j)
	    yy += A[i][j] * K[j];
	K[i] = function(time, yy) * h;
      }
    
    for (unsigned i = 0; i < K.size(); ++i)
      {
	y_low += h * b1[i] * K[i];
	y_high += h * b2[i] * K[i];
      }
    
    return std::make_pair(y_low, y_high);
  };
private:
  ButcherType table; // the specific method to apply
  typename RKFTraits<Type>::FunctionType function; // the ODE to be solved
  RKFResult<Type> result;  
};


/// Output stream for gnuplot.
/// Possible extension: export also an exact solution, if provided.
template <class RKFType>
std::ostream &
operator<<(std::ostream &out, const RKFResult<RKFType> &res)
{
  out << "# Number ot time steps: " << res.time.size() << std::endl
      << "# Number of reductions: " << res.n_reductions << std::endl
      << "# Number of expansions: " << res.n_expansions << std::endl
      << "# Error estimate: " << res.error_estimate << std::endl;

  double h_min = res.time[1] - res.time[0];
  double h_max = h_min;

  for (unsigned int i = 0; i < res.time.size() - 1; ++i)
    {
      const auto dt = res.time[i + 1] - res.time[i];
      h_max         = std::max(h_max, dt);
      h_min         = std::min(h_min, dt);
    }

  out << "# h_min: " << h_min << ", h_max: " << h_max << std::endl;

  out << "# t ";
  if constexpr (std::is_same_v<RKFType, ProblemType::Scalar>)
    out << "y";
  else // if constexpr (std::is_same_v<RKFType, ProblemType::Vector>)
    out << "y[0] ... y[" << res.y[0].size() - 1 << "]";

  out << std::endl;

  size_t i = 0;
  for (const auto &t : res.time)
    {
      out << "  " << t << "\t";
      const auto &yy = res.y[i];

      if constexpr (std::is_same_v<RKFType, ProblemType::Scalar>)
        {
          out << res.y[i];
        }
      else // if constexpr (std::is_same_v<RKFType, ProblemType::Vector>)
        {
          for (int k = 0; k < yy.size(); ++k)
            out << yy[k] << "\t";
        }

      out << "\n";
      ++i;
    }
  return out;
}

#endif /* RKF_HPP */
