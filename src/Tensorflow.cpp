#include "Tensorflow.h"

Tensorflow::Tensorflow() {

}

Tensorflow::~Tensorflow() {

}

int Tensorflow::test() {
    std::cout << "--- TEST ---" << std::endl;

    // Create a tensor from a list, a = [1.0, 2.0, 3.0]
    auto a = cppflow::tensor({1.0, 2.0, 3.0});

    // Create a tensor of shape 3 filled with 1.0, b = [1.0, 1.0, 1.0]
    auto b = cppflow::fill({3}, 1.0);

    std::cout << a + b << std::endl;

    std::cout << "--- END TEST ---" << std::endl;

    return 0;
}

void Tensorflow::loadModel(const std::string &filename) {
    cppflow::model model(filename);

    
}