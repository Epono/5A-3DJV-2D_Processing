#include "Triangulation2D_qcq.h"
#include "Line.h"
#include "Graham_Scan.h"

#define LOG_Triangulation2D_qcq 0

namespace utils
{
	double dotProduct(const Point& vecU, const Point& vecV)
	{
		return vecU.getX()*vecV.getX() + vecU.getY()* vecV.getY();
	}
}


bool myComparator(Point* a, Point* b) 
{// Abscisse croissante ou ou ordonnee croissante si abscisses egales
	return (a->getX() < b->getX() || (a->getX() == b->getX() && a->getY() < b->getY()));
}

Triangulation2D_qcq::Triangulation2D_qcq(std::vector<Point*> points)
	: TriangleStrip(points)
{}

void Triangulation2D_qcq::computeTriangulation() 
{
	// 3 points mini pour trianguler
	if(_points.size() < 3) 
	{
		std::cout << "Nuage de points vide" << std::endl;
		return;
	}
	// On trie les points par x croissant (y croissant si égalité (décroissant pour nous ? vu quec'ets inverse))
	std::sort(_points.begin(), _points.end(), myComparator); // 12 32 45 71(26 33 53 80)

	// Initialisation avec 2-triangulation initial (seulement si y'a des points aliggnés ? wtf)
	Point* firstPoint = _points.at(0);
	std::vector<Point*> minPoints;
	
	for(int i = 0; i < _points.size(); ++i) 
	{
		if(_points.at(i)->getX() == firstPoint->getX()) 
		{
			minPoints.push_back(_points.at(i));
		}
		else 
		{
			break;
		}
	}
	if (minPoints.size() == 1)
	{
		// Si pas assez de points on rajoute le suivant
		minPoints.push_back(_points.at(1));
	}

#if LOG_Triangulation2D_qcq
	std::cout << "1er point : " << *firstPoint << std::endl;
	std::cout << "Liste des points colineaires au 1er point : " << std::endl;
	for(auto& point : minPoints) {
		std::cout << "  " << *point << std::endl;
	}
#endif

	int cpt = minPoints.size();
	// Si tous les points sont colinéaires
	if(minPoints.size() == _points.size()) 
		return;

	// Create start edges
	std::vector<Line*> edges;
	for(int j = 0; j < minPoints.size() - 1; ++j) 
		edges.push_back(new Line(minPoints.at(j), minPoints.at(j + 1)));

#if LOG_Triangulation2D_qcq
	std::cout << "Liste des 1eres aretes : " << std::endl;
	for(auto& line : edges) {
		std::cout << "  " << *line << std::endl;
	}
#endif

	Point* firstOtherPoint = _points.at(cpt);
	std::vector<Line*> edgesToAdd;
	for(auto& line : edges) 
	{
		Line* lineTemp1 = new Line(line->getStartPoint(), firstOtherPoint);
		Line* lineTemp2 = new Line(line->getEndPoint(), firstOtherPoint);
		Triangle* t = new Triangle(line->getStartPoint(), line->getEndPoint(), firstOtherPoint); // Si on le met sous les vectors ça bug u_u
		edgesToAdd.push_back(lineTemp1);
		edgesToAdd.push_back(lineTemp2);
		_triangles.push_back(t);
	}
	edges.insert(edges.end(), edgesToAdd.begin(), edgesToAdd.end());

	++cpt;

	// Itération
	// Pour chaque point restant 
	for(; cpt < _points.size(); ++cpt) 
	{
		Point* nextPoint = _points.at(cpt);

		// Calculer enveloppe convexe des pts deja traites
		std::vector<Point*> tmp(_points.begin(), _points.begin() + cpt);
		Graham_Scan enveloppe(tmp);
		enveloppe.calculEnveloppe();
		std::vector<Point*> pointsEnveloppe = enveloppe.getEnveloppe();
		std::sort(pointsEnveloppe.begin(), pointsEnveloppe.end(), myComparator);
		// Calculer les normales de chaque arete de l'enveloppe
		// Puis produit scalaire avec le nouveau point
		for (auto p = pointsEnveloppe.begin(); p != pointsEnveloppe.end(); ++p)
		{
			Point normal, *p2;
			Point edge;
			if (p == pointsEnveloppe.end() - 1)
				p2 = pointsEnveloppe.front();
			else
				p2 = *(p + 1);
			// Edge coordinate
			edge.setX(p2->getX() - (*p)->getX());
			edge.setY(p2->getY() - (*p)->getY());
			// Normale value
			normal.setX(-edge.getY());
			normal.setY(edge.getX());
			if (utils::dotProduct(normal, edge) > 0)
				normal = -(normal);
			double angle1 = Graham_Scan::OrientedAngle(normal, **p, *nextPoint)
				, angle2 = Graham_Scan::OrientedAngle(*nextPoint, **p, normal);
			if (angle1 < M_PI_2 || angle2 < M_PI_2)
				_triangles.push_back(new Triangle(*p, p2, nextPoint));
		}
	}

	edges.clear();
}