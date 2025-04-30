#ifndef MOUSE_H
#define MOUSE_H

#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_
#include <winsock2.h>
#include <Windows.h>

#include <mutex>
#include <atomic>
#include <chrono>
#include <vector>
#include <utility>
#include <queue>
#include <thread>
#include <condition_variable>

#include "AimbotTarget.h"
#include "rzctl.h"

class MouseThread
{
private:
    double screen_width;
    double screen_height;
    double dpi;
    double prediction_interval;
    double mouse_sensitivity;
    double fov_x;
    double fov_y;
    double max_distance;
    double min_speed_multiplier;
    double max_speed_multiplier;
    double center_x;
    double center_y;
    bool   auto_shoot;
    float  bScope_multiplier;

    double prev_x, prev_y;
    double prev_velocity_x, prev_velocity_y;
    std::chrono::time_point<std::chrono::steady_clock> prev_time;
    std::chrono::steady_clock::time_point last_target_time;
    std::atomic<bool> target_detected{ false };
    std::atomic<bool> mouse_pressed{ false };

    void sendMovementToDriver(int dx, int dy);

    struct Move { int dx; int dy; };

    std::queue<Move>              moveQueue;
    std::mutex                    queueMtx;
    std::condition_variable       queueCv;
    const size_t                  queueLimit = 5;
    std::thread                   moveWorker;
    std::atomic<bool>             workerStop{ false };

    std::vector<std::pair<double, double>> futurePositions;
    std::mutex                    futurePositionsMutex;

    void moveWorkerLoop();
    void queueMove(int dx, int dy);

    bool   wind_mouse_enabled = true;
    double wind_G, wind_W, wind_M, wind_D;
    void   windMouseMoveRelative(int dx, int dy);

    std::pair<double, double> calc_movement(double target_x, double target_y);
    double calculate_speed_multiplier(double distance);

public:
    std::mutex input_method_mutex;

    MouseThread(
        int  resolution,
        int  dpi,
        double sensitivity,
        int  fovX,
        int  fovY,
        double minSpeedMultiplier,
        double maxSpeedMultiplier,
        double predictionInterval,
        bool auto_shoot,
        float bScope_multiplier
    );
    ~MouseThread();

    void updateConfig(int resolution, double dpi, double sensitivity, int fovX, int fovY,
        double minSpeedMultiplier, double maxSpeedMultiplier,
        double predictionInterval, bool auto_shoot, float bScope_multiplier);

    void moveMousePivot(double pivotX, double pivotY);
    std::pair<double, double> predict_target_position(double target_x, double target_y);
    void moveMouse(const AimbotTarget& target);
    void pressMouse(const AimbotTarget& target);
    void releaseMouse();
    void resetPrediction();
    void checkAndResetPredictions();
    bool check_target_in_scope(double target_x, double target_y,
        double target_w, double target_h, double reduction_factor);

    std::vector<std::pair<double, double>> predictFuturePositions(double pivotX, double pivotY, int frames);
    void storeFuturePositions(const std::vector<std::pair<double, double>>& positions);
    void clearFuturePositions();
    std::vector<std::pair<double, double>> getFuturePositions();

    // New getter/setter methods
    void setTargetDetected(bool detected) { target_detected.store(detected); }
    bool getTargetDetected() const { return target_detected.load(); }
    
    void setLastTargetTime(const std::chrono::steady_clock::time_point& t) { last_target_time = t; }
    std::chrono::steady_clock::time_point getLastTargetTime() const { return last_target_time; }
    
    void setMousePressed(bool pressed) { mouse_pressed.store(pressed); }
    bool getMousePressed() const { return mouse_pressed.load(); }
    
    void setWindMouseEnabled(bool enabled) { wind_mouse_enabled = enabled; }
    bool getWindMouseEnabled() const { return wind_mouse_enabled; }
    
    void setWindMouseParams(double G, double W, double M, double D) {
        wind_G = G;
        wind_W = W;
        wind_M = M;
        wind_D = D;
    }
    
    std::tuple<double, double, double, double> getWindMouseParams() const {
        return {wind_G, wind_W, wind_M, wind_D};
    }
    
    void setRZControl(RZControl* newRZControl) {
        std::lock_guard<std::mutex> lock(input_method_mutex);
        rzctl = newRZControl;
    }
    
    RZControl* getRZControl() const {
        std::lock_guard<std::mutex> lock(input_method_mutex);
        return rzctl;
    }
};

#endif // MOUSE_H
