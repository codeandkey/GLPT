#include "RenderController.h"

void RenderController::EventCreate(EntityLoadData* data) {

	render_index[RENDER_STATE_NORMAL]=render_normal;

	SetIdentity("ins_render_controller");

	render_normal=new Shader;
	render_normal->Initialize("render_normal.hlsl",BasicVertex::layout,BasicVertex::layout_size);

	render_normal->SetAsCurrent();

	current_render_state=RENDER_STATE_NORMAL;
}

void RenderController::EventStep(void) {

	try {
		render_index.at(current_render_state);
	} catch(std::out_of_range) {
		PostWarning("Invalid shader index in RenderController!");
		return;
	}

	render_index[current_render_state]->SetAsCurrent();

}