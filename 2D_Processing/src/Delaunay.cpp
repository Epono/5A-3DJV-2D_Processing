#include "Delaunay.h"

void Delaunay::updateBoundigBox(Point* p) {
	int minx = std::min(_minx, p->getX());
	int maxx = std::max(_maxx, p->getX());
	int miny = std::min(_miny, p->getY());
	int maxy = std::max(_maxy, p->getY());
	setBoundigBox(minx, miny, maxx, maxy);
}

QuadEdge* Delaunay::locate(Point* p) {
	// outside the bounding box ?
	if(p->getX() < _minx || p->getX() > _maxx || p->getY() < _miny || p->getY() > _maxy) {
		updateBoundigBox(p);
	}

	QuadEdge* e = _startingEdge;
	while(true) {
		// duplicate point ?
		//TODO:
		if(e->Orig() != nullptr && p->getX() == e->Orig()->getX() && p->getY() == e->Orig()->getY())
			return e;
		if(e->dest() != nullptr && p->getX() == e->dest()->getX() && p->getY() == e->dest()->getY())
			return e;

		// walk
		if(QuadEdge::isAtRightOf(e, p))
			e = e->sym();
		else if(!QuadEdge::isAtRightOf(e->Onext(), p))
			e = e->Onext();
		else if(!QuadEdge::isAtRightOf(e->dprev(), p))
			e = e->dprev();
		else
			return e;
	}
}

Delaunay::Delaunay() : _minx(FLT_MIN), _miny(FLT_MIN), _maxx(FLT_MAX), _maxy(FLT_MAX) {
	QuadEdge* ab = QuadEdge::makeEdge(_a, _b);
	QuadEdge* bc = QuadEdge::makeEdge(_b, _c);
	QuadEdge* cd = QuadEdge::makeEdge(_c, _d);
	QuadEdge* da = QuadEdge::makeEdge(_d, _a);

	QuadEdge::splice(ab->sym(), bc);
	QuadEdge::splice(bc->sym(), cd);
	QuadEdge::splice(cd->sym(), da);
	QuadEdge::splice(da->sym(), ab);

	_startingEdge = ab;
}

void Delaunay::setBoundigBox(float minx, float miny, float maxx, float maxy) {
	// update saved values
	_minx = minx;
	_maxx = maxx;
	_miny = miny;
	_maxy = maxy;

	// extend the bounding-box to surround min/max
	int centerx = (minx + maxx) / 2;
	int centery = (miny + maxy) / 2;
	int x_min = (int) ((minx - centerx - 1) * 10 + centerx);
	int x_max = (int) ((maxx - centerx + 1) * 10 + centerx);
	int y_min = (int) ((miny - centery - 1) * 10 + centery);
	int y_max = (int) ((maxy - centery + 1) * 10 + centery);

	// set new positions
	_a->setX(x_min);
	_a->setY(y_min);
	_b->setX(x_max);
	_b->setY(y_min);
	_c->setX(x_max);
	_c->setY(y_max);
	_d->setX(x_min);
	_d->setY(y_max);
}

void Delaunay::insertPoint(Point* p) {
	QuadEdge* e = locate(p);

	// point is a duplicate -> nothing to do
	if(p->getX() == e->Orig()->getX() && p->getY() == e->Orig()->getY())
		return;
	if(p->getX() == e->dest()->getX() && p->getY() == e->dest()->getY())
		return;

	// point is on an existing edge -> remove the edge
	if(QuadEdge::isOnLine(e, p)) {
		e = e->oprev();
		//TODO:
		//_quadEdges.remove(e->Onext()->sym());
		//_quadEdges.remove(e->Onext());
		QuadEdge::deleteEdge(e->Onext());
	}

	// Connect the new point to the vertices of the containing triangle
	// (or quadrilateral in case of the point is on an existing edge)
	QuadEdge* base = QuadEdge::makeEdge(e->Orig(), p);
	_quadEdges.push_back(base);

	QuadEdge::splice(base, e);
	_startingEdge = base;
	do {
		base = QuadEdge::connect(e, base->sym());
		_quadEdges.push_back(base);
		e = base->oprev();
	} while(e->lnext() != _startingEdge);

	// Examine suspect edges to ensure that the Delaunay condition is satisfied.
	do {
		QuadEdge* t = e->oprev();

		if(QuadEdge::isAtRightOf(e, t->dest()) &&
		   QuadEdge::inCircle(e->Orig(), t->dest(), e->dest(), p)) {
			// flip triangles
			QuadEdge::swapEdge(e);
			e = e->oprev();
		}
		else if(e->Onext() == _startingEdge)
			return; // no more suspect edges
		else
			e = e->Onext()->lprev();  // next suspect edge
	} while(true);
}

