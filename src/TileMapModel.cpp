#include "TileMapModel.h"

TileMapModel::TileMapModel(int _height, int _width, const string &filename) :  height(_height), width(_width), model(filename) {
}

TileMapModel::~TileMapModel() {
}

cppflow::tensor TileMapModel::convertToCppflowInput() const {
    std::vector<float> input;

    int batch_size = tileMaps.size();
    for (int i = 0; i < batch_size; ++i) {
        const std::vector<int>& tileMap = tileMaps[i];

        int min_val = *std::min_element(tileMap.begin(), tileMap.end());
        int max_val = *std::max_element(tileMap.begin(), tileMap.end());

        for (int val : tileMap) {
            float normalized = (max_val != min_val) ? static_cast<float>(val - min_val) / (max_val - min_val) : 0.0f;
            input.push_back(normalized);
        }
    }

    return cppflow::tensor(input, {batch_size, height, width, 1});
}


// Read tilemap from binary data in memory
vector<int> TileMapModel::readTileMapFromMemory(const vector<uint8_t> &binaryTileMap) {
    vector<int> tileMap;
    for (uint8_t tile : binaryTileMap) {
        tileMap.push_back(static_cast<int>(tile));
    }
    return tileMap;
}

// Load data from memory with scores
void TileMapModel::loadDataFromMemory(const vector<vector<uint8_t>> &tileMapsInMemory, const vector<int> &scoresData) {
    tileMaps.clear();
    scores = scoresData;

    for (const auto &binaryTileMap : tileMapsInMemory) {
        tileMaps.push_back(readTileMapFromMemory(binaryTileMap));
    }
    cout << "Loaded data from memory with scores." << endl;
}

void TileMapModel::loadDataFromMemory(const vector<vector<uint8_t>> &tileMapsInMemory) {
    tileMaps.clear();
    scores.clear(); 

    for (const auto &binaryTileMap : tileMapsInMemory) {
        tileMaps.push_back(readTileMapFromMemory(binaryTileMap));
    }
    cout << "Loaded data from memory without scores." << endl;
}

vector<tuple<int, string>> TileMapModel::predict() {
    cout << "Testing model..." << endl;
    vector<tuple<int, string>> predictions; 
    int totalError = 0;
    bool hasScores = !scores.empty();

    auto input_tensor = convertToCppflowInput();
    auto output_tensors = model(
        {{"serving_default_input_layer_input:0", input_tensor}},  // Correct input tensor name
        {"StatefulPartitionedCall:0"}                           // Correct output tensor name
    );

    vector<float> results = output_tensors[0].get_data<float>();

    for (size_t i = 0; i < tileMaps.size(); ++i) {
        auto predictedScore = results[i];
        cout << "Tile map " << i + 1 << ": Predicted score = " << predictedScore << endl;
        predictions.emplace_back(predictedScore, "TileMap_" + to_string(i));

        if (hasScores) {
            int actualScore = scores[i];
            int error = abs(predictedScore - actualScore);
            totalError += error;
            cout << "Tile map " << i + 1 << ": Predicted score = " << predictedScore
                      << ", Actual score = " << actualScore << ", Error = " << error << endl;
        } else {
            //cout << "Tile map " << i + 1 << ": Predicted score = " << predictedScore << endl;
        }
    }

    if (hasScores) {
        cout << "Average error = " << (double)totalError / tileMaps.size() << endl;
    }

    return predictions;
}

vector<vector<uint8_t>> TileMapModel::testModel() {
    vector<tuple<int, string>> predictions = predict();

    vector<size_t> indices(predictions.size());
    iota(indices.begin(), indices.end(), 0);
    sort(indices.begin(), indices.end(), [&](size_t a, size_t b) {
        return get<0>(predictions[a]) > get<0>(predictions[b]);
    });

    if (indices.size() > 20) {
        indices.resize(20);
    }

    vector<vector<uint8_t>> filteredTileMaps;
    for (size_t idx : indices) {
        vector<int>& intTileMap = tileMaps[idx];  // Access the int tile map
        vector<uint8_t> uint8TileMap(intTileMap.begin(), intTileMap.end());  // Convert to uint8_t
        filteredTileMaps.push_back(uint8TileMap);  // Add to filtered results
    }

    cout << "Testing complete. Filtered top 20 tile maps." << endl;

    return filteredTileMaps;
}

