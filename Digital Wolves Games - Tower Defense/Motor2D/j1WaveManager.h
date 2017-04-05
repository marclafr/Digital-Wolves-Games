#ifndef __j1WAVE_MANAGER_H__
#define __j1WAVE_MANAGER_H__

#include "j1Module.h"
#include "j1Timer.h"

#define ENEMY_CREATION_POS_X -2112
#define ENEMY_CREATION_POS_Y 1032

#define WAVE_TIMER 15

class j1WaveManager : public j1Module
{
public:

	j1WaveManager();
	~j1WaveManager();

	bool Start();
	bool Update(float dt);
	bool CleanUp();


private:
	void CreateWave(int wave_num);

	j1Timer wave_timer;
	int wave_num = 1;

	//Creation place
	int creation_pos_x = -2112;
	int creation_pos_y = 1032;

	//Separation between units
	int space_between_x = 24;
	int space_between_y = -14;
};

#endif // __j1WAVE_MANAGER_H__
