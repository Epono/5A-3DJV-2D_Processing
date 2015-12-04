#include "Triangulation2D_Delaunay.h"
#include "Line.h"

double dotProduct(const Point& vecU, const Point& vecV) {
	return vecU.getX()*vecV.getX() + vecU.getY()* vecV.getY();
}

float sign(Point& p1, Point& p2, Point& p3) {
	return (p1.getX() - p3.getX()) * (p2.getY() - p3.getY()) - (p2.getX() - p3.getX()) * (p1.getY() - p3.getY());
}

bool PointInTriangle(Point& pt, Point& v1, Point& v2, Point& v3) {
	bool b1, b2, b3;

	b1 = sign(pt, v1, v2) < 0.0f;
	b2 = sign(pt, v2, v3) < 0.0f;
	b3 = sign(pt, v3, v1) < 0.0f;

	return ((b1 == b2) && (b2 == b3));
}

bool isOnLine(Line* l, Point* p) {
	// test if the vector product is zero
	if((p->getX() - l->getStartPoint()->getX())*(p->getY() - l->getEndPoint()->getY()) == (p->getY() - l->getStartPoint()->getY())*(p->getX() - l->getEndPoint()->getX())) {
		return true;
	}
	return false;
}

bool det33(long m0, long m1, long m2, long m3, long m4, long m5, long m6, long m7, long m8) {
	long det33 = 0;
	det33 += m0 * (m4 * m8 - m5 * m7);
	det33 -= m1 * (m3 * m8 - m5 * m6);
	det33 += m2 * (m3 * m7 - m4 * m6);
	return det33;
}

bool inCircle(Point* a, Point* b, Point* c, Point* d) {
	long a2 = a->getX()*a->getX() + a->getY()*a->getY();
	long b2 = b->getX()*b->getX() + b->getY()*b->getY();
	long c2 = c->getX()*c->getX() + c->getY()*c->getY();
	long d2 = d->getX()*d->getX() + d->getY()*d->getY();

	long det44 = 0;
	det44 += d2  * det33(a->getX(), a->getY(), 1, b->getX(), b->getY(), 1, c->getX(), c->getY(), 1);
	det44 -= d->getX() * det33(a2, a->getY(), 1, b2, b->getY(), 1, c2, c->getY(), 1);
	det44 += d->getY() * det33(a2, a->getX(), 1, b2, b->getX(), 1, c2, c->getX(), 1);
	det44 -= 1 * det33(a2, a->getX(), a->getY(), b2, b->getX(), b->getY(), c2, c->getX(), c->getY());

	if(det44 < 0)
		return true;
	return false;
}

Point* getMinCoordinates(std::vector<Point*> points) {
	float xmin = WIDTH;
	float ymin = HEIGHT;
	for(auto point : points) {
		if(point->getX() < xmin) {
			xmin = point->getX();
		}
		if(point->getY() < ymin) {
			ymin = point->getY();
		}
	}

	return new Point(xmin, ymin);
}

Point* getMaxCoordinates(std::vector<Point*> points) {
	float xmax = 0;
	float ymax = 0;
	for(auto point : points) {
		if(point->getX() > xmax) {
			xmax = point->getX();
		}
		if(point->getY() > ymax) {
			ymax = point->getY();
		}
	}

	return new Point(xmax, ymax);
}

bool myPointComparator(Point* a, Point* b) {
	// Abscisse croissante ou ou ordonnee croissante si abscisses egales
	return (a->getX() < b->getX() || (a->getX() == b->getX() && a->getY() < b->getY()));
}


Point* getMiddleCoordinates(std::vector<Point*> points, Point* newPoint) {
	std::sort(points.begin(), points.end(), myPointComparator);

	for(int i = 0; i < points.size(); ++i) {
		Point* point = points[i];

		//TODO: peut etre inverser
		if(!myPointComparator(point, newPoint)) {
			return point;
		}
	}
}

