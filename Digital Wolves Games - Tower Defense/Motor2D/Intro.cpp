#include "Intro.h"
#include "j1Video.h"
#include "j1Audio.h"

j1Intro::j1Intro() : j1Module()
{
	name.assign("intro");
}

// Destructor
j1Intro::~j1Intro()
{}

// Called before render is available
bool j1Intro::Awake()
{
	//LOG("Loading Scene");

	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Intro::Start()
{

	//TODO 1 Initialize the video - Just call the function
	//App->video->Initialize("final_logo_anim(1).avi");
	//App->audio->PlayMusic("audio/music/logo_anim_audio.ogg", 0.0f);//------

	return true;
}

// Called each loop iteration
bool j1Intro::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Intro::Update(float dt)
{
	//TODO 2 Every frame you should take one frame of the video

	//App->video->GrabAVIFrame();

	//-------
	return true;
}

// Called each loop iteration
bool j1Intro::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool j1Intro::CleanUp()
{
	//TODO 6 Clean the Avi files used
	//App->video->CloseAVI();


	return true;
}

