#include "Overlay.h"
#include "Game.h"

void Overlay::EventCreate(EntityLoadData*) {
	SetIdentity("global_ent_overlay");
	SetEventDepth(-10);

	overlay_shader=new Shader;
	overlay_shader->Initialize("entity_overlay.hlsl",BasicVertex::layout,BasicVertex::layout_size);

	BasicVertex vertices[6];
	BasicVertex::make_rectangle(vertices,10.0f,10.0f);

	drawable_handle.Load(vertices,6,overlay_shader);

	//SetFadeData(0.0f,0.0f,0.0f,0.0f);
}

void Overlay::EventDraw(void) {
	float camera_x=0.0f,camera_y=0.0f,camera_z=-6.0f;

	GameCamera* camera=(GameCamera*) GLPT_iterator->GetByIdentity("global_ent_gamecamera");
	if (camera) {
		camera->GetPosition(&camera_x,&camera_y,&camera_z);
	}

	drawable_handle.Draw(camera_x,camera_y,0.0f,camera_z+5.0f);
}

void Overlay::SetFadeData(float r, float g, float b, float a) {

	_r=r;
	_g=g;
	_b=b;
	_a=a;

	overlay_shader->SetVariant("over_r",r);
	overlay_shader->SetVariant("over_g",g);
	overlay_shader->SetVariant("over_b",b);
	overlay_shader->SetVariant("over_a",a);
}

void Overlay::GetFadeData(float* r,float* g, float* b, float* a) {
	if (r) *r=_r;
	if (g) *g=_g;
	if (b) *b=_b;
	if (a) *a=_a;
}