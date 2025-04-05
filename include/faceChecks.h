#pragma once

constexpr int faceChecks[] = {
	// Front Face
	0, 0, 1,

	// Right Face
	1, 0, 0,

	// Back Face
	0, 0, -1,

	// Left Face
	-1, 0, 0,

	// Top Face
	0, 1, 0,

	// Bottom Face
	0, -1, 0,
};