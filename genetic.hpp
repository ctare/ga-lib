#ifndef INCLUDE_GENETIC
#define INCLUDE_GENETIC

#include <string.h>
#include <iostream>
#include <vector>
#include <valarray>
#include <random>
#include <functional>

namespace genetic {
  struct statics {
    static std::random_device rnd;
    static std::mt19937 mt;
    static std::uniform_int_distribution<> rand100;
    static std::uniform_real_distribution<> rand01;
  };

  inline int rnd100() {
    return statics::rand100(statics::mt);
  }

  inline double rnd01() {
    return statics::rand01(statics::mt);
  }

  class Gene : public std::valarray<bool> {
    public:
      using valarray<bool>::valarray;

      void random();
  };

  class CellCalculator {
    public:
      virtual std::size_t get_cell_size() = 0;
      virtual int fitness_function(bool *begin, bool *end) = 0;
      virtual void eval(bool *begin, bool *end) = 0;
  };

  class Config {
    private:
      std::size_t gene_size = 0;

    public:
      std::vector<CellCalculator*> calculators;
      const std::size_t pair_n;

      Config(std::size_t pair_n);

      inline void add_cell_calculator(CellCalculator* calculator) {
        this -> calculators.push_back(calculator);
        gene_size += calculator -> get_cell_size();
      }

      inline std::size_t get_gene_size() {
        return this -> gene_size;
      }
  };

  class GeneticAlgorithm {
    private:
      std::vector<std::shared_ptr<Gene>> genes;
      std::vector<std::shared_ptr<Gene>> next_generation;
      std::size_t size_of_gene;
      std::vector<int> fitnesses;
      double mutation_rate = 0.1;
      Config& config;

      // inline Gene& gene(int index) {
      //   return *(*this)[index];
      // }

      void copy_gene(int data_index, int target_index);
      void swap_gene(int index1, int index2);

    public:
      GeneticAlgorithm(Config&);
      void calc_fitness();
      void save_elite();
      void reproduction();
      void crossover();
      void mutation();
      void train(int epoch);
      void eval();
      void debug();
  };
}

#endif
