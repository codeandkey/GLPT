#pragma once

#include "GLPT.h"

class Overlay : public Entity {
public:
	void EventCreate(EntityLoadData*);
	void EventDraw(void);

	void SetFadeData(float r, float g, float b, float a);
	void GetFadeData(float* r=NULL, float* g=NULL,float* b=NULL, float* a=NULL);
private:
	Shader* overlay_shader;
	Drawable<BasicVertex> drawable_handle;
	float _r,_g,_b,_a;
};