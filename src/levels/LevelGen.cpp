#include <Urho3D/Urho3D.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Core/CoreEvents.h>

#include "LevelGen.h"
//#include "../framework/src/SimplexNoise.h"
#include <Urho3D/IO/Log.h>

LevelGen::LevelGen(Context* context):
    Object(context),
    tilescale_(20.0f),
    maxsegments_(9)
{
}
LevelGen::~LevelGen(){}

//level gen needs 2 types of tiles
//straight tiles, and split tiles
//tiles stright tiles that come off of split tiles

//as it stands now, each "tile" is 1 batch draw. I need to try and ft as much geo into one batch as I can

void LevelGen::Setup(Node* node, Node* cameraNode, const float tilescale)
{
	
    node_ = node;
    cameraNode_ = cameraNode;
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

    segments_ = 9;

    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(LevelGen, HandleUpdate));


    //node_->SetPosition( Vector3(0.0f,-10.0f,0.0f) );

}
//private
void LevelGen::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    //In here, we query the camera position, and determine if wee need to geneate new tiles, splits etc, while removing old ones
    Vector3 cam_pos = cameraNode_->GetWorldPosition();

    for (unsigned i = 0; i<tiles_.Size(); ++i)
    {
        Vector3 tile_pos = tiles_[i]->GetNode()->GetWorldPosition();
        if( tile_pos.z_ < cam_pos.z_-40.0f )
        {
            //remove one
            tiles_[i]->GetNode()->Remove();
            tiles_.Erase(i);
            //and make a new one
            tiles_.Push( Grid(Vector3(0.0,-4.0f,0.0f)+(Vector3(0.0f,0.0f,tilescale_)*(4.0f*float(segments_)))) );
            ++segments_;
        }
    }
    Vector3 t = tiles_[2]->GetNode()->GetWorldPosition();
    //URHO3D_LOGWARNING(String(t.z_));
}

CustomGeo* LevelGen::Grid(const Vector3 ws_offset)
{
    CustomGeo* cg = new CustomGeo(context_);

    Tile(cg,ws_offset,Vector3(-tilescale_,0.0f,0.0f));//floor
    Tile(cg,ws_offset,Vector3(tilescale_,0.0f,0.0f),1);
    Tile(cg,ws_offset,Vector3(-tilescale_,0.0f,tilescale_*2.0f),2);
    Tile(cg,ws_offset,Vector3(tilescale_,0.0f,tilescale_*2.0f),3);

    Tile(cg,ws_offset,Vector3(-tilescale_*2.0f,0.0f,0.0f),4,-1);//wall
    Tile(cg,ws_offset,Vector3(tilescale_*2.0f,0.0f,0.0f),5,1);//wall
    Tile(cg,ws_offset,Vector3(-tilescale_*2.0f,0.0f,tilescale_*2.0f),6,-1);
    Tile(cg,ws_offset,Vector3(tilescale_*2.0f,0.0f,tilescale_*2.0f),7,1);

    nodes_.Push( node_->CreateChild("tile") );
    nodes_[nodes_.Size()-1]->SetPosition(ws_offset);

    //cg->Subdivide();

    cg->Build(nodes_[nodes_.Size()-1],true,32,63);
    
    return cg;
}
CustomGeo* LevelGen::Split(const Vector3 offset)
{

}
////
//this makes a tile
//a grid is made up of 4 tiles on the floor, and 4 for walls, 2 on each side
void LevelGen::Tile(CustomGeo* cg, Vector3 ws_offset, Vector3 offset, const unsigned short quad, const int wall)
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
        Vector3 large = noise_->Gradient( pt +offset+ws_offset,5000.0f,0.005f,5.0f)*Vector3(15.0f,2.0f,5.0f);
        Vector3 medium = noise_->Gradient( pt +offset+ws_offset,0.01f,0.12f,0.0f)*Vector3(0.25f,0.25f,0.25f);
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