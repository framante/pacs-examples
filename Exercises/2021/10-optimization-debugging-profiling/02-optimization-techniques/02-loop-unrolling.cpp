#include <chrono>
#include <iostream>
#include <vector>

const size_t vec_size = 1000;

int
multiply(const std::vector<int> &data)
{
  int result = 1;

  for (size_t i = 0; i < data.size()- 2; ++i)
      result *= data[i];

  return result;
}
// no loop unrolling, indeed all the loop is done

int
multiply2(const std::vector<int> &data)
{
  int result = 1;

  int data0, data1, data2;

  for (size_t i = 0; i < data.size() - 2; i += 3)
    {
      data0 = data[i];
      data1 = data[i+1];
      data2 = data[i+2];
      result *= data0 * data1 * data2;
    }

  return result;
}
// here with unroll the loop, indeed you proceed 3
// elements at each time

using timer = std::chrono::high_resolution_clock;

int
main(int argc, char **argv)
{
  size_t n_iterations = 1e7;

  // Generate an arbitrary dataset.
  auto data = std::vector<int>(vec_size, 1);

  auto   start = timer::now();
  double result;
  for (size_t i = 0; i < n_iterations; ++i)
    result = multiply2(data);
  auto end = timer::now();

  std::cout << "Multiply: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                     start)
                 .count()
            << " ms."
            << ", result: " << result << std::endl;

  return 0;
}
