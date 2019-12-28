#pragma once
#include "SDK.h"

class Math
{
public:
	void SinCos(float radians, float * sine, float * cosine);
	void AngleVectors(const Vector & angles, Vector * forward);
	void AngleVectors(const QAngle & angles, Vector * forward);
	void CalcAngle(Vector src, Vector dst, Vector & angles);
	void VectorTransform(const Vector in1, const matrix3x4 in2, Vector & out);
	void VectorTransformS(const Vector & vSome, const matrix3x4 & vMatrix, Vector & vOut);
};
extern Math gMath;

