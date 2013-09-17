#include "GLPT.h"
#include "Player.h"

void Player::EventCreate(EntityLoadData* data) {
	SetIdentity("ins_player");

	if (!data) {
		PostWarning("Player created with no initial data. Destroying.");
		Destroy();
		return;
	}

	PhysBodyDesc player_body_desc;

	player_body_desc.angle=dtof(data,"angle");
	player_body_desc.dynamic=true;
	player_body_desc.height=1.0f;
	player_body_desc.width=0.7f;
	player_body_desc.weight=0.3f;
	player_body_desc.x=dtof(data,"x");
	player_body_desc.y=dtof(data,"y");

	phys_object=GLPT_physics->CreateBody(this,player_body_desc);

	BasicVertex vertices[6];
	BasicVertex::make_rectangle(vertices,1.0f,1.0f);

	draw_object.Load(vertices,6);

	ani=new Animation();
	ani->CreateFromFile("resource_player.ani");
	ani->SetAnimationState("Idle");

	draw_object.SetAnimation(ani);

	dead=false;
	initial_position=phys_object->GetPosition();
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

	phys_object->SetFixedRotation(true);

	bool player_can_jump=false;
	bool player_can_move=false;

	b2Vec2 player_position=phys_object->GetPosition();
	b2Vec2 player_velocity=phys_object->GetLinearVelocity();
	b2Vec2 player_dimension=b2Vec2(0.7f,1.0f);

	float player_decel_rate=1.06;

	// player_can_jump assignment. Use physics raycasting and callbacks to determine.
	// Exclude self.

	float min_jump_distance=0.04f;

	b2Vec2 callback_point_1_1=b2Vec2(player_position.x,player_position.y-player_dimension.y);
	b2Vec2 callback_point_1_2=b2Vec2(player_position.x,player_position.y-player_dimension.y-min_jump_distance);

	b2Vec2 callback_point_2_1=b2Vec2(player_position.x+player_dimension.x,player_position.y-player_dimension.y-min_jump_distance);
	b2Vec2 callback_point_2_2=b2Vec2(player_position.x-player_dimension.x,player_position.y-player_dimension.y-min_jump_distance);

	NearestCallback player_jump_callback;
	GLPT_physics->GetWorld()->RayCast(&player_jump_callback,callback_point_1_1,callback_point_1_2);
	
	if (player_jump_callback.hit) player_can_jump=true;

	GLPT_physics->GetWorld()->RayCast(&player_jump_callback,callback_point_2_1,callback_point_2_2);

	if (player_jump_callback.hit) player_can_jump=true;

	if (player_velocity.y!=0) player_can_jump=false;

	// Done with 'player_can_jump'.
	// Now, check for 'player_can_move'.

	// For now, we can let the player move in midair. Remember to use 'player_can_jump', and just slow down the movement.

	player_can_move=true;

	// Now, for literal movement and animation controls.

	// Jumping.
	if (GLPT_input->KD(VK_UP) && player_can_jump) {
		phys_object->SetLinearVelocity(b2Vec2(player_velocity.x,7));
		player_velocity=phys_object->GetLinearVelocity();
		ani->SetAnimationState("BeginJump");

		player_can_jump=false;
	}

	// LR Movement.

	if (player_can_move) {
		if (GLPT_input->KD(VK_RIGHT)) {
			player_velocity.x=max(player_velocity.x,(player_can_jump ? 6 : 3.5f));
			phys_object->SetLinearVelocity(player_velocity);
			draw_object.Flip(false);
			if (player_can_jump) ani->SetAnimationState("Walking");
		}
		if (GLPT_input->KD(VK_LEFT)) {
			player_velocity.x=min(player_velocity.x,(player_can_jump ? -6 : -3.5f));
			phys_object->SetLinearVelocity(player_velocity);
			draw_object.Flip(true);
			if (player_can_jump) ani->SetAnimationState("Walking");
		}
	}

	// Animation control.

	if (!GLPT_input->KD(VK_LEFT) && !GLPT_input->KD(VK_RIGHT) &&
	ani->GetAnimationState()=="Walking" && player_can_jump) {
		ani->SetAnimationState("Idle");
	}

	if (!GLPT_input->KD(VK_LEFT) && !GLPT_input->KD(VK_RIGHT) &&
	ani->GetAnimationState()=="Idle" && player_can_jump) {
		player_velocity.x/=player_decel_rate;
		phys_object->SetLinearVelocity(player_velocity);
	}

	if (ani->GetAnimationState()=="EndJump" && player_can_jump) {
		ani->SetAnimationState("Idle");
	}

	if (ani->GetAnimationState()=="BeginJump" && player_velocity.y < 0) {
		ani->SetAnimationState("EndJump");
	}
}

void Player::EventDestroy(void) {

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