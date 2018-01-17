#include "red_black_tree.hpp"
#include "vector.hpp"
#include <climits>
#include <gtest/gtest.h>

TEST(VectorConstructorTest, VectorConstuctorDefault) {
  mqs::Vector<int> nil;
  ASSERT_EQ(true, nil.empty());
}

TEST(VectorConstructorTest, VectorConstuctorSize) {
  mqs::Vector<int> v(100);
  ASSERT_EQ(100, v.size());
  ASSERT_EQ(200, v.capacity());
}

TEST(VectorConstructorTest, VectorConstuctorFill) {
  mqs::Vector<int> v(100, 9);
  ASSERT_EQ(100, v.size());
  for(size_t i = 0; i < v.size(); i++) {
    ASSERT_EQ(9, v.at(i));
  }
}

TEST(VectorConstructorTest, VectorConstructorCopy) {
  mqs::Vector<int> a(100, 1);
  mqs::Vector<int> b(a);
  for(size_t i = 0; i < b.size(); i++) {
    ASSERT_EQ(a.at(i), b.at(i));
  }
}

TEST(VectorConstructorTest, VectorConstuctorIL) {
  mqs::Vector<int> nums = {1, 5, 3, 2, 6};
  int test[5] = {1, 5, 3, 2, 6};
  for(size_t i = 0; i < nums.size(); i++) {
    ASSERT_EQ(test[i], nums.at(i));
  }
}


TEST(VectorGrowTest, VectorGrowShrink) {
  mqs::Vector<int> nums = {0, 1};
  mqs::Vector<int> to_grow(nums);
  for(int i = 2; i < 4; i++) {
    to_grow.push_back(i);
  }
  ASSERT_EQ(4, to_grow.size());
  ASSERT_EQ(8, to_grow.capacity());
  for(size_t i = 0; i < to_grow.size(); i++) {
    ASSERT_EQ(i, to_grow.at(i));
  }
  for(int i = 0; i < 2; i++) {
    to_grow.pop();
  }
  ASSERT_EQ(2, to_grow.size());
  ASSERT_EQ(4, to_grow.capacity());
  for(size_t i = 0; i < to_grow.size(); i++) {
    ASSERT_EQ(i, to_grow.at(i));
  }

  mqs::Vector<int> nums2 = {0, 1, 2, 3, 4};
  mqs::Vector<int> to_grow2(nums2);
  for(int i = 5; i < 10; i++) {
    to_grow2.push_back(i);
  }
  ASSERT_EQ(10, to_grow2.size());
  ASSERT_EQ(20, to_grow2.capacity());
  for(size_t i = 0; i < to_grow2.size(); i++) {
    ASSERT_EQ(i, to_grow2.at(i));
  }
  for(int i = 0; i < 5; i++) {
    to_grow2.pop();
  }
  ASSERT_EQ(5, to_grow2.size());
  ASSERT_EQ(10, to_grow2.capacity());
  for(size_t i = 0; i < to_grow2.size(); i++) {
    ASSERT_EQ(i, to_grow2.at(i));
  }
}

TEST(VectorInsertTest, VectorInsert)
{
  mqs::Vector<int> verify = {0, 1, 2, 3, 4, 5};
  mqs::Vector<int> test = {0, 2, 3, 4, 5};
  test.insert(1, 1);
  ASSERT_EQ(verify.size(), test.size());
  for(size_t i = 0; i < verify.size(); i++) {
    ASSERT_EQ(verify.at(i), i);
    ASSERT_EQ(verify.at(i), test.at(i));
  }
}

TEST(VectorRemoveTest, VectorRemove)
{
  mqs::Vector<int> test = {0, 1, 2, 2, 3, 4, 5};
  test.remove((size_t)2);
  ASSERT_EQ(6, test.size());
  for(size_t i = 0; i < test.size(); i++) {
    ASSERT_EQ(i, test[i]);
  }

  for(int i = 0; i < 5; i++) {
    test.insert(1, 2);
  }
  ASSERT_EQ(6, test.remove((int)2));
  test.insert(2, 2);
  for(size_t i = 0; i < test.size(); i++) {
    ASSERT_EQ(i, test[i]);
  }
}

TEST(VectorStressTest, VectorStressFind)
{
  mqs::Vector<int> test;
  for(size_t i = 0; i < 5000; i++) {
    test.push_back(i);
  }
  for(size_t i = 0; i < 5000; i++) {
    ASSERT_EQ(i, test.find(i));
  }
}


TEST(RBTInsertTest, RBTInsertFind) {
  std::vector<int> nums = {5, 4, 1, 3, 2, 6, 7, 8};
  mqs::red_black_tree<int> tree(nums);
  for(int n : nums) {
    ASSERT_NE(false, tree.find(n));
  }
  int x = 9999;
  ASSERT_EQ(false, tree.find(x));
}

