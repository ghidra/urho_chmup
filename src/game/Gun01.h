#ifndef GUN01_H
#define GUN01_H

#pragma once

#include "../framework/src/Weapon.h"

using namespace Urho3D;

namespace Urho3D
{

class Node;
class Scene;

}

enum ProjectileType
{
    PT_FIREBALL,
    PT_BLASTER,
    PT_MAX = PT_BLASTER
};

class Gun01 : public Weapon
{

    URHO3D_OBJECT(Gun01,Weapon);

public:

    Gun01(Context* context);
    ~Gun01();
    
    static void RegisterObject(Context* context);

    //virtual void Start();
    //virtual void FixedUpdate(float timeStep);
    virtual void Setup();
    virtual void Fire(float timestep);
    virtual void ReleaseFire();

    void SetProjectileRate(const unsigned short rate);//number of projectiles at once
    void SetRotation(const float speed, const float range, const float offset=0.0f);
    void SetRotationSpeed(const float speed);
    void SetRotationRange(const float range);
    void SetProjectileSpeed(const float speed);
    void SetProjectileRange(const float range);
    void SetProjectileType(const unsigned type);

protected:
    //virtual void HandleNodeCollision(StringHash eventType, VariantMap& eventData);
    virtual void Recoil( const Vector3 dir );
    virtual void SpawnProjectile();

private:

    unsigned short numProjectiles_;
    float rotationTimer_;//to increment a timer for rotation
    Quaternion rotation_;//the angle where we left off on rotation
    
    float rotationSpeed_;//speed to rotate at
    float rotationRange_;//allowable rotation range
    float projectileSpeed_;
    float projectileRange_;

    ProjectileType projectileType_;

};
#endif