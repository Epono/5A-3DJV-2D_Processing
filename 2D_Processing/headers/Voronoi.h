#pragma once
#include <vector>
#include <iostream>

#include "Point.h"
#include "LineStrip.h"
class Voronoi
{
private:
	std::vector<Point*> m_area;
	std::vector<Point*> m_listOfPoints;
	std::vector<Point*> m_listMediatrice;
	std::vector<Point*> m_actualPoint;
	std::vector<float> m_actualRadius;
	std::vector<Point*> m_intersect;

public:
	Voronoi();
	~Voronoi();
	void VoronoiAlgorithm(std::vector<Point*>);
	void DefineArea(std::vector<Point*>);
	float distanceBetweenTwoPoint(Point, Point);
	std::vector<Point*> intersectionBetweenCircle();
	void traceCercle(Point, float);
	void flippingEdge();


	std::vector<Point*>& getArea() { return m_area; }
	std::vector<Point*>& getPoints() { return m_listOfPoints; }
	std::vector<Point*>& getMediatriceP() { return m_listMediatrice; }
	std::vector<Point*>& getActualPoint() { return m_actualPoint; }
	std::vector<float> getRadius(){ return m_actualRadius; }
	std::vector<Point*>& getIntersect() { return m_intersect; }

};

