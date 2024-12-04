#ifndef TILEMAPMODEL_H
#define TILEMAPMODEL_H

#include <vector>
#include <string>
#include <cppflow/cppflow.h>
#include <algorithm>
#include <numeric>

using namespace std;

/**
 * TileMapModel class represents the Neural Network Model used to score generated maps.
 * You can train and test the model with the different methods.
 * At the moment, it is using FANN with an MLP but will soon be using TensorFlow with a CNN.
 */

class TileMapModel {
public:
    TileMapModel(int _height, int _width, const string &filename);
    ~TileMapModel();
    
    vector<int> readTileMapFromMemory(const vector<uint8_t> &binaryTileMap);
    void loadDataFromMemory(const vector<vector<uint8_t>> &tileMaps, const vector<int> &scores);
    void loadDataFromMemory(const vector<vector<uint8_t>> &tileMaps);

    vector<tuple<int, string>> predict();
    vector<vector<uint8_t>> testModel();

private:
    cppflow::tensor convertToCppflowInput() const;

    int height;
    int width;
    cppflow::model model;
    vector<int> scores;
    vector<vector<int>> tileMaps;

// OLD CODE - FANN + MLP Model

/*
#include <vector>
#include <string>
#include <tuple>
#include <fann.h>
#include <fann_cpp.h>
#include <cstdint>
*/

/*
public:
    TileMapModel(int input_size, int output_size);
    ~TileMapModel();

    void createModel(const std::string &type);
    void train();
    std::vector<std::tuple<int, std::string>> predict();
    std::vector<std::vector<uint8_t>> testModel(const std::string &modelFile);
    void saveModel(const std::string &filename);
    void loadModel(const std::string &filename);
    std::vector<int> readTileMapFromMemory(const std::vector<uint8_t> &binaryTileMap);
    void loadDataFromMemory(const std::vector<std::vector<uint8_t>> &tileMaps, const std::vector<int> &scores);
    void loadDataFromMemory(const std::vector<std::vector<uint8_t>> &tileMaps);

private:
    std::vector<fann_type> convertToFANNInput(const std::vector<int> &tileMap);
    std::vector<int> convertFromFANNOutput(fann_type *output);

    struct fann *ann;
    std::vector<std::vector<int>> tileMaps;
    std::vector<int> scores;
    int input_size;
    int output_size;
*/

};

#endif // TILEMAPMODEL_H
