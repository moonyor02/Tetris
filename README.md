
Hi there is a Free time project driven by curiosity from me.😅

# Terminal Tetris (C++)

A lightweight, modular Tetris implementation built for the terminal. This project was developed as a deep-dive into C++ systems programming, focusing on architectural separation, real-time input handling, and terminal-based rendering, game loops, state management, and collision detection.

---

## 🚀 The Technical Challenge: UI & UX
While the core logic (gravity and collisions) was straightforward to implement, the **Terminal UI** was a significant hurdle involving extensive trial-and-error:
* **Flicker-Free Rendering:** Used ANSI escape sequences (`\x1B[H`) to manage screen refreshes without the jarring flicker of system-wide clear commands.
* **Coordinate Mapping:** Developed manual "Global-to-Local" 4x4 grid translations to handle piece rotations against board boundaries.
* **Visual Haptics:** Built a "Keyboard Glow" UI that provides real-time visual feedback in the terminal, ensuring the game feels responsive despite terminal limitations.

## 🛠️ Project Evolution
I have intentionally preserved the history of this project to demonstrate architectural growth:

1. **Phase 1: The Monolith** (Found in `/archive`)
   - A single 400+ line implementation focusing on the "Mathematics of Tetris."
   - Includes original "Nintendo-style" scoring and manual gravity timers.
2. **Phase 2: Modular Refactor** (Current `/tetris_Game_Project_(terminal)`)
   - Split into specialized modules (**Engine**, **Renderer**, **Input**, and **State**).
   - Follows "Single Responsibility" principles for easier debugging and scaling.


## 🛠 Technical Highlights

Unlike a monolithic script, this version is built as a mini-engine to explore how different game systems communicate.

* **Modular Architecture:** Logic is strictly separated into specialized modules (`Engine`, `Physics`, `Renderer`, `Input`, and `State`).
* **Asynchronous Input:** Implements non-blocking keyboard detection (via `kbhit`) to allow the game loop to run independently of user keystrokes.
* **ANSI Graphics:** Uses direct ANSI escape sequences to manage terminal coordinates, colors, and flickering, providing a classic "retro" feel.
* **Global State Sync:** Employs a centralized state management system to coordinate "Game Over" events across all modules.

## 📂 Project Structure

```text
.
├── src/        # Implementation logic (.cpp)
├── include/    # Blueprints and Global State (.h)
├── Makefile    # Automated build system
└── README.md   # Project documentation

## How to Compile and Run

1. Open your terminal and navigate to the project folder:
   ```bash
   cd tetris\ Game\ Project\ \(terminal\)/

   make run
