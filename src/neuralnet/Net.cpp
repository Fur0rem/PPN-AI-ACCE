#include<iostream>
#include<cmath>
#include<vector>
#include <Eigen/Dense>


#include<../include/Layer.h>
#include<../include/Net.h>
#include<../include/Activation_Function.h>


    // Layer::Layer(unsigned numInputs ,unsigned numOutput){

    //         Weights_ = MatrixXd::Random(numInputs,numOutput)
    //         Bias = MatrixXd::Random(numOutput,1)


    //         // Eigen::Matrix<float, numInputs, output> Weights;
    //         // Eigen::Vector<float , numOutput> bias;
    // }


    // Layer::Layer(unsigned numInputs, unsigned numOutput) {
    //     // Resize the matrix and vector to the appropriate sizes to ensure proper dimension at runtime 
    //     // Weights_.resize(numInputs, numOutput);
    //     Weights_.resize(numOutput, numInputs);
    //     Bias_.resize(numOutput);

    //     // Initialize weights and biases with random values 
    //     Weights_ = Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>::Random(numOutput, numInputs);
    //     Bias_ = Eigen::VectorXf::Random(numOutput);
    // }


    Net::Net(unsigned int numInputs, unsigned int numOutput){
        Layer l1 = Layer(5,16);
        net.push_back(l1); 

        Layer l2 = Layer(16,32);
        net.push_back(l2); 
        
        Layer l3 = Layer(32,32);
        net.push_back(l3); 

        Layer l4 = Layer(32,16);

        net.push_back(l4); 

        Layer l5 = Layer(16,1);

        net.push_back(l5); 
        
        

    }


     

    // Layer::Eigen::Vector<float , numOutput> feedForward(const Eigen::Vector<float , numInputs>  &inputs;){
        
    //     Eigen::Vector<float , numOutput>  output  =  Weights_ * inputs + Bias_ ; 
    //     return output;

    // }



    Eigen::VectorXf Net::feedForward(const Eigen::VectorXf& inputs) {
        // Eigen::VectorXf output = Weights_ * inputs + Bias_;
        // Eigen::VectorXf  output ; 
        Eigen::VectorXf  output = net[0].feedForward(inputs); 
        std::cout<<"We are in the loop \n"; 


        for (int i =1 ; i<net.size() ;i++){
            // Eigen::VectorXf  output = net.feedForward(inputs);

            std::cout<<"We are in the poop \n"; 
            std::cout<<"here is output:\n"<<output.size()<< " \n"; 
            
            output = net[i].feedForward(output);


        }
        return output;
    }




    // void Layer::setWeights(const Eigen::VectorXf& weights){
        
    //     Weights_ = weights; 
    // }
        

    // Eigen::MatrixXf Layer::getWeights(){
    //     return Weights_;



    // }
    
    

    
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






