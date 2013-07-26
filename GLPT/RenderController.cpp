#include "RenderController.h"

void RenderController::EventCreate(EntityLoadData* data) {

	render_index[RENDER_STATE_NORMAL]=render_normal;

	SetIdentity("ins_render_controller");
	//SetEventDepth(-10);

	render_normal=new Shader;
	render_normal->Initialize("render_normal.hlsl",BasicVertex::layout,BasicVertex::layout_size);

	render_normal->SetAsCurrent();

	current_render_state=RENDER_STATE_NORMAL;
}

void RenderController::EventStep(void) {

	try {
		render_index.at(current_render_state);
	} catch(std::out_of_range) {
		GLPT_logger.Print((std::string("[GLPT] obj_render_controller : Invalid shader index ") + static_cast<char>(current_render_state)) + "!");
		return;
	}

	render_index[current_render_state]->SetAsCurrent();

}