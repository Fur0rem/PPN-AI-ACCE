
#ifndef ACITVATION_FUNCTION_H
#define ACITVATION_FUNCTION_H

#include <cmath>
#include <vector>
#include <Eigen/Dense>


inline float Sigmoid(float x){
    return 1/(1+exp(-x)); 
}

inline float ReLu(float x){
   if(x<=0) return 0; 
   else return x;  
}

inline float DeReLu(float x){
   if(x<=0) return 0; 
   else return 1;  
}


// inline Eigen::VectorXf ReLu_vec(Eigen::VectorXf input){
//     for (float &element : input) {
//         ReLu(element); // Double each element
//     }
// }


inline Eigen::VectorXf ReLu_vec(Eigen::VectorXf input) {
    for (float &element : input) {
        element = ReLu(element); // Apply ReLu and store the result
    }
    return input; // 
}



inline Eigen::VectorXf DeReLu_vec(Eigen::VectorXf input) {
    for (float &element : input) {
        element = DeReLu(element); // Apply ReLu and store the result
    }
    return input; // 
}



inline float MSE(std::vector<float> output , std::vector<float> targets  ){
    float loss =0; 
    for (int i =0 ; i< targets.size() ; i++){
        loss += (output[i] - targets[i])*(output[i] - targets[i]);

    }
    return loss/targets.size() ; /*we divide by N at the end in order to diminish 
                                error accumulation */
}


inline float MAE(std::vector<float> output , std::vector<float> targets  ){
    float loss =0; 
    for (int i =0 ; i< targets.size() ; i++){
        loss += abs(output[i] - targets[i]); 

    }
    return loss/targets.size();  /*we divide by N at the end in order to diminish 
                                error accumulation */
}



#endif