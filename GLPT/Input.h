#ifndef _GLPT_INPUT
#define _GLPT_INPUT

class Input;

extern Input* GLPT_input;

enum ControlKey {
	C_MOVE_LEFT,
	C_MOVE_RIGHT,
	C_JUMP,
};

class Input {
public:
	static void Create(void) {
		GLPT_input=new Input;
	}
	static void Release(void) {
		delete GLPT_input;
	}
	void DefaultControls(void);
	bool KD(int key);
	bool KD(ControlKey key);
	void UpdateCallback(int key,bool press);
private:
	Input(void) {
		for(int i=0;i<1024;i++) {
			key_states[i]=false;
		}
	}
	bool key_states[1024];
	static std::map<ControlKey,int> control_map;
};

#endif