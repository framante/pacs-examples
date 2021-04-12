#include "sparse_matrix.hpp"

#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

int
main(int argc, char **argv)
{
  const unsigned int n = 10;

  std::vector<double> x(n, 1.0);

  sparse_matrix A;
  A.resize(n);

  for (unsigned int ii = 0; ii < n; ++ii)
    {
      if (ii > 0)
        A[ii][ii - 1] = -1;

      if (ii < n - 1)
        A[ii][ii + 1] = -1;

      A[ii][ii] = 4;
    }

  std::cout << "Loop through matrix entries:" << std::endl;
  for (unsigned int ii = 0; ii < n; ++ii)
    for (const auto &[jj, val] : A[ii])
      std::cout << "A[" << ii << "][" << jj << "] = " << val << std::endl;
  std::cout << std::endl << std::endl;

  std::cout << "Stream operator:" << std::endl;
  std::cout << A;
  std::cout << std::endl;
  
  std::vector<unsigned int> irow, jcol;
  std::vector<double>       v;

  A.csr(v, jcol, irow);

  std::cout << std::endl;
  std::cout << "CSR vectors:" << std::endl;
  for (auto ii : irow)
    std::cout << ii << " ";
  std::cout << std::endl;

  for (auto ii : jcol)
    std::cout << ii << " ";
  std::cout << std::endl;

  for (auto ii : v)
    std::cout << ii << " ";
  std::cout << std::endl << std::endl;

  std::cout << "Matrix entries from CSR:" << std::endl;
  for (unsigned int ii = 0; ii < n; ++ii)
    for (unsigned int jj = irow[ii]; jj < irow[ii + 1]; ++jj)
      std::cout << "A[" << ii << "][" << jcol[jj] << "] = " << v[jj]
                << std::endl;

  
  std::vector<unsigned int> iroww, jcolw;
  std::vector<double>       w;

  A.aij(w, jcolw, iroww);
  std::cout << std::endl;
  std::cout << "aij vectors:" << std::endl;
  for (auto ii : iroww)
    std::cout << ii << " ";
  std::cout << std::endl;

  for (auto ii : jcolw)
    std::cout << ii << " ";
  std::cout << std::endl;

  for (auto ii : w)
    std::cout << ii << " ";
  std::cout << std::endl << std::endl;

  std::cout << "Matrix entries from aij:" << std::endl;
  for (unsigned int i = 0; i < w.size(); ++i)
    std::cout << "A[" << iroww[i] << "][" << jcolw[i] << "] = " << w[i]
                << std::endl;

  
}
