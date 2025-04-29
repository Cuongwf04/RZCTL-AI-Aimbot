#ifndef MOUSE_H
#define MOUSE_H

#include <windows.h>
#include <atomic>
#include <mutex>
#include <chrono>
#include <vector>
#include <deque>
#include "rzctl.h"
#include "AimbotTarget.h"

class MouseThread
{
private:
    int detection_resolution;
    int dpi;
    float sensitivity;
    int fovX;
    int fovY;
    float minSpeedMultiplier;
    float maxSpeedMultiplier;
    int predictionInterval;
    bool auto_shoot;
    float bScope_multiplier;

    std::mutex input_method_mutex;
    RZControl* rzctl;

    std::chrono::steady_clock::time_point lastTargetTime;
    std::atomic<bool> targetDetected;
    std::deque<std::pair<int, int>> futurePositions;
    std::mutex futurePositionsMutex;

    void pressMouse(const AimbotTarget& target);
    void releaseMouse();
    std::vector<std::pair<int, int>> predictFuturePositions(int x, int y, int steps);

public:
    MouseThread(
        int detection_resolution,
        int dpi,
        float sensitivity,
        int fovX,
        int fovY,
        float minSpeedMultiplier,
        float maxSpeedMultiplier,
        int predictionInterval,
        bool auto_shoot,
        float bScope_multiplier,
        RZControl* rzctl
    );

    void updateConfig(
        int detection_resolution,
        int dpi,
        float sensitivity,
        int fovX,
        int fovY,
        float minSpeedMultiplier,
        float maxSpeedMultiplier,
        int predictionInterval,
        bool auto_shoot,
        float bScope_multiplier
    );

    void moveMousePivot(int x, int y);
    void setLastTargetTime(std::chrono::steady_clock::time_point time);
    void setTargetDetected(bool detected);
    void storeFuturePositions(const std::vector<std::pair<int, int>>& positions);
    void clearFuturePositions();
    void checkAndResetPredictions();
    void setRZControl(RZControl* rzctl);
};

#endif // MOUSE_H
