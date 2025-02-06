#pragma once


struct Vector2 {
	double x;
	double y;

	void print(Vector2& vector) {
		printf("X: %f | Y: %f\n", vector.x, vector.y);
	}
};