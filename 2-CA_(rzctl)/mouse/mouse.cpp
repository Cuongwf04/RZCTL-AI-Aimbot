#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_
#include <winsock2.h>
#include <Windows.h>

#include <cmath>
#include <algorithm>
#include <chrono>
#include <mutex>
#include <atomic>
#include <vector>

#include "mouse.h"
#include "capture.h"
#include "rzctl.h"

MouseThread::MouseThread(
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
) : detection_resolution(detection_resolution),
    dpi(dpi),
    sensitivity(sensitivity),
    fovX(fovX),
    fovY(fovY),
    minSpeedMultiplier(minSpeedMultiplier),
    maxSpeedMultiplier(maxSpeedMultiplier),
    predictionInterval(predictionInterval),
    auto_shoot(auto_shoot),
    bScope_multiplier(bScope_multiplier),
    rzctl(rzctl),
    targetDetected(false)
{
}

void MouseThread::updateConfig(
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
) {
    this->detection_resolution = detection_resolution;
    this->dpi = dpi;
    this->sensitivity = sensitivity;
    this->fovX = fovX;
    this->fovY = fovY;
    this->minSpeedMultiplier = minSpeedMultiplier;
    this->maxSpeedMultiplier = maxSpeedMultiplier;
    this->predictionInterval = predictionInterval;
    this->auto_shoot = auto_shoot;
    this->bScope_multiplier = bScope_multiplier;
}

void MouseThread::moveMousePivot(int x, int y) {
    if (!rzctl) return;

    // Calculate center of screen
    int centerX = detection_resolution / 2;
    int centerY = detection_resolution / 2;

    // Calculate movement needed
    int moveX = x - centerX;
    int moveY = y - centerY;

    // Apply sensitivity
    moveX = static_cast<int>(moveX * sensitivity);
    moveY = static_cast<int>(moveY * sensitivity);

    // Apply FOV scaling
    moveX = static_cast<int>(moveX * (fovX / 100.0f));
    moveY = static_cast<int>(moveY * (fovY / 100.0f));

    // Apply DPI scaling
    moveX = static_cast<int>(moveX * (dpi / 800.0f));
    moveY = static_cast<int>(moveY * (dpi / 800.0f));

    // Move mouse using RZControl
    rzctl->moveMouse(moveX, moveY, false);
}

void MouseThread::pressMouse(const AimbotTarget& target) {
    if (!rzctl) return;
    rzctl->mouseClick(MouseClick::LEFT_DOWN);
}

void MouseThread::releaseMouse() {
    if (!rzctl) return;
    rzctl->mouseClick(MouseClick::LEFT_UP);
}

std::vector<std::pair<int, int>> MouseThread::predictFuturePositions(int x, int y, int steps) {
    std::vector<std::pair<int, int>> positions;
    positions.reserve(steps);

    // Simple linear prediction
    for (int i = 0; i < steps; i++) {
        positions.emplace_back(x, y);
    }

    return positions;
}

void MouseThread::setLastTargetTime(std::chrono::steady_clock::time_point time) {
    lastTargetTime = time;
}

void MouseThread::setTargetDetected(bool detected) {
    targetDetected.store(detected);
}

void MouseThread::storeFuturePositions(const std::vector<std::pair<int, int>>& positions) {
    std::lock_guard<std::mutex> lock(futurePositionsMutex);
    futurePositions.clear();
    futurePositions.insert(futurePositions.end(), positions.begin(), positions.end());
}

void MouseThread::clearFuturePositions() {
    std::lock_guard<std::mutex> lock(futurePositionsMutex);
    futurePositions.clear();
}

void MouseThread::checkAndResetPredictions() {
    if (!targetDetected.load()) {
        clearFuturePositions();
    }
}

void MouseThread::setRZControl(RZControl* newRzctl) {
    std::lock_guard<std::mutex> lock(input_method_mutex);
    rzctl = newRzctl;
}