#include <Urho3D/Urho3D.h>
#include <Urho3D/Scene/Scene.h>

#include "LevelGen.h"

LevelGen::LevelGen(Context* context):
    Object(context)
{
}
LevelGen::~LevelGen(){}

void LevelGen::Setup(Node* node)
{
	customgeo_ = new CustomGeo(context_);
	
	node_ = node;

	customgeo_->AddPoint(Vector3(0.0f,0.5f,0.0f));//0
	customgeo_->AddPoint(Vector3(0.5f,-0.5f,0.5f));//1
	customgeo_->AddPoint(Vector3(0.5f,-0.5f,-0.5f));//2
	customgeo_->AddPoint(Vector3(-0.5f,-0.5f,0.5f));//3
	customgeo_->AddPoint(Vector3(-0.5f,-0.5f,-0.5f));//4

	customgeo_->AddTriangle(0,1,2);
	customgeo_->AddTriangle(0,3,1);
	customgeo_->AddTriangle(0,4,3);
	customgeo_->AddTriangle(0,2,4);
	customgeo_->AddTriangle(2,1,3);
	customgeo_->AddTriangle(2,3,4);

	customgeo_->Build(node_);
}