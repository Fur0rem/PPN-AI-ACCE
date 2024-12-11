#include<iostream>
#include<cmath>
#include<vector>
#include <Eigen/Dense>


#include<../include/Layer.h>
#include<../include/Net.h>
#include<../include/Activation_Function.h>




int main(){


    Layer l1 = Layer(5,4); 

    Eigen::VectorXf Input = Eigen::VectorXf::Random(5);

    Eigen::VectorXf output = l1.feedForward(Input);

    Eigen::IOFormat VectorFmt(4, Eigen::DontAlignCols, ", ", "; ", "[", "]", "", "");

    std::cout << "Formatted vector:" << std::endl;
    std::cout << output.format(VectorFmt) << std::endl;


    Net reseau1 =Net(5,1); 
    

    Eigen::VectorXf input = Eigen::VectorXf::Random(5);




    Eigen::VectorXf ouput_reseau = reseau1.feedForward(input); 

    


    ////////////////////////////////////////////////

    // Define a format for printing
    Eigen::IOFormat CleanFormat(4, 0, ", ", "\n", "[", "]");

    // Print the vector with the format
    std::cout << ouput_reseau.format(CleanFormat) << std::endl;


    std::cout <<"This is the weights of the 2nd layer \n";
    // std::cout <<reseau1.net[2].getWeights()<<"\n";

    std::cout<<"this is the unformated output "<<ouput_reseau<<"\n"; 



    // Eigen::VectorXf thetarget(1);
    // thetarget(0) = 4.0f; 


    // reseau1.backward(thetarget); 


    // Eigen::MatrixXf targetMatrix(1, 1);
    // targetMatrix(0, 0) = 4.0f;

    // Eigen::VectorXf thetarget = targetMatrix.col(0); // Or targetMatrix.row(0).transpose()
    // reseau1.backward(thetarget);

    Eigen::VectorXf targets(1); 
    targets(0) = 4.0f; // Set the target value

    // Debugging dimensions
    std::cout << "Target dimensions: " << targets.rows() << " x " << targets.cols() << std::endl;

    // Call backward
    reseau1.backward(targets);

    return 0;


    

    

    ////////////////////////////////////////////////

    // std::cout <<"this is the output of the network " <<ouput_reseau<<std::endl;
    // std::cout << output.format(ouput_reseau) << std::endl;







    


    // input  = 
    

}