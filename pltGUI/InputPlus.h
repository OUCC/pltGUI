#pragma once

class InputPlus
{
	//MiniWindow内にないときに入力を制限するため
	bool local_locked=false;
	//popupが出ているときなどに他全ての入力を制限するため
	bool global_locked=false;

	Input input;

public:
	
	InputPlus(Input input):input(input) {}

	bool down() {
		return !local_locked && !global_locked && input.down();
	}
	bool pressed() {
		return !local_locked && !global_locked && input.pressed();
	}
	bool up() {
		return !local_locked && !global_locked && input.up();
	}
	void setLocalLock(bool lock) {
		local_locked = lock;
	}
	void setGlobalLock(bool lock) {
		global_locked = lock;
	}

};


InputPlus MouseLeft{MouseL};
