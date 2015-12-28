#ifndef LEVELGEN_H
#define LEVELGEN_H

#pragma once

#include <Urho3D/Core/Object.h>
#include "../game/CustomGeo.h"

using namespace Urho3D;

class LevelGen : public Object
{

    URHO3D_OBJECT(LevelGen,Object);

public:

   	LevelGen(Context* context);
    	~LevelGen();
    
    	virtual void Setup(Node* node);

private:

	SharedPtr<Node> node_;
	SharedPtr<CustomGeo> customgeo_;
};

#endif