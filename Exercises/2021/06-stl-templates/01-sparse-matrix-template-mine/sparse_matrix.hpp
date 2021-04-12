#ifndef SPARSE_MATRIX_HPP
#define SPARSE_MATRIX_HPP

#include <iostream>
#include <map>
#include <vector>

/// Class for sparse row-oriented matrix.
template<class T>
class sparse_matrix : public std::vector<std::map<unsigned int, T>>
{
private:
  using col_type = std::map<unsigned int, T>; // actually this is a row
  using row_type = std::vector<col_type>; // actually this is the matrix

  size_t nnz; /// Number of nonzero elements.
  size_t m;   /// Number of nonempty columns.
  

public:
  /// Default constructor.
  sparse_matrix();

  /// Recompute sparse matrix properties.
  void
  update_properties();

  /// Convert row-oriented sparse matrix to AIJ format.
  void
  aij(std::vector<T> &      a,
      std::vector<unsigned int> &i,
      std::vector<unsigned int> &j);

  /// Convert row-oriented sparse matrix to CSR format.
  void
  csr(std::vector<T> &      a,
      std::vector<unsigned int> &col_ind,
      std::vector<unsigned int> &row_ptr);

  /// Set all entries to 0 preserving storage structure.
  void
  reset();

  /// Output stream operator
  template<class U>
  friend
  std::ostream &
  operator<<(std::ostream & stream, sparse_matrix<U> & M);

  /// Matrix vector product
  template<class U>
  friend
  std::vector<U>
  operator*(const sparse_matrix<U> & M, const std::vector<U> & rhs);

};

template<class T>
sparse_matrix<T>::sparse_matrix() :
  nnz{0}, m{0} {}

template<class T>
void
sparse_matrix<T>::update_properties()
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

template<class T>
void
sparse_matrix<T>::aij(std::vector<T> & a, std::vector<unsigned int> &i,
		   std::vector<unsigned int> &j)
{
  this->update_properties();
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

template<class T>
void
sparse_matrix<T>::csr(std::vector<T> & a, std::vector<unsigned int> &col_ind,
		   std::vector<unsigned int> &row_ptr)
{
  this->update_properties();
  a.reserve(nnz);
  col_ind.reserve(nnz);
  row_ptr.reserve(this->size()+1);
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

template<class T>
void
sparse_matrix<T>::reset()
{
  this->clear();
  nnz = 0;
  m = 0;
}


template<class T>
std::ostream &
operator<<(std::ostream & stream, sparse_matrix<T> & M)
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


template<class T>
std::vector<T>
operator*(const sparse_matrix<T> & M, const std::vector<T> & rhs)
{
  std::vector<T> res{};
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
    


#endif /* SPARSE_MATRIX_HPP */
