#include "calcs.hpp"

namespace calculators {
  DummyCalculator::DummyCalculator(std::size_t size) : size(size) {}

  int DummyCalculator::fitness_function(bool *begin, bool *end) {
    int result = 0;
    for(bool *loop_end = begin + std::distance(begin, end) / 2; begin != loop_end; begin++) {
      try {
        result += *begin == 0 ? 0 : 1;
      } catch(...) {
        std::cout << "err: " << *begin << std::endl;
      }
    }
    for(; begin != end; begin++) {
      try {
        result += *begin == 1 ? 0 : 1;
      } catch(...) {
        std::cout << "err: " << *begin << std::endl;
      }
    }
    return result;
  }

  void DummyCalculator::eval(bool *begin, bool *end) {
    std::cout << fitness_function(begin, end) << std::endl;
  }
}
