
#ifndef UTILS_H
#define UTILS_H

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

bool endsWith(const std::string& str, const std::string& suffix);
void changeColor(Color& color, ColorState colorState);
std::string stringifyColor(Color& color);
void eraseAtMousePosition(std::vector<Circle>& circles, float eraserRadius);
Color colorifyString(std::string color);
void saveAsBasicArt(std::vector<Circle>& circles, const char* filename);
std::vector<Circle> loadBasicArtFile(const char *filename);
void drawCircles(std::vector<Circle>& circles);
void zoomAtCursor(float zoomFactor, Camera2D& camera, Vector2 mouseScreen);

#endif