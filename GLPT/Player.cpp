#include "GLPT.h"
#include "Player.h"

void Player::EventCreate(EntityLoadData* data) {
	SetIdentity("ins_player");

	if (!data) {
		PostWarning("Player created with no initial data. Destroying.");
		Destroy();
		return;
	}

	phys_object=GLPT_physics->CreateBody(true,.3f,1.0f,dtof(data,"x"),dtof(data,"y"),0.0f,1.0f);
	phys_object->SetFixedRotation(true);

	BasicVertex vertices[6];
	BasicVertex::make_rectangle(vertices,1.0f,1.0f);

	draw_object.Load(vertices,6);
	draw_object.Texturize(dtostr(data,"texture"));

	ani=new Animation();
	ani->CreateFromFile("resource_player.ani");
	ani->SetAnimationState("Idle");

	draw_object.SetAnimation(ani);
}

void Player::EventDraw(void) {

	static float x=0.0f,y=0.0f,angle=0.0f;

	x=phys_object->GetPosition().x;
	y=phys_object->GetPosition().y;
	angle=phys_object->GetAngle();

	draw_object.Draw(x,y,angle);

}

void Player::EventStep(void) {
	// Movement control here.

	float x,y,angle;
	float speed;

	x=phys_object->GetPosition().x;
	y=phys_object->GetPosition().y;
	angle=phys_object->GetAngle();
	b2Vec2 velo=phys_object->GetLinearVelocity();

	speed=sqrt(pow(velo.x,2) + pow(velo.y,2));

	b2World* world=GLPT_physics->GetWorld();

	if (!GLPT_input->KD(VK_LEFT) && !GLPT_input->KD(VK_RIGHT)){
		ani->SetAnimationState("Idle");
	}

	// Use quick raycasting to see if there's anything below us.

	NearestCallback cast_cb;

	world->RayCast(&cast_cb,b2Vec2(x-0.3f,y-1.0f),b2Vec2(x-0.3f,y-1.05f));
	world->RayCast(&cast_cb,b2Vec2(x+0.3f,y-1.0f),b2Vec2(x+0.3f,y-1.05f));

	if (cast_cb.hit) {
		if (GLPT_input->KD(VK_UP)) {
			phys_object->ApplyForceToCenter(b2Vec2(0,500));
		}
		if (GLPT_input->KD(VK_LEFT)) {
			if (speed<5) phys_object->ApplyForceToCenter(b2Vec2(-10,0));
			draw_object.Flip(true);
			ani->SetAnimationState("Walking");
		}
		if (GLPT_input->KD(VK_RIGHT)) {
			if (speed<5) phys_object->ApplyForceToCenter(b2Vec2(10,0));
			draw_object.Flip(false);
			ani->SetAnimationState("Walking");
		}
	} else {

	}

}

void Player::GetPosition(float* x,float* y) {

	b2Vec2 pos=phys_object->GetPosition();

	if (x) {
		*x=pos.x;
	}

	if (y) {
		*y=pos.y;
	}

}