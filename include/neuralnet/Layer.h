#ifndef LAYER_H
#define LAYER_H

#include<iostream>
#include <vector>
#include <Eigen/Dense>
#include <cmath>

class Layer {
public:
    Layer(unsigned numInputs ,unsigned numOutput);
    
    // Layer(unsigned numInputs ,unsigned numOutput  
    // Eigen::Matrix<float, numInputs, output> Weights
    // Eigen::Vector<float , numOutput> bias);


    // Eigen::Vector<float , numOutput> feedForward(const Eigen::Vector<float , numInputs>  &inputs;);
    // Eigen::Vector<float , numOutput> feedForward(const Eigen::Vector<float , numInputs>  &inputs;);

    Eigen::VectorXf feedForward(const Eigen::VectorXf& inputs);


    // void setWeights(const std::vector<float>& weights);/*use at the creation of the neuron and
                                                        //  during a backpropagation */


    void setWeights(const Eigen::VectorXf& weights);/*use at the creation of the neuron and
                                                         during a backpropagation */
    // std::vector<float> getWeights() const;
    Eigen::MatrixXf getWeights(); 


    /*I should implement print method to debug at multiple step of the program */
    // void print_dim(){ std::cout<<}

private:
    // std::vector<float> weights;
    // Eigen::Matrix3f Weights;
    // Eigen::Matrix<float, numInputs, output> Weights_;
    // Eigen::Vector<float , numOutput> Bias_;


    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> Weights_;
    Eigen::VectorXf Bias_;                                         // Bias: vector of size (numOutput)
    // Eigen::Matrix<float, Eigen::Dynamic, 1> Bias_;

    // for simplicity we are gonna start by saying all layer have 
    
    // float activationFunction(float x);
};

#endif 