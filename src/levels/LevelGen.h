#ifndef LEVELGEN_H
#define LEVELGEN_H

#pragma once

#include <Urho3D/Core/Object.h>
#include "../game/CustomGeo.h"
#include "../framework/src/SimplexNoise.h"

using namespace Urho3D;

class LevelGen : public Object
{

    URHO3D_OBJECT(LevelGen,Object);

public:

   	LevelGen(Context* context);
	~LevelGen();

	void Setup(Node* node, const float tilescale = 5.0f );

private:

	SharedPtr<Node> node_;
	PODVector<Node*> nodes_;//nodes to hold each tile
	PODVector<CustomGeo*> tiles_;

	CustomGeo* Tile(const Vector3 offset = Vector3() );
	CustomGeo* Split(const Vector3 offset = Vector3() );

	float tilescale_;

	SharedPtr<SimplexNoise> noise_;
};

#endif