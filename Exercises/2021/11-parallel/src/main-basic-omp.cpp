#include <omp.h>
#include <iostream>
#include <vector>

int
main(int argc, char ** argv)
{
  int thread_num;
  int num_threads;
    
#pragma omp parallel shared(num_threads) private(thread_num)
  {
    /* it makes more sense here to let the first available thread assign num_threads
     * instead of necessarily make master thread do it, at the end num_threads is known
     * by all threads
     * this block will be anyway run by one thread only
    */
    #pragma omp single
    {
      num_threads = omp_get_num_threads();
    }
    
    thread_num = omp_get_thread_num();

    std::vector<int> a(10);
#pragma omp for schedule(dynamic)
    for (int i = 0; i < a.size(); ++i)
      {
	a[i] = i;
	#pragma omp critical
	{
	  std::cout << "iteration " << i << " done by thread " << thread_num << std::endl;
	}
      }
        
    /* the next is a block executed by all threads BUT once at a time
    #pragma omp critical
    {
      std::cout << "hello world from thread " << thread_num << " of " << num_threads << std::endl;
    }
    */
  }
  
  /*
   * if you want to run in parallel a for loop outside of a parallel block
   * you need to use the following sintax
   * #pragma omp parallel for
   */
  
  return 0;
}
