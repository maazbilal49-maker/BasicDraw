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

#define WIN_W 800
#define WIN_H 600

int main(int argc, char **argv){
    InitWindow((float)WIN_W, (float)WIN_H, "Raylib drawing simulator");

    bool erasing = false;
    char* filename = NULL;
    BrushState brushState = STATE_CIRCLE;
    SetTargetFPS(120);

    int brushSize = 1.0f;

    Drawing drawing;

    Camera2D camera = {0};
    camera.target = (Vector2){WIN_W/2.0f, WIN_H/2.0f};
    camera.offset = (Vector2){WIN_W/2.0f, WIN_H/2.0f};
    camera.zoom = 1.0f;

    if(argc > 1){
        filename = argv[1];
        std::string str(filename);
        std::filesystem::path path = filename;
        if(endsWith(str, ".basicart")){
            drawing = loadBasicArtFile(filename);
        }else if(path.has_extension()){
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
            colorState = static_cast<ColorState>((colorState + 1) % COLOR_COUNT);
            changeColor(currentColor, colorState);
        }

        if(IsKeyPressed(KEY_B)){
            switchBrushState(brushState);
        }

        if((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyDown(KEY_KP_ADD)){
            brushSize = std::min(brushSize + 1, 100);
        } 

        if((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyDown(KEY_KP_SUBTRACT)){
            brushSize = std::max(1, brushSize - 1);
        }

        if(IsKeyDown(KEY_C)){
            drawing.circles.clear();
            drawing.rectangles.clear();
        }

        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            if(erasing){
                eraseAtMousePosition(drawing, brushSize);
            }   

            else{
                Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), camera);
                if(brushState == STATE_CIRCLE){
                    drawing.circles.push_back(CircleBrush{
                        mouseWorld.x,
                        mouseWorld.y,
                        currentColor,
                        (float)brushSize
                    });
                }
                else if(brushState == STATE_RECTANGLE){

                    drawing.rectangles.push_back(RectangleBrush{
                        mouseWorld.x,
                        mouseWorld.y,
                        currentColor,
                        (float)brushSize,
                        (float)brushSize
                    });
                }
            }
            
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


                if(!erasing){
                    if(brushState == STATE_CIRCLE){
                        DrawCircleV(GetScreenToWorld2D(mousePos, camera),
                                brushSize,
                                currentColor);
                    }
                    else if(brushState == STATE_RECTANGLE){
                        DrawRectangleV(GetScreenToWorld2D(mousePos, camera),
                                (Vector2){brushSize, brushSize},
                                currentColor);
                    }
                }
                else{
                    DrawRectangleLinesEx(Rectangle{GetScreenToWorld2D(mousePos, camera).x, GetScreenToWorld2D(mousePos, camera).y, brushSize, brushSize},
                                        1.0f,
                                        BLACK);
                }
                drawDrawing(drawing);

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
            saveAsBasicArt(drawing, filename);
        }
    }

    CloseWindow();

    return 0;
}