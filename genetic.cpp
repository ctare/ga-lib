#include "genetic.hpp"

namespace genetic {
  std::random_device statics::rnd;
  std::mt19937 statics::mt(statics::rnd());
  std::uniform_int_distribution<> statics::rand100(0, 99);
  std::uniform_real_distribution<> statics::rand01(0.0, 1.0);

  Config::Config(std::size_t pair_n) : pair_n(pair_n) {
  }

  void Gene::random() {
    for(int i = 0, i_end = this -> size(); i < i_end; i++) {
      (*this)[i] = rnd100() < 50;
    }
  }

  GeneticAlgorithm::GeneticAlgorithm(Config& config) : fitnesses(config.pair_n * 2 + 1), config(config) {
    for(std::size_t i = 0, i_end = this -> config.pair_n * 2 + 1; i < i_end; i++) {
      Gene gene(config.get_gene_size());
      gene.random();
      this -> genes.push_back(std::make_shared<Gene>(gene));

      Gene emp_gene(config.get_gene_size());
      this -> next_generation.push_back(std::make_shared<Gene>(emp_gene));

    }
    size_of_gene = sizeof(*(this -> genes[0]));
  }

  void GeneticAlgorithm::debug() {
    this -> calc_fitness();

    for(std::size_t i = 0, i_end = this -> genes.size(); i < i_end; i++) {
      for(auto cell: *(this -> genes[i])) {
        std::cout << cell << " ";
      }
      std::cout << " : " << this -> fitnesses[i] << std::endl;
    }
  }

  void GeneticAlgorithm::calc_fitness() {
    for(int i = 0, i_end = this -> genes.size(); i < i_end; i++) {
      int result = 0;
      bool *itr = std::begin(*(this -> genes[i]));
      for(CellCalculator* calc : this -> config.calculators) {
        bool *end = itr + calc -> get_cell_size();
        result += calc -> fitness_function(itr, end);
        itr = end;
      }
      this -> fitnesses[i] = result;
    }
  }

  void GeneticAlgorithm::copy_gene(int data_index, int target_index) {
    // memcpy(this -> genes[target_index].get(), this -> genes[data_index].get(), this -> size_of_gene);
    *(this -> genes[target_index]) = *(this -> genes[data_index]);
    this -> fitnesses[target_index] = this -> fitnesses[data_index];
  }

  void GeneticAlgorithm::swap_gene(int index1, int index2) {
    this -> genes[index1] -> swap(*(this -> genes[index2]));
    std::swap(this -> fitnesses[index1], this -> fitnesses[index2]);
  }

  void GeneticAlgorithm::save_elite() {
    int max_index = std::distance(this -> fitnesses.begin(), std::max_element(this -> fitnesses.begin(), this -> fitnesses.end()));
    int min_index = std::distance(this -> fitnesses.begin(), std::min_element(this -> fitnesses.begin(), this -> fitnesses.end()));

    copy_gene(max_index, min_index);
    swap_gene(max_index, 0);
  }

  void GeneticAlgorithm::reproduction() {
    int sum = accumulate(this -> fitnesses.begin(), this -> fitnesses.end(), 0);

    int num;
    int border;
    double r;

    *(this -> next_generation[0]) = *(this -> genes[0]);

    for(int i = 1, i_end = this -> genes.size(); i < i_end; i++) {
      r = sum * (rnd100() / 100.0);
      num = 0;
      border = fitnesses[0];

      while (border < r) {
        border += fitnesses[++num];
      }
      *(this -> next_generation[i]) = *(this -> genes[num]);
    }

    genes.swap(next_generation);
  }

  void GeneticAlgorithm::crossover() {
    int cross_position = (int) ((this -> genes.size() - 2) * (rnd100() / 100.0)) + 1;
    for(int i = 1, i_end = this -> genes.size() - 1; i < i_end; i += 2) {
      for(int j = cross_position, j_end = this -> config.get_gene_size(); j < j_end; j++) {
        std::swap((*(this -> genes[i]))[j], (*(this -> genes[i + 1]))[j]);
      }
    }
  }

  void GeneticAlgorithm::mutation() {
    for(std::vector<std::shared_ptr<Gene>>::iterator ptr = genes.begin() + 1, end = genes.end(); ptr != end; ptr++) {
      for(int i = 0, i_end = this -> config.get_gene_size(); i < i_end; i++) {
        if(rnd01() < this -> mutation_rate) {
          (**ptr)[i] = !(**ptr)[i];
        }
      }
    }
  }

  void GeneticAlgorithm::train(int epoch) {
    for(int i = 0; i < epoch; i++) {
      this -> calc_fitness();
      this -> save_elite();
      this -> reproduction();
      this -> crossover();
      this -> mutation();
    }
    this -> calc_fitness();
  }

  void GeneticAlgorithm::eval() {
    bool *itr = std::begin(*(this -> genes[0]));
    for(CellCalculator* calc : this -> config.calculators) {
      bool *end = itr + calc -> get_cell_size();
      calc -> eval(itr, end);
      itr = end;
    }
  }
}