std::vector<std::vector<Point*>*> Delaunay::computeEdges() {
	std::vector<std::vector<Point*>*> edges;
	// do not return edges pointing to/from surrouding triangle
	for(QuadEdge* q : _quadEdges) {
		if(q->Orig() == _a || q->Orig() == _b || q->Orig() == _c || q->Orig() == _d) continue;
		if(q->dest() == _a || q->dest() == _b || q->dest() == _c || q->dest() == _d) continue;
		std::vector<Point*>* edgesTemp = new std::vector<Point*>;
		edgesTemp->push_back(q->Orig());
		edgesTemp->push_back(q->dest());
		edges.push_back(edgesTemp);
	}
	return edges;
}

std::vector<std::vector<Point*>*> Delaunay::computeTriangles() {
	std::vector<std::vector<Point*>*> triangles;

	// do not process edges pointing to/from surrouding triangle
	// --> mark them as already computed
	for(QuadEdge* q : _quadEdges) {
		q->Mark(false);
		q->sym()->Mark(false);
		if(q->Orig() == _a || q->Orig() == _b || q->Orig() == _c || q->Orig() == _d)
			q->Mark(true);
		if(q->dest() == _a || q->dest() == _b || q->dest() == _c || q->dest() == _d)
			q->sym()->Mark(true);
	}

	// compute the 2 triangles associated to each quadEdge
	for(QuadEdge* qe : _quadEdges) {
		// first triangle
		QuadEdge* q1 = qe;
		QuadEdge* q2 = q1->lnext();
		QuadEdge* q3 = q2->lnext();
		if(!q1->Mark() && !q2->Mark() && !q3->Mark()) {
			std::vector<Point*>* trianglesTemp = new std::vector<Point*>;
			trianglesTemp->push_back(q1->Orig());
			trianglesTemp->push_back(q2->Orig());
			trianglesTemp->push_back(q3->Orig());
			triangles.push_back(trianglesTemp);
		}

		// second triangle
		QuadEdge* qsym1 = qe->sym();
		QuadEdge* qsym2 = qsym1->lnext();
		QuadEdge* qsym3 = qsym2->lnext();
		if(!qsym1->Mark() && !qsym2->Mark() && !qsym3->Mark()) {
			std::vector<Point*>* trianglesTemp = new std::vector<Point*>;
			trianglesTemp->push_back(qsym1->Orig());
			trianglesTemp->push_back(qsym2->Orig());
			trianglesTemp->push_back(qsym3->Orig());
			triangles.push_back(trianglesTemp);
		}

		// mark as used
		qe->Mark(true);
		qe->sym()->Mark(true);
	}

	return triangles;
}

std::vector<std::vector<Point*>*> Delaunay::computeVoronoi() {
	std::vector<std::vector<Point*>*> voronoi;

	// do not process edges pointing to/from surrouding triangle
	// --> mark them as already computed
	for(QuadEdge* q : _quadEdges) {
		q->Mark(false);
		q->sym()->Mark(false);
		if(q->Orig() == _a || q->Orig() == _b || q->Orig() == _c || q->Orig() == _d)
			q->Mark(true);
		if(q->dest() == _a || q->dest() == _b || q->dest() == _c || q->dest() == _d)
			q->sym()->Mark(true);
	}

	for(QuadEdge* qe : _quadEdges) {

		// walk throught left and right region
		for(int b = 0; b <= 1; b++) {
			QuadEdge* qstart = (b == 0) ? qe : qe->sym();
			if(qstart->Mark())
				continue;

			// new region start
			std::vector<Point*> poly;

			// walk around region
			QuadEdge* qregion = qstart;
			while(true) {
				qregion->Mark(true);

				// compute CircumCenter if needed
				if(qregion->Rot()->Orig() == nullptr) {
					QuadEdge* q1 = qregion;
					Point* p0 = q1->Orig();
					QuadEdge* q2 = q1->lnext();
					Point* p1 = q2->Orig();
					QuadEdge* q3 = q2->lnext();
					Point* p2 = q3->Orig();

					double ex = p1->getX() - p0->getX(), ey = p1->getY() - p0->getY();
					double nx = p2->getY() - p1->getY(), ny = p1->getX() - p2->getX();
					double dx = (p0->getX() - p2->getX())*0.5, dy = (p0->getY() - p2->getY())*0.5;
					double s = (ex*dx + ey*dy) / (ex*nx + ey*ny);
					double cx = (p1->getX() + p2->getX())*0.5 + s*nx;
					double cy = (p1->getY() + p2->getY())*0.5 + s*ny;

					Point* p = new Point(cx, cy);
					qregion->Rot()->Orig(p);
				}

				poly.push_back(qregion->Rot()->Orig());

				qregion = qregion->Onext();
				if(qregion == qstart)
					break;
			}

			// add region to output list
			// TODO:
			//voronoi.add(poly.toArray(new Point[0]));
		}
	}
	return voronoi;
}