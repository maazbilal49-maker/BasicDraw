#include <raylib.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <filesystem>

#define WIN_W 800
#define WIN_H 600

typedef struct{
    float x, y;
    Color color;
    float size;
}Circle;

typedef enum{
    COLOR_BLACK,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_WHITE,
    COUNT
}ColorState;

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

int main(int argc, char **argv){
    InitWindow((float)WIN_W, (float)WIN_H, "Raylib drawing simulator");

    bool erasing = false;
    char* filename = NULL;

    SetTargetFPS(120);

    int brushSize = 1.0f;

    std::vector<Circle> circles;

    Camera2D camera = {0};
    camera.target = (Vector2){WIN_W/2.0f, WIN_H/2.0f};
    camera.offset = (Vector2){WIN_W/2.0f, WIN_H/2.0f};
    camera.zoom = 1.0f;

    if(argc > 1){
        filename = argv[1];
        std::string str(filename);
        std::filesystem::path path = filename;
        if(endsWith(str, ".basicart")){
            circles = loadBasicArtFile(filename);
        }else{
            std::cout << "Can only load .basicart files." << '\n';
            return 1;
        }
    }

    Color currentColor = BLACK;
    ColorState colorState = COLOR_BLACK;

    while(!WindowShouldClose()){
        Vector2 mousePos = GetMousePosition();
        Vector2 cursorBrush = {mousePos.x, mousePos.y};
        ClearBackground(RAYWHITE);

        float scroll = GetMouseWheelMove();
        if(scroll > 0){
            zoomAtCursor(1.1f, camera, mousePos);
        }
        else if(scroll < 0){
            zoomAtCursor(0.9f, camera, mousePos);
        }

        if(IsKeyPressed(KEY_N)){
            colorState = static_cast<ColorState>((colorState + 1) % COUNT);
            changeColor(currentColor, colorState);
        }

        if((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyDown(KEY_KP_ADD)){
            brushSize = std::min(brushSize + 1, 100);
        }

        if((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyDown(KEY_KP_SUBTRACT)){
            brushSize = std::max(1, brushSize - 1);
        }

        if(IsKeyDown(KEY_C)){
            circles.clear();
        }

        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){   
            Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), camera);

            circles.push_back(Circle{
                mouseWorld.x,
                mouseWorld.y,
                currentColor,
                (float)brushSize
            });
        }

        if(IsKeyPressed(KEY_BACKSPACE)){
            if(erasing){
                erasing = false;
            }
            else{erasing = true;}
        }
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode2D(camera);

                drawCircles(circles);

                if(!erasing)
                    DrawCircleV(GetScreenToWorld2D(mousePos, camera),
                                brushSize,
                                currentColor);

            EndMode2D();

            DrawText(TextFormat("Brush Size: %d", brushSize),
                    20, 50, 20, BLACK);

        EndDrawing();
        if((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyDown(KEY_S)){
            Image image = LoadImageFromScreen();
            ExportImage(image, "painting.png");
            UnloadImage(image);
        }

        if((IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)) && IsKeyDown(KEY_S)){
            saveAsBasicArt(circles, filename);
        }
    }

    CloseWindow();

    return 0;
}