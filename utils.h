
#ifndef UTILS_H
#define UTILS_H

typedef struct{
    float x, y;
    Color color;
    float size;
}CircleBrush;

typedef struct{
    float x, y;
    Color color;
    float width, height;
}RectangleBrush;

typedef struct{
    std::vector<CircleBrush> circles;
    std::vector<RectangleBrush> rectangles;
}Drawing;

typedef enum{
    COLOR_BLACK,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_YELLOW,
    COLOR_CYAN,
    COLOR_MAGENTA,
    COLOR_ORANGE,
    COLOR_PURPLE,
    COLOR_PINK,
    COLOR_BROWN,
    COLOR_GRAY,
    COLOR_LIGHTGRAY,
    COLOR_DARKGRAY,
    COLOR_GOLD,
    COLOR_SILVER,
    COLOR_NAVY,
    COLOR_TEAL,
    COLOR_OLIVE,
    COLOR_MAROON,
    COLOR_WHITE,
    COLOR_COUNT
}ColorState;

typedef enum{
    STATE_CIRCLE,
    STATE_RECTANGLE,
    BRUSH_COUNT
}BrushState;

bool endsWith(const std::string& str, const std::string& suffix);
void changeColor(Color& color, ColorState colorState);
void switchBrushState(BrushState& brushState);
std::string stringifyColor(Color& color);
void eraseAtMousePosition(Drawing& drawing, float eraserRadius);
Color colorifyString(std::string color);
void saveAsBasicArt(Drawing& drawing, const char* filename);
Drawing loadBasicArtFile(const char *filename);
void drawDrawing(Drawing& drawing);
void zoomAtCursor(float zoomFactor, Camera2D& camera, Vector2 mouseScreen);

#endif