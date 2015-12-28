#ifndef CUSTOMGEO_H
#define CUSTOMGEO_H

#pragma once

#include <Urho3D/Core/Object.h>

using namespace Urho3D;

class CustomGeo : public Object
{

    URHO3D_OBJECT(CustomGeo,Object);

public:

   	CustomGeo(Context* context);
	~CustomGeo();

	virtual void AddPoint(const Vector3 p);
	virtual void AddTriangle(const unsigned p1, const unsigned p2, const unsigned p3);
	virtual void Build(Node* node);
private:
	Vector3 Normal(const Vector3& p1, const Vector3& p2, const Vector3& p3);
	void Debug(const String label, const String value);

	SharedPtr<Node> node_;

	PODVector<Vector3> points_;
	PODVector<unsigned> ids_;
	PODVector<Vector3> normals_;

	Vector3 bbmin_;
	Vector3 bbmax_;
};

#endif