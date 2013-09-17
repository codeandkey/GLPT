#include "Parallax.h"
#include "GameCamera.h"

void Parallax::EventDraw(void) {
	// Requires lots of horizontal wrapping.
	// How should each layer be positioned?
	// Based on the camera X value, and DIVIDE by inverse depth.

	GameCamera* camera=(GameCamera*) GLPT_iterator->GetByIdentity("global_ent_gamecamera");

	float camera_x;
	camera->GetPosition(&camera_x,NULL,NULL);

	if (!camera) {
		PostWarning("Parallax failure : no gamecamera");
		Destroy();
	}

	for (unsigned int i=0;i<parallax_buffer.size();i++) {
		parallax_buffer[i].Draw(camera_x/(parallax_buffer.size()-i));
	}
}