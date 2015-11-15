#include "Triangulation2D_qcq.h"

bool myComparator(Point* a, Point* b) {
	return (a->getX() != b->getX() ? a->getX() < b->getX() : a->getY() > b->getY());
}


void Triangulation2D_qcq::computeTriangulation() {
	if(_points.empty()) {
		return;
	}

	// On trie les points par x croissant (y croissant si égalité (décroissant pour nous ? vu quec'ets inverse))
	std::sort(_points.begin(), _points.end(), myComparator); // 12 32 45 71(26 33 53 80)

	// Initialisation avec 2-triangulation initial (seulement si y'a des points aliggnés ? wtf)

	// Itération

}