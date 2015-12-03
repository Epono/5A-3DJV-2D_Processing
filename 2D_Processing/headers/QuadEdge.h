#pragma once

#include <vector>
#include "Point.h"

class QuadEdge {
private:
	QuadEdge* _onext;
	QuadEdge* _rot;
	Point* _orig;
	bool _mark = false;

private:
	QuadEdge(QuadEdge* onext, QuadEdge* rot, Point* orig) : _onext(onext), _rot(rot), _orig(orig) {}

public:

	QuadEdge* Onext() const { return _onext; }
	void Onext(QuadEdge* val) { _onext = val; }
	QuadEdge* Rot() const { return _rot; }
	void Rot(QuadEdge* val) { _rot = val; }
	Point* Orig() const { return _orig; }
	void Orig(Point* val) { _orig = val; }
	bool Mark() const { return _mark; }
	void Mark(bool val) { _mark = val; }

	QuadEdge* sym() { return _rot->Rot(); }
	Point* dest() { return sym()->Orig(); }
	QuadEdge* rotSym() { return _rot->sym(); }
	QuadEdge* oprev() { return _rot->_onext->_rot; }
	QuadEdge* dprev() { return rotSym()->_onext->rotSym(); }
	QuadEdge* lnext() { return rotSym()->_onext->_rot; }
	QuadEdge* lprev() { return _onext->sym(); }

	static QuadEdge* makeEdge(Point* orig, Point* dest);
	static void splice(QuadEdge* a, QuadEdge* b);
	static QuadEdge* connect(QuadEdge* a, QuadEdge* b);
	static void swapEdge(QuadEdge* e);
	static void deleteEdge(QuadEdge* a);

	static bool isOnLine(QuadEdge* e, Point* p);
	static bool isAtRightOf(QuadEdge* a, Point* p);
	static bool isCounterClockwise(Point* a, Point* b, Point* c);
	static bool inCircle(Point* a, Point* b, Point* c, Point* d);
	static bool det33(long m0, long m1, long m2, long m3, long m4, long m5, long m6, long m7, long m8);
};