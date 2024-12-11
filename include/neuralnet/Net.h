#ifndef NET_H
#define NET_H

#include <vector>
#include <Eigen/Dense>
#include <cmath>
// #include <stdlib.h>

#include <Layer.h>



class Net {
public:
    // Net(Layer Layers[], std::string loss);
    // Net(int taille_input, int taille_output, std::string loss);
    Net(unsigned int taille_input,unsigned int taille_output);

    // Net 
    // Eigen::Vector3f feedForward(const Eigen::Vector3f &inputs;); /*give us the ouput*/
    Eigen::VectorXf feedForward(const Eigen::VectorXf& inputs);
    void backward (const Eigen::VectorXf& targets);



    // void feedForward(const Eigen::VectorXf& Loss);


    // void setWeights(const std::vector<float>& weights);/*use at the creation of the neuron and
                                                        //  during a backpropagation */
    // std::vector<float> getWeights() const;

// private:

    // Layer Layers[]; /*our */
    
    // std::vector<float> getWeights() const;
    


    std::vector<Layer> net ; 


    /**/
    // Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> Weights_;
    // Eigen::VectorXf Bias_;  


    


    // float activationFunction(float x);
};

#endif 