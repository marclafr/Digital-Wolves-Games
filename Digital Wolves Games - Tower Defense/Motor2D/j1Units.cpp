#include "j1Units.h"
#include "j1App.h"
j1Units::j1Units() : j1Module()
{
	name.create("Units");
}

j1Units::~j1Units() {}


bool j1Units::Awake() { return true; }

bool j1Units::CleanUp() {

	if(sprites != nullptr)
	App->tex->UnLoad(sprites);

	std::list<j1Units*>::iterator item = unit_list.begin();
	while (item != unit_list.end()) {
		RELEASE(item._Ptr->_Myval);
		item++;
	}
	unit_list.clear();

	//Borrar enemigos
	//for(uint i = 0;             ; ++i)
		//if(unit_list[i])
	return true;
}


bool j1Units::Update() {

	//a través de la lista donde 
	//tenemos los enemigos ejecutamos los siguientes pasos

	//funcion de movimiento de cada enemigo

	//Draw de los enemigos

	//si han muerto y han acabado su animacion de muerte hacer delete
	return true;
}

bool j1Units::AddEnemy(UNITS_TYPE type, int x, int y) {
	/*j1Units* minion;
	if (type == VILLAGER) {
		minion->attack = 1;
		minion->hp = 1;
		minion->Position.x = x;
		minion->Position.y = y;
		minion->speed = 1;
	}
	unit_list.push_back(minion);*/
	return true;
}