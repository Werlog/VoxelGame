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
- Simple main menu
- Clouds
- Player Movement and Player collision
- Really basic lighting

## Improvements over the old project

- Faster chunk loading
- Capable of handling larger render distances
- Smoother player movement
- Improved collision
- Breaks less at larger distances

## How to build
Open the project folder with Visual Studio, open CMakeLists.txt and press <kbd>Ctrl + S</kbd>.
Select **VoxelGame.exe** from the run dropdown, then press <kbd>Ctrl + F5</kbd> to build and run.