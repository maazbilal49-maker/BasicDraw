#include <raylib.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <filesystem>

#include "utils.h"

bool endsWith(const std::string& str, const std::string& suffix){
    if(suffix.size() > str.size()) return false;
    return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

void changeColor(Color& color, ColorState colorState){
    switch(colorState){
        case COLOR_BLACK: color = BLACK; break;
        case COLOR_RED: color = RED; break;
        case COLOR_GREEN: color = GREEN; break;
        case COLOR_BLUE: color = BLUE; break;
        case COLOR_WHITE: color = WHITE; break;
    }
}

std::string stringifyColor(Color& color){
    if(ColorIsEqual(color, BLACK)) return "black";
    else if(ColorIsEqual(color, RED)) return "red";
    else if(ColorIsEqual(color, GREEN)) return "green";
    else if(ColorIsEqual(color, BLUE)) return "blue";
    else if(ColorIsEqual(color, WHITE)) return "white";

    return "";
}

void eraseAtMousePosition(std::vector<Circle>& circles, float eraserRadius){
    Vector2 mousePos = GetMousePosition();

    circles.erase(std::remove_if(circles.begin(), circles.end(), [mousePos, eraserRadius](const Circle circle){
        return CheckCollisionCircles((Vector2){circle.x, circle.y}, circle.size, mousePos, eraserRadius);
    }), circles.end());
}

Color colorifyString(std::string color){
    if(color == "black") return BLACK;
    else if(color == "red") return RED;
    else if(color == "green") return GREEN;
    else if(color == "blue") return BLUE;
    else if(color == "white") return WHITE;

    return BLACK;
}

//saving to a .basicart file
void saveAsBasicArt(std::vector<Circle>& circles, const char* filename){
    std::ofstream file;
    if(filename == NULL){
        file.open("painting.basicart");
    }else{
        std::string str(filename);
        if(!endsWith(str, ".basicart")){
            file.open(str + ".basicart");
        }else{
            file.open(str);
        }
    }

    for(Circle& circle : circles){
        std::string color = stringifyColor(circle.color);
        file << circle.x << " " << circle.y << " " << circle.size << " " << color << '\n';
    }
}   

//loading basic art file, use const char* to load from command line arguments
std::vector<Circle> loadBasicArtFile(const char *filename){
    std::ifstream file(filename);
    if(!file.is_open()){
        std::cerr << "Could not open file." << '\n';
        return {};
    }

    std::string line;

    std::vector<Circle> result;

    while(std::getline(file, line)){
        std::stringstream iss(line);
        float x, y, size;
        std::string strColor;

        iss >> x >> y >> size;
        iss >> strColor;
        Color color = colorifyString(strColor);

        Circle circle = {x, y, color, size};

        result.push_back(circle);
    }

    return result;
}

void drawCircles(std::vector<Circle>& circles){
    for(const Circle& circle : circles){
        
        DrawCircleV((Vector2){circle.x, circle.y}, circle.size, circle.color);
    }
}

void zoomAtCursor(float zoomFactor, Camera2D& camera, Vector2 mouseScreen)
{
    // World position currently underneath the cursor
    Vector2 mouseWorldBefore = GetScreenToWorld2D(mouseScreen, camera);

    // Zoom
    camera.zoom *= zoomFactor;

    // Find where the cursor maps in world space after zooming
    Vector2 mouseWorldAfter = GetScreenToWorld2D(mouseScreen, camera);

    // Move the camera so the world point stays underneath the cursor
    camera.target.x += mouseWorldBefore.x - mouseWorldAfter.x;
    camera.target.y += mouseWorldBefore.y - mouseWorldAfter.y;
}