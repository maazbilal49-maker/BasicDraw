# BasicDraw

A simple drawing program made in **C++** using the **raylib** library.

BasicDraw is a lightweight drawing application that lets you create drawings using circles, change brush colors and sizes, erase parts of your drawing, zoom in and out around the cursor, and save your work.

## Features

* 🖌️ Freehand drawing
* 🎨 Multiple brush colors

  * Black
  * Red
  * Green
  * Blue
  * White
* 📏 Adjustable brush size
* 🧹 Eraser mode
* 🔍 Zoom in/out
* 🎯 Cursor-centered zooming
* 💾 Save drawings as `.basicart`
* 📂 Load `.basicart` files from the command line
* 🖼️ Export drawings as PNG
* ⚡ Built with C++ and raylib

## Controls

| Key / Input           | Action              |
| --------------------- | ------------------- |
| **Left Mouse Button** | Draw / Erase        |
| **N**                 | Change brush color  |
| **Ctrl + Numpad +**   | Increase brush size |
| **Ctrl + Numpad -**   | Decrease brush size |
| **Backspace**         | Toggle eraser       |
| **C**                 | Clear the drawing   |
| **Mouse Wheel Up**    | Zoom in             |
| **Mouse Wheel Down**  | Zoom out            |
| **Ctrl + S**          | Export as PNG       |
| **Alt + S**           | Save as `.basicart` |

## BasicArt Format

BasicDraw uses its own simple file format with the `.basicart` extension.

Each line represents one circle in the drawing:

```text
x y size color
```

For example:

```text
120 250 5 black
125 252 5 black
130 254 5 red
```

This stores the drawing as vector-like circle data instead of storing it as a raster image.

## Loading a Drawing

A `.basicart` file can be loaded by passing its filename as a command-line argument:

```bash
BasicDraw.exe painting.basicart
```

This will load the circles stored in `painting.basicart` when the program starts.

## Building

BasicDraw requires:

* A C++17-compatible compiler
* [raylib](https://www.raylib.com/)
* Make (if using the provided Makefile)

Build the project with:

```bash
make
```

The resulting executable will be:

```text
BasicDraw.exe
```

To remove the compiled files:

```bash
make clean
```

## Technologies

* **C++17**
* **raylib**
* **Make**

## License

This project is currently unlicensed.
