#include <Urho3D/Urho3D.h>
#include <Urho3D/Scene/Scene.h>

#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/IndexBuffer.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/VertexBuffer.h>
#include <Urho3D/Resource/ResourceCache.h>

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

	if(p.x_>bbmax_.x_)bbmax_.x_=p.x_;
	if(p.y_>bbmax_.y_)bbmax_.y_=p.y_;
	if(p.z_>bbmax_.z_)bbmax_.z_=p.z_;
	if(p.x_<bbmin_.x_)bbmin_.x_=p.x_;
	if(p.y_<bbmin_.y_)bbmin_.y_=p.y_;
	if(p.z_<bbmin_.z_)bbmin_.z_=p.z_;
}

void CustomGeo::AddTriangle(const unsigned p1, const unsigned p2, const unsigned p3)
{
	ids_.Push(p1);
	ids_.Push(p2);
	ids_.Push(p3);
	normals_.Push(Normal(points_[p1],points_[p2],points_[p3]));
}

void CustomGeo::Build(Node* node)
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

	Node* newnode = node_->GetScene()->CreateChild("FromScratchObject");
	newnode->SetPosition(Vector3(0.0f, 3.0f, 0.0f));
	StaticModel* object = newnode->CreateComponent<StaticModel>();
	object->SetModel(fromScratchModel);
}

Vector3 CustomGeo::Normal(const Vector3& p1, const Vector3& p2, const Vector3& p3)
{
	Vector3 edge1 = p1 - p2;
           Vector3 edge2 = p1 - p3;
	return edge1.CrossProduct(edge2).Normalized();
}

void CustomGeo::Debug(const String label, const String value){
    GetSubsystem<DebugHud>()->SetAppStats(label+" : ", value );
}