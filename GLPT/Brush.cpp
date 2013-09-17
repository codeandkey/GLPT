#include "Brush.h"

void Brush::EventCreate(EntityLoadData* dat) {

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

	ani->SetAnimationState("Grass");

	float x,y,angle=0.0f;

	x=phys_object->GetPosition().x;
	y=phys_object->GetPosition().y;
	angle=phys_object->GetAngle();

	draw_object.Draw(x,y,angle);

}