#include <Urho3D/Urho3D.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Zone.h>
#include <Urho3D/Scene/Scene.h>

#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Graphics/RenderPath.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputEvents.h>

#include "Main.h"
#include "framework/src/Controller.h"
#include "framework/src/State.h"
#include "framework/src/CameraLogic.h"
#include "framework/src/RagDoll.h"
#include "game/Gun01.h"
#include "framework/src/Projectile.h"
#include "game/Character.h"
#include "game/Obstacle.h"
#include "game/ObstacleDebris.h"
#include "game/PickUpGun.h"
#include "game/ProjectileFireball.h"
#include "game/ProjectileBlaster.h"
//#include "game/CustomGeo.h"

#include <Urho3D/DebugNew.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Engine/DebugHud.h>

URHO3D_DEFINE_APPLICATION_MAIN(Main)

Main::Main(Context* context) :
    ApplicationHandler(context)
{
    CameraLogic::RegisterObject(context);
    Character::RegisterObject(context);
    RagDoll::RegisterObject(context);
    Gun01::RegisterObject(context);
    Projectile::RegisterObject(context);
    ProjectileFireball::RegisterObject(context);
    ProjectileBlaster::RegisterObject(context);
    Obstacle::RegisterObject(context);
    ObstacleDebris::RegisterObject(context);
    PickUpGun::RegisterObject(context);
}
Main::~Main(){}

void Main::Start()
{
    // Execute base class startup
    ApplicationHandler::Start();

    applicationInput_ = new ApplicationInput(context_);
    SetApplicationInput(applicationInput_);//this also set up the basic camera
    applicationInput_->SetCameraType(String("shmup"));
    
    VariantMap camParms;
    camParms["targetOffset"] = Vector3(0.0f,0.0f,10.0f);
    camParms["distance"] = 50.0f;
    camParms["distance_max"] = 100.0f;
    camParms["orientation"] = Quaternion(80.0f,0.0f,0.0f);

    applicationInput_->SetCameraParameters(camParms);


    PhysicsWorld* pw = scene_->GetComponent<PhysicsWorld>();
    pw->SetGravity(Vector3(0.0f,-9.0f,0.0f));

    //----------------

    level_ = new ChmupDebugLevel(context_);
    level_->Setup(scene_, cameraNode_);

    //----------------

    //hook the controller up and set the camera to follow the character
    SharedPtr<Node> characterNode_ = level_->GetCharacterNode();
    Character* character_ = characterNode_->GetComponent<Character>();
    character_->Possess(applicationInput_);
    character_->GiveCameraNode(applicationInput_->GetCameraNode());
    applicationInput_->SetCameraTarget(characterNode_);

    ///---mess with default zone
    Renderer* renderer = GetSubsystem<Renderer>();
    Zone* dzone = renderer->GetDefaultZone();
    //dzone->SetHeightFog(false);
    dzone->SetFogStart(20.0f);
    dzone->SetFogEnd(60.0f);

    SubscribeToEvents();

    //-----add in the renderpath
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Viewport* vp = GetSubsystem<Renderer>()->GetViewport(0);
    RenderPath* rp = vp->GetRenderPath();
    rp->Append(cache->GetResource<XMLFile>("RenderPaths/Glow.xml"));
    // Make the bloom mixing parameter more pronounced
    //effectRenderPath->SetShaderParameter("BloomMix", Vector2(0.9f, 0.6f));
    rp->SetEnabled("Glow", true);
    vp->SetRenderPath(rp);

    //custom keyhandling
    SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(Main, HandleKeyDown));
}

void Main::HandleKeyDown(StringHash eventType, VariantMap& eventData)
{
    Input* input = GetSubsystem<Input>();
    if (input->GetKeyPress('R'))
    {
        RenderPath* rp = GetSubsystem<Renderer>()->GetViewport(0)->GetRenderPath();
       rp->ToggleEnabled("Glow");
    }

}