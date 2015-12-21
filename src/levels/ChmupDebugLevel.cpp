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

#include "../framework/src/SimplexNoise.h"

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

    scene_ = scene;
    cameraNode_ = cameraNode;

    ResourceCache* cache = GetSubsystem<ResourceCache>();

    Node* zoneNode = scene_->CreateChild("Zone");
    Zone* zone = zoneNode->CreateComponent<Zone>();
    zone->SetBoundingBox(BoundingBox(-1000.0f, 1000.0f));
    zone->SetAmbientColor(Color(0.15f, 0.15f, 0.15f));
    zone->SetFogColor(Color(0.5f, 0.5f, 0.7f));
    zone->SetFogStart(100.0f);
    zone->SetFogEnd(300.0f);

    // Create a directional light to the world. Enable cascaded shadows on it
    Node* lightNode = scene_->CreateChild("DirectionalLight");
    lightNode->SetDirection(Vector3(0.6f, -1.0f, 0.8f));
    Light* light = lightNode->CreateComponent<Light>();
    light->SetLightType(LIGHT_DIRECTIONAL);
    light->SetCastShadows(true);
    light->SetShadowBias(BiasParameters(0.00025f, 0.5f));
    // Set cascade splits at 10, 50 and 200 world units, fade shadows out at 80% of maximum shadow distance
    light->SetShadowCascade(CascadeParameters(10.0f, 50.0f, 200.0f, 0.0f, 0.8f));

    {

        Node* floorNode = scene_->CreateChild("Floor");
        floorNode->SetPosition(Vector3(0.0f, -1.0f, 0.0f));
        floorNode->SetScale(Vector3(1000.0f, 1.0f, 1000.0f));
        StaticModel* floorObject = floorNode->CreateComponent<StaticModel>();
        floorObject->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
        floorObject->SetMaterial(cache->GetResource<Material>("Materials/StoneTiled.xml"));

        RigidBody* body = floorNode->CreateComponent<RigidBody>();
        CollisionShape* shape = floorNode->CreateComponent<CollisionShape>();
        body->SetCollisionLayer(32);
        body->SetCollisionMask(63);
        shape->SetBox(Vector3::ONE);
    }

    cameraNode_->SetPosition(Vector3(0.0f, 5.0f, 0.0f));

    //lets test some simplex noise
    SimplexNoise* noise = new SimplexNoise(context_);
    Node* b = scene_->CreateChild("boxes");
    for (unsigned i = 0; i<20; ++i)
    {
        Node* nb = b->CreateChild("b");
        Vector2 rx = Vector2(float(Random(999)),float(Random(999)));
        Vector2 rz = Vector2(float(Random(999)),float(Random(999)));
        Vector2 scl = Vector2(0.01f,0.01f);
        float nx = noise->Get(rx,scl);
        float nz = noise->Get(rz,scl);
        nb->SetPosition(Vector3(nx,0.0f,nz));

        StaticModel* boxObject = nb->CreateComponent<StaticModel>();
        boxObject->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
        boxObject->SetMaterial(cache->GetResource<Material>("Materials/Stone.xml"));
        boxObject->SetCastShadows(true);

        //URHO3D_LOGWARNING(String(   noise->Get(Vector2(float(Random(999)),float(Random(999))))  ));
        //URHO3D_LOGWARNING(String(rz));
        //URHO3D_LOGWARNING(String(Vector3(nx,0.0f,nz)));
    }

}