#pragma once

#include "GLPT.h"

struct PointLight {
	float x,y,r,g,b;
	float brightness;
	float radius;
};

class Radiosity : public Entity {
public:
	void EventCreate(EntityLoadData*);
	void EventDraw(void);

	int AddLight(PointLight i);
	void RemoveLight(int light_id);
private:
	Shader* light_shader,*light_overlay_shader;
	Drawable<BasicVertex> light_drawable;
	Drawable<BasicVertex> overlay_drawable;

	Animation animation_handle;

	std::vector<PointLight> point_light_buffer;

	ID3D10Texture2D* dx_render_texture;
	ID3D10ShaderResourceView* dx_render_texture_resource;
	ID3D10RenderTargetView* dx_render_target;
};