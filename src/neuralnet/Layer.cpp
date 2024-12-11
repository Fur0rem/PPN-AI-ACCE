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


        std::cout << "Layer initialized: Weights_ (" << Weights_.rows() << " x " << Weights_.cols() << "), "
              << "Bias_ size = " << Bias_.size() << "\n";

    }



     

    // Layer::Eigen::Vector<float , numOutput> feedForward(const Eigen::Vector<float , numInputs>  &inputs;){
        
    //     Eigen::Vector<float , numOutput>  output  =  Weights_ * inputs + Bias_ ; 
    //     return output;

    // }



    Eigen::VectorXf Layer::feedForward(const Eigen::VectorXf& inputs) {
        // Debugging dimensions
        std::cout << "Layer feedForward: Weights_ (" << Weights_.rows() << " x " << Weights_.cols() << "), "<< "Inputs (" << inputs.size() << "), Bias_ (" << Bias_.size() << ")\n";

        // Check dimensions
        assert(Weights_.cols() == inputs.size() && "Input size does not match the number of columns in Weights_.");
        assert(Bias_.size() == Weights_.rows() && "Bias size does not match the number of rows in Weights_.");




        
        Eigen::VectorXf output = Weights_ * inputs + Bias_;
        Z_outpout_.resize(output.size()); /*to ensure same size */
        activation_output_.resize(output.size());
        
        Z_outpout_ =output; 
        activation_output_ = ReLu_vec(output);
        std::cout<<"the output have been to this layer \n";

        std::cout << "Layer feedForward: output size = " << output.size() << "\n";

        return activation_output_;
    }


    // void Layer::setWeights(const Eigen::VectorXf& weights){
        
    //     Weights_ = weights; 
    // }
        

    Eigen::MatrixXf Layer::getWeights(){
        return Weights_;



    }
    
    


