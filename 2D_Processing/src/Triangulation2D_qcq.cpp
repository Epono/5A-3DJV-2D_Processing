#include "Triangulation2D_qcq.h"
#include "Line.h"

#define LOG_Triangulation2D_qcq 0

bool myComparator(Point* a, Point* b) {
	return (a->getX() != b->getX() ? a->getX() < b->getX() : a->getY() > b->getY());
}

Triangulation2D_qcq::Triangulation2D_qcq(std::vector<Point*> points) {
	_points = points;
}

void Triangulation2D_qcq::computeTriangulation() {
	if(_points.empty()) {
		return;
	}

	// On trie les points par x croissant (y croissant si égalité (décroissant pour nous ? vu quec'ets inverse))
	std::sort(_points.begin(), _points.end(), myComparator); // 12 32 45 71(26 33 53 80)

	// Initialisation avec 2-triangulation initial (seulement si y'a des points aliggnés ? wtf)
	Point* firstPoint = _points.at(0);
	std::vector<Point*> minPoints;
	int i;
	for(i = 0; i < _points.size(); ++i) {
		if(_points.at(i)->getX() == firstPoint->getX()) {
			minPoints.push_back(_points.at(i));
		}
		else {
			break;
		}
	}

#if LOG_Triangulation2D_qcq
	std::cout << "1er point : " << *firstPoint << std::endl;
	std::cout << "Liste des points colineaires au 1er point : " << std::endl;
	for(auto& point : minPoints) {
		std::cout << "  " << *point << std::endl;
	}
#endif

	// Si tous les points sont colinéaires
	if(i >= _points.size()) {
		return;
	}

	std::vector<Line*> edges;
	Point* firstOtherPoint = _points.at(i);

	if(minPoints.size() > 1) {
		for(int j = 0; j < minPoints.size() - 1; ++j) {
			edges.push_back(new Line(minPoints.at(j), minPoints.at(j + 1)));
		}
	}

#if LOG_Triangulation2D_qcq
	std::cout << "Liste des 1eres aretes : " << std::endl;
	for(auto& line : edges) {
		std::cout << "  " << *line << std::endl;
	}
#endif

	std::vector<Line*> edgesToAdd;
	for(auto& line : edges) {
		Line* lineTemp1 = new Line(line->getStartPoint(), firstOtherPoint);
		Line* lineTemp2 = new Line(line->getEndPoint(), firstOtherPoint);
		Triangle* t = new Triangle(line->getStartPoint(), line->getEndPoint(), firstOtherPoint); // Si on le met sous les vectors ça bug u_u
		edgesToAdd.push_back(lineTemp1);
		edgesToAdd.push_back(lineTemp2);
		_triangles.push_back(t);
	}
	edges.insert(edges.end(), edgesToAdd.begin(), edgesToAdd.end());

	++i;

	// Itération
	// Pour chaque point restant 
	for(i = i; i < _points.size(); ++i) {
		Point* tempPoint = _points.at(i);
		std::vector<Line*> edgesToAdd2;
		// Pour chaque arete existant visible par le point
		for(auto& line : edges) {
			// Manque la condition
			if(true) {
				Line* lineTemp1 = new Line(line->getStartPoint(), tempPoint);
				Line* lineTemp2 = new Line(line->getEndPoint(), tempPoint);
				Triangle* t = new Triangle(line->getStartPoint(), line->getEndPoint(), tempPoint); // Si on le met sous les vectors ça bug u_u
				edgesToAdd2.push_back(lineTemp1);
				edgesToAdd2.push_back(lineTemp2);
				_triangles.push_back(t);
			}
		}
		edges.insert(edges.end(), edgesToAdd2.begin(), edgesToAdd2.end());
	}

	edges.clear();
}