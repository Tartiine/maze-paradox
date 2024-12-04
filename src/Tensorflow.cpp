#include "Tensorflow.h"
#include <vector>

Tensorflow::Tensorflow(const std::string &filename) : model(filename) {}

Tensorflow::~Tensorflow() {}

int Tensorflow::test() {
    std::cout << "--- TEST ---" << std::endl;

    // Step 2: Prepare the input tensor
    // Creating an input with shape (1, 22, 40, 1) filled with some dummy data (e.g., all ones)
    std::vector<float> input_data(22 * 40, 1.0f); // Fill with 1.0 for testing
    cppflow::tensor input_tensor(input_data, {1, 22, 40, 1});

    // Step 3: Perform inference using the updated cppflow API
    // Pass both input and output names as separate arguments
    auto output_tensors = model(
        {{"serving_default_conv2d_4_input:0", input_tensor}},  // Correct input tensor name
        {"StatefulPartitionedCall:0"}                           // Correct output tensor name
    );

    // Step 4: Extract the output data from the resulting tensor
    cppflow::tensor output_tensor = output_tensors[0];
    std::vector<float> output_data = output_tensor.get_data<float>();

    // Step 5: Display the output
    std::cout << "Model Output:" << std::endl;
    for (const auto& val : output_data) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    std::cout << "--- END TEST ---" << std::endl;

    return 0;
}

