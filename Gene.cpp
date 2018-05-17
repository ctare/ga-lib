#include <iostream>
#include "Gene.hpp"

namespace genetic {
  std::random_device statics::rnd;
  std::mt19937 statics::mt(statics::rnd());
  std::uniform_int_distribution<> statics::rand100(0, 99);

  SimpleCell::SimpleCell() {
    this -> push_back(0);
  }

  InitConfig::InitConfig(int pair_n, Fitness fitness_f) {
    this -> set_pair_n(pair_n);
    this -> fitness_f = fitness_f;
  }

  void InitConfig::copy(Gene& holder) {
    for(int i = 0, i_end = this -> cell_length(); i < i_end; i++) {
      this -> get(i)(holder);
    }
  }

  Gene::Gene(InitConfig& config) {
    config.copy(*this);
  }

  GeneticAlgorithm::GeneticAlgorithm(InitConfig& config) {
    this -> fitness = config.fitness_f;
    for(int i = 0, i_end = config.get_pair_n() * 2 + 1; i < i_end; i++) {
      Gene gene(config);
      this -> push_back(std::make_shared<Gene>(gene));
      this -> fitnesses.push_back(0);
    }
    elite = (*this)[0];
  }

  void GeneticAlgorithm::debug() {
    for(auto gene: *this) {
      for(auto cell: *gene) {
        for(auto v: *cell) {
          std::cout << v << " ";
        }
      }
      std::cout << " : " << this -> fitness(*gene) << std::endl;
    }
  }

  void GeneticAlgorithm::calc_fitness() {
    for(int i = 0, i_end = this -> size(); i < i_end; i++) {
      this -> fitnesses[i] = this -> fitness(*(*this)[i]);
    }
  }

  void GeneticAlgorithm::save_elite() {
    int max_index = distance(this -> fitnesses.begin(), max_element(this -> fitnesses.begin(), this -> fitnesses.end()));
    int min_index = distance(this -> fitnesses.begin(), min_element(this -> fitnesses.begin(), this -> fitnesses.end()));

    copy_gene(max_index, min_index, gene, fitness);
    swap_gene(0, max_index, gene, fitness);
  }
}
