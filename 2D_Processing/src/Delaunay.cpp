#include "Delaunay.h"

QuadEdge* Delaunay::locate(Point* x) {
	QuadEdge* e = _startingEdge;
	while(true) {
		// On regarde si le point existe deja
		if(x == e->getOrig() || x == e->dest()) {
			return e;
		}

		// walk
		if(QuadEdge::isAtRightOf(e, x)) {
			e = e->sym();
		}
		else if(!QuadEdge::isAtRightOf(e->getOnext(), x)) {
			e = e->getOnext();
		}
		else if(!QuadEdge::isAtRightOf(e->dprev(), x)) {
			e = e->dprev();
		}
		else {
			return e;
		}
	}
}

Delaunay::Delaunay() {
	_bottomLeft = new Point(0, 0);
	_bottomRight = new Point(WIDTH, 0);
	_topRight = new Point(WIDTH, HEIGHT);
	_topLeft = new Point(0, HEIGHT);

	QuadEdge* bottom = QuadEdge::makeEdge(_bottomLeft, _bottomRight);
	QuadEdge* right = QuadEdge::makeEdge(_bottomRight, _topRight);
	QuadEdge* top = QuadEdge::makeEdge(_topRight, _topLeft);
	QuadEdge* left = QuadEdge::makeEdge(_topLeft, _bottomLeft);

	QuadEdge::splice(bottom->sym(), right);
	QuadEdge::splice(right->sym(), top);
	QuadEdge::splice(top->sym(), left);
	QuadEdge::splice(left->sym(), bottom);

	_startingEdge = bottom;
}

void Delaunay::insertPoint(Point* newPoint) {
	QuadEdge* e = locate(newPoint);

	// On regarde si le point existe deja
	if(e->getOrig() != nullptr && newPoint->getX() == e->getOrig()->getX() && newPoint->getY() == e->getOrig()->getY())
		return;
	if(e->dest() != nullptr && newPoint->getX() == e->dest()->getX() && newPoint->getY() == e->dest()->getY())
		return;

	// Si le point est sur une edge, on supprime l'edge
	if(QuadEdge::isOnLine(e, newPoint) == 1) {
		e = e->oprev();
		//TODO:
		//_quadEdges.remove(e->Onext()->sym());
		//_quadEdges.remove(e->Onext());
		QuadEdge::deleteEdge(e->getOnext());
	}

	// On connecte le point aux sommets du triangle le contenant
	QuadEdge* base = QuadEdge::makeEdge(e->getOrig(), newPoint);
	_quadEdges.push_back(base);

	QuadEdge::splice(base, e);
	_startingEdge = base;
	do {
		base = QuadEdge::connect(e, base->sym());
		_quadEdges.push_back(base);
		e = base->oprev();
	} while(e->lnext() != _startingEdge);

	// On vérifie si la condition de Delaunay est vérifiée (pas d'autres points à l'intérieur du cercle circonscrit)
	while(true) {
		QuadEdge* t = e->oprev();

		if(QuadEdge::isAtRightOf(e, t->dest()) &&
		   QuadEdge::inCircle(e->getOrig(), t->dest(), e->dest(), newPoint)) {
			QuadEdge::swapEdge(e);
			e = e->oprev();
		}
		else if(e->getOnext() == _startingEdge)
			return;
		else
			e = e->getOnext()->lprev();
	}
}

std::vector<Line*> Delaunay::computeEdges() {
	std::vector<Line*> edges;
	for(QuadEdge* q : _quadEdges) {
		// Ignore le triangle global
		if(q->getOrig() == _bottomLeft || q->getOrig() == _bottomRight || q->getOrig() == _topRight || q->getOrig() == _topLeft)
			continue;
		if(q->dest() == _bottomLeft || q->dest() == _bottomRight || q->dest() == _topRight || q->dest() == _topLeft)
			continue;
		edges.push_back(new Line(q->getOrig(), q->dest()));
	}
	return edges;
}