void Triangulation2D_Delaunay::InsertPoint(Point* newPoint) {
	if(_triangles.empty()) {
		// 1er point
		if(_points.empty()) {
			_points.push_back(newPoint);
		}
		// 2em point
		else if(_points.size() == 1) {
			_points.push_back(newPoint);
			_edges.push_back(new Line(_points[0], newPoint));
		}
		// 3+em point
		else {
			_points.push_back(newPoint);
			// Si le point est colinéaire aux autres
			if(isOnLine(_edges[0], newPoint)) {
				// Si le point est à l'extremité "basse" des points colinéaires
				Point* minPoint = getMinCoordinates(_points);
				Point* maxPoint = getMaxCoordinates(_points);

				if(newPoint->getX() < minPoint->getX() || (newPoint->getX() == minPoint->getX() && newPoint->getY() < minPoint->getY())) {
					_edges.push_back(new Line(newPoint, nullptr));
				}
				// Si le point est à l'extremité "haute" des points colinéaires
				else if(newPoint->getX() > maxPoint->getX() || (newPoint->getX() == maxPoint->getX() && newPoint->getY() > maxPoint->getY())) {
					_edges.push_back(new Line(nullptr, newPoint));
				}
				// Si le point est en plein milieu
				// On update l'ancienne arete et on ajoute une nouvelle arête
				else {
					Point* pointBefore = getMiddleCoordinates(_points, newPoint);
					for(auto edge : _edges) {
						if(edge->getStartPoint() == pointBefore) {
							_edges.push_back(new Line(newPoint, edge->getEndPoint()));
							edge->setEndPoint(newPoint);
							break;
						}
					}

				}

				delete minPoint;
				delete maxPoint;
			}
			// Si le point n'est pas colinéaire
			// (on passera ici qu'une fois normalement)
			else {
				for(auto point : _points) {
					Line* tempLine = new Line(point, newPoint);
					_edges.push_back(tempLine);
					point->setLine(tempLine);
				}

				for(auto edge : _edges) {
					Line* tempLineA = edge->getStartPoint()->getLine();
					Line* tempLineB = edge->getEndPoint()->getLine();
					// Peut etre à l'envers
					_triangles.push_back(new Triangle(edge, tempLineA, tempLineB));
					// ou 
					//_triangles.push_back(new Triangle(edge, tempLineB, tempLineA));
					// en fonction de la position de newPoint ?
				}
			}
		}
	}
	// Y'a deja des triangles
	else {
		_points.push_back(newPoint);
		// Si le point est dans un triangle
		bool inTriangle = false;
		Triangle* triangleContainingPoint = nullptr;
		std::vector<Line*> edges;

		std::vector<Triangle*>::iterator it;
		for(it = _triangles.begin(); it != _triangles.end(); it++) {
			Triangle* triangle = *it;
			if(PointInTriangle(*newPoint, *triangle->getPointA(), *triangle->getPointB(), *triangle->getPointC())) {
				inTriangle = true;
				triangleContainingPoint = triangle;
				break;
			}
		}
		if(inTriangle) {
			_triangles.erase(it);
			edges.push_back(triangleContainingPoint->LineA());
			edges.push_back(triangleContainingPoint->LineB());
			edges.push_back(triangleContainingPoint->LineC());
		}
		else {
			//TODO: déterminer la liste des aretes vues par le point
			// Avec l'enveloppe c'est plus opti
			for(auto edge : _edges) {
				Point normal(-(edge->getEndPoint()->getY() - (edge->getStartPoint()->getY())), edge->getEndPoint()->getX() - (edge->getStartPoint())->getX());
				Point edgee(newPoint->getX() - edge->getEndPoint()->getX(), newPoint->getY() - edge->getEndPoint()->getY());
				double dot = dotProduct(normal, edgee);
				if(dot < 0) {
					edges.push_back(edge);
				}
			}
		}

		while(!edges.empty()) {
			Line* a = edges.back();
			edges.pop_back();

			bool pointInCircle = false;
			std::vector<Triangle*>::iterator it2;
			for(it2 = _triangles.begin(); it2 != _triangles.end(); it2++) {
				Triangle* triangle2 = *it2;
				if(inCircle(triangle2->getPointA(), triangle2->getPointB(), triangle2->getPointC(), newPoint)) {
					pointInCircle = true;
					break;
				}
			}

			// Si l'arete a un triangle incident t dont le cercle circonscrit contient newPoint, suprimer le triangle t et l'arete a et ajouter les 2 autres aretes du triangle à la liste
			if(pointInCircle) {
				std::cout << "je passe jamais !" << std::endl;
				Triangle* triangle = *it;
				edges.erase(edges.begin());
				if(triangle->LineA() != a) {
					edges.push_back(triangle->LineA());
				}
				if(triangle->LineB() != a) {
					edges.push_back(triangle->LineB());
				}
				if(triangle->LineC() != a) {
					edges.push_back(triangle->LineC());
				}
				_triangles.erase(it);
			}
			else {
				// creer aretes start - new et end - new et le triangle a a1 a2 (ou a2 a1 a)
				Line* a1 = new Line(a->getStartPoint(), newPoint);
				Line* a2 = new Line(a->getEndPoint(), newPoint);
				_edges.push_back(a1);
				_edges.push_back(a2);
				_triangles.push_back(new Triangle(a, a1, a2));
			}
		}
	}
}

void Triangulation2D_Delaunay::RemovePoint(Point* pointToRemove) {
	if(_triangles.empty()) {
		if(_points.size() == 1) {
			_points.clear();
		}
		else if(true) {

		}
		else {

		}
	}
}

void Triangulation2D_Delaunay::computeTriangulation() {

}