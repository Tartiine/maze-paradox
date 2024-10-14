#ifndef TILEMAPMODEL_H
#define TILEMAPMODEL_H

#include <vector>
#include <string>
#include <fann.h>
#include <fann_cpp.h>

/**
 * TileMapModel class represents the Neural Network Model used to score generated maps.
 * You can train and test the model with the different methods.
 * At the moment, it is using FANN with an MLP but will soon be using TensorFlow with a CNN.
 */

class TileMapModel {
public:
    TileMapModel(int input_size, int output_size);
    ~TileMapModel();

    void createModel(const std::string &type);
    void train(const std::string &directory);
    std::vector<std::tuple<int, std::string>> predict(const std::string &directory);
    void testModel(const std::string &datasetDirectory, const std::string &modelFile);
    void saveModel(const std::string &filename);
    void loadModel(const std::string &filename);

private:
    void loadData(const std::string &directory);
    std::vector<int> readTileMap(const std::string &filename);
    std::vector<fann_type> convertToFANNInput(const std::vector<int> &tileMap);
    std::vector<int> convertFromFANNOutput(fann_type *output);

    struct fann *ann;
    std::vector<std::vector<int>> tileMaps;
    std::vector<int> scores;
    int input_size;
    int output_size;
};

#endif // TILEMAPMODEL_H
