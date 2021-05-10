#include "matrix.hpp"

#include <algorithm>
#include <cassert>

#include <Eigen/Dense>
matrix
matrix::transpose() const
{
  matrix result(this->get_cols(), this->get_rows());

  /*
   * standard implementation
  for(std::size_t i = 0; i < result.get_rows(); ++i)
    for(std::size_t j = 0; j < result.get_cols(); ++j)
      result(i, j) = this->operator()(j, i);
  */

  Eigen::Map<Eigen::MatrixXd> result_eigen(result.get_data(),
					   result.get_rows(),
					   result.get_cols());
  result_eigen.transpose();
  
  return result;
}

matrix operator*(const matrix &A, const matrix &B)
{
  matrix result(A.get_rows(), B.get_cols());

  /*
   * standard implementation
  matrix A_t = A.transpose();
  
  for(std::size_t i = 0; i < result.get_rows(); ++i)
    for(std::size_t j = 0; j < result.get_cols(); ++j)
      for(std::size_t k = 0; k < A.get_cols(); ++k)
	result(i, j) += A_t(k, i) * B(k, j);
  */

  Eigen::Map<const Eigen::MatrixXd> A_eigen(A.get_data(),
					    A.get_rows(),
					    A.get_cols());
  Eigen::Map<const Eigen::MatrixXd> B_eigen(B.get_data(),
					    B.get_rows(),
					    B.get_cols());
  Eigen::Map<Eigen::MatrixXd> result_eigen(result.get_data(),
					   result.get_rows(),
					   result.get_cols());
  result_eigen = A_eigen * B_eigen;
  return result;  
}

void
matrix::solve(matrix &rhs)
{
  unsigned int ii, jj, kk;
  double       f;

  // Factorize
  if (!factorized)
    {
      factorize();
      std::cout << "factorize !" << std::endl;
    }

  double *b = rhs.get_data();
  // Do Forward Substitution
  std::cout << "fwdsub !" << std::endl;
  for (ii = 0; ii < get_rows(); ++ii)
    {
      f = b[p[ii]];
      for (kk = 0; kk < ii; ++kk)
        f -= value(p[ii], kk) * b[p[kk]];
      b[p[ii]] = f;
    }

  // Do Backward Substitution
  std::cout << "bwdsub !" << std::endl;
  for (jj = 1; jj <= get_rows(); ++jj)
    {
      ii = get_rows() - jj;
      f  = b[p[ii]];
      for (kk = ii + 1; kk < get_cols(); ++kk)
        f -= value(p[ii], kk) * b[p[kk]];
      b[p[ii]] = f / value(p[ii], ii);
    }
}

void
matrix::factorize()
{
  p.resize(rows, 0);
  for (unsigned int ii = 0; ii < rows; ++ii)
    p[ii] = ii;

  int    m = this->get_rows();
  int    n = this->get_cols();
  int    ii, jj, kk;
  double pivot = 0., maxpivot = 0.;
  int    imaxpivot = 0;

  assert(m == n);
  for (ii = 0; ii < m - 1; ++ii)
    {
      maxpivot  = value(p[ii], ii);
      imaxpivot = ii;
      for (kk = ii + 1; kk < m; ++kk)
        if (value(p[kk], ii) > maxpivot)
          {
            maxpivot  = value(p[kk], ii);
            imaxpivot = kk;
          }

      if (imaxpivot != ii)
        std::swap(p[ii], p[imaxpivot]);

      pivot = value(p[ii], ii);
      for (jj = ii + 1; jj < m; ++jj)
        {
          value(p[jj], ii) = value(p[jj], ii) / pivot;

          for (kk = ii + 1; kk < n; ++kk)
            value(p[jj], kk) += -value(p[ii], kk) * value(p[jj], ii);
        }
    }
  factorized = true;
}
