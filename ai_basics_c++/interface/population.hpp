#pragma once

#include <vector>

template <typename ChromosomeType>
class PopulationBase{
public:
    
    PopulationBase() = default;
    virtual ~PopulationBase() {}

    typedef ChromosomeType ChromosomeBaseType;

    virtual std::vector<ChromosomeType> population() const = 0;

    virtual std::vector<double> adaptability() const = 0;

    virtual void show() const = 0;

    virtual void cross() = 0;

    virtual void mutate() = 0;
};