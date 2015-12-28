#include <Urho3D/Urho3D.h>
#include <Urho3D/Scene/Scene.h>

#include "LevelGen.h"

#include <Urho3D/DebugNew.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Engine/DebugHud.h>

LevelGen::LevelGen(Context* context):
    Object(context)
{
}
LevelGen::~LevelGen(){}

void LevelGen::Setup(Node* node)
{
	customgeo_ = new CustomGeo(context_);
	
	node_ = node;

	/*
	0.0f, 0.5f, 0.0f
        	0.5f, -0.5f, 0.5f
        	0.5f, -0.5f, -0.5f

        	0.0f, 0.5f, 0.0f
        	-0.5f, -0.5f, 0.5f
        	0.5f, -0.5f, 0.5f

        	0.0f, 0.5f, 0.0f
        	-0.5f, -0.5f, -0.5f
        	-0.5f, -0.5f, 0.5f

        	0.0f, 0.5f, 0.0f
        	0.5f, -0.5f, -0.5f
        	-0.5f, -0.5f, -0.5f

        	0.5f, -0.5f, -0.5f
        	0.5f, -0.5f, 0.5f
        	-0.5f, -0.5f, 0.5f

        	0.5f, -0.5f, -0.5f
        	-0.5f, -0.5f, 0.5f
        	-0.5f, -0.5f, -0.5f
        	*/

	customgeo_->AddPoint(Vector3(0.0f,0.5f,0.0f));//0
	customgeo_->AddPoint(Vector3(0.5f,-0.5f,0.0f));//1
	customgeo_->AddPoint(Vector3(0.5f,-0.5f,-0.5f));//2
	customgeo_->AddPoint(Vector3(-0.5f,-0.5f,0.5f));//3
	customgeo_->AddPoint(Vector3(0.5f, -0.5f, 0.5f));//4
	customgeo_->AddPoint(Vector3(-0.5f, -0.5f, -0.5f));//5

	customgeo_->AddTri(0,1,2);
	customgeo_->AddTri(0,3,4);
	customgeo_->AddTri(0,5,3);
	customgeo_->AddTri(0,2,5);
	customgeo_->AddTri(2,4,3);
	customgeo_->AddTri(2,3,5);

	customgeo_->Setup(node_);
}