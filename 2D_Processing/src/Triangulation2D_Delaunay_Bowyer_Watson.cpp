#include "Triangulation2D_Delaunay_Bowyer_Watson.h"
#include "Line.h"

bool Triangulation2D_Delaunay_Bowyer_Watson::Circumcircle(const Point& p0, const Point& p1, const Point& p2, Point& center, float& radius) {
	float dA, dB, dC, aux1, aux2, div;

	dA = p0.getX() * p0.getX() + p0.getY() * p0.getY();
	dB = p1.getX() * p1.getX() + p1.getY() * p1.getY();
	dC = p2.getX() * p2.getX() + p2.getY() * p2.getY();

	aux1 = (dA*(p2.getY() - p1.getY()) + dB*(p0.getY() - p2.getY()) + dC*(p1.getY() - p0.getY()));
	aux2 = -(dA*(p2.getX() - p1.getX()) + dB*(p0.getX() - p2.getX()) + dC*(p1.getX() - p0.getX()));
	div = (2 * (p0.getX()*(p2.getY() - p1.getY()) + p1.getX()*(p0.getY() - p2.getY()) + p2.getX()*(p1.getY() - p0.getY())));

	if(div == 0) {
		return false;
	}

	center.setX(aux1 / div);
	center.setY(aux2 / div);

	radius = sqrt((center.getX() - p0.getX())*(center.getX() - p0.getX()) + (center.getY() - p0.getY())*(center.getY() - p0.getY()));

	return true;
}

bool Triangulation2D_Delaunay_Bowyer_Watson::IsInsideCircle(Point& center, float radius, Point& p) {
	return std::pow(p.getX() - center.getX(), 2) + std::pow(p.getY() - center.getY(), 2) < std::pow(radius, 2);
}

bool MyLineComparatorEquals(Line& l1, Line& l2) {
	return (l1.getStartPoint() == l2.getStartPoint() && l1.getEndPoint() == l2.getEndPoint());
}

bool MyLineComparatorLess(Line& l1, Line& l2) {
	return (l1.getStartPoint()->getX() < l2.getStartPoint()->getX());
}

