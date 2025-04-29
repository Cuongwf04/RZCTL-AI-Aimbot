#ifndef CA_RZCTL_H
#define CA_RZCTL_H

#include "config.h"
#include "detector.h"
#include "directml_detector.h"
#include "mouse.h"
#include "rzctl.h"
#include "optical_flow.h"

extern Config config;
extern Detector detector;
extern DirectMLDetector* dml_detector;
extern MouseThread* globalMouseThread;
extern RZControl* rzctl;
extern std::atomic<bool> input_method_changed;
extern OpticalFlow opticalFlow;
extern std::atomic<bool> aiming;
extern std::atomic<bool> shooting;
extern std::atomic<bool> zooming;

#endif // CA_RZCTL_H