#ifndef INCLUDE_CALCS
#define INCLUDE_CALCS

#include "genetic.hpp"

namespace calculators {
  class DummyCalculator : public genetic::CellCalculator {
    private:
      const std::size_t size;

    public:
      DummyCalculator(std::size_t size);

      inline std::size_t get_cell_size() override {
        return this -> size;
      }

      int fitness_function(bool *begin, bool *end) override;
      void eval(bool *begin, bool *end) override;
  };
}

#endif
