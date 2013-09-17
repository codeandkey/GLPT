#include "Parallax.h"
#include "GameCamera.h"

void Parallax::EventDraw(void) {
	// Requires lots of horizontal wrapping.
	// How should each layer be positioned?
	// Based on the camera X value, and DIVIDE by inverse depth.

	GameCamera* camera=(GameCamera*) GLPT_iterator->GetByIdentity("global_ent_gamecamera");

	float camera_x,camera_y;
	camera->GetPosition(&camera_x,&camera_y,NULL);

	if (!camera) {
		PostWarning("Parallax failure : no gamecamera");
		Destroy();
	}

	for (unsigned int i=0;i<parallax_buffer.size();i++) {
		parallax_buffer[i]->Draw(camera_x/(parallax_buffer.size()-i+1),camera_y/(parallax_buffer.size()-i+1),0.0f,1.0f);
	}
}

void Parallax::EventCreate(EntityLoadData*) {
	std::string parallax_depth_1="scene/grass_0";

	Drawable<BasicVertex>* next_drawable=new Drawable<BasicVertex>;

	BasicVertex vertices[6];
	BasicVertex::make_rectangle(vertices,10.0f,10.0f,1.0f,1.0f);

	next_drawable->Load(vertices,6,Shader::GetCurrentShader());
	next_drawable->Texturize(parallax_depth_1);

	parallax_buffer.push_back(next_drawable);
	parallax_buffer.push_back(next_drawable);

}