// ----------------------------------------------------
// d1Module.h
// Interface for all engine modules
// ----------------------------------------------------

#ifndef __d1MODULE_H__
#define __d1MODULE_H__

#include "c2SString.h"
#include "PugiXml\src\pugixml.hpp"

class d1App;

class d1Module
{
public:

	d1Module() : active(false)
	{}

	void Init()
	{
		active = true;
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node&) const
	{
		return true;
	}

public:

	c2SString	name;
	bool		active;

};

#endif // __d1MODULE_H__