#include "Delaunay.h"


Delaunay::Delaunay()
{
}


Delaunay::~Delaunay()
{
}

void Delaunay::TriangulationDelaunay(std::vector<Edge> A)
{
	std::vector<Edge> Ac;
	std::list<Edge> AL;
	Edge currentEdge;
	Ac = A;

	Edge A1;
	Edge A2;
	Edge A3;
	Edge A4;

	Point S1;
	Point S2;
	Point S3;
	Point S4;

	LineStrip T1;
	LineStrip T2;


	while (Ac.size() != 0)
	{
		AL.remove(currentEdge);
		//Critère de delaunay
		//if (DelaunayCriteria == false)
		//{
			// S1, S2, S3, S4, A1, A2, A3, A4, T1, T2
			//add point
			//AL = S3 , S4
			//T1 = A, A2, A1
			//T2 = A, A4, A3
			//addTop(A1);
			//addTop(A2);
			//addTop(A3);
			//addTop(A4);
		//}
	}
}

void Delaunay::Voronoi()
{
	//Algorithme Pre implementation
	std::vector<Edge> liste;
	//determiner une triangulation de Delaunay T de l'ensemble de points S
	TriangulationDelaunay(liste);
	
	//if ()

}
