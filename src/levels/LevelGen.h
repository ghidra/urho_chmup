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

	void Setup(Node* node, Node* cameraNode, const float tilescale = 20.0f );

private:

	SharedPtr<Node> node_;
	SharedPtr<Node> cameraNode_;
	PODVector<Node*> nodes_;//nodes to hold each tile
	PODVector<CustomGeo*> tiles_;

	void HandleUpdate(StringHash eventType, VariantMap& eventData);

	CustomGeo* Grid(const Vector3 ws_offset = Vector3(0.0f,0.0f,0.0f) );
	CustomGeo* Split(const Vector3 offset = Vector3(0.0f,0.0f,0.0f) );
	void Tile(CustomGeo* cg, Vector3 ws_offset, Vector3 offset, const unsigned short quad=0, const int wall=0);

	float tilescale_;
	unsigned short maxsegments_;//the max number of segments
	unsigned short segments_;//segment count

	SharedPtr<SimplexNoise> noise_;
};

#endif