std::vector<Triangle*> Delaunay::computeTriangles() {
	std::vector<Triangle*> triangles;

	// Ignore le triangle global en les marquant 
	for(QuadEdge* q : _quadEdges) {
		q->setMarked(false);
		q->sym()->setMarked(false);
		if(q->getOrig() == _bottomLeft || q->getOrig() == _bottomRight || q->getOrig() == _topRight || q->getOrig() == _topLeft)
			q->setMarked(true);
		if(q->dest() == _bottomLeft || q->dest() == _bottomRight || q->dest() == _topRight || q->dest() == _topLeft)
			q->sym()->setMarked(true);
	}

	// Process les 2 triangles de chaque QuadEdge
	for(QuadEdge* qe : _quadEdges) {
		QuadEdge* q1 = qe;
		QuadEdge* q2 = q1->lnext();
		QuadEdge* q3 = q2->lnext();
		// Si aucun des QuadEdge n'est marqué, on ajoute le triangle
		if(!q1->isMarked() && !q2->isMarked() && !q3->isMarked()) {
			triangles.push_back(new Triangle(q1->getOrig(), q2->getOrig(), q3->getOrig()));
		}

		QuadEdge* qsym1 = qe->sym();
		QuadEdge* qsym2 = qsym1->lnext();
		QuadEdge* qsym3 = qsym2->lnext();
		// Si aucun des QuadEdge n'est marqué, on ajoute le triangle
		if(!qsym1->isMarked() && !qsym2->isMarked() && !qsym3->isMarked()) {
			triangles.push_back(new Triangle(qsym1->getOrig(), qsym2->getOrig(), qsym3->getOrig()));
		}

		// Marque la QuadEdge comme ayant été process
		qe->setMarked(true);
		qe->sym()->setMarked(true);
	}

	return triangles;
}

std::vector<LineStrip*> Delaunay::computeVoronoi() {
	std::vector<LineStrip*> voronoi;

	// do not process edges pointing to/from surrouding triangle
	// --> mark them as already computed
	for(QuadEdge* q : _quadEdges) {
		q->setMarked(false);
		q->sym()->setMarked(false);
		if(q->getOrig() == _bottomLeft || q->getOrig() == _bottomRight || q->getOrig() == _topRight || q->getOrig() == _topLeft)
			q->setMarked(true);
		if(q->dest() == _bottomLeft || q->dest() == _bottomRight || q->dest() == _topRight || q->dest() == _topLeft)
			q->sym()->setMarked(true);
	}

	for(QuadEdge* qe : _quadEdges) {

		// walk throught left and right region
		for(int b = 0; b <= 1; b++) {
			QuadEdge* qstart = (b == 0) ? qe : qe->sym();
			if(qstart->isMarked())
				continue;

			// new region start
			LineStrip* poly = new LineStrip();

			// walk around region
			QuadEdge* qregion = qstart;
			while(true) {
				qregion->setMarked(true);

				// compute CircumCenter if needed
				if(qregion->getRot()->getOrig() == nullptr) {
					QuadEdge* q1 = qregion;
					Point* p0 = q1->getOrig();
					QuadEdge* q2 = q1->lnext();
					Point* p1 = q2->getOrig();
					QuadEdge* q3 = q2->lnext();
					Point* p2 = q3->getOrig();

					double ex = p1->getX() - p0->getX(), ey = p1->getY() - p0->getY();
					double nx = p2->getY() - p1->getY(), ny = p1->getX() - p2->getX();
					double dx = (p0->getX() - p2->getX())*0.5, dy = (p0->getY() - p2->getY())*0.5;
					double s = (ex*dx + ey*dy) / (ex*nx + ey*ny);
					double cx = (p1->getX() + p2->getX())*0.5 + s*nx;
					double cy = (p1->getY() + p2->getY())*0.5 + s*ny;

					Point* p = new Point(cx, cy);
					qregion->getRot()->setOrig(p);
				}

				poly->addPoint(qregion->getRot()->getOrig());

				qregion = qregion->getOnext();
				if(qregion == qstart)
					break;
			}

			// add region to output list
			voronoi.push_back(poly);
		}
	}
	return voronoi;
}