#include "Jarvis.h"

Jarvis::Jarvis(std::vector<Point> const points) {
	m_points = points;
}

float length(Point PiPj) {
	return sqrt((PiPj.getY() * PiPj.getY()) + (PiPj.getX() * PiPj.getX()));
}

float angle(Point v, Point PiPj) {
	std::cout << "Angle de " << v.getX() << ", " << v.getY() << " avec " << PiPj.getX() << ", " << PiPj.getY() << " : ";
	v.setX(v.getX() / length(v));
	v.setY(v.getY() / length(v));
	PiPj.setX(PiPj.getX() / length(PiPj));
	PiPj.setY(PiPj.getY() / length(PiPj));

	float dot = v.getX() * PiPj.getX() + v.getY() * PiPj.getY();
	float det = v.getX() * PiPj.getX() - v.getY() * PiPj.getY();

	//float angle = atan2(PiPj.getY(), PiPj.getX()) - atan2(v.getY(), v.getX());
	//float angle = atan2(PiPj.getX(), PiPj.getY()) - atan2(v.getX(), v.getY());

	float angle = atan2(det, dot);
	if(angle < 0)
		angle += 3.14;

	std::cout << angle * (180 / 3.14) << std::endl;
	// L'angle est dans le sens horaire
	return angle;
}

void Jarvis::computeJarvis() {
	Point start = m_points.at(0);
	int index = 0;

	for(int i = 1; i < m_points.size(); i++) {
		Point point = m_points.at(i);
		if(point.getX() < start.getX() || (point.getX() == start.getX() && point.getY() < start.getY())) {
			index = i;
			start = point;
		}
	}

	Point v(0, -1);
	std::vector<Point> poly;
	int i = index;

	do {
		poly.push_back(m_points.at(i));
		int j;
		if(i == 0) {
			j = 1;
		}
		else {
			j = 0;
		}

		// PiPj est un vecteur (x = distance en x, y = distance en y)
		Point PiPj(m_points.at(j).getX() - m_points.at(i).getX(), m_points.at(j).getY() - m_points.at(i).getY());

		float amin = angle(v, PiPj);
		float lmax = length(PiPj);
		int inew = j;

		//Recheche du point le plus proche (en angle) de la droite
		for(j = inew + 1; j < m_points.size(); j++) {
			if(j != i) {
				Point PiPj2(m_points.at(j).getX() - m_points.at(i).getX(), m_points.at(j).getY() - m_points.at(i).getY());
				float a = angle(v, PiPj2);
				if(amin > a || (amin == a && lmax < length(PiPj2))) {
					amin = a;
					lmax = length(PiPj2);
					inew = j;
				}
			}
		}

		// Mise à jour du pivot directeur
		v.setX(m_points.at(inew).getX() - m_points.at(i).getX());
		v.setY(m_points.at(inew).getY() - m_points.at(i).getY());
		i = inew;
	} while(i != index);

	//poly.push_back(m_points.at(index));
	m_enveloppe = poly;
}

void Jarvis::addPoint(Point& const point) {
	m_points.push_back(point);

	computeJarvis();
}