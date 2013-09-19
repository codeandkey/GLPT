#pragma once

#include "GLPT.h"

struct PointLight {
	float r,g,b;
	float brightness;
	float radius;
};

class LightSimulator : public Entity {
public:
	void EventCreate(EntityLoadData*);
	void EventDraw(void);

	int AddLight(PointLight i);
	void RemoveLight(int light_id);
private:
	Shader* light_shader;
	std::vector<PointLight> point_light_buffer;
};