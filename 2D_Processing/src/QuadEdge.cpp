#include "QuadEdge.h"

QuadEdge* QuadEdge::makeEdge(Point* orig, Point* dest) {
	QuadEdge* q0 = new QuadEdge(nullptr, nullptr, orig);
	QuadEdge* q1 = new QuadEdge(nullptr, nullptr, nullptr);
	QuadEdge* q2 = new QuadEdge(nullptr, nullptr, dest);
	QuadEdge* q3 = new QuadEdge(nullptr, nullptr, nullptr);

	// create the segment
	q0->setOnext(q0);
	q2->setOnext(q2); // lonely segment: no "next" quadedge
	q1->setOnext(q3);
	q3->setOnext(q1); // in the dual: 2 communicating facets

	// dual switch
	q0->setRot(q1);
	q1->setRot(q2);
	q2->setRot(q3);
	q3->setRot(q0);

	return q0;
}

void QuadEdge::splice(QuadEdge* a, QuadEdge* b) {
	QuadEdge* alpha = a->getOnext()->getRot();
	QuadEdge* beta = b->getOnext()->getRot();

	QuadEdge* t1 = b->getOnext();
	QuadEdge* t2 = a->getOnext();
	QuadEdge* t3 = beta->getOnext();
	QuadEdge* t4 = alpha->getOnext();

	a->setOnext(t1);
	b->setOnext(t2);
	alpha->setOnext(t3);
	beta->setOnext(t4);
}

QuadEdge* QuadEdge::connect(QuadEdge* a, QuadEdge* b) {
	QuadEdge* q = makeEdge(a->dest(), b->dest());
	splice(q, a->lnext());
	splice(q->sym(), b);
	return q;
}

void QuadEdge::swapEdge(QuadEdge* e) {
	QuadEdge* a = e->oprev();
	QuadEdge* b = e->sym()->oprev();
	splice(e, a);
	splice(e->sym(), b);
	splice(e, a->lnext());
	splice(e->sym(), b->lnext());
	e->setOrig(a->dest());
	e->sym()->setOrig(b->dest());
}

void QuadEdge::deleteEdge(QuadEdge* a) {
	splice(a, a->oprev());
	splice(a->sym(), a->sym()->oprev());
}

bool QuadEdge::isOnLine(QuadEdge* e, Point* p) {
	// test if the vector product is zero
	if((p->getX() - e->getOrig()->getX())*(p->getY() - e->dest()->getY()) == (p->getY() - e->getOrig()->getY())*(p->getX() - e->dest()->getX())) {
		return true;
	}
	return false;
}

bool QuadEdge::isAtRightOf(QuadEdge* a, Point* p) {
	bool result = isCounterClockwise(p, a->dest(), a->getOrig());
	std::cout << "Le point " << *p << (result ? " est " : " n'est pas ") << "a droite du segment " << *(a->getOrig()) << " - " << *(a->dest()) << std::endl;
	return result;
}

bool QuadEdge::isCounterClockwise(Point* a, Point* b, Point* c) {
	// test the sign of the determinant of ab x cb
	if((a->getX() - b->getX())*(b->getY() - c->getY()) > (a->getY() - b->getY())*(b->getX() - c->getX())) {
		return true;
	}
	return false;
}

bool QuadEdge::inCircle(Point* a, Point* b, Point* c, Point* d) {
	/*
	if "d" is strictly INSIDE the circle, then

	|d² dx dy 1|
	|a² ax ay 1|
	det |b² bx by 1| < 0
	|c² cx cy 1|

	*/
	long a2 = a->getX()*a->getX() + a->getY()*a->getY();
	long b2 = b->getX()*b->getX() + b->getY()*b->getY();
	long c2 = c->getX()*c->getX() + c->getY()*c->getY();
	long d2 = d->getX()*d->getX() + d->getY()*d->getY();

	long det44 = 0;
	det44 += d2  * det33(a->getX(), a->getY(), 1, b->getX(), b->getY(), 1, c->getX(), c->getY(), 1);
	det44 -= d->getX() * det33(a2, a->getY(), 1, b2, b->getY(), 1, c2, c->getY(), 1);
	det44 += d->getY() * det33(a2, a->getX(), 1, b2, b->getX(), 1, c2, c->getX(), 1);
	det44 -= 1 * det33(a2, a->getX(), a->getY(), b2, b->getX(), b->getY(), c2, c->getX(), c->getY());

	if(det44 < 0) return true;
	return false;
}

bool QuadEdge::det33(long m0, long m1, long m2, long m3, long m4, long m5, long m6, long m7, long m8) {
	long det33 = 0;
	det33 += m0 * (m4 * m8 - m5 * m7);
	det33 -= m1 * (m3 * m8 - m5 * m6);
	det33 += m2 * (m3 * m7 - m4 * m6);
	return det33;
}

