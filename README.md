# BasicDraw

BasicDraw is a lightweight C++ drawing application built with raylib. It lets you sketch freeform circles, create rectangle shapes, change colors, erase sections of the drawing, zoom around the cursor, and save or export your work.

## Overview

The program opens a 800x600 window and uses a camera system so you can draw on a larger virtual canvas while zooming in and out around the mouse. It stores shapes as vector-like data rather than as a flat bitmap, which makes it easy to save and reload artwork as `.basicart` files.

## Features

- 🖌️ Draw circles with the left mouse button
- ▭ Draw rectangles by toggling the brush mode
- 🎨 Cycle through the available colors:
  - Black
  - Red
  - Green
  - Blue
  - White
- 📏 Adjust brush size using Ctrl + Numpad + / Ctrl + Numpad -
- 🧹 Toggle eraser mode with Backspace
- 🗑️ Clear the current drawing with C
- 🔍 Scroll-wheel zoom around the cursor position
- 💾 Save drawings as `.basicart`
- 📂 Load a `.basicart` file from the command line on startup
- 🖼️ Export the current canvas as a PNG image
- ⚡ Built in C++17 using raylib

## Controls

| Input | Action |
| --- | --- |
| Left Mouse Button | Draw the current shape or erase while eraser mode is enabled |
| N | Cycle to the next brush color |
| B | Toggle brush mode between circle and rectangle |
| Ctrl + Numpad + | Increase brush size |
| Ctrl + Numpad - | Decrease brush size |
| Backspace | Toggle eraser mode on/off |
| C | Clear the drawing canvas |
| Mouse Wheel Up | Zoom in at the cursor |
| Mouse Wheel Down | Zoom out at the cursor |
| Ctrl + S | Export the screen as `painting.png` |
| Alt + S | Save the current drawing as a `.basicart` file |

## Drawing Behavior

### Brush modes

The app supports two drawing tools:

- Circle brush: creates circular strokes at the mouse position
- Rectangle brush: creates square/rectangular strokes at the mouse position

The indicator preview is drawn under the cursor in the active tool color, so you can see the next shape before you place it.

### Eraser mode

When eraser mode is active, the left mouse button removes existing shape data near the cursor. The eraser uses a circular region based on the current brush size.

### Zoom behavior

Zooming is centered on the mouse cursor, so the point under the pointer stays in place while the camera scales around it.

## File Format

The saved `.basicart` format stores a sequence of shape records, one per line.

### Circle entry

```text
c x y radius color
```

Example:

```text
c 120 250 5 black
c 125 252 5 black
c 130 254 5 red
```

### Rectangle entry

```text
r x y width height color
```

Example:

```text
r 100 100 18 18 blue
r 150 120 30 20 green
```

The loader accepts both circle and rectangle records and ignores blank lines. Unknown shape types are reported to the console.

## Loading a Drawing

Pass a `.basicart` filename as the first command-line argument:

```bash
BasicDraw.exe painting.basicart
```

If the file exists, it is loaded when the program starts. The app only accepts `.basicart` files when using the command-line loader; other file types are rejected.

If you save with Alt + S and provide a filename without an extension, the app appends `.basicart` automatically. If no filename is set, it saves to `painting.basicart` by default.

## Exporting Images

Hold Ctrl + S to export the current window contents to a PNG image file named:

```text
painting.png
```

The export is created in the current working directory.

## Build Instructions

### Requirements

- C++17-compatible compiler
- [raylib](https://www.raylib.com/)
- Make

### Build

From the project directory, run:

```bash
make
```

This produces:

```text
BasicDraw.exe
```

### Clean

To remove built artifacts:

```bash
make clean
```

## Project Notes

- The project uses a fixed window size of 800x600.
- The target FPS is set to 120.
- The build system uses the Windows-specific raylib libraries required for OpenGL, GDI, and Windows multimedia support.

## Technologies

- C++17
- raylib
- Make

## License

This project is currently unlicensed.
