#include "GLPT.h"
#include "Game.h"

int main(int argc,char** argv) {

	EResult input;

	srand(static_cast<unsigned int>(time(NULL)));

	Engine::Create();

	input=GLPT_engine->Initialize(argc,argv);

	if (input.Error()) {
		GLPT_logger.Print("[GLPT_engine] Initialization failed with error : " + input.ErrorMessage());
		return -1;
	}

	GLPT_iterator->BindEntityName<RenderController>("ent_render_controller");
	GLPT_iterator->BindEntityName<Player>("ent_player");
	GLPT_iterator->BindEntityName<Brush>("ent_brush");

	Shader* render_normal;
	render_normal=new Shader;
	render_normal->Initialize("render_normal.hlsl",BasicVertex::layout,BasicVertex::layout_size);
	render_normal->SetAsCurrent();

	// temporary.

	std::string iin;
	std::cout << "[GLPT_input] Enter map to initialize : ";
	std::cin >> iin;

	GLPT_iterator->Load(iin,false);
	GLPT_iterator->Push(new GameCamera());

	GLPT_engine->Begin();

	return 0;

}