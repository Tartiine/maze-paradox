#include "TileMapModel.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

using namespace std;

TileMapModel::TileMapModel(int input_size, int output_size) 
    : input_size(input_size), output_size(output_size) {
    ann = fann_create_standard(3, input_size, 10, output_size);
    cout << "Created neural network model." << endl;
}

TileMapModel::~TileMapModel() {
    fann_destroy(ann);
}

void TileMapModel::saveModel(const string &filename) {
    int result = fann_save(ann, filename.c_str());
    if (result == -1) {
        cerr << "Error saving model to file: " << filename << endl;
    } else {
        cout << "Model saved successfully to '" << filename << "'" << endl;
    }
}

vector<int> TileMapModel::readTileMap(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return {};
    }

    vector<int> tileMap;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int value;
        while (ss >> value) {
            tileMap.push_back(value);
        }
    }
    return tileMap;
}

vector<fann_type> TileMapModel::convertToFANNInput(const vector<int> &tileMap) {
    vector<fann_type> fann_input(tileMap.begin(), tileMap.end());
    for (auto &val : fann_input) {
        val = val / 2.0;
    }
    return fann_input;
}

vector<int> TileMapModel::convertFromFANNOutput(fann_type *output) {
    vector<int> tileMap(output, output + output_size);
    return tileMap;
}

void TileMapModel::loadTrainingData(const string &directory) {
    string scorefile = directory + "/scores.txt";
    for (const auto &entry : fs::directory_iterator(directory)) {
        if (entry.path().extension() == ".txt" && entry.path().filename() != "scores.txt") {
            tileMaps.push_back(readTileMap(entry.path().string()));
        }
    }
    ifstream scoreFile(scorefile);
    int score;
    while (scoreFile >> score) {
        scores.push_back(score);
    }
    cout << "Loaded training data from " << directory << endl;
}

void TileMapModel::train() {
    cout << "Training model..." << endl;
    struct fann_train_data *train_data = fann_create_train(tileMaps.size(), input_size, 1);
    for (size_t i = 0; i < tileMaps.size(); ++i) {
        vector<fann_type> input = convertToFANNInput(tileMaps[i]);
        copy(input.begin(), input.end(), train_data->input[i]);
        train_data->output[i][0] = scores[i] / 20.0; 
    }
    fann_train_on_data(ann, train_data, 1000, 10, 0.01);
    fann_destroy_train(train_data);
    cout << "Training complete." << endl;
}

int TileMapModel::predict(const vector<int> &tileMap) {
    vector<fann_type> input = convertToFANNInput(tileMap);
    fann_type *output = fann_run(ann, input.data());
    return static_cast<int>(output[0] * 20);  
}