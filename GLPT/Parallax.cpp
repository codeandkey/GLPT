#include "Parallax.h"
#include "GameCamera.h"

void Parallax::EventDraw(void) {
	// Requires lots of horizontal wrapping.
	// How should each layer be positioned?
	// Based on the camera X value, and DIVIDE by inverse depth.

	static GameCamera* camera=(GameCamera*) GLPT_iterator->GetByIdentity("global_ent_gamecamera");
	if (!camera) camera=(GameCamera*) GLPT_iterator->GetByIdentity("global_ent_gamecamera");

	float camera_x,camera_y;
	camera->GetPosition(&camera_x,&camera_y,NULL);

	if (!camera) {
		PostWarning("Parallax failure : no gamecamera");
		Destroy();
	}

	for (int x=MAX_PARALLAX_LAYERS-1;x>=0;x--) {
		for (int i=parallax_buffer[x].size()-1;i>=0;i--) {
			float x_offset=parallax_buffer[x][i].x_offset;
			float y_offset=parallax_buffer[x][i].y_offset;
			parallax_buffer[x][i].drawable_handle->Draw(camera_x/(MAX_PARALLAX_LAYERS-x+1)+x_offset,camera_y/(MAX_PARALLAX_LAYERS-x+1)+y_offset,0.0f,x+1);
		}
	}
}

void Parallax::EventCreate(EntityLoadData*) {
	SetEventDepth(5);

	ZeroMemory(parallax_buffer,sizeof(std::vector<ParallaxObject>)*MAX_PARALLAX_LAYERS);

	std::string parallax_depth_1="scene/grass_0";

	ParallaxObject o_to_push;
	o_to_push.drawable_handle=new Drawable<BasicVertex>;

	BasicVertex vertices[6];
	BasicVertex::make_rectangle(vertices,500.0f,7.5f,20.0f,1.0f);

	o_to_push.drawable_handle->Load(vertices,6,Shader::GetCurrentShader());
	o_to_push.drawable_handle->Texturize(parallax_depth_1);

	parallax_buffer[0].push_back(o_to_push);

	o_to_push.x_offset=rand()%20/20;
	o_to_push.y_offset=0.0f;

	parallax_buffer[5].push_back(o_to_push);

}