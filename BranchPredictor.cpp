#include "BranchPredictor.h"

BranchPredictor::BranchPredictor(PredictorType type) : type(type) {}

bool BranchPredictor::predict(int pc) {
    int state = predictionTable[pc];

    if (type == PredictorType::ONE_BIT) {
        return state == 1;
    } else {
        return state >= 2; // 2 or 3 means predict taken
    }
}

void BranchPredictor::update(int pc, bool actualOutcome) {
    int& state = predictionTable[pc];

    if (type == PredictorType::ONE_BIT) {
        state = actualOutcome ? 1 : 0;
    } else {
        if (actualOutcome) {
            if (state < 3) state++;
        } else {
            if (state > 0) state--;
        }
    }
}
