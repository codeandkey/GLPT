#include "Physbox.h"
void Physbox::EventCreate(EntityLoadData* data) {
	if (!data) {
		PostWarning("Physbox created with no initial data. Destroying.");
		Destroy();
		return;
	}

	phys_object=GLPT_physics->CreateBody(true,.3f,1.0f,dtof(data,"x"),dtof(data,"y"),0.0f,1.0f);

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