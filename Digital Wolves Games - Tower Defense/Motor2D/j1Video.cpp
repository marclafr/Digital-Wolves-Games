#include "j1Video.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include <windows.h>
#include "p2Log.h"
#include "SDL_image\include\SDL_image.h"



j1Video::j1Video() : j1Module()
{
	name.assign("video");
}

// Destructor
j1Video::~j1Video()
{

}

bool j1Video::Awake(pugi::xml_node &)
{
	return true;
}

bool j1Video::CleanUp()
{
	return true;
}

void j1Video::Initialize(char* file_path)
{
	std::string path = SDL_GetBasePath();
	path += "..\\Game\\";
	std::string file_name = file_path;

	std::string temp = path + file_name;
	wString = temp.c_str();
	OpenAvi(wString);

}

void j1Video::OpenAvi(LPCSTR szFile)
{

	AVIFileInit();												// Opens The AVIFile Library

																// Opens The AVI Stream
	if (AVIStreamOpenFromFile(&pavi, szFile, streamtypeVIDEO, 0, OF_READ, NULL) != 0)
	{
		LOG("Can't open AVI File - Check Path -");
	}

	AVIStreamInfo(pavi, &psi, sizeof(psi));							// Reads Information About The Stream Into psi
	width = psi.rcFrame.right - psi.rcFrame.left;					// Width Is Right Side Of Frame Minus Left
	height = psi.rcFrame.bottom - psi.rcFrame.top;					// Height Is Bottom Of Frame Minus Top	
	lastframe = AVIStreamLength(pavi);								// The Last Frame Of The Stream

	pgf = AVIStreamGetFrameOpen(pavi, (LPBITMAPINFOHEADER)AVIGETFRAMEF_BESTDISPLAYFMT);						// Create The PGETFRAME	Using Our Request Mode
	if (pgf == NULL)
	{
		LOG("Can't take Avi Frame - Check format of the Avi file -");
	}


}

bool j1Video::GrabAVIFrame()
{

	//This takes the data of the actual frame
	LPBITMAPINFOHEADER lpbi;													// Holds The Bitmap Header Information
	lpbi = (LPBITMAPINFOHEADER)AVIStreamGetFrame(pgf, frame);					// Grab Data From The AVI Stream
	pdata = (char *)lpbi + lpbi->biSize + lpbi->biClrUsed * sizeof(RGBQUAD);	// Pointer To Data Returned By AVIStreamGetFrame

	int BitCount = lpbi->biBitCount;
	int pitch = 3;

	//TODO 3 Create a surface
	//This surface will recieve the data of actual frame.
	SDL_Surface *surface;

	surface = SDL_CreateRGBSurfaceFrom(pdata, width, height, BitCount, width * 3, 0, 0, 0, 0);

	//-----

	//TODO 4 Use this surface to create a SDL_Texture, and Blit it.
	//Also, After picking up the frame, you should go to the next one
	SDL_Texture* image = App->tex->LoadSurface(surface);

	App->render->Blit(image, 0, 0, NULL, SDL_FLIP_HORIZONTAL);


	frame++;
	if (frame >= lastframe)
		frame = 0;
	//----

	//TODO 5 Unload texture and surface
	App->tex->UnLoad(image);
	SDL_FreeSurface(surface);
	//-----





	return true;
}

void j1Video::CloseAVI() {
	AVIStreamGetFrameClose(pgf);								// Deallocates The GetFrame Resources
	AVIStreamRelease(pavi);										// Release The Stream
	AVIFileExit();												// Release The File

}


