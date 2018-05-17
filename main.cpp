#include "Gene.hpp"

#include <iostream>

#include <functional>
#include <vector>
#include <random>

#define FOR(i, f, n) for(int (i) = (f); (i) < (n); (i)++)
#define RFOR(i, f, n) for(int (i) = (f); (i) >= (n); (i)--)
#define REP(i, n) for(int (i) = 0; (i) < (n); (i)++)
#define RREP(i, n) for(int (i) = (n); (i) >= 0; (i)--)
#define rep(i, n) for(int (i) = 0; (i) < (n); (i)++)
#define rrep(i, n) for(int (i) = (n); (i) >= 0; (i)--)
#define forin(i, a) for(auto i: (a))
#define p(...) printf(__VA_ARGS__); cout << endl;
#define in(...) __VA_ARGS__; input__(__VA_ARGS__);
#define input(...) in(__VA_ARGS__)
#define out(...) output__(__VA_ARGS__);
#define output(...) out(__VA_ARGS__)
#define list_input(x, n) x[(n)]; for(int (i) = 0; (i) < (n); (i)++) cin >> x[i];
#define lambda(a, b, c) [](auto const &a, auto const &b){ return c ;}
#define key(t, x, compare) [](const t &a, const t &b){ return a x compare b x ;}
#define unless(a) if(!(a))

using namespace std;

using pii = pair<int, int>;
using Gene_ = vector<vector<bool>>;

template <class... T>
void input__(T &...a) {
  for(auto v: {&a...}) {
    cin >> *v;
  }
}

void output__(){
  cout << endl;
}

template <class Head, class... T>
void output__(Head const &v, T const &...a) {
  if(sizeof...(a) == 0) {
    cout << v;
  } else {
    cout << v << " ";
  }
  output__(a...);
}

random_device rnd;
mt19937 mt(rnd());
uniform_int_distribution<> rand100(0, 99);

void init_gene(Gene_ &gene) {
  rep(i, gene.size()) {
    rep(j, gene[i].size()) {
      gene[i][j] = (rand100(mt) >= 50);
    }
  }
}

vector<int> calc_fitness(Gene_ &gene, function<int(vector<bool>&)> fitness_function) {
  vector<int> fitness(gene.size(), 0);
  rep(i, gene.size()) {
    fitness[i] = fitness_function(gene[i]);
  }
  return fitness;
}

void debug(vector<int> &fitness) {
  float avg_fit = accumulate(fitness.begin(), fitness.end(), 0) / (float)fitness.size();
  int max_fit = *max_element(fitness.begin(), fitness.end());

  out("avg:", avg_fit);
  out("max:", max_fit);
  out("--------------")
}

void swap_gene(int index1, int index2, Gene_ &gene, vector<int> &fitness) {
  vector<bool> tmp1 = gene[index1];
  gene[index1] = gene[index2];
  gene[index2] = tmp1;

  int tmp2 = fitness[index1];
  fitness[index1] = fitness[index2];
  fitness[index2] = tmp2;
}

void copy_gene(int index1, int index2, Gene_ &gene, vector<int> &fitness) {
  gene[index2] = vector<bool>(gene[index1].begin(), gene[index1].end());
  fitness[index2] = fitness[index1];
}

void elite(Gene_ &gene, vector<int> &fitness) {
  int max_index = distance(fitness.begin(), max_element(fitness.begin(), fitness.end()));
  int min_index = distance(fitness.begin(), min_element(fitness.begin(), fitness.end()));
  copy_gene(max_index, min_index, gene, fitness);
  swap_gene(0, max_index, gene, fitness);
}

void reproduction(Gene_ &gene, vector<int> &fitness) {
  int sum = accumulate(fitness.begin(), fitness.end(), 0);
  Gene_ gene_holder(gene.size(), vector<bool>(gene[0].size(), 0));

  rep(i, gene.size()) {
    double r = sum * (rand100(mt) / 100.0);

    int num = 0;
    int border = fitness[0];
    while(border < r) {
      num++;
      border += fitness[num];
    }

    gene_holder[i] = vector<bool>(gene[num].begin(), gene[num].end());
  }

  FOR(i, 1, gene.size()) {
    gene[i] = gene_holder[i];
  }
}

void crossover(Gene_ &gene) {
  int c_pos = (int) ((gene.size() - 2) * (rand100(mt) / 100.0)) + 1;
  for(int i = 1, tmp = gene.size() - 1; i < tmp; i += 2) {
    FOR(j, c_pos, gene.size()) {
      int value_tmp = gene[i][j];
      gene[i][j] = gene[i + 1][j];
      gene[i + 1][j] = value_tmp;
    }
  }
}

void mutation(Gene_ &gene, double rate) {
  for(auto itr = gene.begin() + 1; itr != gene.end(); itr++) {
    rep(i, (*itr).size()) {
      if(rand100(mt) < rate) {
        (*itr)[i] = ((*itr)[i] + 1) % 2;
      }
    }
  }
}


int fitness_f(vector<bool> &gene_row) {
  int c1 = 0;
  rep(i, gene_row.size() / 2) {
    if(gene_row[i]) c1++;
  }

  int c2 = 0;
  FOR(i, gene_row.size() / 2, gene_row.size()) {
    if(!gene_row[i]) c2++;
  }
  return c1 + c2;
}

int all_1(vector<bool> &gene_row) {
  return accumulate(gene_row.begin(), gene_row.end(), 0);
}

int dummy(genetic::Gene& gene) {
  int result = 0;
  rep(i, gene.size()) {
    forin(c, *(gene[i])) {
      if(i < gene.size() / 2) {
        result += c ? 0 : 1;
      } else {
        result += c ? 1 : 0;
      }
    }
  }
  return result;
}


int main() {
  genetic::InitConfig config(5, dummy);

  genetic::SimpleCell simple_cell;

  rep(i, 10) {
    config.add(simple_cell);
  }

  genetic::GeneticAlgorithm algorithm(config);
  algorithm.calc_fitness();
  algorithm.debug();
  exit(0);

  out(genetic::rnd100());
  int pop_size = 51;
  int gene_length = 7 * 2;
  int try_n = 30;
  auto f = all_1;
  Gene_ gene(pop_size, vector<bool>(gene_length, 0));
  init_gene(gene);

  rep(i, try_n) {
    auto fitness = calc_fitness(gene, f);
    rep(i, gene.size()) {
      rep(j, gene[i].size()) {
        cout << gene[i][j] << " ";
      }
      out();
    }
    debug(fitness);
    elite(gene, fitness);
    reproduction(gene, fitness);
    crossover(gene);
    mutation(gene, 0.1);
  }

  out("==============");
  out("final:");
  rep(i, gene.size()) {
    rep(j, gene[i].size()) {
      cout << gene[i][j] << " ";
    }
    out();
  }
  auto tmp = calc_fitness(gene, f);
  debug(tmp);
}