/////


    void Layer::set_Prev_inputs(const Eigen::VectorXf& inputs){
        Prev_inputs_ = inputs; 
    }                     


    void Layer::set_Z_outpout(const Eigen::VectorXf& Z_outpout){
        Z_outpout_ = Z_outpout; 
    }
    // void Layer::set_error(const Eigen::VectorXf& error){
    //     error_ = error; 
    // }           




    // void Layer::set_error(const Eigen::VectorXf& previous_error,Eigen::MatrixXf previous_weight ){

    //     assert(previous_weight.cols() == previous_error.size() && "Dimension mismatch in set_error.");


    //     Eigen::VectorXf temp = (previous_weight.transpose())*previous_error; 

    //     error_.resize(temp.size()); // Resize explicitly to temp's size
    //     // error_ = temp.cwiseProduct(Z_outpout_).transpose(); 
    //     error_ = temp.cwiseProduct(Z_outpout_); 
    //     // error_  = temp.cwiseProduct(de_z).transpose(); 


    //     // Debug dimensions
    //     std::cout << "previous_weight: " << previous_weight.rows() << " x " << previous_weight.cols() << std::endl;
    //     std::cout << "previous_error: " << previous_error.rows() << " x " << previous_error.cols() << std::endl;
    //     std::cout << "error_: " << error_.rows() << " x " << error_.cols() << std::endl;
    // }        





    void Layer::set_error(const Eigen::VectorXf& previous_error, Eigen::MatrixXf previous_weight) {
        // Ensure dimensions are compatible
        assert(previous_weight.cols() == previous_error.size() && "Dimension mismatch in set_error.");

        // Calculate the error
        Eigen::VectorXf temp = previous_weight.transpose() * previous_error;
        Eigen::VectorXf de_z = DeReLu_vec(Z_outpout_);

        // Explicit resizing
        error_.resize(temp.size());
        error_ = temp.cwiseProduct(de_z);

        // Debug dimensions
        std::cout << "set_error: error_ size = " << error_.size() << "\n";
    }

















    // // void set_Z_outpout(const Eigen::VectorXf& Z_outpout);                                                         
    // // void set_error_last_layer(const Eigen::VectorXf& error);

    // void Layer::set_error_last_layer(const Eigen::VectorXf& pred 
    // , const Eigen::VectorXf& targets 
    // ,const Eigen::VectorXf& last_z_output ){

    //     // error_ = (pred -target)
    //     Eigen::VectorXf temp = pred -targets; 
    //     Eigen::VectorXf de_z = DeReLu_vec(last_z_output); 
    //     error_  = temp.cwiseProduct(de_z).transpose(); 
    // }


    // void Layer::set_error_last_layer(const Eigen::VectorXf& pred 
    // , const Eigen::VectorXf& targets ){

    //     // error_ = (pred -target)
    //     Eigen::VectorXf temp = pred -targets; 
    //     // Eigen::VectorXf de_z = DeReLu_vec(last_z_output); 
    //     Eigen::VectorXf de_z = DeReLu_vec(Z_outpout_); 
    //     error_  = temp.cwiseProduct(de_z).transpose(); 
    // }

    // void Layer::set_error_last_layer( const Eigen::VectorXf& targets ){

    //     assert(targets.size() == Z_outpout_.size() && "Targets and outputs must have the same size.");
    //     // error_ = (pred -target)
    //     // Eigen::VectorXf temp = pred -targets; 
    //     Eigen::VectorXf temp = activation_output_ -targets; 
        
    //     // Eigen::VectorXf de_z = DeReLu_vec(last_z_output); 
    //     Eigen::VectorXf de_z = DeReLu_vec(Z_outpout_); 
    //     error_.resize(temp.size()); // Ensure it matches temp's dimensions
    //     error_  = temp.cwiseProduct(de_z).transpose(); 
    //     // error_  = temp.cwiseProduct(de_z); 


    //     // Debug dimensions
    //     std::cout << "activation_output_: " << activation_output_.rows() << " x " << activation_output_.cols() << std::endl;
    //     std::cout << "targets: " << targets.rows() << " x " << targets.cols() << std::endl;
    //     std::cout << "error_: " << error_.rows() << " x " << error_.cols() << std::endl;


    // }



    void Layer::set_error_last_layer(const Eigen::VectorXf& targets) {
        // Ensure dimensions match
        assert(targets.size() == activation_output_.size() && "Targets and outputs must have the same size.");

        // Calculate the error
        Eigen::VectorXf temp = activation_output_ - targets;
        Eigen::VectorXf de_z = DeReLu_vec(Z_outpout_);

        // Explicit resizing
        error_.resize(temp.size());
        error_ = temp.cwiseProduct(de_z);

        // Debug dimensions
        std::cout << "set_error_last_layer: error_ size = " << error_.size() << "\n";
    }










    // std::vector<float> getWeights() const;


    // Eigen::MatrixXf weight_delta(const Eigen::VectorXf& error, const Eigen::VectorXf& inputs) {
    //     // Compute the outer product of error and inputs this will create a matrix of the 
    //     // weight delta we can later use in the update weight method 
    //     Eigen::MatrixXf weightGradient = error * inputs.transpose();
    //     return weightGradient;
    // }

    Eigen::MatrixXf Layer::weight_delta( const Eigen::VectorXf& inputs) {

        std::cout << "weight_delta: activation_output size = " << activation_output_.size() << ", error_ size = " << error_.size() << "\n";
        // Compute the outer product of error and inputs this will create a matrix of the 
        // weight delta we can later use in the update weight method 


        // Ensure the activation_output is compatible with Weights_
        std::cout << "thus is error size " <<error_.size() <<"\n";
        std::cout << "thus is Weight rows " <<Weights_.rows() <<"\n";
        std::cout << "thus is activation size " <<activation_output_.size() <<"\n";
        std::cout << "thus is inputs size " <<inputs.size() <<"\n";

        assert(activation_output_.size() == Weights_.cols() && "Activation output size does not match the number of columns in Weights_.");
        assert(error_.size() == Weights_.rows() && "Error size does not match the number of rows in Weights_.");

        Eigen::MatrixXf weightGradient = error_ * inputs.transpose();

        std::cout << "weight_delta: delta size = " << weightGradient.rows() << " x " << weightGradient.cols() << "\n";

        return weightGradient;
    }


    Eigen::VectorXf Layer::bias_delta( ){

        std::cout << "bias_delta: error_ size = " << error_.size() << "\n";
        return error_; 
    }




    // Eigen::MatrixXf Bias_delta(const Eigen::VectorXf& error ) {
    //     // Compute the outer product of error and inputs this will create a matrix of the 
    //     // weight delta we can later use in the update weight method 
    //     Eigen::MatrixXf weightGradient = error * inputs.transpose();
    //     return weightGradient;
    // }




    /*I should implement print method to debug at multiple step of the program */
    // void print_dim(){ std::cout<<}

// private:
//     // std::vector<float> weights;
//     // Eigen::Matrix3f Weights;
//     // Eigen::Matrix<float, numInputs, output> Weights_;
//     // Eigen::Vector<float , numOutput> Bias_;


//     Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> Weights_;
//     Eigen::VectorXf Bias_; // Bias: vector of size (numOutput)
//     Eigen::VectorXf Prev_inputs; // Bias: vector of size (numOutput)
//     Eigen::VectorXf Z_outpout; //
//     Eigen::VectorXf error; // 




// /////


    


    
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






