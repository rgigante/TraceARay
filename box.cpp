//
//  box.cpp
//  TraceARay
//
//  Created by Riccardo Gigante on 10/02/2020.
//

#include "box.hpp"

Box::Box(char const* name, const Vec3 & pt1, const Vec3 & pt2, Material const * mat, bool visible /*= true*/)
{
	SetPoints(pt1, pt2);
	SetName(name);
	SetVisible(visible);
	SetMaterial(mat);
//	if (GetBBox())
//		GetBBox()->SetPoints(pt1, pt2);
}

void Box::UpdateBoxCenterAndRadius()
{
	_c = (_a + _b) / 2;
	_r = _a - _c;
}


void Box::SetPoints(const Vec3 & pt1, const Vec3 & pt2)
{
	for (int i = 0; i < 3; i ++)
	{
		_a[i] = pt1[i] > pt2[i] ? pt1[i] : pt2[i];
		_b[i] = pt1[i] > pt2[i] ? pt2[i] : pt1[i];
	}
	
	UpdateBoxCenterAndRadius();
}

void Box::SetCenterAndRadius(const Vec3 & c, const Vec3 & r)
{
	_c = c;
	_r = r;
	_a = _c + _r;
	_b = _c - _r;
}

bool Box::HitPrimitive (const Ray& r, float t_min, float t_max, HitRecord& rec, bool debugRay /*= false*/)
{
	rec.mat = GetMaterial();
	
	const Vec3 rpos = r.GetOrigin();
	const Vec3 rdir = r.GetDirection();
	
	Vec3 invRayDir;
	
	//check ray dir
	invRayDir[0] = abs(rdir[0]) >= _dirThreshold ? 1 / rdir[0] : _invDirThreshold;
	invRayDir[1] = abs(rdir[1]) >= _dirThreshold ? 1 / rdir[1] : _invDirThreshold;
	invRayDir[2] = abs(rdir[2]) >= _dirThreshold ? 1 / rdir[2] : _invDirThreshold;
	
	
	Vec3 bRpos = _b - rpos;
	// check that ray position and bounding box don't overlap on each axis
	// if overlap make the term of the delta slightly positive
	Vec3 aRpos = _a - rpos;
	// check that ray position and bounding box don't overlap on each axis
	// if overlap make the term of the delta slightly negative
	// adjust by the given pos threshold
	for (int i = 0; i < 3; i++)
	{
		aRpos[i] = abs(aRpos[i]) < _posThreshold ? _posThreshold : aRpos[i];
		bRpos[i] = abs(bRpos[i]) < _posThreshold ?  _posThreshold : bRpos[i];
		assert(abs(bRpos[i]) >= _posThreshold);
		assert(abs(aRpos[i]) >= -_posThreshold);
	}
	
	float smallest_tmax = 0, largest_tmin = 0;
	Vec3 tminValues, tmaxValues;
	
	// calculate intersections for x,y,z
	for (int i = 0; i < 3 ; i++)
	{
		float val_b = bRpos[i] * invRayDir[i];
		float val_a = aRpos[i] * invRayDir[i];
		// identify min and max values
		tminValues[i] = val_b < val_a ? val_b : val_a;
		tmaxValues[i] = val_b >= val_a ? val_b : val_a;
	}
	
	// identify
	if (tminValues[0] >= tminValues[1] && tminValues[0] >= tminValues[2])
		largest_tmin = tminValues[0];
	else if (tminValues[1] >= tminValues[0] && tminValues[1] >= tminValues[2])
		largest_tmin = tminValues[1];
	else // if (tminZ >= tminX && tminZ >= tminY)
		largest_tmin = tminValues[2];
	
	if (tmaxValues[0] <= tmaxValues[1] && tmaxValues[0] <= tmaxValues[2])
		smallest_tmax = tmaxValues[0];
	else if (tmaxValues[1] <= tmaxValues[0] && tmaxValues[1] <= tmaxValues[2])
		smallest_tmax = tmaxValues[1];
	else // if (tmaxZ <= tmaxX && tmaxZ <= tmaxY)
		smallest_tmax = tmaxValues[2];
	
	
	if (debugRay) std::cout << "\t\t\t\t- a/b: "<< _a << "/" << _b << "\n"
		<< "\t\t\t\t- c/r: "<< _c << "/" << _r << "\n"
		<< "\t\t\t\t- tmin(s):" << tminValues << "\n"
		<< "\t\t\t\t- tmax(s):" << tmaxValues << "\n"
		<< "\t\t\t\t- smaller_tmax:" << smallest_tmax <<" / larger_tmin:" << largest_tmin << "\n";

	if ((smallest_tmax >  largest_tmin) && (largest_tmin > t_min && largest_tmin < t_max))
	{
		rec.t = largest_tmin;
		rec.p = r.PointAtParameter(largest_tmin);
		const Vec3 recpa = rec.p - _a;
		const Vec3 recpb = rec.p - _b;
		rec.n = Vec3(0.0);
		for (int i = 0; i < 3; i++)
		{
			if (abs(recpa[i]) < _posThreshold)
				rec.n[i] = 1;
			if (abs(recpb[i]) < _posThreshold)
				rec.n[i] = -1;
		}
		return true;
	}
	
	return false;
}
