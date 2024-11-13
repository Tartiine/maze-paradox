#ifndef TENSORFLOW_H
#define TENSORFLOW_H

#include <string>
#include <cppflow/cppflow.h>

class Tensorflow {
public:
    Tensorflow();
    ~Tensorflow();

    int test();
    void loadModel(const std::string &filename);
};

#endif // TENSORFLOW_H