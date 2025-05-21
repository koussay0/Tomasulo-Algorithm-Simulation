#pragma once
#include <unordered_map>

enum class PredictorType {
    ONE_BIT,
    TWO_BIT
};

class BranchPredictor {
private:
    PredictorType type;

    std::unordered_map<int, int> predictionTable; // PC -> state

public:
    BranchPredictor(PredictorType type);

    bool predict(int pc); // Returns predicted branch outcome (true = taken)

    void update(int pc, bool actualOutcome); // Updates predictor based on actual result
};
