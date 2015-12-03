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


bool myComparator(const Point& a, const Point& b) 
{// Abscisse croissante ou ou ordonnee croissante si abscisses egales
	return (a.getX() < b.getX() || (a.getX() == b.getX() && a.getY() < b.getY()));
}

Triangulation2D_qcq::Triangulation2D_qcq(const std::vector<Point>& points)
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
	// On trie les points par x croissant (y croissant si �galit� (d�croissant pour nous ? vu quec'ets inverse))
	std::sort(_points.begin(), _points.end(), myComparator); // 12 32 45 71(26 33 53 80)

	// Initialisation avec 2-triangulation initial (seulement si y'a des points aliggn�s ? wtf)
	Point firstPoint = _points.at(0);
	std::vector<Point> minPoints;
	
	for(int i = 0; i < _points.size(); ++i) 
	{
		if(_points.at(i).getX() == firstPoint.getX()) 
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
	// Si tous les points sont colin�aires
	if(minPoints.size() == _points.size()) 
		return;

	// Create start edges
	std::vector<Line> edges;
	for(int j = 0; j < minPoints.size() - 1; ++j) 
		edges.push_back(Line(minPoints.at(j), minPoints.at(j + 1)));

#if LOG_Triangulation2D_qcq
	std::cout << "Liste des 1eres aretes : " << std::endl;
	for(auto& line : edges) {
		std::cout << "  " << *line << std::endl;
	}
#endif

	Point firstOtherPoint = _points.at(cpt);
	std::vector<Line> edgesToAdd;
	for(auto& line : edges) 
	{
		Line lineTemp1(line.getStartPoint(), firstOtherPoint);
		Line lineTemp2(line.getEndPoint(), firstOtherPoint);
		Triangle t(line.getStartPoint(), line.getEndPoint(), firstOtherPoint); // Si on le met sous les vectors �a bug u_u
		edgesToAdd.push_back(lineTemp1);
		edgesToAdd.push_back(lineTemp2);
		_triangles.push_back(t);
	}
	edges.insert(edges.end(), edgesToAdd.begin(), edgesToAdd.end());

	++cpt;

	// It�ration
	// Pour chaque point restant 
	for(; cpt < _points.size(); ++cpt) 
	{
		Point nextPoint = _points.at(cpt);

		// Calculer enveloppe convexe des pts deja traites
		std::vector<Point> tmp(_points.begin(), _points.begin() + cpt);
		Graham_Scan enveloppe(tmp);
		enveloppe.calculEnveloppe();
		std::vector<Point> pointsEnveloppe = enveloppe.getEnveloppe();
		// Calculer les normales de chaque arete de l'enveloppe
		// Puis produit scalaire avec le nouveau point
		for (auto p = pointsEnveloppe.begin(); p != pointsEnveloppe.end(); ++p)
		{
			Point normal, p2, p3;
			Point edge;
			if (p == pointsEnveloppe.end() - 1)
				p2 = pointsEnveloppe.front();
			else
				p2 = *(p + 1);
			if (p == pointsEnveloppe.begin())
				p3 = pointsEnveloppe.back();
			else
				p3 = *(p - 1);
			// Normale value
			normal.setX(-(p2.getY() - (*p).getY()));
			normal.setY(p2.getX() - (*p).getX());
			edge.setX(p3.getX() - (*p).getX());
			edge.setY(p3.getY() - (*p).getY());
			double dot = utils::dotProduct(normal, edge);
			if (dot < 0)
				normal = -(normal);
			edge.setX(nextPoint.getX()- (*p).getX());
			edge.setY(nextPoint.getY() - (*p).getY());
			dot = utils::dotProduct(normal, edge);
			if (dot < 0)
				_triangles.push_back(Triangle(*p, p2, nextPoint));
		}
	}

	edges.clear();
}