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

	BasicVertex vertices[6];
	BasicVertex::make_rectangle(vertices,1.0f,1.0f);

	draw_object.Load(vertices,6);
	draw_object.Texturize(dtostr(data,"texture"));

	ani=new Animation();
	ani->CreateFromFile("resource_player.ani");
	ani->SetAnimationState("Idle");

	draw_object.SetAnimation(ani);

	dead=false;
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

	if (dead) {
		ani->SetAnimationState("Dead");
		phys_object->SetFixedRotation(false);
		return;
	}

	float x,y,angle;
	float speed;

	x=phys_object->GetPosition().x;
	y=phys_object->GetPosition().y;
	angle=phys_object->GetAngle();
	std::string ani_state=ani->GetAnimationState();
	b2Vec2 velo=phys_object->GetLinearVelocity();

	speed=sqrt(pow(velo.x,2) + pow(velo.y,2));

	b2World* world=GLPT_physics->GetWorld();

	// Use quick raycasting to see if there's anything below us.

	NearestCallback cast_cb;

	world->RayCast(&cast_cb,b2Vec2(x-0.31f,y-1.01f),b2Vec2(x-0.31f,y-1.1f));
	world->RayCast(&cast_cb,b2Vec2(x+0.31f,y-1.01f),b2Vec2(x+0.31f,y-1.1f));
	world->RayCast(&cast_cb,b2Vec2(x,y-1.01f),b2Vec2(x,y-1.2f));

	if (cast_cb.hit) {

		// Dead checking.
		//if (((phys_object->GetAngle()>=3.141f && phys_object->GetAngle()<=3.141f*2.0f) || (phys_object->GetAngle()<=3.141f/-2.0f && phys_object->GetAngle()>=-3.141f*3.0f/2.0f))) {
			//phys_object->SetLinearVelocity(b2Vec2(0,0));
			//dead=true;
		//}

		phys_object->SetFixedRotation(true);

		if (GLPT_input->KD(VK_UP) && (ani_state=="Walking" || ani_state=="Idle") && velo.y==0.0f) {
			ani->SetAnimationState("BeginJump");
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
		if (!GLPT_input->KD(VK_LEFT) && !GLPT_input->KD(VK_RIGHT)){
			ani->SetAnimationState("Idle");
		}
	} else {
		if (velo.y<0) {
			ani->SetAnimationState("EndJump");
		} else {
			ani->SetAnimationState("BeginJump");
		}
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