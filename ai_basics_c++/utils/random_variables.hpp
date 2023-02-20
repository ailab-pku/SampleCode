#include <random>
#include <ctime>

// Singleton Pattern
class RandomVariables{
private:

    RandomVariables() : random_engine(time(nullptr)) {}
    ~RandomVariables() = default;

    std::default_random_engine random_engine;
    std::uniform_real_distribution<double> uniform_dist;
    std::uniform_int_distribution<int> uniform_int_dist;

    static RandomVariables rv;

public:

    static int uniform_int(){
        return rv.uniform_int_dist(rv.random_engine);
    }

    static double uniform_real(){
        return rv.uniform_dist(rv.random_engine);
    }
};

RandomVariables RandomVariables::rv;