#include <mpi.h>
#include <vector>
#include <iostream>


int main(int argc, char ** argv)
{
  MPI_Init(&argc, &argv);

  int rank, size; // size = 4
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  size_t n_rows;
  size_t n_cols;
  size_t count;
  size_t remainder;
  
  std::vector<double> matrix;
  std::vector<int> send_count; //  used to send the matrix
  std::vector<int> send_offset; // used to send the matrix
  std::vector<int> recv_count; // used to receive the rhs once done the matrix-vector product
  std::vector<int> recv_offset; // used to receive the rhs once done the matrix-vector product
  
  // 1. rank 0 creates matrix or eventually reads it from a file
  if (rank == 0)
    {
      n_rows = 10;
      n_cols = 5;

      count = n_rows / size; // 2
      remainder = n_rows % size; // 2
      matrix.resize(n_rows * n_cols, 0.);
      // matrix[i * n_cols + j] = A[i][j];

      for (size_t i = 0; i < n_rows; ++i)
	for (size_t j = 0; j < n_cols; ++j)
	  matrix[i * n_cols + j] = i * n_cols + j;

      /*
      // print matrix
      std::cout << "matrix in rank " << rank << std::endl;
      for (size_t i = 0; i < n_rows; ++i)
	{
	  for (size_t j = 0; j < n_cols; ++j)
	    std::cout << matrix[i * n_cols + j] << " ";
	  std::cout << std::endl;
	}
      std::cout << std::endl;
      */
      
      // 2. Distribute "matrix" in parallel to other ranks
      send_count.resize(size, 0) ; // [3*n_cols, 3*n_cols, 2*n_cols, 2*n_cols]
      send_offset.resize(size, 0); // [0, 3*n_cols, 6*n_cols, 8*n_cols];
      recv_count.resize(size, 0) ; // [3, 3, 2, 2]
      recv_offset.resize(size, 0); // [0, 3, 6, 8];

      unsigned cum_sum = 0;      
      for (unsigned r = 0; r < size; ++r)
	{
	  recv_count[r] = (r < remainder) ? (count + 1) : count;
	  recv_offset[r] = cum_sum;
	  
      	  send_count[r] = recv_count[r] * n_cols;
	  send_offset[r] = cum_sum * n_cols;
	  
	  cum_sum += recv_count[r];
	}
    }

  MPI_Bcast(&n_rows, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);
  MPI_Bcast(&n_cols, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);
  MPI_Bcast(&count, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);
  MPI_Bcast(&remainder, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);
  
  const size_t n_local_rows = (rank < remainder) ? count + 1 : count;
  std::vector<double> local_matrix(n_local_rows * n_cols);
    
  MPI_Scatterv(matrix.data(), send_count.data(), send_offset.data(), MPI_DOUBLE,
	       local_matrix.data(), local_matrix.size(), MPI_DOUBLE, 0,
	       MPI_COMM_WORLD);
  
  // 3. Every processor computes the matrix-vector product
  std::vector<double> x(n_cols, 1.);
  std::vector<double> local_b(n_local_rows, 0.);
  for (unsigned i = 0; i < n_local_rows; ++i)
    for (unsigned j = 0; j < n_cols; ++j)
      local_b[i] = local_matrix[i * n_cols + j] * x[j];

  // very useful code to make different ranks execute a command
  // in order (to one you wish actually)
  for (unsigned r = 0; r < size; ++r)
    {
      if (r==rank)
	{
	  std::cout << "rank = " << rank << std::endl;
	  
	  std::cout << "local matrix" << std::endl;
	  for (unsigned i = 0; i < n_local_rows; ++i)
	    {
	      for (size_t j = 0; j < n_cols; ++j)
		std::cout << local_matrix[i * n_cols + j] << " ";
	      std::cout << std::endl;
	    }

	  
	  std::cout << "local rhs" << std::endl;
	  for (unsigned i = 0; i < n_local_rows; ++i)
	    std::cout << local_b[i] << " ";
	  std::cout << std::endl;

	  std::cout << std::endl;
	}
      else
	MPI_Barrier(MPI_COMM_WORLD);
    }

  // 4. Gather everything in rank 0
  std::vector<double> b;
  if (rank == 0)
    b.resize(n_rows, 0.);

  MPI_Gatherv(local_b.data(), local_b.size(), MPI_DOUBLE, b.data(), recv_count.data(),
	      recv_offset.data(), MPI_DOUBLE, 0, MPI_COMM_WORLD);

  if (rank == 0)
    {
      std::cout << "final rhs assembled at rank " << rank << std::endl;
      for(auto & elem : b)
	std::cout << elem << " ";
      std::cout << std::endl;
    }
  

  
  MPI_Finalize();
  return 0;
}
