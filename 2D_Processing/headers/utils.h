#pragma once

#include "Point.h"

#define WIDTH 1280
#define HEIGHT 720

// Represents the state of the creation action
typedef enum creationState {
	WAITING_FOR_FIRST_CLICK,
	WAITING_FOR_NEXT_CLICK,
	PENDING,
	SELECT_POINT,
	SCALING,
	ROTATING,
	TRANSLATING
};

typedef enum algorithm {
	NONE, GRAHAM_SCAN, JARVIS, TRIANGULATION2D_QCQ, VORONOI
};

typedef struct color_rgb {
	color_rgb(float r, float g, float b) :_r(r), _g(g), _b(b) {}
	float _r;
	float _g;
	float _b;
} color_rgb;
