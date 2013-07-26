#pragma once
#include "Library.h"
class Camera;

extern Camera* GLPT_camera;

class Camera {
public:
	static void Create(void) {
		GLPT_camera=new Camera;
	}
	static void Release(void) {
		delete GLPT_camera;
	}
	void LookAt(float x, float y, float z);
	void SetPosition(float x, float y, float z);
	void GetPosition(float* x,float* y,float* z);
	void SetFOV(float fov);
	void Set2D(bool d) {mode_2d=d;}
	void DrawPosition(float x, float y, float z);
	void DrawAngle(float xy, float xz , float yz);
	D3DXMATRIX GetTransform(void);
private:
	Camera(void);
	bool mode_2d;
	float x,y,z,lx,ly,lz;
	float fov;
	float dx,dy,dz,dxy,dxz,dyz;
};