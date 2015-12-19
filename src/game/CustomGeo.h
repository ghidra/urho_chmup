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
    
    	virtual void Setup(Node* node);
private:
	Vector3 Normal(const Vector3& p1, const Vector3& p2, const Vector3& p3);
	void Debug(const String label, const String value);

	SharedPtr<Node> node_;
};

#endif