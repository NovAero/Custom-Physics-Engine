#include "Transform.h"

Vec2 operator*(Vec2 v, Transform t)
{
	return v * t.rotation + t.translation;
}

Vec2& operator*=(Vec2& v, Transform t)
{
	v = v * t;
	return v;
}

Transform operator*(Transform t1, Transform t2)
{
	Transform newTransform;
	newTransform.rotation = t1.rotation * t2.rotation;
	newTransform.translation = t1.translation * t2.rotation + t2.translation;
	return newTransform;
}

Transform& operator*=(Transform& t1, Transform t2)
{
	t1.rotation *= t2.rotation;
	t1.translation *= t2.rotation;
	t1.translation += t2.translation;
	return t1;
}

Transform operator-(Transform t)
{
	Transform result;
	result.rotation = -t.rotation;
	result.translation = -t.translation;
	return result;
}
