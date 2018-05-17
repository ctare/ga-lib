#ifndef INCLUDE_GENE
#define INCLUDE_GENE

#include <iostream>
#include <vector>
#include <random>
#include <functional>

namespace genetic {
  struct statics {
    static std::random_device rnd;
    static std::mt19937 mt;
    static std::uniform_int_distribution<> rand100;
  };

  inline int rnd100() {
    return statics::rand100(statics::mt);
  }

  class Cell : std::vector<bool> {
    public:
      using std::vector<bool>::operator[];
      using std::vector<bool>::push_back;
      using std::vector<bool>::size;
      using std::vector<bool>::begin;
      using std::vector<bool>::end;
      void random() {
        for(int i = 0, i_end = this -> size(); i < i_end; i++) {
          (*this)[i] = rnd100() >= 50;
        }
      }
  };

  class SimpleCell: public Cell {
    public:
      SimpleCell();
  };

  class InitConfig;

  class Gene : std::vector<std::shared_ptr<Cell>> {
    public:
      Gene(InitConfig&);
      using std::vector<std::shared_ptr<Cell>>::operator[];
      using std::vector<std::shared_ptr<Cell>>::push_back;
      using std::vector<std::shared_ptr<Cell>>::size;
      using std::vector<std::shared_ptr<Cell>>::begin;
      using std::vector<std::shared_ptr<Cell>>::end;
  };

  using Fitness = std::function<int(Gene&)>;

  class InitConfig {
    private:
      std::vector<std::function<void(Gene&)>> cells_info;
      int pair_n;

    public:
      Fitness fitness_f;

      InitConfig(int pair_n, Fitness fitness_f);

      inline void set_pair_n(int pair_n) {
        this -> pair_n = pair_n;
      }

      inline std::size_t get_pair_n() {
        return this -> pair_n;
      }

      inline std::size_t cell_length() {
        return this -> cells_info.size();
      }

      template <typename T>
      inline void add(T& base_cell) {
        std::function<void(Gene&)> f = [&](Gene& gene) {
          auto base_cell_ptr = std::make_shared<T>();
          base_cell_ptr -> random();
          gene.push_back(base_cell_ptr);
        };
        cells_info.push_back(f);
      }

      inline std::function<void(Gene&)> get(int index) {
        return this -> cells_info[index];
      }

      void copy(Gene&);
  };

  class GeneticAlgorithm : std::vector<std::shared_ptr<Gene>> {
    private:
      vector<int> fitnesses;
      Fitness fitness;

      void copy_gene(int index0, int index1) {
      }

    public:
      GeneticAlgorithm(InitConfig&);
      using std::vector<std::shared_ptr<Gene>>::operator[];
      using std::vector<std::shared_ptr<Gene>>::push_back;
      using std::vector<std::shared_ptr<Gene>>::begin;
      using std::vector<std::shared_ptr<Gene>>::end;
      void calc_fitness();
      void save_elite();
      void debug();
  };
}

#endif
