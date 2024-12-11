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

        // Layer l6 =Layer(1 ,1);
        // net.push_back(l6); 


    }




     

    // Layer::Eigen::Vector<float , numOutput> feedForward(const Eigen::Vector<float , numInputs>  &inputs;){
        
    //     Eigen::Vector<float , numOutput>  output  =  Weights_ * inputs + Bias_ ; 
    //     return output;

    // }



    Eigen::VectorXf Net::feedForward(const Eigen::VectorXf& inputs) {
        // Eigen::VectorXf output = Weights_ * inputs + Bias_;
        // Eigen::VectorXf  output ; 
        Eigen::VectorXf  output = net[0].feedForward(inputs); 
        // std::cout<<"We are in the loop \n"; 

        std::cout << "Net feedForward: Initial layer output size = " << output.size() << "\n";


        for (int i =1 ; i<net.size() ;i++){
            std::cout << "Net feedForward: Layer " << i << ", input size = " << output.size() << "\n";
            // Eigen::VectorXf  output = net.feedForward(inputs);

            // std::cout<<"We are in the poop \n"; 
            // std::cout<<"here is output:\n"<<output.size()<< " \n"; 
            assert(net[i].Weights_.cols() == output.size() && "Layer input size does not match the previous layer output size.");            
            
            output = net[i].feedForward(output);


            std::cout << "Net feedForward: Layer " << i << ", output size = " << output.size() << "\n";
            // net[i].


        }
        return output;
    }


	//FIXME 
    void Net::backward (const Eigen::VectorXf& targets){

        std::cout << "Target dimensions: " << targets.rows() << " x " << targets.cols() << std::endl;
        std::cout << "Output layer dimensions: " << net.back().Weights_.rows() << " x " << net.back().Weights_.cols() << std::endl;




        int N = net.size()-1; 

        // net[net.size()-1].set_error_last_layer(targets); 
        /*We calculate the error for the last layer this one is special */
        net[N].set_error_last_layer(targets); 

        /*We calculate the gradient of the weights */
        Eigen::MatrixXf deltaW = net[N].weight_delta(net[N-1].activation_output_); 
        /*WEIGHT UPDATE */
        net[N].Weights_ = net[N].Weights_ - 0.01* deltaW; 


        /*we calculate gradient of bias */
        Eigen::VectorXf deltab = net[N].bias_delta(); 
        /*BIAS UPDATE */
        net[N].Bias_ = net[N].Bias_ - 0.01* deltab; 

        /*loop for the rest of the weights */
        for( int i =N-1 ; i>1 ; i--){
            net[i].set_error(net[i+1].Z_outpout_ ,net[i+1].Weights_ ); 

            Eigen::VectorXf dW =  net[i].weight_delta(net[i-1].activation_output_);
            net[i].Weights_ = net[i].Weights_ - 0.01*dW; 


            Eigen::VectorXf dB =  net[i].bias_delta(); 
            net[i].Bias_ = net[i].Bias_ - 0.01*dB; 

            std::cout << "Backward pass: Layer " << i << ", error_ size = " << net[i].error_.size() << "\n";


        }


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






