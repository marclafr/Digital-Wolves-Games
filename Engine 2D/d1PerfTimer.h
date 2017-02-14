#ifndef __d1PERFTIMER_H__
#define __d1PERFTIMER_H__

#include "c2Defs.h"

class d1PerfTimer
{
public:

	// Constructor
	d1PerfTimer();

	void Start();
	double ReadMs() const;
	uint64 ReadTicks() const;

private:
	uint64	started_at;
	static uint64 frequency;
};

#endif //__d1PERFTIMER_H__