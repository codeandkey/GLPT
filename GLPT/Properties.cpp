#include "Properties.h"

void Property::EventCreate(EntityLoadData* data) {
	SetIdentity("property");

	if (data) {
		death_position=dtof(data,"death_line");
	}

}

float Property::GetDeathPosition(void) {
	return death_position;
}