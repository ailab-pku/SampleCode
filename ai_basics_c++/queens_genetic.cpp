#include <ctime>
#include <random>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "interface/population.hpp"

class QueensPopulation : public PopulationBase<std::vector<int> >{
private:

    typedef std::vector<int> ChromosomeType;
    
    std::vector<ChromosomeType> _population;
    std::vector<double> _adaptability;
    double mutation_rate;
    int best_index, second_index;

    mutable std::default_random_engine random_engine;
    mutable std::uniform_real_distribution<double> uniform_dist;
    mutable std::uniform_int_distribution<int> uniform_int_dist;

    double evaluate_chromosome(const ChromosomeType& c) const {
        int n_queens = c.size();
        std::vector<int> lr(n_queens << 1), rl(n_queens << 1);
        std::vector<int> row(n_queens), column(n_queens);
        double value = 0;
        for (int i = 0, xi, yi; i < n_queens; ++ i){
            xi = i;
            yi = c[i];
            ++ row[yi];
            ++ column[xi];
            ++ rl[xi+yi];
            ++ lr[xi-yi+n_queens-1];   
        }

        for (int i = 0, xi, yi; i < n_queens; ++ i){
            xi = i;
            yi = c[i];
            for (int j = i+1, xj, yj; j < n_queens; ++ j){
                xj = j;
                yj = c[j];
                value += int(yi == yj or
                    xi+yi == xj+yj or
                    xi-yi == xj-yj
                );                
            }
        }
        return ((n_queens * (n_queens - 1)) >> 1) - value;
    }
    
    void update_adaptability(){
        for (int i = 0; i < _population.size(); ++ i){
            _adaptability[i] = evaluate_chromosome(_population[i]);
        }
    }

    const ChromosomeType& select_chromosome_with_adaptability() const {
        
        int coin = uniform_int_dist(random_engine) & 1;
        return coin == 0 ? _population[second_index] : _population[best_index];
    }

    ChromosomeType cross(const ChromosomeType& c1, const ChromosomeType& c2) const {
        int index = uniform_int_dist(random_engine) % c1.size();
        ChromosomeType result(c1.begin(), c1.begin() + index);
        result.insert(result.end(), c2.begin() + index, c2.end());
        return result;
    }

    void mutate(ChromosomeType& c) const {
        int index1 = uniform_int_dist(random_engine) % c.size();
        int index2 = uniform_int_dist(random_engine) % (c.size() - 1);
        if (index2 >= index1){
            ++ index2;
        }
        std::swap(c[index1], c[index2]);

        int index = uniform_int_dist(random_engine) % c.size();
        c[index] = uniform_int_dist(random_engine) % c.size();
    }

    void update_best_and_second(){
        for (int i = 0; i < _adaptability.size(); ++ i){
            if (_adaptability[i] > _adaptability[best_index]){
                second_index = best_index;
                best_index = i;
            } else if (_adaptability[i] > _adaptability[second_index]){
                second_index = i;
            }
        }
    }

public:

    std::vector<ChromosomeType> population() const {return _population;}
    std::vector<double> adaptability() const {return _adaptability;}
    
    void cross() override {
        update_best_and_second();
        std::vector<ChromosomeType> new_population(_population.size());
        for (int i = 0; i < _population.size(); ++ i){
            ChromosomeType c1 = select_chromosome_with_adaptability();
            ChromosomeType c2 = select_chromosome_with_adaptability();
            new_population[i] = cross(c1, c2);
        }
        _population = new_population;
        update_adaptability();
    }

    void mutate() override {
        for (int i = 0; i < _population.size(); ++ i){
            if (uniform_dist(random_engine) > mutation_rate){
                continue;
            }
            auto new_c = _population[i];
            mutate(new_c);
            _population[i] = new_c;
        }
        update_adaptability();
    }
    
    void show() const override {
        for (int i = 0; i < _population.size(); ++ i){
            std::cout << "<" << i << "> Adaptability: " << _adaptability[i] << std::endl;
            for (auto x : _population[i]){
                std::cout << x << ' ';
            }
            std::cout << std::endl;
        }
    }

    QueensPopulation(const std::vector<ChromosomeType>& p, double _mutation_rate) : _population(p), mutation_rate(_mutation_rate), _adaptability(p.size()) {
        random_engine.seed(time(nullptr));
        update_adaptability();
    }
};

std::vector<std::vector<int> > generate_queens_population(int n_queens, int population_size){
    std::vector<std::vector<int> > p(population_size);
    std::uniform_int_distribution<int> dist;
    std::default_random_engine engine;

    for (int i = 0; i < population_size; ++ i){
        for (int j = 0; j < n_queens; ++ j){
            p[i].push_back(dist(engine) % n_queens);

            std::cout << p[i][j] << " ";
        }
        std::cout << std::endl;
    }
    return p;
}

#include "algorithm/genetic.hpp"

int n = 20;

/* output format: 
<epoch_id> Adaptability: _
queen_1 queen_2 ... queen_n


Target adaptability is n*(n-1)/2, which means no confliction appears between all queen pairs.
*/

int main(){

    auto p = generate_queens_population(n, n*2);
    QueensPopulation pop(p, .5);
    GeneticAlgorithm<QueensPopulation> gen(pop);
    gen.evolve(5000);
    return 0;
}