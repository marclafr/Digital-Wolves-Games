#ifndef __d1TIMER_H__
#define __d1TIMER_H__

#include "c2Defs.h"

class d1Timer
{
public:

	// Constructor
	d1Timer();

	void Start();
	uint32 Read() const;
	float ReadSec() const;

private:
	uint32	started_at;
};

#endif //__d1TIMER_H__