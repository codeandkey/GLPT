#include "Physbox.h"
void Physbox::EventCreate(EntityLoadData* data) {
	if (!data) {
		PostWarning("Physbox created with no initial data. Destroying.");
		Destroy();
		return;
	}

	PhysBodyDesc brush_body_desc;

	brush_body_desc.angle=dtof(data,"angle");
	brush_body_desc.dynamic=true;
	brush_body_desc.height=1.0f;
	brush_body_desc.width=1.0f;
	brush_body_desc.weight=0.3f;
	brush_body_desc.x=dtof(data,"x");
	brush_body_desc.y=dtof(data,"y");

	phys_object=GLPT_physics->CreateBody(this,brush_body_desc);

	BasicVertex vertices[6];
	BasicVertex::make_rectangle(vertices,1.0f,1.0f);

	ani.CreateFromFile("resource_physbox.ani");

	draw_object.Load(vertices,6);
	draw_object.Texturize("physbox/" + dtostr(data,"texture"));
	//draw_object.SetAnimation(&ani);
}

void Physbox::EventDraw(void) {

	static float x=0.0f,y=0.0f,angle=0.0f;

	x=phys_object->GetPosition().x;
	y=phys_object->GetPosition().y;
	angle=phys_object->GetAngle();

	draw_object.Draw(x,y,angle);

}

void Physbox::GetPosition(float* x,float* y) {

	b2Vec2 pos=phys_object->GetPosition();

	if (x) {
		*x=pos.x;
	}

	if (y) {
		*y=pos.y;
	}

}