#include <Urho3D/Urho3D.h>
#include <Urho3D/Scene/Scene.h>

#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/IndexBuffer.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/VertexBuffer.h>
#include <Urho3D/Resource/ResourceCache.h>

#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Physics/CollisionShape.h>

#include "CustomGeo.h"

#include <Urho3D/DebugNew.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Engine/DebugHud.h>

CustomGeo::CustomGeo(Context* context):
    Object(context)
{
}
CustomGeo::~CustomGeo(){}

void CustomGeo::AddPoint(const Vector3 p){
	points_.Push(p);
	FitBB(p);
}

/*void CustomGeo::SetPoint(const unsigned short i, const Vector3 p)
{
	points_[i] = p;
	FitBB(p);
}*/

void CustomGeo::AddTriangle(const unsigned short p1, const unsigned short p2, const unsigned short p3)
{
	ids_.Push(p1);
	ids_.Push(p2);
	ids_.Push(p3);
	normals_.Push(Normal(points_[p1],points_[p2],points_[p3]));
}
//http://stackoverflow.com/questions/12662891/c-passing-member-function-as-argument
void CustomGeo::Surface(const unsigned short slices, const unsigned short stacks, Vector3 (*fptr)(void*, float, float), void* context)
{
	for(unsigned i=0; i<slices+1; ++i)
	{
		float theta = float(i)*3.1415/float(slices);
		for(unsigned j=0; j<stacks; ++j)
		{
			float phi = float(j)*2.0*3.1415/float(stacks);
			Vector3 p = fptr(context,theta,phi);
			AddPoint(p);
		}
	}
	unsigned short v = 0;
	for(unsigned i=0; i<slices; ++i)
	{
		for(unsigned j=0; j<stacks; ++j)
		{
			unsigned short next = (j+1)%stacks;
			AddTriangle(v+j,v+next,v+j+stacks);
			AddTriangle(v+next,v+next+stacks,v+j+stacks);
		}
		v+=stacks;
	}
}
void CustomGeo::Subdivide(const unsigned short depth){
	for(unsigned i=0; i<depth; ++i)
	{
		DoSubdivide();
	}
}
void CustomGeo::DoSubdivide(){
	unsigned tris = ids_.Size()/3;
	for(unsigned fi=0; fi<tris; ++fi)
	{
		//make new points
		unsigned fii = fi*3;
		Vector3 a = points_[ids_[fii]];
		Vector3 b = points_[ids_[fii+1]];
		Vector3 c = points_[ids_[fii+3]];
		points_.Push( a+(Vector3(b-a)*0.5f) );
		points_.Push( b+(Vector3(c-b)*0.5f) );
		points_.Push( c+(Vector3(a-c)*0.5f) );
		
		//add new ids
		unsigned i = points_.Size()-3;
		unsigned j = i+1;
		unsigned k = i+2;
		AddTriangle(i,j,k);
		AddTriangle(ids_[fii],i,k);
		AddTriangle(i,ids_[fii+1],j);

		ids_[fii]=k;
		ids_[fii+1]=j;
		normals_[fi] = Normal(points_[k],points_[j],points_[ids_[fii+2]]);

	}
}
void CustomGeo::Build(Node* node, const bool rigid, const unsigned layer, const unsigned mask)
{
	node_ = node;

	unsigned num = ids_.Size();
	const unsigned numVertices = num;
	float vertexData[num*6];
	unsigned short indexData[num];

	for(unsigned i = 0; i < numVertices; ++i)
	{
		unsigned ii = i*6;
		vertexData[ii] = points_[ids_[i]].x_;
		vertexData[ii+1] = points_[ids_[i]].y_;
		vertexData[ii+2] = points_[ids_[i]].z_;

		//the normals a poly based.. so I need to grab them in orger the polys were made
		vertexData[ii+3] = normals_[ids_[i/3]].x_;
		vertexData[ii+4] = normals_[ids_[i/3]].y_;
		vertexData[ii+5] = normals_[ids_[i/3]].z_;

		indexData[i]=i;
	}
	/////
	/////
	/////

	SharedPtr<Model> fromScratchModel(new Model(context_));
	SharedPtr<VertexBuffer> vb(new VertexBuffer(context_));
	SharedPtr<IndexBuffer> ib(new IndexBuffer(context_));
	SharedPtr<Geometry> geom(new Geometry(context_));

	// Shadowed buffer needed for raycasts to work, and so that data can be automatically restored on device loss
	vb->SetShadowed(true);
	vb->SetSize(numVertices, MASK_POSITION|MASK_NORMAL);
	vb->SetData(vertexData);

	ib->SetShadowed(true);
	ib->SetSize(numVertices, false);
	ib->SetData(indexData);

	geom->SetVertexBuffer(0, vb);
	geom->SetIndexBuffer(ib);
	geom->SetDrawRange(TRIANGLE_LIST, 0, numVertices);

	fromScratchModel->SetNumGeometries(1);
	fromScratchModel->SetGeometry(0, 0, geom);
	fromScratchModel->SetBoundingBox(BoundingBox(bbmin_, bbmax_));

	//Node* newnode = node_->GetScene()->CreateChild("FromScratchObject");
	//newnode->SetPosition(Vector3(0.0f, 3.0f, 0.0f));
	StaticModel* object = node_->CreateComponent<StaticModel>();
	object->SetModel(fromScratchModel);
	object->SetCastShadows(true);

	//make a triangle mesh rigid shape if flagged
	if(rigid)
	{
		RigidBody* body_ = node_->CreateComponent<RigidBody>();
    	body_->SetCollisionLayer(layer);
    	body_->SetCollisionMask(mask);
    	body_->SetMass(0.0f);
    	//body_->SetFriction(friction);
    	CollisionShape* shape_ = node_->CreateComponent<CollisionShape>();
    	shape_->SetTriangleMesh(fromScratchModel);
	}
}

void CustomGeo::FitBB(const Vector3 p)
{
	if(p.x_>bbmax_.x_)bbmax_.x_=p.x_;
	if(p.y_>bbmax_.y_)bbmax_.y_=p.y_;
	if(p.z_>bbmax_.z_)bbmax_.z_=p.z_;
	if(p.x_<bbmin_.x_)bbmin_.x_=p.x_;
	if(p.y_<bbmin_.y_)bbmin_.y_=p.y_;
	if(p.z_<bbmin_.z_)bbmin_.z_=p.z_;
}

Vector3 CustomGeo::Normal(const Vector3& p1, const Vector3& p2, const Vector3& p3)
{
	Vector3 edge1 = p1 - p2;
           	Vector3 edge2 = p1 - p3;
	return edge1.CrossProduct(edge2).Normalized();
}
///-----surfaces
Vector3 CustomGeo::Sphere(void* context, const float u, const float v)
{
	return Vector3::ZERO;
}
void CustomGeo::GetSphere(const unsigned short u, const unsigned short v)
{
	//Surface(u,v,&Sphere,0);
}
//------debug
void CustomGeo::Debug(const String label, const String value){
    GetSubsystem<DebugHud>()->SetAppStats(label+" : ", value );
}