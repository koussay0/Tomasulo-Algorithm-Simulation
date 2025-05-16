#pragma once

struct PipelineStage {
    int issue = -1;
    int startExec = -1;
    int endExec = -1;
    int writeBack = -1;
};