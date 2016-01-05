#include <Urho3D/Urho3D.h>
#include <Urho3D/Scene/Scene.h>

#include <Urho3D/Core/Context.h>
//#include <Urho3D/Graphics/AnimatedModel.h>

#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Model.h>

#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Physics/PhysicsEvents.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Physics/RigidBody.h>

#include <Urho3D/Resource/ResourceCache.h>

#include "../framework/src/Projectile.h"

#include "Obstacle.h"

Obstacle::Obstacle(Context* context) :
    Actor(context)
{
    SetUpdateEventMask(USE_FIXEDUPDATE);
    mesh_ = String("Box.mdl");
    health_ = 1.0f;
    maxHealth_ = 1.0f;
    collision_layer_ = 16;
    collision_mask_ = 51;
}

//-------------------
//-------------------
void Obstacle::RegisterObject(Context* context)
{
    context->RegisterFactory<Obstacle>();
}

void Obstacle::Start()
{
    SubscribeToEvent(GetNode(), E_NODECOLLISION, URHO3D_HANDLER(Obstacle, HandleNodeCollision));
}

void Obstacle::Setup()
{
    Actor::Setup();//do the basic set up with stored and set values

    shape_ = node_->CreateComponent<CollisionShape>();
    //shape_->SetBox(Vector3(1.0f,1.0f,1.0f));
    shape_->SetSphere( 2.0f );

    body_->SetUseGravity(false);//dont use gravity
    body_->SetLinearFactor(Vector3(1.0f,0.0f,1.0f));
    body_->SetMass(30.0f);
}
//--------
void Obstacle::FixedUpdate(float timeStep)
{
    Actor::FixedUpdate(timeStep);
}
//----
void Obstacle::HandleNodeCollision(StringHash eventType, VariantMap& eventData)
{
    using namespace NodeCollision;

    RigidBody* otherBody = static_cast<RigidBody*>(eventData[P_OTHERBODY].GetPtr());
    Node* otherNode = static_cast<Node*>(eventData[P_OTHERNODE].GetPtr());

    Projectile* proj = otherNode->GetDerivedComponent<Projectile>();//this allows me to get a derived from pawn class component!
    if(proj != NULL)
    {
        //subtract damage from health
        health_-=proj->GetDamage();
    }
    //if health is low, destroy the obstacle
    if(health_<=0.0f)
    {
        //lets make some more nodes and cubes for debris
        ResourceCache* cache = GetSubsystem<ResourceCache>();
        Vector3 wp = node_->GetWorldPosition();
        Vector3 vel = otherBody->GetLinearVelocity();

        for(unsigned i=0; i<4+unsigned(Random(6));++i)
        {
            Node* d = node_->GetScene()->CreateChild("debris");
            Vector3 rp = Vector3( Random(-1.0f,1.0f), Random(-1.0f,1.0f), Random(-1.0f,1.0f) );
            Vector3 dir = vel.Normalized().Lerp( rp.Normalized(), 0.2);

            d->SetWorldPosition( Vector3(wp+rp) );

            StaticModel* sm = d->CreateComponent<StaticModel>();
            sm->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
            RigidBody* rb = d->CreateComponent<RigidBody>();
            CollisionShape* cs = d->CreateComponent<CollisionShape>();

            rb->SetUseGravity(true);
            cs->SetBox(Vector3(1.0f,1.0f,1.0f));
            d->SetScale(0.35f);
            rb->SetCollisionLayer(32);
            rb->SetCollisionMask(63);
            rb->SetMass(1.0f);
            //Obstacle* p = node->CreateComponent<Obstacle>();
            //p->Setup();
            //rb->SetEnabled(true);
            rb->ApplyImpulse(dir*Random(20.0f));
        }
        node_->Remove();
    }

}