#include <Urho3D/Urho3D.h>
#include <Urho3D/Scene/Scene.h>

#include "LevelGen.h"
//#include "../framework/src/SimplexNoise.h"

LevelGen::LevelGen(Context* context):
    Object(context),
    tilescale_(5.0f)
{
}
LevelGen::~LevelGen(){}

//level gen needs 2 types of tiles
//straight tiles, and split tiles
//tiles stright tiles that come off of split tiles

void LevelGen::Setup(Node* node, const float tilescale)
{
	
    node_ = node;
    tilescale_ = tilescale;
    noise_ = new SimplexNoise(context_);

    Vector3 down = Vector3(0.0,-2.0f,0.0f);
    Vector3 forward = Vector3(0.0f,0.0f,tilescale_);

    tiles_.Push( Tile(down) );
    tiles_.Push( Tile(down+(forward*2.0f)) );
    tiles_.Push( Tile(down+(forward*4.0f)) );
    tiles_.Push( Tile(down+(forward*6.0f)) );
    tiles_.Push( Tile(down+(forward*8.0f)) );

    //node_->SetPosition( Vector3(0.0f,-10.0f,0.0f) );

}
//private
CustomGeo* LevelGen::Tile(const Vector3 offset)
{
    CustomGeo* cg = new CustomGeo(context_);

    const Vector3 pts[9]={
        Vector3(-tilescale_,0.0f,0.0f),
        Vector3(0.0f,0.0f,0.0f),
        Vector3(tilescale_,0.0f,0.0f),
        Vector3(-tilescale_,0.0f,tilescale_),
        Vector3(0.0f,0.0f,tilescale_),
        Vector3(tilescale_,0.0f,tilescale_),
        Vector3(-tilescale_,0.0f,tilescale_*2.0f),
        Vector3(0.0f,0.0f,tilescale_*2.0f),
        Vector3(tilescale_,0.0f,tilescale_*2.0f)
    };

    //Noise it up
    for (unsigned i = 0; i<9; ++i)
    {
        Vector3 nv = noise_->Gradient( pts[i] +offset,0.01f,0.12f,0.0f);
        cg->AddPoint(pts[i]+offset+nv);
    }

    cg->AddTriangle(0,3,1);
    cg->AddTriangle(1,3,4);

    cg->AddTriangle(1,4,2);
    cg->AddTriangle(2,4,5);
    
    cg->AddTriangle(3,6,4);
    cg->AddTriangle(4,6,7);
    
    cg->AddTriangle(4,7,5);
    cg->AddTriangle(5,7,8);
    
    nodes_.Push( node_->CreateChild("tile") );
    //nodes_[nodes_.Size()-1]->SetPosition(offset);

    cg->Build(nodes_[nodes_.Size()-1]);
    
    return cg;
}
CustomGeo* LevelGen::Split(const Vector3 offset)
{

}