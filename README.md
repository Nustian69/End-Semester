Multiplayer Snake Game

Description:

This project is a grid-based multiplayer Snake game designed for competitive play. Players control snakes on a shared game board, trying to grow their length by consuming food while avoiding collisions with walls, other snakes, or themselves. The game supports multiple players and tracks individual scores.

Features:

Multiplayer Gameplay: Multiple players can control their own snakes on the same board.

Grid-Based Movement: Smooth, step-based movement for precise control.

Score Tracking: Each player has a unique score that increases with each food consumed.

Collision Detection: The game ends when a snake collides with a wall, itself, or another snake.

Requirements:

C++ Compiler: A modern C++ compiler that supports C++11 or higher.

SFML Library: Version 2.5 or newer.

Font File: Ensure you have the font file LuckiestGuy-Regular.ttf in the same directory as the executable.

Controls:

Single Player Mode

Arrow keys: Control Snake 1’s movement

Multiplayer Mode

Arrow keys: Control Snake 1’s movement

W/A/S/D: Control Snake 2’s movement

General:

Escape: Pause/Resume the game

Enter: Confirm selection in menus

Up/Down: Navigate menu options

Game Rules:

Snakes must avoid colliding with themselves, each other, or the walls.

Food grants 10 points and makes the snake grow.

Losing all three lives ends the game.

In multiplayer mode, the snake with the higher score is declared the winner.

File Structure:

main.cpp: Contains the game logic, rendering, and input handling.

LuckiestGuy-Regular.ttf: Font file for displaying text.

Customization:

Grid Size: Change CELL_SIZE, GRID_WIDTH, or GRID_HEIGHT constants in main.cpp.

Snake Colors: Modify the COLORS array for custom snake colors.

Game Speed: Adjust window.setFramerateLimit(10) in the main() function.

Known Issues:

None reported as of now. Please feel free to report issues via GitHub.
