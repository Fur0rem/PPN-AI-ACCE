#include<iostream>
#include<cmath>
#include<vector>
#include <Eigen/Dense>


#include<../include/Layer.h>
#include<../include/Activation_Function.h>


    // Layer::Layer(unsigned numInputs ,unsigned numOutput){

    //         Weights_ = MatrixXd::Random(numInputs,numOutput)
    //         Bias = MatrixXd::Random(numOutput,1)


    //         // Eigen::Matrix<float, numInputs, output> Weights;
    //         // Eigen::Vector<float , numOutput> bias;
    // }


    Layer::Layer(unsigned numInputs, unsigned numOutput) {
        // Resize the matrix and vector to the appropriate sizes to ensure proper dimension at runtime 
        // Weights_.resize(numInputs, numOutput);
        Weights_.resize(numOutput, numInputs);
        Bias_.resize(numOutput);

        // Initialize weights and biases with random values 
        Weights_ = Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>::Random(numOutput, numInputs);
        Bias_ = Eigen::VectorXf::Random(numOutput);
    }



     

    // Layer::Eigen::Vector<float , numOutput> feedForward(const Eigen::Vector<float , numInputs>  &inputs;){
        
    //     Eigen::Vector<float , numOutput>  output  =  Weights_ * inputs + Bias_ ; 
    //     return output;

    // }



    Eigen::VectorXf Layer::feedForward(const Eigen::VectorXf& inputs) {
        Eigen::VectorXf output = Weights_ * inputs + Bias_;
        return output;
    }


    void Layer::setWeights(const Eigen::VectorXf& weights){
        
        Weights_ = weights; 
    }
        

    Eigen::MatrixXf Layer::getWeights(){
        return Weights_;



    }
    
    

    
        /*use at the creation of the neuron andvoid */

    // float Neuron::feedForward(const std::vector<float>& inputs){
    //     float output= 0;
    //     for(int i =0 ; i < inputs.size() ; i++){
    //         output += inputs[i] * weights[i];  

    //     }
    //     return ReLu(output); 
    // }

    // float 


    // Eigen::Matrix3f  Neuron::getWeights() const{
    //     return std::vector<float> weights;
    // }

    // void Neuron::setWeights(const std::vector<float>& weights){
        

    // }






