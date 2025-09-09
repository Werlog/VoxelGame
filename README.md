# Voxel Game

A basic voxel game/minecraft clone I'm making in C++ and OpenGL.
This is a rewrite of my old [OpenGLVoxelGame](https://github.com/Werlog/OpenGLVoxelGame).

## Controls

WASD - Movement

Space - Jumping

Left Click - Destroy block

Right Click - Place block

Middle Click - Selects the block you are looking at

Mouse Scroll - Swap between blocks

E - Open block picker

Right Shift - Open Dev Menu (Allows to toggle flight/collisions)

## Current Features

- Procedural World Generation
	- Infinite world generation
	- Trees
- Block Placing and Breaking
- Main menu
- Clouds
- Player Movement and Player collision
- World saving
- Really basic lighting
- Block behaviours

## Improvements over the old project

- Faster chunk loading
- Capable of handling larger render distances
- Smoother player movement
- Improved collision
- Breaks less at larger distances
- Added world saving
- Added UI
- Implemented different block shapes
- Implemented different block behaviours (Cactus growth, sand falling...)

## How to build
Open the project folder with Visual Studio, open CMakeLists.txt and press <kbd>Ctrl + S</kbd>.
Select **VoxelGame.exe** from the run dropdown, then press <kbd>Ctrl + F5</kbd> to build and run.
All dependencies are already included in the thirdparty directory.

## Screenshots

![In-Game Screenshot](https://github.com/Werlog/VoxelGame/blob/26fe21f114588a4fab7d7a0ccacbd440b36a344c/screenshots/1.JPG)
![In-Game Screenshot](https://github.com/Werlog/VoxelGame/blob/26fe21f114588a4fab7d7a0ccacbd440b36a344c/screenshots/2.png)
