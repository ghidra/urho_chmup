#include <Urho3D/Urho3D.h>
#include <Urho3D/Scene/Scene.h>

#include "LevelGen.h"
//#include "../framework/src/SimplexNoise.h"
#include <Urho3D/IO/Log.h>

LevelGen::LevelGen(Context* context):
    Object(context),
    tilescale_(20.0f)
{
}
LevelGen::~LevelGen(){}

//level gen needs 2 types of tiles
//straight tiles, and split tiles
//tiles stright tiles that come off of split tiles

//as it stands now, each "tile" is 1 batch draw. I need to try and ft as much geo into one batch as I can

void LevelGen::Setup(Node* node, const float tilescale)
{
	
    node_ = node;
    tilescale_ = tilescale/4.0f;
    noise_ = new SimplexNoise(context_);

    Vector3 down = Vector3(0.0,-4.0f,0.0f);
    Vector3 forward = Vector3(0.0f,0.0f,tilescale_);

    tiles_.Push( Grid(down) );
    tiles_.Push( Grid(down+(forward*4.0f)) );
    tiles_.Push( Grid(down+(forward*8.0f)) );
    tiles_.Push( Grid(down+(forward*12.0f)) );
    tiles_.Push( Grid(down+(forward*16.0f)) );
    tiles_.Push( Grid(down+(forward*20.0f)) );
    tiles_.Push( Grid(down+(forward*24.0f)) );
    tiles_.Push( Grid(down+(forward*28.0f)) );
    tiles_.Push( Grid(down+(forward*32.0f)) );


    //node_->SetPosition( Vector3(0.0f,-10.0f,0.0f) );

}
//private
CustomGeo* LevelGen::Grid(const Vector3 offset)
{
    CustomGeo* cg = new CustomGeo(context_);

    Tile(cg,offset+Vector3(-tilescale_,0.0f,0.0f));//floor
    Tile(cg,offset+Vector3(tilescale_,0.0f,0.0f),1);
    Tile(cg,offset+Vector3(-tilescale_,0.0f,tilescale_*2.0f),2);
    Tile(cg,offset+Vector3(tilescale_,0.0f,tilescale_*2.0f),3);

    Tile(cg,offset+Vector3(-tilescale_*2.0f,0.0f,0.0f),4,-1);//wall
    Tile(cg,offset+Vector3(tilescale_*2.0f,0.0f,0.0f),5,1);//wall
    Tile(cg,offset+Vector3(-tilescale_*2.0f,0.0f,tilescale_*2.0f),6,-1);
    Tile(cg,offset+Vector3(tilescale_*2.0f,0.0f,tilescale_*2.0f),7,1);

    nodes_.Push( node_->CreateChild("tile") );

    cg->Build(nodes_[nodes_.Size()-1],true,32,63);
    
    return cg;
}
CustomGeo* LevelGen::Split(const Vector3 offset)
{

}
void LevelGen::Tile(CustomGeo* cg, Vector3 offset, const unsigned short quad, const int wall)
{
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

    float w;
    Quaternion rot;

    //deal with the walls
    if(wall!=0)
    {
        w = float(wall);//turn this to a float to multiply agaist for rotation and wall offset
        rot = Quaternion(45.0f,Vector3(0.0f,0.0f,w));
    }

    for (unsigned i = 0; i<9; ++i)
    {
        Vector3 pt = pts[i];
        if(wall!=0)
        {
            //rotate if we are a wall
            pt = rot*(pts[i]+(Vector3(tilescale_,0.0f,0.0f)*w) );
        }
        Vector3 large = noise_->Gradient( pt +offset,0.1f,0.005f,5.0f)*Vector3(75.0f,2.0f,25.0f);
        Vector3 medium = noise_->Gradient( pt +offset,0.01f,0.12f,0.0f)*Vector3(1.0f,2.0f,1.0f);
        cg->AddPoint(pt+offset+large+medium);
    }

    unsigned short ido = quad*9;
    cg->AddTriangle(0+ido,3+ido,1+ido);
    cg->AddTriangle(1+ido,3+ido,4+ido);

    cg->AddTriangle(1+ido,4+ido,5+ido);
    cg->AddTriangle(5+ido,2+ido,1+ido);
    
    cg->AddTriangle(3+ido,6+ido,7+ido);
    cg->AddTriangle(7+ido,4+ido,3+ido);
    
    cg->AddTriangle(4+ido,7+ido,5+ido);
    cg->AddTriangle(5+ido,7+ido,8+ido);
}