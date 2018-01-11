#include <iostream>
#include "vector.hpp"

using std::cout;
using std::endl;

int main()
{
  mqs::Vector<int> nums = {0, 1, 2, 3, 4};
  mqs::Vector<int> to_grow(nums);
  for(int i = 0; i < 5; i++) {
    cout << nums[i] << endl;
  }
  for(int i = 5; i < 10; i++) {
    to_grow.push_back(i);
  }
  cout << to_grow.size() << endl;
  cout << to_grow.capacity() << endl; 
  for(size_t i = 0; i < to_grow.size(); i++) {
    cout << to_grow.at(i) << endl;
  }
}
