#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include <cmath>

class Neuron {
public:
    Neuron(unsigned numInputs);
    float feedForward(const std::vector<float>& inputs);
    void setWeights(const std::vector<float>& weights);/*use at the creation of the neuron and
                                                         during a backpropagation */
    std::vector<float> getWeights() const;

private:
    std::vector<float> weights;
    // float activationFunction(float x);
};

#endif // NEURON_H