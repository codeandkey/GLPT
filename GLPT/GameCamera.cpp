#include "Player.h"
#include "GameCamera.h"

void GameCamera::EventCreate(EntityLoadData*) {

	x=y=z=tx=ty=tz=0.0f;
	tz=-10.0f;
	following_player=true;

}

void GameCamera::EventStep(void) {

	if (following_player) {
		Player* inst_player=(Player*) GLPT_iterator->GetByIdentity("ins_player");

		if (inst_player) {
			inst_player->GetPosition(&tx,&ty);
		} else {
			PostWarning("GameCamera can't follow, no player entity.");
		}
	}

	x=(tx-x)/4.0f+x;
	y=(ty-y)/4.0f+y;
	z=(tz-z)/4.0f+z;

	GLPT_camera->Set2D(false);
	GLPT_camera->SetPosition(x,y,z);
	GLPT_camera->LookAt(x,y,z+10);
}

void GameCamera::FollowPlayer(bool f) {
	following_player=f;
}