// OLD CODE - FANN + MLP Model

/*
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <numeric>

using namespace std;
namespace fs = filesystem;

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

// Read tilemap from binary data in memory
vector<int> TileMapModel::readTileMapFromMemory(const vector<uint8_t> &binaryTileMap) {
    vector<int> tileMap;
    for (uint8_t tile : binaryTileMap) {
        tileMap.push_back(static_cast<int>(tile));
    }
    return tileMap;
}

// Load data from memory with scores
void TileMapModel::loadDataFromMemory(const vector<vector<uint8_t>> &tileMapsInMemory, const vector<int> &scoresData) {
    tileMaps.clear();
    scores = scoresData;

    for (const auto &binaryTileMap : tileMapsInMemory) {
        tileMaps.push_back(readTileMapFromMemory(binaryTileMap));
    }
    cout << "Loaded data from memory with scores." << endl;
}


void TileMapModel::loadDataFromMemory(const vector<vector<uint8_t>> &tileMapsInMemory) {
    tileMaps.clear();
    scores.clear(); 

    for (const auto &binaryTileMap : tileMapsInMemory) {
        tileMaps.push_back(readTileMapFromMemory(binaryTileMap));
    }
    cout << "Loaded data from memory without scores." << endl;
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

void TileMapModel::train() {
    
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

vector<tuple<int, string>> TileMapModel::predict() {
    cout << "Testing model..." << endl;
    vector<tuple<int, string>> predictions; 
    int totalError = 0;
    bool hasScores = !scores.empty();


    for (size_t i = 0; i < tileMaps.size(); ++i) {
        vector<fann_type> input = convertToFANNInput(tileMaps[i]);
        fann_type *output = fann_run(ann, input.data());

        float predictedScore = output[0] * 5.0;
        predictions.emplace_back(predictedScore, "TileMap_" + to_string(i));

        if (hasScores) {
            int actualScore = scores[i];
            int error = abs(predictedScore - actualScore);
            totalError += error;
            cout << "Tile map " << i + 1 << ": Predicted score = " << predictedScore
                      << ", Actual score = " << actualScore << ", Error = " << error << endl;
        } else {
            //cout << "Tile map " << i + 1 << ": Predicted score = " << predictedScore << endl;
        }
    }

    if (hasScores) {
        cout << "Average error = " << (double)totalError / tileMaps.size() << endl;
    }

    return predictions;
}

vector<vector<uint8_t>> TileMapModel::testModel(const string &modelFile) {
    loadModel(modelFile);
    vector<tuple<int, string>> predictions = predict();

    vector<size_t> indices(predictions.size());
    iota(indices.begin(), indices.end(), 0);
    sort(indices.begin(), indices.end(), [&](size_t a, size_t b) {
        return get<0>(predictions[a]) > get<0>(predictions[b]);
    });

    if (indices.size() > 20) {
        indices.resize(20);
    }

    vector<vector<uint8_t>> filteredTileMaps;
    for (size_t idx : indices) {
        vector<int>& intTileMap = tileMaps[idx];  // Access the int tile map
        vector<uint8_t> uint8TileMap(intTileMap.begin(), intTileMap.end());  // Convert to uint8_t
        filteredTileMaps.push_back(uint8TileMap);  // Add to filtered results
    }

    cout << "Testing complete. Filtered top 20 tile maps." << endl;

    return filteredTileMaps;
}
*/

//FIXME :  modify with tensorflow