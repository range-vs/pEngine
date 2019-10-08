﻿#pragma once

#include "Plane3.h"

namespace gmath
{
	plane_3::plane_3(const Vector3 & p0, const Vector3 & p1, const Vector3 & p2)
	{
		Vector3 u = p1 - p0;
		Vector3 v = p2 - p0;
		Vector3 n = u.cross(v);
		n.normalize();
		createPlaneNormalAndPoint(p0, n);
	}

	plane_3::plane_3(const Vector3 & p0, const Vector3 & n)
	{
		createPlaneNormalAndPoint(p0, n);
	}

	plane_3::plane_3(const plane_3 & p)
	{
		pl = p.pl;
	}

	plane_3 & plane_3::operator=(const plane_3 & p)
	{
		if (this != &p)
		{
			pl = p.pl;
		}
		return *this;
	}

	void plane_3::setNormal(const Vector3 & n)
	{
		pl[vx] = n[vx];
		pl[vy] = n[vy];
		pl[vz] = n[vz];
	}

	void plane_3::setD(float d)
	{
		pl[vw] = d;
	}

	void plane_3::normalize()
	{
		float norm = sqrt(pl[vx] * pl[vx] + pl[vy] * pl[vy] + pl[vz] * pl[vz]);
		if (norm)
		{
			pl[vx] /= norm;
			pl[vy] /= norm;
			pl[vz] /= norm;
			pl[vw] /= norm;
		}
		else
		{
			pl[vx] = 0.0f;
			pl[vy] = 0.0f;
			pl[vz] = 0.0f;
			pl[vw] = 0.0f;
		}
	}

	void plane_3::createPlaneNormalAndPoint(const Vector3& p0, const Vector3& n)
	{
		pl[vx] = n[vx];
		pl[vy] = n[vy];
		pl[vz] = n[vz];
		pl[vw] = -Vector3::dot(p0, n);
	}

	float plane_3::shortDistanseFromPointToPlane(const Point & p0)
	{
		return pl[vx]*p0[vx] + pl[vy]*p0[vy] + pl[vz]*p0[vz] + pl[vw] * 1; // если расстрояние до плоскости < 0 - точка сзади(вектор смотрит от точки), иначе точка спереди(векто смотрит на точку),
		// == 0 - точка на плоскости
	}

	Vector3 plane_3::getNormal()
	{
		return (Vector3)pl;
	}

	float & plane_3::getD()
	{
		return pl[vw];
	}

	Vector & plane_3::getPlane()
	{
		return pl;
	}

}