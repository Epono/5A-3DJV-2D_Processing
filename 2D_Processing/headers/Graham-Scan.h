#pragma once

#include <vector>

class Point;

class Graham_Scan {
private:
	Point* m_centroid;
	std::vector<Point*> m_points;

public:
	Graham_Scan();

	void computeCentroid() const;
	inline Point* getCentroid() const { return m_centroid; };
};