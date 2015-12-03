#include "QuadEdge.h"

QuadEdge* QuadEdge::makeEdge(Point* orig, Point* dest) {
	QuadEdge* q0 = new QuadEdge(nullptr, nullptr, orig);
	QuadEdge* q1 = new QuadEdge(nullptr, nullptr, nullptr);
	QuadEdge* q2 = new QuadEdge(nullptr, nullptr, dest);
	QuadEdge* q3 = new QuadEdge(nullptr, nullptr, nullptr);

	// create the segment
	q0->Onext(q0);
	q2->Onext(q2); // lonely segment: no "next" quadedge
	q1->Onext(q3);
	q3->Onext(q1); // in the dual: 2 communicating facets

	// dual switch
	q0->Rot(q1);
	q1->Rot(q2);
	q2->Rot(q3);
	q3->Rot(q0);

	return q0;
}

void QuadEdge::splice(QuadEdge* a, QuadEdge* b) {
	QuadEdge* alpha = a->Onext()->Rot();
	QuadEdge* beta = b->Onext()->Rot();

	QuadEdge* t1 = b->Onext();
	QuadEdge* t2 = a->Onext();
	QuadEdge* t3 = beta->Onext();
	QuadEdge* t4 = alpha->Onext();

	a->Onext(t1);
	b->Onext(t2);
	alpha->Onext(t3);
	beta->Onext(t4);
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
	e->Orig(a->dest());
	e->sym()->Orig(b->dest());
}

void QuadEdge::deleteEdge(QuadEdge* a) {
	splice(a, a->oprev());
	splice(a->sym(), a->sym()->oprev());
}

bool QuadEdge::isOnLine(QuadEdge* e, Point* p) {
	// test if the vector product is zero
	if((p->getX() - e->Orig()->getX())*(p->getY() - e->dest()->getY()) == (p->getY() - e->Orig()->getY())*(p->getX() - e->dest()->getX()))
		return true;
	return false;
}

bool QuadEdge::isAtRightOf(QuadEdge* a, Point* p) {
	return isCounterClockwise(p, a->dest(), a->Orig());

}

bool QuadEdge::isCounterClockwise(Point* a, Point* b, Point* c) {
	// test the sign of the determinant of ab x cb
	if(a != nullptr && b != nullptr && c != nullptr && (a->getX() - b->getX())*(b->getY() - c->getY()) > (a->getY() - b->getY())*(b->getX() - c->getX()))
		return true;
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

