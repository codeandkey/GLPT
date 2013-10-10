#include "Radiosity.h"
#include "GameCamera.h"
#include "Player.h"

void Radiosity::EventCreate(EntityLoadData*) {
	SetEventDepth(-20);
	SetIdentity("global_ent_radiosity");

	// Initialize the rendering textures.
	
	HRESULT hr;
	D3D10_TEXTURE2D_DESC dx_texture_desc;
	D3D10_SHADER_RESOURCE_VIEW_DESC dx_srv_desc;

	ZeroMemory(&dx_texture_desc,sizeof dx_texture_desc);

	dx_texture_desc.ArraySize=1;
	dx_texture_desc.MipLevels=1;
	dx_texture_desc.BindFlags=D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
	dx_texture_desc.SampleDesc.Count=1;
	dx_texture_desc.Width=GLPT_width;
	dx_texture_desc.Height=GLPT_height;
	dx_texture_desc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;

	hr=GLPT_graphics->GetGraphicsDevice()->CreateTexture2D(&dx_texture_desc,NULL,&dx_render_texture);

	if (FAILED(hr)) {
		PostWarning("[LightSimulator] Failed to create rendering texture! Destroying.");
		Destroy();
	}

	ZeroMemory(&dx_srv_desc,sizeof dx_srv_desc);

	dx_srv_desc.Format=dx_texture_desc.Format;
	dx_srv_desc.Texture2D.MipLevels=1;
	dx_srv_desc.ViewDimension=D3D10_SRV_DIMENSION_TEXTURE2D;

	hr=GLPT_graphics->GetGraphicsDevice()->CreateShaderResourceView(dx_render_texture,&dx_srv_desc,&dx_render_texture_resource);

	if (FAILED(hr)) {
		PostWarning("[LightSimulator] Failed to create render texture shader resource. Destroying.");
		Destroy();
	}

	hr=GLPT_graphics->GetGraphicsDevice()->CreateRenderTargetView(dx_render_texture,NULL,&dx_render_target);

	if (FAILED(hr)) {
		PostWarning("[LightSimulator] Failed to create render texture target resource. Destroying.");
		Destroy();
	}

	light_shader=new Shader;

	if (light_shader->Initialize("radiosity.hlsl",BasicVertex::layout,BasicVertex::layout_size).Error()) {
		PostWarning("[LightSimulator] Failed to load radiosity shader! Destroying.");
	}

	light_overlay_shader=new Shader;

	if (light_overlay_shader->Initialize("radiosity_overlay.hlsl",BasicVertex::layout,BasicVertex::layout_size).Error()) {
		PostWarning("[LightSimulator] Failed to load radiosity overlay shader! Destroying.");
	}

	BasicVertex vertices[6];

	BasicVertex::make_rectangle(vertices,3.0f,3.0f,1.0f,1.0f);

	light_drawable.Load(vertices,6,light_shader);

	BasicVertex::make_rectangle(vertices,11.0f,12.0f,1.0f,1.0f);

	overlay_drawable.Load(vertices,6,light_overlay_shader);
	
	animation_handle.CreateFromFile("resource_light.ani");
	animation_handle.SetAnimationState("LightPoint");
	
	light_drawable.SetAnimation(&animation_handle);

	overlay_drawable.Texturize(dx_render_texture_resource);

}

void Radiosity::EventDraw(void) {
	
	float cl_color[4]={0.0f};

	GLPT_graphics->GetGraphicsDevice()->ClearRenderTargetView(dx_render_target,cl_color);

	ID3D10RenderTargetView* dx_previous_render_target;
	ID3D10DepthStencilView* dx_previous_depth_stencil;
	GLPT_graphics->GetGraphicsDevice()->OMGetRenderTargets(1,&dx_previous_render_target,&dx_previous_depth_stencil); // If something goes wrong, look here. Depth may be interfering.
	GLPT_graphics->GetGraphicsDevice()->OMSetRenderTargets(1,&dx_render_target,NULL);

	for(unsigned int i=0;i<point_light_buffer.size();i++) {
		light_drawable.Draw(point_light_buffer[i].x,point_light_buffer[i].y);
	}

	static Player* player_handle=(Player*) GLPT_iterator->GetByIdentity("ins_player");
	if (!player_handle) player_handle=(Player*) GLPT_iterator->GetByIdentity("ins_player");

	float px=0.0f,py=0.0f;

	if (player_handle) {

		player_handle->GetPosition(&px,&py);
		light_drawable.Draw(px,py);
	}

	light_shader->Texture(NULL);
	light_shader->Bind();

	GLPT_graphics->GetGraphicsDevice()->OMSetRenderTargets(1,&dx_previous_render_target,dx_previous_depth_stencil);
	
	float camera_x=0.0f,camera_y=0.0f,camera_z=0.0f;

	static GameCamera* camera_handle=(GameCamera*) GLPT_iterator->GetByIdentity("global_ent_gamecamera");
	if (!camera_handle) camera_handle=(GameCamera*) GLPT_iterator->GetByIdentity("global_ent_gamecamera");

	if (camera_handle) {
		camera_handle->GetPosition(&camera_x,&camera_y,&camera_z);
		GLPT_camera->GetPosition(&camera_x,&camera_y,&camera_z);
	}

	static float rot=0.0f;rot+=0.01f;

	overlay_drawable.Draw(camera_x,camera_y,0.0f,-0.001f);
}

int Radiosity::AddLight(PointLight light) {
	point_light_buffer.push_back(light);

	return point_light_buffer.size()-1;
}

void Radiosity::RemoveLight(int light_id) {
	point_light_buffer.erase(point_light_buffer.begin()+light_id);
}