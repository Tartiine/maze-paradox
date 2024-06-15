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
}

TileMapModel::~TileMapModel() {
    fann_destroy(ann);
}

void TileMapModel::createModel(const string &type) {
    if (type == "rb") {
        ann = fann_create_standard(4, input_size, 512, 128, output_size);
        fann_set_learning_rate(ann, 0.0001);
    
    } else if (type == "nb") {
        ann = fann_create_standard(4, input_size, 512, 128, output_size);
        fann_set_learning_rate(ann, 0.0001);
    }
    
    cout << "Created neural network model." << endl;
}
void TileMapModel::saveModel(const string &filename) {
    int result = fann_save(ann, filename.c_str());
    if (result == -1) {
        cerr << "Error saving model to file: " << filename << endl;
    } else {
        cout << "Model saved successfully to '" << filename << "'" << endl;
    }
}

void TileMapModel::loadModel(const string &filename) {
    ann = fann_create_from_file(filename.c_str());
    if (!ann) {
        cerr << "Error loading model from file: " << filename << endl;
    } else {
        cout << "Model loaded successfully from '" << filename << "'" << endl;
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

    fann_type min_val = *min_element(fann_input.begin(), fann_input.end());
    fann_type max_val = *max_element(fann_input.begin(), fann_input.end());

    for (auto &val : fann_input) {
        val = (val - min_val) / (max_val - min_val);
    }

    return fann_input;
}

vector<int> TileMapModel::convertFromFANNOutput(fann_type *output) {
    vector<int> tileMap(output, output + output_size);
    return tileMap;
}

void TileMapModel::loadData(const string &directory) {
    tileMaps.clear();
    scores.clear();

    string scorefile = directory + "/scores.txt";
    bool scoresFileExists = fs::exists(scorefile);

    for (const auto &entry : fs::directory_iterator(directory)) {
        if (entry.path().extension() == ".txt" && entry.path().filename() != "scores.txt") {
            vector<int> tileMap = readTileMap(entry.path().string());
            tileMaps.push_back(tileMap);
        }
    }

    if (scoresFileExists) {
        ifstream scoreFile(scorefile);
        if (!scoreFile.is_open()) {
            cerr << "Error opening score file: " << scorefile << endl;
            return;
        }

        int score;
        while (scoreFile >> score) {
            scores.push_back(score);
        }
        cout << "Loaded scores from " << scorefile << endl;
    } else {
        cout << "No score file found. Proceeding without scores." << endl;
    }

    cout << "Loaded data from " << directory << endl;
}

void TileMapModel::train(const string &directory) {
    loadData(directory);
    
    cout << "Training model..." << endl;
    struct fann_train_data *train_data = fann_create_train(tileMaps.size(), input_size, 1);
    for (size_t i = 0; i < tileMaps.size(); ++i) {
        vector<fann_type> input = convertToFANNInput(tileMaps[i]);
        copy(input.begin(), input.end(), train_data->input[i]);
        train_data->output[i][0] = scores[i] / 5.0; 
    }
    
    int max_epochs = 1000;
    int report_interval = 200;
    float desired_error = 0.0001;

    fann_train_on_data(ann, train_data, max_epochs, report_interval, desired_error);
    fann_destroy_train(train_data);
    cout << "Training complete." << endl;
}

vector<tuple<int, string>> TileMapModel::predict(const string &directory) {
    loadData(directory);

    cout << "Testing model..." << endl;
    vector<tuple<int, string>> predictions; 
    int totalError = 0;
    bool hasScores = !scores.empty();

    auto tileMapIt = tileMaps.begin();
    auto entryIt = fs::directory_iterator(directory);

    for (size_t i = 0; i < tileMaps.size(); ++i, ++tileMapIt, ++entryIt) {
        while (entryIt->path().extension() != ".txt" || entryIt->path().filename() == "scores.txt") {
            ++entryIt;
        }
        
        vector<fann_type> input = convertToFANNInput(*tileMapIt);

        fann_type *output = fann_run(ann, input.data());

        float predictedScore = static_cast<float>(output[0] * 5.0);
        predictions.emplace_back(predictedScore, entryIt->path().string());

        if (hasScores) {
            int actualScore = scores[i];
            int error = abs(predictedScore - actualScore);
            totalError += error;
            cout << "Tile map " << i + 1 << ": Predicted score = " << predictedScore << ", Actual score = " << actualScore << ", Error = " << error << endl;
        } else {
            cout << "Tile map " << i + 1 << ": Predicted score = " << predictedScore << endl;
        }
    }

    if (hasScores) {
        cout << "Average error = " << (double)totalError / tileMaps.size() << endl;
    }

    return predictions;
}

void TileMapModel::testModel(const string &datasetDirectory, const string &modelFile) {
    loadModel(modelFile);

    vector<tuple<int, string>> predictions = predict(datasetDirectory);

    sort(predictions.begin(), predictions.end(), [](const auto &a, const auto &b) {
        return get<0>(a) > get<0>(b);
    });

    for (size_t i = 0; i < predictions.size(); ++i) {
        if (i > 20) {
            //cout << "Deleting file: " << get<1>(predictions[i]) << " with predicted score: " << get<0>(predictions[i]) << endl;
            fs::remove(get<1>(predictions[i]));
        }
    }
    cout << "Testing and file cleanup complete." << endl;
}

//FIXME :  modify with tensorflow