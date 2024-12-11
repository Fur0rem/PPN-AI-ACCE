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



    // void backward(const Eigen::VectorXf& inputs , 
    // const Eigen::VectorXf& previous_error,
    //                 const Eigen::VectorXf&  z_output,
    //                 Eigen::MatrixXf previous_weight);/* in this function we will implement
    //             a check to see if we are in the last layer or not 
    //             -if we are in the last layer special formula 
    //             -if we are note usual formula */


    // void setWeights(const std::vector<float>& weights);/*use at the creation of the neuron and
                                                        //  during a backpropagation */


    // void setWeights(const Eigen::VectorXf& weights);
    /*use at the creation of the neuron and   during a backpropagation */

    void set_Prev_inputs(const Eigen::VectorXf& inputs);                                                         
    void set_Z_outpout(const Eigen::VectorXf& Z_outpout);                                                         
    
    
    // void set_error(const Eigen::VectorXf& previous_error,
    //                 const Eigen::VectorXf&  z_output,
    //                 Eigen::MatrixXf previous_weight);  

    
    void set_error(const Eigen::VectorXf& previous_error,
                    Eigen::MatrixXf previous_weight);  


    /*set_error(const Eigen::VectorXf& previous_error,
                    const Eigen::VectorXf&  z_output ,Eigen::MatrixXf previous_weight )*/                    
    
    // void set_error_last_layer(const Eigen::VectorXf& pred 
    // , const Eigen::VectorXf& targets 
    // ,const Eigen::VectorXf& last_z_output );

    void set_error_last_layer(const Eigen::VectorXf& targets  );

    /* might be unecessary */
    // Eigen::VectorXf bias_delta(const Eigen::VectorXf& error );
    Eigen::VectorXf bias_delta( );
    // void weight_delta(const Eigen::VectorXf& error , const Eigen::VectorXf& inputs)
    // Eigen::MatrixXf weight_delta(const Eigen::VectorXf& error , const Eigen::VectorXf& inputs);
    Eigen::MatrixXf weight_delta( const Eigen::VectorXf& inputs);

    // std::vector<float> getWeights() const;
    Eigen::MatrixXf getWeights(); 


    /*I should implement print method to debug at multiple step of the program */
    // void print_dim(){ std::cout<<}

// private:
    // std::vector<float> weights;
    // Eigen::Matrix3f Weights;
    // Eigen::Matrix<float, numInputs, output> Weights_;
    // Eigen::Vector<float , numOutput> Bias_;


    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> Weights_;
    Eigen::VectorXf Bias_; // Bias: vector of size (numOutput)
    Eigen::VectorXf Prev_inputs_; // Bias: vector of size (numOutput)
    Eigen::VectorXf Z_outpout_; //
    Eigen::VectorXf activation_output_; //
    
    Eigen::VectorXf error_; // 
    // Eigen::Matrix<float, Eigen::Dynamic, 1> Bias_;

    // for simplicity we are gonna start by saying all layer have 
    
    // float activationFunction(float x);
};

#endif 