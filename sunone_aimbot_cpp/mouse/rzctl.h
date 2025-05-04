#ifndef RZCTL_H
#define RZCTL_H

#include <windows.h>
#include <string>
#include <vector>
#include <utility> // For std::pair

/**
 * @brief Enum class for mouse click events
 */
enum class MouseClick {
    LEFT_DOWN = 1,      ///< Left mouse button down
    LEFT_UP = 2,        ///< Left mouse button up
    RIGHT_DOWN = 4,     ///< Right mouse button down
    RIGHT_UP = 8,       ///< Right mouse button up
    SCROLL_CLICK_DOWN = 16,  ///< Middle mouse button down
    SCROLL_CLICK_UP = 32,    ///< Middle mouse button up
    BACK_DOWN = 64,     ///< Back button down
    BACK_UP = 128,      ///< Back button up
    FORWARD_DOWN = 256, ///< Forward button down
    FORWARD_UP = 512,   ///< Forward button up
    SCROLL_DOWN = 0x1000,  ///< Mouse wheel down
    SCROLL_UP = 0x2000    ///< Mouse wheel up
};

/**
 * @brief Enum class for keyboard input types
 */
enum class KeyboardInputType {
    KEYBOARD_DOWN = 0,  ///< Key press
    KEYBOARD_UP = 1     ///< Key release
};

/**
 * @brief Class for controlling Razer devices through rzctl.dll
 */
class RZControl {
private:
    HINSTANCE dllHandle;  ///< Handle to the loaded DLL

    // Define function pointer types
    using InitFunc = BOOL (*)();
    using MouseMoveFunc = void (*)(int, int, BOOL);
    using MouseClickFunc = void (*)(int);
    using KeyboardInputFunc = void (*)(SHORT, int);

    // Store function pointers
    InitFunc init;              ///< Pointer to init() function
    MouseMoveFunc mouse_move;   ///< Pointer to mouse_move() function
    MouseClickFunc mouse_click; ///< Pointer to mouse_click() function
    KeyboardInputFunc keyboard_input; ///< Pointer to keyboard_input() function

    /**
     * @brief Loads function pointers from the DLL
     * @throws std::runtime_error if any function pointer cannot be loaded
     */
    void setupFunctions();

public:
    /**
     * @brief Constructor that loads the DLL and function pointers
     * @param dll_path Path to the rzctl.dll file
     * @throws std::runtime_error if DLL cannot be loaded
     */
    explicit RZControl(const std::wstring& dll_path);

    /**
     * @brief Destructor that frees the DLL
     */
    ~RZControl();

    // Disable copy constructor and assignment operator
    RZControl(const RZControl&) = delete;
    RZControl& operator=(const RZControl&) = delete;

    /**
     * @brief Initialize the RZCONTROL device
     * @return true if initialization successful, false otherwise
     * @throws std::runtime_error if initialization fails
     */
    bool initialize();

    /**
     * @brief Move the mouse cursor
     * @param x X coordinate or relative movement
     * @param y Y coordinate or relative movement
     * @param from_start_point If true, coordinates are absolute; if false, movement is relative
     * @throws std::invalid_argument if relative movement is 0,0
     */
    void moveMouse(int x, int y, bool from_start_point);

    /**
     * @brief Process multiple mouse movement instructions
     * @param instructions Vector of x,y coordinate pairs
     * @param from_start_point If true, coordinates are absolute; if false, movement is relative
     * @param delay_ms Delay between movements in milliseconds
     */
    void processMoveInstructions(const std::vector<std::pair<int, int>>& instructions,
                               bool from_start_point = false,
                               DWORD delay_ms = 0);

    /**
     * @brief Send keyboard input
     * @param scan_code The scan code of the key
     * @param up_down Whether the key is being pressed or released
     */
    void sendKeyboardInput(short scan_code, KeyboardInputType up_down);

    /**
     * @brief Send mouse click event
     * @param click_type The type of click event to send
     */
    void mouseClick(MouseClick click_type);
};

#endif // RZCTL_H