//subroutine triangulate
//input : vertex list
//	output : triangle list
//			 initialize the triangle list
//			 determine the supertriangle
//			 add supertriangle vertices to the end of the vertex list
//			 add the supertriangle to the triangle list
//			 for each sample point in the vertex list
//				 initialize the edge buffer
//				 for each triangle currently in the triangle list
//					 calculate the triangle circumcircle center and radius
//					 if the point lies in the triangle circumcircle then
//						 add the three triangle edges to the edge buffer
//						 remove the triangle from the triangle list
//					 endif
//				 endfor
//				 delete all doubly specified edges from the edge buffer
//					this leaves the edges of the enclosing polygon only
//				 add to the triangle list all triangles formed between the point
//					and the edges of the enclosing polygon
//			 endfor
//			 remove any triangles from the triangle list that use the supertriangle vertices
//			 remove the supertriangle vertices from the vertex list
//			 end
void Triangulation2D_Delaunay_Bowyer_Watson::computeTriangulation() {

	// Créer un méga triangle qui contient tous les points
	Triangle* supertriangle = new Triangle(new Point(-10 * WIDTH, -10 * HEIGHT), new Point(10 * WIDTH, -10 * HEIGHT), new Point(WIDTH / 2, 10 * HEIGHT));
	_triangles.push_back(supertriangle);

	// Ajouter les points du triangle à la fin de la liste des points
	_points.push_back(supertriangle->getPointA());
	_points.push_back(supertriangle->getPointB());
	_points.push_back(supertriangle->getPointC());

	// Pour chaque point de la liste
	for(auto& point : _points) {
		// Initialiser le edge buffer ?
		std::vector<Line> edgeBuffer;

		// Pour chaque triangle dans la liste
		for(int i = 0; i < _triangles.size(); ++i) {
			Triangle* tempTriangle = _triangles.at(i);
			float radius;
			Point center;

			// Calculer le circumcircle center et le rayon du triangle
			Circumcircle(*tempTriangle->getPointA(), *tempTriangle->getPointA(), *tempTriangle->getPointA(), center, radius);

			// Si le point est dans le circumcircle
			if(IsInsideCircle(center, radius, *point)) {
				// Ajouter les 3 aretes du triangle au edge buffer et retirer le triangle de la liste
				Point* pointA = tempTriangle->getPointA();
				Point* pointB = tempTriangle->getPointB();
				Point* pointC = tempTriangle->getPointC();

				edgeBuffer.push_back(Line(pointA, pointB));
				edgeBuffer.push_back(Line(pointB, pointC));
				edgeBuffer.push_back(Line(pointC, pointA));

				_triangles.erase(_triangles.begin() + i);
				i--;
			}
		}

		// Supprimer toutes les aretes doublon
		// http://stackoverflow.com/a/1041939
		std::sort(edgeBuffer.begin(), edgeBuffer.end(), MyLineComparatorLess);
		edgeBuffer.erase(std::unique(edgeBuffer.begin(), edgeBuffer.end(), MyLineComparatorEquals), edgeBuffer.end());

		// Ajouter tous les triangles formés par le point avec les aretes (edge buffer)
		for(auto& edge : edgeBuffer) {
			_triangles.push_back(new Triangle(point, edge.getStartPoint(), edge.getEndPoint()));
		}
	}

	// Supprimer tous les triangles qui utilisent un des sommets du supertriangle
	//for(auto& triangle : _triangles) {
	//	if(triangle->getPointA() == supertriangle->getPointA() || triangle->getPointA() == supertriangle->getPointB() || triangle->getPointA() == supertriangle->getPointC() ||
	//	   triangle->getPointB() == supertriangle->getPointA() || triangle->getPointB() == supertriangle->getPointB() || triangle->getPointB() == supertriangle->getPointC() ||
	//	   triangle->getPointC() == supertriangle->getPointA() || triangle->getPointC() == supertriangle->getPointB() || triangle->getPointC() == supertriangle->getPointC()) {
	//	}
	//}

	//for(int i = 0; i < _triangles.size(); ++i) {
	//	Triangle* triangle = _triangles.at(i);

	//	if(triangle->getPointA() == supertriangle->getPointA() || triangle->getPointA() == supertriangle->getPointB() || triangle->getPointA() == supertriangle->getPointC() ||
	//	   triangle->getPointB() == supertriangle->getPointA() || triangle->getPointB() == supertriangle->getPointB() || triangle->getPointB() == supertriangle->getPointC() ||
	//	   triangle->getPointC() == supertriangle->getPointA() || triangle->getPointC() == supertriangle->getPointB() || triangle->getPointC() == supertriangle->getPointC()) {
	//		_triangles.erase(_triangles.begin() + i);
	//		//i--;
	//	}
	//	else {
	//		std::cout << "tamere" << std::endl;
	//	}
	//}

	_triangles.erase(std::remove_if(_triangles.begin(), _triangles.end(), [supertriangle](Triangle* triangle) {
		return triangle->getPointA() == supertriangle->getPointA() || triangle->getPointA() == supertriangle->getPointB() || triangle->getPointA() == supertriangle->getPointC() ||
			triangle->getPointB() == supertriangle->getPointA() || triangle->getPointB() == supertriangle->getPointB() || triangle->getPointB() == supertriangle->getPointC() ||
			triangle->getPointC() == supertriangle->getPointA() || triangle->getPointC() == supertriangle->getPointB() || triangle->getPointC() == supertriangle->getPointC();
	}), _triangles.end());

	//_triangles.erase(std::remove_if(_triangles.begin(), _triangles.end(), [](Triangle* triangle) {
	//	return triangle->getPointA()->getX() < 0 || triangle->getPointA()->getY() < 0 || triangle->getPointA()->getX() > WIDTH || triangle->getPointA()->getY() > HEIGHT
	//		|| triangle->getPointB()->getX() < 0 || triangle->getPointB()->getY() < 0 || triangle->getPointB()->getX() > WIDTH || triangle->getPointB()->getY() > HEIGHT
	//		|| triangle->getPointC()->getX() < 0 || triangle->getPointC()->getY() < 0 || triangle->getPointC()->getX() > WIDTH || triangle->getPointC()->getY() > HEIGHT;
	//}), _triangles.end());

	// Supprimer les sommets du supertriangle de la liste
	_points.pop_back();
	_points.pop_back();
	_points.pop_back();
}