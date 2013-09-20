#include "Library.h"
#include "Physics.h"
#include "Iterator.h"

Physics* GLPT_physics;

PhysBody Physics::CreateBody(Entity* caller,PhysBodyDesc desc) {

	b2BodyDef body_def;
	PhysBody body_new;
	b2PolygonShape body_shape;

	body_def.position.Set(desc.x,desc.y);
	body_def.angle=desc.angle;
	body_def.type=(desc.dynamic) ? b2_dynamicBody : b2_staticBody;
	body_def.userData=caller;

	body_new=world_handle->CreateBody(&body_def);

	body_shape.SetAsBox(desc.width,desc.height);
	body_new->CreateFixture(&body_shape,desc.weight / (desc.width*desc.height*4));

	return body_new;
}

Physics::Physics(void) {

	b2Vec2 gravity(0.0f,-13.0f);

	world_handle=new b2World(gravity);
	world_handle->SetAllowSleeping(false);

}

void Physics::UpdateWorld(void) {

	float32 timestep;
	int ite,pos;

	timestep=1.0f/60.0f;

	ite=8;
	pos=3;
	
	world_handle->Step(timestep,ite,pos);

}

void Physics::DestroyBody(PhysBody body) {
	world_handle->DestroyBody(body);
}

float32 NearestCallback::ReportFixture(b2Fixture* fixture,const b2Vec2& point,const b2Vec2& normal,float32 fraction) {
	hit=true;

	hit_fraction=fraction;
	hit_point=point;
	hit_normal=normal;

	return fraction;
}