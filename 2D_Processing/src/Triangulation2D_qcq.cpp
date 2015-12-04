#include "Triangulation2D_qcq.h"
#include "Line.h"
#include "Graham_Scan.h"
#include <list>
#include <iterator>
#include "Jarvis.h"

#define LOG_Triangulation2D_qcq 0

namespace utils {
	double dotProduct(const Point& vecU, const Point& vecV) {
		return vecU.getX()*vecV.getX() + vecU.getY()* vecV.getY();
	}
}


bool myComparator(Point* a, Point* b) {// Abscisse croissante ou ou ordonnee croissante si abscisses egales
	return (a->getX() < b->getX() || (a->getX() == b->getX() && a->getY() < b->getY()));
}

Triangulation2D_qcq::Triangulation2D_qcq(std::vector<Point*> points)
	: TriangleStrip(points) {
}

void Triangulation2D_qcq::computeTriangulation() {
	// 3 points mini pour trianguler
	if(_points.size() < 3) {
		std::cout << "Nuage de points vide" << std::endl;
		return;
	}
	// On trie les points par x croissant (y croissant si égalité (décroissant pour nous ? vu quec'ets inverse))
	std::sort(_points.begin(), _points.end(), myComparator); // 12 32 45 71(26 33 53 80)

	// Initialisation avec 2-triangulation initial (seulement si y'a des points aliggnés ? wtf)
	Point* firstPoint = _points.at(0);
	std::vector<Point*> minPoints;

	for(int i = 0; i < _points.size(); ++i) {
		if(_points.at(i)->getX() == firstPoint->getX()) {
			minPoints.push_back(_points.at(i));
		}
		else {
			break;
		}
	}
	if(minPoints.size() == 1) {
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
	for(auto& line : edges) {
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
	for(; cpt < _points.size(); ++cpt) {
		Point* nextPoint = _points.at(cpt);

		// Calculer enveloppe convexe des pts deja traites
		std::vector<Point*> tmp(_points.begin(), _points.begin() + cpt);
		Graham_Scan enveloppe(tmp);
		enveloppe.calculEnveloppe();
		std::vector<Point*> pointsEnveloppe = enveloppe.getEnveloppe();
		// Calculer les normales de chaque arete de l'enveloppe
		// Puis produit scalaire avec le nouveau point
		for(auto p = pointsEnveloppe.begin(); p != pointsEnveloppe.end(); ++p) {
			Point normal, *p2, *p3;
			Point edge;
			if(p == pointsEnveloppe.end() - 1)
				p2 = pointsEnveloppe.front();
			else
				p2 = *(p + 1);
			if(p == pointsEnveloppe.begin())
				p3 = pointsEnveloppe.back();
			else
				p3 = *(p - 1);
			// Normale value
			normal.setX(-(p2->getY() - (*p)->getY()));
			normal.setY(p2->getX() - (*p)->getX());
			edge.setX(p3->getX() - (*p)->getX());
			edge.setY(p3->getY() - (*p)->getY());
			double dot = utils::dotProduct(normal, edge);
			if(dot < 0)
				normal = -(normal);
			edge.setX(nextPoint->getX() - (*p)->getX());
			edge.setY(nextPoint->getY() - (*p)->getY());
			dot = utils::dotProduct(normal, edge);
			if(dot < 0)
				_triangles.push_back(new Triangle(*p, p2, nextPoint));
		}
	}

	edges.clear();
}


bool MyLineComparatorEqual(Line& l1, Line& l2) {
	return ((l1.getStartPoint() == l2.getStartPoint() && l1.getEndPoint() == l2.getEndPoint())
			|| (l1.getEndPoint() == l2.getStartPoint() && l1.getStartPoint() == l2.getEndPoint()));
}


void Triangulation2D_qcq::flippingEdges() {
	Line * actualEdge = new Line(new Point(), new Point());
	std::list<Line*> listEdge;
	std::vector<Line> vectorEdge;
	std::vector<Line>::iterator itEdge;
	std::vector<Line>::iterator othItEdge;
	std::vector<std::vector<Line>::iterator> itToD;
	Point tempStartP;
	Point tempEndP;

	std::vector<Point *> listTop;
	std::vector<Point *>::iterator it;
	//TriangleStrip T1;
	Triangle * T2 = new Triangle(new Point(), new Point(), new Point());
	Triangle * T1 = new Triangle(new Point(), new Point(), new Point());

	//On travaille sur une structure composé de 4 sommets 
	// 4 arretes et 2 triangles donc
	bool alreadyIn = false;

	for(int i = 0; i < _triangles.size(); ++i) {
		//On verifie que le point n'est pas deja dans la liste pour l'écrire
		for(it = listTop.begin(); it != listTop.end(); ++it) {
			if(_triangles.at(i)->getPointA() == *it) {
				alreadyIn = true;
			}
		}

		if(alreadyIn == false) {
			listTop.push_back(_triangles.at(i)->getPointA());
		}
		else {
			alreadyIn = false;
		}

		for(it = listTop.begin(); it != listTop.end(); ++it) {
			if(_triangles.at(i)->getPointB() == *it) {
				alreadyIn = true;
			}
		}

		if(alreadyIn == false) {
			listTop.push_back(_triangles.at(i)->getPointB());
		}
		else {
			alreadyIn = false;
		}

		for(it = listTop.begin(); it != listTop.end(); ++it) {
			if(_triangles.at(i)->getPointC() == *it) {
				alreadyIn = true;
			}
		}

		if(alreadyIn == false) {
			listTop.push_back(_triangles.at(i)->getPointC());
		}
		else {
			alreadyIn = false;
		}




		vectorEdge.push_back(*new Line(_triangles.at(i)->getPointA(), _triangles.at(i)->getPointB()));
		vectorEdge.push_back(*new Line(_triangles.at(i)->getPointB(), _triangles.at(i)->getPointC()));
		vectorEdge.push_back(*new Line(_triangles.at(i)->getPointC(), _triangles.at(i)->getPointA()));



	}

	// Supprimer toutes les aretes doublon
	// http://stackoverflow.com/a/1041939

	//vectorEdge.erase(std::unique(vectorEdge.begin(), vectorEdge.end(), MyLineComparatorEqual), vectorEdge.end());
	int count = 0;
	bool itAlrdyIN = false;
	for(itEdge = vectorEdge.begin(); itEdge != vectorEdge.end(); ++itEdge) {
		tempStartP = *itEdge->getStartPoint();
		tempEndP = *itEdge->getEndPoint();
		for(othItEdge = vectorEdge.begin(); othItEdge != vectorEdge.end(); ++othItEdge) {
			if(itEdge != othItEdge) {

				if((tempStartP == *othItEdge->getStartPoint() && tempEndP == *othItEdge->getEndPoint())) {
					for(int i = 0; i < itToD.size(); ++i) {
						if((othItEdge->getStartPoint() == itToD[i]->getStartPoint() && othItEdge->getEndPoint() == itToD[i]->getEndPoint()) || (othItEdge->getEndPoint() == itToD[i]->getStartPoint() && othItEdge->getEndPoint() == itToD[i]->getStartPoint())) {
							itAlrdyIN = true;
						}
					}

					if(!itAlrdyIN) {
						itToD.push_back(othItEdge);

					}


				}
				itAlrdyIN = false;
				//else if( tempEndP == *othItEdge->getStartPoint() && tempStartP == *othItEdge->getEndPoint() )
				//{
				//	itToD.push_back(othItEdge);
				//	count++;
				//}
			}

		}
	}
	for(int i = 0; i<itToD.size(); ++i) {
		vectorEdge.erase(itToD[i]);
	}

	/*int j = 0;
	for (int i = 0; i < listTop.size(); ++i)
	{
	if (i == listTop.size() - 1)
	{
	listEdge.push_back(new Line(listTop.at(i), listTop.at(j)));
	}
	else
	{
	listEdge.push_back(new Line(listTop.at(i), listTop.at(i+1)));
	}

	}*/




	while(!listEdge.empty()) {
		listEdge.pop_front();
		if(actualEdge) {
			//_triangles.at(); 


			//actualEdge.setStartPoint();
			//actualEdge.setEndPoint();

			//T1 <-[A,A2,A1]
			T1->LineA(actualEdge);
			T1->setPointA(actualEdge->getStartPoint());
			//T1->setPointB();
			T1->setPointC(actualEdge->getEndPoint());

			//T2 <-[A,A4,A3]
			T2->setPointA(actualEdge->getStartPoint());
			//T2->setPointB();
			T2->setPointC(actualEdge->getEndPoint());


			//T2->LineB();
			//T3->LineC();

			//Mettre a jour les liens


			//listEdge.push_back()
		}

	}
}