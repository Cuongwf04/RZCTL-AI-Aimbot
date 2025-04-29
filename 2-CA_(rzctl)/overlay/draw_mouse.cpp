#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_
#include <winsock2.h>
#include <Windows.h>

#include <shellapi.h>

#include "imgui/imgui.h"
#include "2-CA_(rzctl).h"
#include "include/other_tools.h"

void draw_mouse()
{
    ImGui::SliderInt("DPI", &config.dpi, 400, 5000);
    ImGui::SliderFloat("Sensitivity", &config.sensitivity, 0.1f, 10.0f, "%.1f");
    ImGui::SliderInt("FOV X", &config.fovX, 10, 120);
    ImGui::SliderInt("FOV Y", &config.fovY, 10, 120);
    ImGui::SliderFloat("Min Speed Multiplier", &config.minSpeedMultiplier, 0.1f, 30.0f, "%.1f");
    ImGui::SliderFloat("Max Speed Multiplier", &config.maxSpeedMultiplier, 0.1f, 30.0f, "%.1f");
    ImGui::SliderFloat("Prediction Interval", &config.predictionInterval, 0.00f, 0.10f, "%.2f");
    if (config.predictionInterval == 0.00f)
    {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(255, 0, 0, 255), "-> Disabled");
    }

    ImGui::Separator();

    // No recoil settings
    ImGui::Checkbox("Easy No Recoil", &config.easynorecoil);
    if (config.easynorecoil)
    {
        ImGui::SliderFloat("No Recoil Strength", &config.easynorecoilstrength, 0.1f, 500.0f, "%.1f");
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Left/Right Arrow keys: Adjust recoil strength by 10");
        
        if (config.easynorecoilstrength >= 100.0f)
        {
            ImGui::TextColored(ImVec4(255, 255, 0, 255), "WARNING: High recoil strength may be detected.");
        }
    }

    ImGui::Separator();

    ImGui::Checkbox("Auto Shoot", &config.auto_shoot);
    if (config.auto_shoot)
    {
        ImGui::SliderFloat("bScope Multiplier", &config.bScope_multiplier, 0.5f, 2.0f, "%.1f");
    }

    // INPUT METHODS
    ImGui::Separator();
    std::vector<std::string> input_methods = { "WIN32", "RZCONTROL" };

    int current_method = 0;
    for (int i = 0; i < input_methods.size(); i++)
    {
        if (input_methods[i] == config.input_method)
        {
            current_method = i;
            break;
        }
    }

    if (ImGui::Combo("Input Method", &current_method, [](void* data, int idx, const char** out_text) {
        *out_text = ((std::vector<std::string>*)data)->at(idx).c_str();
        return true;
    }, &input_methods, input_methods.size())) {
        config.input_method = input_methods[current_method];
    }

    if (config.input_method == "RZCONTROL")
    {
        ImGui::TextColored(ImVec4(255, 255, 255, 255), "Using Razer Control for mouse input");
        ImGui::TextColored(ImVec4(255, 0, 0, 255), "Use at your own risk, the method is detected in some games.");
    }
    else
    {
        ImGui::TextColored(ImVec4(255, 255, 255, 255), "Using Windows API for mouse input");
        ImGui::TextColored(ImVec4(255, 0, 0, 255), "Use at your own risk, the method is detected in some games.");
    }
}

void draw_mouse_settings() {
    if (ImGui::CollapsingHeader("Mouse Settings")) {
        // Input method selection
        std::vector<std::string> input_methods = { "WIN32", "RZCONTROL" };
        int current_method = 0;
        for (int i = 0; i < input_methods.size(); i++) {
            if (input_methods[i] == config.input_method) {
                current_method = i;
                break;
            }
        }

        if (ImGui::Combo("Input Method", &current_method, [](void* data, int idx, const char** out_text) {
            *out_text = ((std::vector<std::string>*)data)->at(idx).c_str();
            return true;
        }, &input_methods, input_methods.size())) {
            config.input_method = input_methods[current_method];
        }

        // Mouse settings
        if (ImGui::SliderInt("DPI", &config.dpi, 100, 16000)) {
            // Update DPI
        }
        if (ImGui::SliderFloat("Sensitivity", &config.sensitivity, 0.1f, 10.0f)) {
            // Update sensitivity
        }
        if (ImGui::SliderInt("FOV X", &config.fovX, 1, 100)) {
            // Update FOV X
        }
        if (ImGui::SliderInt("FOV Y", &config.fovY, 1, 100)) {
            // Update FOV Y
        }
        if (ImGui::SliderFloat("Min Speed Multiplier", &config.minSpeedMultiplier, 0.1f, 10.0f)) {
            // Update min speed multiplier
        }
        if (ImGui::SliderFloat("Max Speed Multiplier", &config.maxSpeedMultiplier, 0.1f, 10.0f)) {
            // Update max speed multiplier
        }
        if (ImGui::SliderFloat("Prediction Interval", &config.predictionInterval, 0.01f, 1.0f)) {
            // Update prediction interval
        }
        if (ImGui::Checkbox("Easy No Recoil", &config.easynorecoil)) {
            // Update easy no recoil
        }
        if (ImGui::SliderFloat("Easy No Recoil Strength", &config.easynorecoilstrength, 0.0f, 10.0f)) {
            // Update easy no recoil strength
        }
    }
}