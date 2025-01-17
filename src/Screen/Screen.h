#pragma once
#include <windows.h>
#include <wingdi.h>
#include <iostream>
#include <string>
#include <dwmapi.h> 
#include <thread>
#include <vector>
#include "../Structs/BoundingBox.h"

#pragma comment(lib, "Dwmapi.lib") 

namespace screen {

    void clearScreen();
    void drawBox(int minX, int minY, int maxX, int maxY, float confidence);

    void add_box(BoundingBox box);
    void clear_boxes();

    void set_should_draw(bool value);

    // Function to handle window messages
    LRESULT CALLBACK windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    int run();
    void main_loop();

}