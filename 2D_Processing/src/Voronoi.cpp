#include "Voronoi.h"
#include <algorithm>
#include "Line.h"


Voronoi::Voronoi()
{
}


Voronoi::~Voronoi()
{
}

void Voronoi::VoronoiAlgorithm(std::vector<Point*> points)
{


	//Delaunay
	//if ();
	//points
		//je prend 3 points je calcule leur barycentre 
		//a partir du barycentre je dessine un cercle et je vérifie qu'aucun n'autre point ne soit dans ce cercle
		//si pas de points : on dessine un triangle
		//sinon on abort
		//On fais ça avec tous les points

	/*for (int i = 0; i < points.size()-2;++i)
	{
		Point* test; 
		test = new Point();

		std::vector<Point*> workingList;
		workingList.push_back(points.at(i));
		workingList.push_back(points.at(i+1));
		workingList.push_back(points.at(i+2));

		

		std::cout << "avant" << std::endl;
		for (int j = 0; j < points.size();++j)
		{
			std::cout << points.at(j) << " ";
		}
		

		std::sort(points.begin(), points.end());
		std::cout << "apres" << std::endl;
		for (int j = 0; j < points.size(); ++j)
		{
			std::cout << points.at(j) << " ";
		}


		int xAB = (workingList.at(0)->getX() + workingList.at(1)->getX())/2;
		int yAB = (workingList.at(0)->getY() + workingList.at(1)->getY())/2;
		std::cout << "AB -> x: "<< xAB << "y: "<< yAB <<std::endl;
		
		test->setX(xAB); test->setY(yAB);


		int xAC = (workingList.at(0)->getX() + workingList.at(2)->getX())/2;
		int yAC = (workingList.at(0)->getY() + workingList.at(2)->getY())/2;
		std::cout << "AC -> x: " << xAC << "y: " << yAC << std::endl;

		int xBC = (workingList.at(1)->getX() + workingList.at(2)->getX())/2;
		int yBC = (workingList.at(1)->getY() + workingList.at(2)->getY())/2;
		std::cout << "BC -> x: " << xBC << "y: " << yBC << std::endl;

		
		int slope = (workingList.at(1)->getY() - workingList.at(0)->getY()) / (workingList.at(1)->getX() - workingList.at(0)->getX());
		std::cout << "SLOPE : " << slope << std::endl;
		if (slope < 0)
		{
			slope;
		}



		//Calcul vecteur AB
		std::vector<Point *> vectorList;
		int ABx = workingList.at(1)->getX() - workingList.at(0)->getX();
		int ABy = workingList.at(1)->getY() - workingList.at(0)->getY();

		vectorList.push_back(new Point(ABx, ABy));

		int ACx = workingList.at(2)->getX() - workingList.at(0)->getX();
		int ACy = workingList.at(2)->getY() - workingList.at(0)->getY();

		vectorList.push_back(new Point(ACx, ACy));

		int scalar = (vectorList.at(0)->getX() * vectorList.at(1)->getX()) + (vectorList.at(0)->getY() * vectorList.at(1)->getY());
		std::cout << "Produit Scalaire AB AC : " << scalar << std::endl;


		int yolo = std::sqrt(std::pow(workingList.at(1)->getX() - workingList.at(0)->getX(), 2) + std::pow(workingList.at(1)->getY() - workingList.at(0)->getY(), 2));
		int yolo2 = std::sqrt(std::pow(workingList.at(2)->getX() - workingList.at(0)->getX(), 2) + std::pow(workingList.at(2)->getY() - workingList.at(0)->getY(), 2));



		m_listOfPoints.push_back(points.at(i));
		m_listOfPoints.push_back(points.at(i+1));
		m_listOfPoints.push_back(points.at(i+2));
		m_listMediatrice.push_back(test);
		m_listMediatrice.push_back(points.at(i+2));

	}*/
	

	//VRAI VORONOI ALGO
	//calcul distance AB on divise par 2 et on crée un cercle avec un rayon plus grand que AB/2
	Point * A = new Point();
	Point * B = new Point();
	A = points[0];
	B = points[1];

	float distanceforRadius;
	distanceforRadius = distanceBetweenTwoPoint(*A, *B)/2;

	std::cout << distanceforRadius << std::endl;
	distanceforRadius *= 1.5f;
	m_actualPoint.push_back(A);
	m_actualPoint.push_back(B);
	m_actualRadius.push_back(distanceforRadius) ;


	//calcul d'intersection
	m_intersect = intersectionBetweenCircle();




}
void Voronoi::DefineArea(std::vector<Point*> areaSet)
{
	
	

	Point* one;
	Point* two;
	Point* three;
	Point* four;

	one = areaSet.at(0);
	three = areaSet.at(1);
	two = new Point;
	four = new Point;
	
	two->setX(areaSet.at(0)->getX());
	
	four->setX(areaSet.at(1)->getX());

	
	two->setY(areaSet.at(1)->getY());
	
	four->setY(areaSet.at(0)->getY());

	m_area.push_back(one);
	m_area.push_back(two);
	m_area.push_back(three);
	m_area.push_back(four);

	//TO DO RECALCULER LA TAILLE

}

float Voronoi::distanceBetweenTwoPoint(Point A, Point B)
{
	float distance;
	distance = std::sqrt(std::pow(B.getX() - A.getX(), 2) + std::pow(B.getY() - A.getY(), 2));
	return distance;
}

void Voronoi::traceCercle(Point center, float radius)
{

	

	;
	
}


void Voronoi::flippingEdge()
{
	std::vector<Line*> listEdge;

}


std::vector<Point*> Voronoi::intersectionBetweenCircle()
{
	std::vector<Point*> listOfIntersection;
	float xValue, yValue;
	double A, B, C ,N;
	double delta;
	float xFinalA, xFinalB;
	float yFinalA, yFinalB;

	xValue = m_actualPoint[0]->getX() - m_actualPoint[1]->getX();
	yValue = m_actualPoint[0]->getY() - m_actualPoint[1]->getY();
	//http://math.15873.pagesperso-orange.fr/IntCercl.html a regarder pour comprendre

	N = ((-std::pow(m_actualPoint[1]->getX(), 2)) 
		+ std::pow(m_actualPoint[0]->getX(), 2) 
		- (std::pow(m_actualPoint[1]->getY(), 2)) 
		+ (std::pow(m_actualPoint[0]->getY(), 2))) 
		/ (2 * yValue);

	A = std::pow((xValue / yValue), 2) + 1;

	B = (2 * m_actualPoint[0]->getY() * (xValue / yValue))
		- (2 * N * (xValue / yValue)) 
		- (2 * m_actualPoint[0]->getX());

	C = std::pow(m_actualPoint[0]->getX(), 2) 
		+ std::pow(m_actualPoint[0]->getY(), 2) 
		+ std::pow(N, 2) 
		- std::pow(m_actualRadius[0], 2) 
		- (2 * m_actualPoint[0]->getY() * N);


	delta = std::sqrt(std::pow(B, 2) - (4 * A * C));


	xFinalA = (-B + delta) / (2 * A);
	xFinalB = (-B - delta) / (2 * A);

	yFinalA = N - xFinalA * (xValue / yValue);
	yFinalB = N - xFinalB * (xValue / yValue);
	
	listOfIntersection.push_back(new Point(xFinalA, yFinalA));
	listOfIntersection.push_back(new Point(xFinalB, yFinalB));
	m_intersect.push_back(new Point(xFinalA, yFinalA));
	m_intersect.push_back(new Point(xFinalB, yFinalB));
	return listOfIntersection;
}