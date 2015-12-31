#include <Urho3D/Urho3D.h>

#include <Urho3D/Graphics/Camera.h>
//#include <Urho3D/Engine/Engine.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Resource/XMLFile.h>

#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Physics/CollisionShape.h>

#include <Urho3D/Graphics/Animation.h>
#include <Urho3D/Graphics/AnimatedModel.h>
#include <Urho3D/Graphics/AnimationState.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/Graphics/Zone.h>

#include "ChmupDebugLevel.h"

#include "LevelGen.h"
#include "../framework/src/SimplexNoise.h"
#include "../game/Character.h"
#include "../game/Gun01.h"

#include <Urho3D/DebugNew.h>
#include <Urho3D/IO/Log.h>


ChmupDebugLevel::ChmupDebugLevel(Context* context) :
    Object(context)
{
}

//-------------------
//-------------------
void ChmupDebugLevel::Setup(SharedPtr<Scene> scene, SharedPtr<Node> cameraNode)
{

    //scene_ = scene;
    cameraNode_ = cameraNode;

    ResourceCache* cache = GetSubsystem<ResourceCache>();

    //saves zones for later when i know what to do with them

    //Node* zoneNode = scene_->CreateChild("Zone");
    //Zone* zone = zoneNode->CreateComponent<Zone>();
    //zone->SetBoundingBox(BoundingBox(-1000.0f, 1000.0f));
    //zone->SetAmbientColor(Color(0.15f, 0.15f, 0.15f));
    //zone->SetFogColor(Color(0.5f, 0.5f, 0.7f));
    //zone->SetFogStart(20.0f);
    //zone->SetFogEnd(60.0f);

    // Create a directional light to the world. Enable cascaded shadows on it
    

    //------
    characterNode_ = scene->CreateChild("Jack");
    Character* character_ = characterNode_->CreateComponent<Character>();
    character_->Setup();
    //create another node for the weapon
    Node* weaponNode = characterNode_->CreateChild("Weapon");
    weaponNode->SetPosition(Vector3(0.5f, 1.0f, 0.0f));
    Gun01* weapon = weaponNode->CreateComponent<Gun01>();
    weapon->Setup();
    character_->EquipWeapon(weapon);
    //cameraNode_->SetPosition(Vector3(0.0f, 5.0f, 0.0f));

    //light attached to the character
    Node* light_ = characterNode_->CreateChild("SpotLight");
    //light_->SetDirection( Vector3(0.8f, -1.0f, 0.2f).Normalized() );
    Light* light = light_->CreateComponent<Light>();
    //light->SetLightType(LIGHT_DIRECTIONAL);
    light->SetLightType(LIGHT_SPOT);
    light->SetBrightness(10.0f);
    light->SetCastShadows(true);
    light->SetShadowBias(BiasParameters(0.00025f, 0.5f));
    // Set cascade splits at 10, 50 and 200 world units, fade shadows out at 80% of maximum shadow distance
    light->SetShadowCascade(CascadeParameters(10.0f, 50.0f, 200.0f, 0.0f, 0.8f));
    light->SetRange(100.0f);
    light->SetFov(90.0f);
    light->SetAspectRatio(1.75f);

    Node* light2_ = characterNode_->CreateChild("SpotLight");
    light2_->SetPosition(Vector3(0.0f,1.0f,1.0f));
    //light_->SetDirection( Vector3(0.8f, -1.0f, 0.2f).Normalized() );
    Light* light2 = light2_->CreateComponent<Light>();
    light2->SetLightType(LIGHT_POINT);

    //lets test some simplex noise
    /*SimplexNoise* noise = new SimplexNoise(context_);
    Node* b = scene_->CreateChild("boxes");
    for (unsigned i = 0; i<100; ++i)
    {
        Node* nb = b->CreateChild("b");

        //just spread them around
        //Vector2 npos = noise->Gradient( Vector2(float(i)*0.33,0.0f) ,3.3f,0.3f,0.0f); 
        //nb->SetPosition(Vector3(npos.x_,0.0f,npos.y_)*5.0f);


        //lets make a grid and see what noise does to them
        Vector3 newpos = Vector3(float(i%10),0.0f,float(i/10))*1.5f;
        Vector2 npos = noise->Gradient( Vector2(newpos.x_,newpos.z_) ,0.01f,0.12f,0.0f);
        Vector3 npos3 = Vector3(npos.x_,0.0f,npos.y_);
        nb->SetPosition(newpos+npos3);

        StaticModel* boxObject = nb->CreateComponent<StaticModel>();
        boxObject->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
        boxObject->SetMaterial(cache->GetResource<Material>("Materials/Stone.xml"));
        boxObject->SetCastShadows(true);
    }*/

    ////////testing the level gen code
    LevelGen* level_ = new LevelGen(context_);
    Node* lnode = scene->CreateChild("levelgen");
    level_->Setup(lnode,cameraNode_); 

    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(ChmupDebugLevel, HandleUpdate));
}

void ChmupDebugLevel::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    //In here, we query the camera position, and determine if wee need to geneate new tiles, splits etc, while removing old ones
    /*Vector3 char_pos = character_->GetWorldPosition();

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
    //URHO3D_LOGWARNING(String(t.z_));*/
}