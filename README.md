# Voxel Game

A basic voxel game/minecraft clone I'm making in C++ and OpenGL.
This is a rewrite of my old [OpenGLVoxelGame](https://github.com/Werlog/OpenGLVoxelGame).

## Controls

WASD - Movement

Space - Jumping

Left Click - Destroy block

Right Click - Place block

Mouse Scroll - Swap between blocks

Right Shift - Open Dev Menu (Allows to toggle flight/collisions)

## Current Features

- Procedural World Generation
	- Infinite world generation
	- Trees
- Block Placing and Breaking
- Main menu
- Clouds
- Player Movement and Player collision
- Really basic lighting

## Improvements over the old project

- Faster chunk loading
- Capable of handling larger render distances
- Smoother player movement
- Improved collision
- Breaks less at larger distances
- Added UI

## How to build
Open the project folder with Visual Studio, open CMakeLists.txt and press <kbd>Ctrl + S</kbd>.
Select **VoxelGame.exe** from the run dropdown, then press <kbd>Ctrl + F5</kbd> to build and run.
All dependencies are already included in the thirdparty directory.

## Screenshots

![In-Game Screenshot](https://github.com/Werlog/VoxelGame/blob/1c354ab01821943649911ab68ea33b55d894ce40/screenshots/1.JPG)
![In-Game Screenshot](https://github.com/Werlog/VoxelGame/blob/1c354ab01821943649911ab68ea33b55d894ce40/screenshots/2.png)