TEST(RBTInsertTest, RBTInsertCorrectness) {
  std::vector<int> nums = {5, 4, 1, 3, 2, 6, 7, 8};
  mqs::red_black_tree<int> tree;
  std::vector<std::pair<int, bool>> verify0 = { {5, true} };
  std::vector<std::pair<int, bool>> verify1 = { {5, true}, {4, false} };
  std::vector<std::pair<int, bool>> verify2 = { {4, true}, {1, false}, {5, false} };
  std::vector<std::pair<int, bool>> verify3 = { {4, true}, {1, true}, {3, false}, {5, true} };
  std::vector<std::pair<int, bool>> verify4 = { {4, true}, {2, true}, {1, false}, {3, false}, {5, true} };
  std::vector<std::pair<int, bool>> verify5 = { {4, true}, {2, true}, {1, false}, {3, false}, {5, true}, {6, false} };
  std::vector<std::pair<int, bool>> verify6 = { {4, true}, {2, true}, {1, false}, {3, false}, {6, true}, {5, false}, {7, false} };
  std::vector<std::pair<int, bool>> verify7 = { {4, true}, {2, true}, {1, false}, {3, false}, {6, false}, {5, true}, {7, true}, {8, false} };
  std::vector<std::pair<int, bool>> dump;
  tree.insert(nums[0]);
  ASSERT_EQ(tree.height(), 0);
  dump = tree.dump();
  ASSERT_EQ(verify0.size(), dump.size());
  ASSERT_EQ(verify0[0], dump[0]);

  tree.insert(nums[1]);
  ASSERT_EQ(tree.height(), 1);
  dump = tree.dump();
  ASSERT_EQ(verify1.size(), dump.size());
  for(int i = 0; i < 2; i++) {
    ASSERT_EQ(verify1[i], dump[i]);
  }

  tree.insert(nums[2]);
  ASSERT_EQ(tree.height(), 1);
  dump = tree.dump();
  ASSERT_EQ(verify2.size(), dump.size());
  for(int i = 0; i < 3; i++) {
    ASSERT_EQ(verify2[i], dump[i]);
  }

  tree.insert(nums[3]);
  ASSERT_EQ(tree.height(), 2);
  dump = tree.dump();
  ASSERT_EQ(verify3.size(), dump.size());
  for(int i = 0; i < 4; i++) {
    ASSERT_EQ(verify3[i], dump[i]);
  }

  tree.insert(nums[4]);
  ASSERT_EQ(tree.height(), 2);
  dump = tree.dump();
  ASSERT_EQ(verify4.size(), dump.size());
  for(int i = 0; i < 5; i++) {
    ASSERT_EQ(verify4[i], dump[i]);
  }

  tree.insert(nums[5]);
  ASSERT_EQ(tree.height(), 2);
  dump = tree.dump();
  ASSERT_EQ(verify5.size(), dump.size());
  for(int i = 0; i < 6; i++) {
    ASSERT_EQ(verify5[i], dump[i]);
  }

  tree.insert(nums[6]);
  ASSERT_EQ(tree.height(), 2);
  dump = tree.dump();
  ASSERT_EQ(verify6.size(), dump.size());
  for(int i = 0; i < 7; i++) {
    ASSERT_EQ(verify6[i], dump[i]);
  }

  tree.insert(nums[7]);
  ASSERT_EQ(tree.height(), 3);
  dump = tree.dump();
  ASSERT_EQ(verify7.size(), dump.size());
  for(int i = 0; i < 8; i++) {
    ASSERT_EQ(verify7[i], dump[i]);
  }
}

TEST(RBTRemoveTest, RBTRemoveSuccess) {
  std::vector<int> nums = {5, 4, 1, 3, 2, 6, 7, 8};
  mqs::red_black_tree<int> tree(nums);
  for(unsigned int i = 0; i < nums.size(); i++) {
    ASSERT_EQ(true, tree.remove(nums[i]));
    ASSERT_EQ(false, tree.find(nums[i]));
    ASSERT_EQ(nums.size()-i-1, tree.size());
    for(unsigned int j = i+1; j < nums.size(); j++) {
      ASSERT_EQ(true, tree.find(nums[j]));
    }
  }
}

TEST(RBTRandomTest, RBTRandomSuccess) {
  srand(time(NULL));
  mqs::red_black_tree<int> tree;
  std::vector<int> nums;
  for(size_t i = 0; i < 10000; i++) {
    int x = rand() % UINT_MAX;
    if(tree.insert(x)) {
      nums.push_back(x);
    }
  }
  for(int x : nums) {
    ASSERT_EQ(true, tree.find(x));
  }
  for(int x : nums) {
    tree.remove(x);
    ASSERT_EQ(false, tree.find(x));
  }
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
