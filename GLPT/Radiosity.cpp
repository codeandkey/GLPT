#include "Radiosity.h"

void LightSimulator::EventCreate(EntityLoadData*) {

}

void LightSimulator::EventDraw(void) {
	
}

int LightSimulator::AddLight(PointLight light) {
	point_light_buffer.push_back(light);

	return point_light_buffer.size()-1;
}

void LightSimulator::RemoveLight(int light_id) {
	point_light_buffer.erase(point_light_buffer.begin()+light_id);
}