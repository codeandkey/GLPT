#pragma once
#include "Library.h"
#include "linmath.h"

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
	void GetTransform(mat4x4 dest);
private:
	Camera(void);
	void Update(void);

	bool mode_2d;
	float x,y,z,lx,ly,lz;
	float fov;
	float dx,dy,dz,dxy,dxz,dyz;

	mat4x4 world,view,proj;
	mat4x4 fin, fin_2d;
};
