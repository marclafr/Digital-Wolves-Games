#ifndef __j1WAVE_MANAGER_H__
#define __j1WAVE_MANAGER_H__

#include "j1Module.h"
#include "j1Timer.h"

#define ENEMY_CREATION_POS_X1 -1968
#define ENEMY_CREATION_POS_Y1 960

#define ENEMY_CREATION_POS_X2 -2064
#define ENEMY_CREATION_POS_Y2 1008

#define WAVE_TIMER 3

class j1WaveManager : public j1Module
{
public:

	j1WaveManager();
	~j1WaveManager();

	bool Start();
	bool Update(float dt);
	bool CleanUp();


private:
	int CreateWave(int wave_num, int time);
	uint oldtime = 0;
	j1Timer wave_time_order;
	j1Timer wave_timer;
	j1Timer wave_timer2;
	j1Timer wave_timer3;
	j1Timer wave_timer4;
	j1Timer wave_timer5;
	j1Timer wave_timer6;
	j1Timer wavetime;
	j1Timer totaltimer;
	int wave_num = 1;

	//Creation place
	int creation_pos_x1 = -2112;
	int creation_pos_y1 = 1032;

	int creation_pos_x2 = -2112;
	int creation_pos_y2 = 1032;
	//Separation between units
	int space_between_x = 24;
	int space_between_y = -14;
};

#endif // __j1WAVE_MANAGER_H__
