#include <Urho3D/Urho3D.h>

#include <Urho3D/Engine/Application.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Engine/Console.h>
#include <Urho3D/UI/Cursor.h>
#include <Urho3D/Engine/DebugHud.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/UI/Sprite.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Core/Timer.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/Resource/XMLFile.h>


#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/Core/ProcessUtils.h>
#include <Urho3D/UI/Text.h>

#include "Main.h"
#include "framework/src/Controller.h"
#include "framework/src/State.h"
#include "framework/src/CameraLogic.h"
#include "game/Gun01.h"
#include "framework/src/Projectile.h"
#include "game/Character.h"


#include <Urho3D/DebugNew.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Engine/DebugHud.h>

#include <iostream>

URHO3D_DEFINE_APPLICATION_MAIN(Main)

Main::Main(Context* context) :
    ApplicationHandler(context)
{
    CameraLogic::RegisterObject(context);
    Character::RegisterObject(context);
    RagDoll::RegisterObject(context);
    Gun01::RegisterObject(context);
    Projectile::RegisterObject(context);
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

    characterNode_ = scene_->CreateChild("Jack");
    Character* character_ = characterNode_->CreateComponent<Character>();
    character_->GetNode()->SetPosition(Vector3(0.0f, 1.0f, 0.0f));
    character_->Setup();
    character_->Possess(applicationInput_);
    //character_->EquipWeapon();

    //----my first enemy
    //enemyNode_ = scene_->CreateChild("Jack");
    //Character* enemy_ = enemyNode_->CreateComponent<Character>();

    applicationInput_->SetCameraTarget(characterNode_);

    SubscribeToEvents();
}