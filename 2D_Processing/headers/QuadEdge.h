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

	QuadEdge* getOnext() const { return _onext; }
	void setOnext(QuadEdge* val) { _onext = val; }
	QuadEdge* getRot() const { return _rot; }
	void setRot(QuadEdge* val) { _rot = val; }
	Point* getOrig() const { return _orig; }
	void setOrig(Point* val) { _orig = val; }
	bool isMarked() const { return _mark; }
	void setMarked(bool val) { _mark = val; }

	QuadEdge* sym() { return getRot()->getRot(); }
	Point* dest() { return sym()->getOrig(); }
	QuadEdge* rotSym() { return getRot()->sym(); }
	QuadEdge* oprev() { return getRot()->getOnext()->getRot(); }
	QuadEdge* dprev() { return rotSym()->getOnext()->rotSym(); }
	QuadEdge* lnext() { return rotSym()->getOnext()->getRot(); }
	QuadEdge* lprev() { return getOnext()->sym(); }

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