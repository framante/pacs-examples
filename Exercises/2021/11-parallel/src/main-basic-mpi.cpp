#include <mpi.h>
#include <iostream>
#include <vector>


int
main(int argc, char ** argv)
{
  MPI_Init(&argc, &argv);
  int size, rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  unsigned int elements = 10;
  std::vector<double> x(elements, 0.);

  if (rank == 0)
    std::fill(x.begin(), x.end(), 5.);

  MPI_Bcast(x.data(), elements, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  
  for(unsigned i = 0; i < x.size(); ++i)
    std::cout << x[i];
  std::cout << std::endl;
  
  /*
  std::cout << "hello world from rank " << rank
	    << " out of " << size << std::endl;
  */
  MPI_Finalize();
  return 0;
}
