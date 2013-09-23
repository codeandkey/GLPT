#include "Brush.h"
#include "Player.h"

void Brush::EventCreate(EntityLoadData* dat) {
	SetEventDepth(0);


	if (!dat) {
		PostWarning("Trying to create a brush without initialization data! Destroying.");
		Destroy();
		return;
	}

	PhysBodyDesc phys_desc;
	
	phys_desc.angle=dtof(dat,"angle");
	phys_desc.dynamic=false;
	phys_desc.weight=1.0f;
	phys_desc.width=dtof(dat,"width");
	phys_desc.height=dtof(dat,"height");
	phys_desc.x=dtof(dat,"x");
	phys_desc.y=dtof(dat,"y");

	phys_object=GLPT_physics->CreateBody(this,phys_desc);

	BasicVertex vertices[6];

	BasicVertex::make_rectangle(vertices,phys_desc.width,phys_desc.height);
	ani=new Animation;
	ani->CreateFromFile("resource_brush.ani");

	draw_object.Load(vertices,6);
	draw_object.SetAnimation(ani);

	ani->SetAnimationState(dtostr(dat,"texture"));
}

void Brush::EventDraw(void) {

	float x,y,angle=0.0f;
	static Player* player_object=(Player*) GLPT_iterator->GetByIdentity("ins_player");

	x=phys_object->GetPosition().x;
	y=phys_object->GetPosition().y;
	angle=phys_object->GetAngle();

	if (player_object) {
		float player_x,player_y;

		player_object->GetPosition(&player_x,&player_y);


		if (sqrt(pow(player_x-x,2)+pow(player_y-y,2)) >  30.0f) {
			return;
		}
	} else {
		player_object=(Player*) GLPT_iterator->GetByIdentity("ins_player");
	}

	draw_object.Draw(x,y,angle);

}