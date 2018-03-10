#include "Camera.h"

Camera* GLPT_camera;

Camera::Camera(void) {
	mode_2d=false;
	x=y=z=0.0f;
	lx=ly=0.0f;
	lz=10.0f;
	fov=60.0f * 3.141f / 180.0f;
	dx=dy=dz=dxy=dxz=dyz=0.0f;
}

void Camera::SetPosition(float x,float y, float z) {
	this->x=x;
	this->y=y;
	this->z=z;

	Update();
}

void Camera::GetPosition(float* x,float* y,float* z) {

	if (x) *x=this->x;
	if (y) *y=this->y;
	if (z) *z=this->z;

}

void Camera::SetFOV(float fov) {
	this->fov=fov * 3.141f / 180.0f;

	Update();
}

void Camera::LookAt(float x, float y, float z) {
	lx=x;
	ly=y;
	lz=z;

	Update();
}

void Camera::DrawPosition(float x, float y, float z) {
	dx=x;
	dy=y;
	dz=z;

	Update();
}

void Camera::DrawAngle(float xy,float xz, float yz) {
	dxy=xy;
	dxz=xz;
	dyz=yz;

	Update();
}

void Camera::Update(void) {
	mat4x4 trans;
	mat4x4_translate(trans, dx, dy, dz);
	mat4x4_rotate(world, trans, 0.0f, 0.0f, 1.0f, dxy);

	vec3 eye = {x, y, z}, center = {lx, ly, lz}, up = {0, 1, 0};
	mat4x4_look_at(view, eye, center, up);

	mat4x4_perspective(proj, fov, (float) GLPT_width / (float) GLPT_height, 0.1f, 100.0f);

	//mat4x4_identity(world);
}

void Camera::GetTransform(mat4x4 dest) {
	if (mode_2d) {
		mat4x4_ortho(proj, -1.0f, 1.0f, -1.0f, 1.0f, -10.0f, 10.0f);
		mat4x4_mul(dest, proj, view);
		printf("in mode2d\n");
		return;
	}

	mat4x4 prefin;
	mat4x4_identity(prefin);
	mat4x4_mul(prefin, view, world);
	mat4x4_mul(dest, proj, prefin);
}
