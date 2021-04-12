#include "sparse_matrix.hpp"

sparse_matrix::sparse_matrix() :
  nnz{0}, m{0} {}

void
sparse_matrix::update_properties()
{
  nnz = 0;
  m = 0;
  // vector::size() returns the number of rows
  for(std::size_t r = 0; r < this->size(); ++r)
    {
      if(!this->operator[](r).empty())
	{
	  nnz += this->operator[](r).size();
	  std::size_t temp = this->operator[](r).rbegin()->first + 1;
	  m = std::max(temp, m);
	}
    }
}

void
sparse_matrix::aij(std::vector<double> & a, std::vector<unsigned int> &i,
		   std::vector<unsigned int> &j)
{
  update_properties();
  a.reserve(nnz);
  i.reserve(nnz);
  j.reserve(nnz);
  for(std::size_t r = 0; r < this->size(); ++r)
    {
      for(auto &[c, elem] : (*this)[r])
	{
	  a.push_back(elem);
	  i.push_back(r);
	  j.push_back(c);
	}
    }
}

void
sparse_matrix::csr(std::vector<double> & a, std::vector<unsigned int> &col_ind,
		   std::vector<unsigned int> &row_ptr)
{
  update_properties();
  a.reserve(nnz);
  col_ind.reserve(nnz);
  row_ptr.reserve(size()+1);
  row_ptr.push_back(0);
  for(std::size_t r = 0; r < this->size(); ++r)
    {
      row_ptr.push_back(this->operator[](r).size() + row_ptr.back());
      for(auto &[c, elem] : this->operator[](r))
	{
	  a.push_back(elem);
	  col_ind.push_back(c);
	}
    }
}


void
sparse_matrix::reset()
{
  this->clear();
  nnz = 0;
  m = 0;
}


std::ostream &
operator<<(std::ostream & stream, sparse_matrix & M)
{

  M.update_properties();
  stream << "Number of nonzero elements: " << M.nnz << std::endl;
  stream << "Number of nonempty colums: " << M.m << std::endl;

  for(std::size_t r = 0; r < M.size(); ++r)
    {
      for(auto &[c, elem] : M[r])
	stream << "A[" << r << "][" << c << "] = " << elem << std::endl;
    }
    return stream;
}


std::vector<double>
operator*(const sparse_matrix & M, const std::vector<double> & rhs)
{
  std::vector<double> res{};
  if (M.m != rhs.size())
    return res;
  else
    {
      res.resize(M.size(), 0.);
      for(std::size_t r = 0; r < M.size(); ++r)
	for(auto &[c, elem] : M[r])
	  res[r] += elem * rhs[c];
      return res;
    }
}
    

void
sparse_matrix::operator+=(const sparse_matrix & rhs)
{
  if (this->size() != rhs.size())
    return;
  else
    {
      for (std::size_t r = 0; r < this->size(); ++r)
	{
	  for(auto &[c, elem] : rhs[r])
	  {
	    if (this->operator[](r).find(c) != this->operator[](r).cend())
	      this->operator[](r)[c] += elem;
	    else
	      this->operator[](r)[c] = elem;
	  }
	}
    }
}
