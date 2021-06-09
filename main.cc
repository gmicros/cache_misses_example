#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>

// params
int const N = 1000;
int const range = 800000;
int min;

// storage
int cache_int[range] = {0};
bool cache_bool[range] = {false};
char cache_char[range/8] = {0x0};

inline bool InIntCache(int val) {
  if (val < min) {
    return false;
  }

  if (val - min > range) {
    return false;
  }
  return cache_int[val-min];
}

inline bool InBoolCache(int val) {
  if (val < min) {
    return false;
  }

  if (val - min > range) {
    return false;
  }
  return cache_bool[val-min];
}

inline bool InCharCache(int val) {
  if (val < min) {
    return false;
  }

  if (val - min > range) {
    return false;
  }

  int const byte_index = (val-min) / 8;
  int const byte_shift = (val-min) % 8;
  char temp = cache_char[byte_index];
  return temp >> byte_shift && 0x1;
}

int main() {
  using std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
  using std::chrono::duration;
  using std::chrono::milliseconds;

  min = std::rand() % 10000;
  // generate random input
  std::vector<int> input;
  for (int i = 0; i < N; i++) {
    input.push_back(std::rand() %range + min);
  }

  // store input into caches
  for (auto &x : input) {
    cache_bool[x-min] = true;
    cache_int[x-min] = 1;
    int byte_index = (x-min) / 8;
    int byte_shift = (x-min) % 8;
    char val = cache_char[byte_index];
    val = val || 0x1 << byte_shift;
    cache_char[byte_index] = val;
  }

  int iter = 1000000;
  // generate test values
  std::vector<int> test_values;
  for (int i = 0; i < iter; i++) {
    test_values.push_back(std::rand() % range + min);
  }

  // test int cache
  auto t1 = high_resolution_clock::now();
  for (auto &val : test_values) {
    InIntCache(val);
  }
  auto t2 = high_resolution_clock::now();
  duration<double, std::milli> ms_double = t2 - t1;
  std::cout << "Int cache: " << ms_double.count() << std::endl;

  // test bool cache
  t1 = high_resolution_clock::now();
  for (auto &val : test_values) {
    InBoolCache(val);
  }
  t2 = high_resolution_clock::now();
  ms_double = t2 - t1; 
  std::cout << "Bool cache: " << ms_double.count() << std::endl;
  
  // test char cache
  t1 = high_resolution_clock::now();
  for (auto &val : test_values) {
    InCharCache(val);
  }
  t2 = high_resolution_clock::now();
  ms_double = t2 - t1;
  std::cout << "Char cache: " << ms_double.count() << std::endl;
}
