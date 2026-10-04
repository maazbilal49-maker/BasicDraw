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
        case COLOR_YELLOW: color = YELLOW; break;
        case COLOR_CYAN: color = Color{0, 255, 255, 255}; break;
        case COLOR_MAGENTA: color = MAGENTA; break;
        case COLOR_ORANGE: color = ORANGE; break;
        case COLOR_PURPLE: color = PURPLE; break;
        case COLOR_PINK: color = PINK; break;
        case COLOR_BROWN: color = BROWN; break;
        case COLOR_GRAY: color = GRAY; break;
        case COLOR_LIGHTGRAY: color = LIGHTGRAY; break;
        case COLOR_DARKGRAY: color = DARKGRAY; break;
        case COLOR_GOLD: color = GOLD; break;
        case COLOR_SILVER: color = Color{192, 192, 192, 255}; break;
        case COLOR_NAVY: color = Color{0, 0, 128, 255}; break;
        case COLOR_TEAL: color = Color{0, 128, 128, 255}; break;
        case COLOR_OLIVE: color = Color{128, 128, 0, 255}; break;
        case COLOR_MAROON: color = MAROON; break;
        default: color = BLACK; break;
    }
}

void switchBrushState(BrushState& brushState){
    brushState = static_cast<BrushState>((brushState + 1) % BRUSH_COUNT);
}

std::string stringifyColor(Color& color){
    if(ColorIsEqual(color, BLACK)) return "black";
    else if(ColorIsEqual(color, RED)) return "red";
    else if(ColorIsEqual(color, GREEN)) return "green";
    else if(ColorIsEqual(color, BLUE)) return "blue";
    else if(ColorIsEqual(color, WHITE)) return "white";
    else if(ColorIsEqual(color, YELLOW)) return "yellow";
    else if(ColorIsEqual(color, Color{0, 255, 255, 255})) return "cyan";
    else if(ColorIsEqual(color, MAGENTA)) return "magenta";
    else if(ColorIsEqual(color, ORANGE)) return "orange";
    else if(ColorIsEqual(color, PURPLE)) return "purple";
    else if(ColorIsEqual(color, PINK)) return "pink";
    else if(ColorIsEqual(color, BROWN)) return "brown";
    else if(ColorIsEqual(color, GRAY)) return "gray";
    else if(ColorIsEqual(color, LIGHTGRAY)) return "lightgray";
    else if(ColorIsEqual(color, DARKGRAY)) return "darkgray";
    else if(ColorIsEqual(color, GOLD)) return "gold";
    else if(ColorIsEqual(color, Color{192, 192, 192, 255})) return "silver";
    else if(ColorIsEqual(color, Color{0, 0, 128, 255})) return "navy";
    else if(ColorIsEqual(color, Color{0, 128, 128, 255})) return "teal";
    else if(ColorIsEqual(color, Color{128, 128, 0, 255})) return "olive";
    else if(ColorIsEqual(color, MAROON)) return "maroon";

    return "";
}

void eraseAtMousePosition(Drawing& drawing, float eraserRadius){
    Vector2 mousePos = GetMousePosition();

    drawing.circles.erase(std::remove_if(drawing.circles.begin(), drawing.circles.end(), [mousePos, eraserRadius](const CircleBrush circle){
        return CheckCollisionCircles((Vector2){circle.x, circle.y}, circle.size, mousePos, eraserRadius);
    }), drawing.circles.end());

    drawing.rectangles.erase(std::remove_if(drawing.rectangles.begin(), drawing.rectangles.end(), [mousePos, eraserRadius](const RectangleBrush rectangle){
        return CheckCollisionCircleRec(mousePos, eraserRadius, (Rectangle){rectangle.x, rectangle.y, rectangle.width, rectangle.height});
    }), drawing.rectangles.end());
}

Color colorifyString(std::string color){
    if(color == "black") return BLACK;
    else if(color == "red") return RED;
    else if(color == "green") return GREEN;
    else if(color == "blue") return BLUE;
    else if(color == "white") return WHITE;
    else if(color == "yellow") return YELLOW;
    else if(color == "cyan") return Color{0, 255, 255, 255};
    else if(color == "magenta") return MAGENTA;
    else if(color == "orange") return ORANGE;
    else if(color == "purple") return PURPLE;
    else if(color == "pink") return PINK;
    else if(color == "brown") return BROWN;
    else if(color == "gray") return GRAY;
    else if(color == "lightgray") return LIGHTGRAY;
    else if(color == "darkgray") return DARKGRAY;
    else if(color == "gold") return GOLD;
    else if(color == "silver") return Color{192, 192, 192, 255};
    else if(color == "navy") return Color{0, 0, 128, 255};
    else if(color == "teal") return Color{0, 128, 128, 255};
    else if(color == "olive") return Color{128, 128, 0, 255};
    else if(color == "maroon") return MAROON;
    else{
        std::cerr << "Unknown color: " << color << '\n';
        return BLACK;
    }

    return BLACK;
}

//saving to a .basicart file
void saveAsBasicArt(Drawing& drawing, const char* filename){
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

    for(CircleBrush& circle : drawing.circles){
        std::string color = stringifyColor(circle.color);
        file << "c " << circle.x << " " << circle.y << " " << circle.size << " " << color << '\n';
    }
    for(RectangleBrush& rectangle : drawing.rectangles){
        std::string color = stringifyColor(rectangle.color);
        file << "r " << rectangle.x << " " << rectangle.y << " " << rectangle.width << " " << rectangle.height << " " << color << '\n';
    }
}   

//loading basic art file, use const char* to load from command line arguments
Drawing loadBasicArtFile(const char *filename){
    std::ifstream file(filename);
    if(!file.is_open()){
        std::cerr << "Could not open file." << '\n';
        return {};
    }

    std::string line;

    Drawing result;

    char shapeType;

    while(std::getline(file, line)){
        if(line.empty()){
            continue;
        }

        std::stringstream iss(line);

        iss >> shapeType;

        if(shapeType == 'c'){
            float x, y, size;
            std::string strColor;

            iss >> x >> y >> size >> strColor;
            if(iss.fail()){
                std::cerr << "Error reading line: " << line << '\n';
                continue;
            }

            Color color = colorifyString(strColor);

            CircleBrush circle = {x, y, color, size};
            result.circles.push_back(circle);
        }
        else if(shapeType == 'r'){
            float x, y, width, height;
            std::string strColor;

            iss >> x >> y >> width >> height >> strColor;
            if(iss.fail()){
                std::cerr << "Error reading line: " << line << '\n';
                continue;
            }

            Color color = colorifyString(strColor);

            RectangleBrush rectangle = {
                x, y, color, width, height
            };

            result.rectangles.push_back(rectangle);
        }
        else{
            std::cerr << "Unknown shape type in line: " << line << '\n';
        }
    }

    return result;
}

void drawDrawing(Drawing& drawing){
    for(const CircleBrush& circle : drawing.circles){
        
        DrawCircleV((Vector2){circle.x, circle.y}, circle.size, circle.color);
    }
    for(const RectangleBrush& rectangle : drawing.rectangles){
        
        DrawRectangleV((Vector2){rectangle.x, rectangle.y}, (Vector2){rectangle.width, rectangle.height}, rectangle.color);
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