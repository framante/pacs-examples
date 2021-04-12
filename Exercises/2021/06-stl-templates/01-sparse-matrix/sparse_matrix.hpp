#ifndef SPARSE_MATRIX_HPP
#define SPARSE_MATRIX_HPP

#include <iostream>
#include <map>
#include <vector>

/// Class for sparse row-oriented matrix.
class sparse_matrix : public std::vector<std::map<unsigned int, double>>
{
private:
  using col_type = std::map<unsigned int, double>; // actually this is a row
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
  aij(std::vector<double> &      a,
      std::vector<unsigned int> &i,
      std::vector<unsigned int> &j);

  /// Convert row-oriented sparse matrix to CSR format.
  void
  csr(std::vector<double> &      a,
      std::vector<unsigned int> &col_ind,
      std::vector<unsigned int> &row_ptr);

  /// Set all entries to 0 preserving storage structure.
  void
  reset();

  /// Output stream operator
  friend
  std::ostream &
  operator<<(std::ostream & stream, sparse_matrix & M);

  /// Matrix vector product
  friend
  std::vector<double>
  operator*(const sparse_matrix & M, const std::vector<double> & rhs);

};



#endif /* SPARSE_MATRIX_HPP */
