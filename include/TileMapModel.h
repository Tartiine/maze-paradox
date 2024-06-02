#ifndef TILEMAPMODEL_H
#define TILEMAPMODEL_H

#include <vector>
#include <string>
#include <fann.h>
#include <fann_cpp.h>

using namespace std;

class TileMapModel {
public:
    TileMapModel(int input_size, int output_size);
    ~TileMapModel();

    void loadTrainingData(const string &directory);
    void train();
    int predict(const vector<int> &tileMap);
    void saveModel(const string &filename);

private:
    vector<int> readTileMap(const string &filename);
    vector<fann_type> convertToFANNInput(const vector<int> &tileMap);
    vector<int> convertFromFANNOutput(fann_type *output);

    struct fann *ann;
    vector<vector<int>> tileMaps;
    vector<int> scores;
    int input_size;
    int output_size;
};

#endif // TILEMAPMODEL_H
