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

    return 0;
    ////////////////////////////////////////////////

    // std::cout <<"this is the output of the network " <<ouput_reseau<<std::endl;
    // std::cout << output.format(ouput_reseau) << std::endl;







    


    // input  = 
    

}