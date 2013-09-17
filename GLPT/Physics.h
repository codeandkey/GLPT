#pragma once

#include "Library.h"
#include "Iterator.h"

class Physics;
typedef b2Body* PhysBody;
extern Physics* GLPT_physics;

struct PhysBodyDesc {
	float x,y,width,height;
	float angle;
	float weight;
	bool dynamic;
	PhysBodyDesc(void) {
		x=y=angle=0.0f;
		dynamic=true;
		width=height=1.0f;
	}
};

class Physics {
public:
	static void Create(void) {
		GLPT_physics=new Physics;
	}
	static void Release(void) {
		delete GLPT_physics;
	}
	PhysBody CreateBody(Entity*,PhysBodyDesc desc);
	void UpdateWorld(void);
	void DestroyBody(PhysBody body);
	b2World* GetWorld(void) {return world_handle;}
private:
	b2World* world_handle;
	Physics(void);
};

class NearestCallback : public b2RayCastCallback {
public:
	float32 ReportFixture(b2Fixture* fixture,const b2Vec2& point,const b2Vec2& normal,float32 fraction);
	NearestCallback(void) {hit=false;}
	bool hit;
	b2Vec2 hit_point;
	b2Vec2 hit_normal;
	float32 hit_fraction;
};
