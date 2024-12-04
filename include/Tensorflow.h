#ifndef TENSORFLOW_H
#define TENSORFLOW_H

#include <string>
#include <cppflow/cppflow.h>
#include <vector>

class Tensorflow {
public:
    Tensorflow(const std::string &filename);
    ~Tensorflow();

    int test();

private:
    cppflow::model model;
};

#endif // TENSORFLOW_H