# Digital Wolves Games presents... Age of Empires: Defenders:

## Introduction:

Age Of Empires 2: Defenders is a game for the Project 2 subject on the Design and Development of Videogames degree
at Centre de la Imatge i la Tecnologia Multimedia (CITM), Terrassa. Our aim will be to create a Tower Defense with some mechanics, units, buildings and art from Age of Empires II: The Age of Kings, developed by Ensemble Studios.

## How to install the game:

Here you have the link to the download:

https://github.com/marclafr/Digital-Wolves-Games/releases/tag/1.2

Once it is done, extract the files from the "AoE II - Defenders - Gold.zip" archive, got o the "Game" folder and double click in the "AOEII_Defenders.exe"

## How to win:

Defend yourself from the attacks during 25 minutes, you have the timer in the top right corner with other useful information about the game.

If your townhall is destroyed you will lose the game, its HP bar is always on screen so you can easily know if it is attacked.


## Controls:

Game controls are based in a mouse and keyboard. While the keyboard can be used for “hot-keys” the mouse is used for most of the game control:

- Mouse:
 	- left click: interact with UI (mainly click buttons). 		Create soldiers and towers using the UI as well.
 	- left click and drag: select units or buildings. 			Preference for units. 
	- right click: is used to move selected units and chose 		targets.

Shortcuts
- Keyboard (they can be changed in the config.xml file you will find in the same folder of the game):

	- 1: To create simple tower.
	- 2: To create bombard tower.
	- 3: To create wood wall.
	
	- W: Move Camera Up.
	- A: Move Camera Left.
	- S: Move Camera Down.
	- D: Move Camera Right.
	- Space Bar: Center camera on town hall.
	- ESC: Go to menu.

### Debug:

	- F1: Show game stats.
	- F2: Show Quadtree.
	- F10: Activate debug mode.

		While on debug mode:
			- F3: Complete all investigations instantly.
			- F4: +1000 of every resource.
			- F5: +1000 food resources.
			- F6: +1000 wood resources.
			- F7: +1000 gold resources.
			- F8: +1000 stone resources.
			- F9: Shows walkability map.

You can select on townhall to create buildings or train different units with buttons on left corner.
Also you can select the university to research about elementary towers and faster recollection of resources. Once you have the elementary investigation, you
can select our tower and upgrade with the button located on the left corner.


## Changelist:

### Gold v1.2:
- Bug fixes.
- Visual map improvement.
- Added sounds.
- Completed all icons and added some news.
- You can't lose your enemies from your sight. Towers textures 	change if there is an enemy behind so you can see him.
- Achievements finished.
- Tutorial perfectioned.
- Included grey icons when you can't do an investigation or 	upgrade a tower.

### Beta v1.1:
- Bug fixes
- Performance improvements
- Stabilization all around
- Visual map improvement.
- Siege Ram creates some enemies when defeated.
- Included a wave timer and a sign in the minimap when it 	starts.

### v1.0:
- Area Terrain Damage (Bombard Fire/Ice Towers).
- Ice Floor Animation Finished. (Bombard Ice Towers).
- Enemies IA upgraded :)
- University investigations divided by type (towers, units and resources).

### v0.9 Alpha:
- Save & Load.
- Tutorial.
- Minimap.
- Minimap shows enemies.
- Investigations full operative.
- Towers elementary upgrades.
- Ice Slows units.
- Area Damage.

### v0.8:
- New Wall placement system.
- University and it's investigations avaible.
- Turrets able to be upgraded after doing its investigations.
- Tutorial implemented.
- You're able to destroy your own turrets.

### v0.7:
- Performance enhanced :)
- Units creation is from townhall (and towers).
- New Units (with costs):
	- Champion.
	- Paladin.
	- Arbalest.
	- Heavy cavalry archer.
- New icons for units/towers/resources.

### v0.6:
- All resources implemented.
- Investigations implemented in code. 
- Perfectioned arrows trajectory.
- Final map implemented (with some visual bugs :( ).
- Optimization on units pathfinding (JPS).
- Now you can withdraw the allies from the battle and move 	them at your will.
- GOD character implemented. Kill everything that moves with 	him!
- Enemies printed in a different color (red).
- Added new enemy units. 
- Animations of fire, ice and air in projectiles implemented.
- Sound and trash when a building is destroyed.
- New score scene implemented.
- Trophies.
- Bombs implemented.
- Added a new button to jump on waves but not functionally 	yet.


### v0.5:
- Resource management is available with two resources: wood and stone.
- Turret construction and walls to defend himself / herself with, with one turret type without upgrades and a wall building.
- A punctuation system to determine the best players as well as the end goal or victory condition.
- One unit type is operational and is usable for defense.
- There's one enemy type and obviously one wave type.
- Both enemy and allied units are the two-handed swordsman shown in our wiki, on the Units section of our GDD: https://github.com/marclafr/Digital-Wolves-Games/wiki/Game-Design-Document#units
- Our map is fully operational. The concept and design on which said map is based can be found in the map section of our GDD: https://github.com/marclafr/Digital-Wolves-Games/wiki/Game-Design-Document#map

### v0.4:
- Units look for enemies and if they find someone, they go to fight with them. You can't stop them though.
- Towers attack units when they are built.
- You can see if you can build the tower to the place you want.
- Units delete correctly! No more bugs in combat.
- We have canceled unit collision temporarily in order to have a good combat system; it will be in the next version!

### v0.3:
- Added new amazing map!
- Solved some bugs.

### v0.2:
- Enemies walk to their destination if you don't stop them.
- We can press the buttons of the HUD and spawn the tower and two different units (ally/enemy) with left click on a walkable tiled.
- The units will fight with allies if they are next to each other automatically.
- You can check each unit HP, attack and armor if they are selected alone.

### v0.1:

- Move units, thought they will end up together in the same point.
- Build a tower by clicking the number 1
- Move the camera with the directional arrows.



## About final Game:

You are the commander of a small village and you will need to defend to the different civilizations.
You will need to survive the maximum number of waves to repel the attack. Each wave will increase the difficulty of the enemies’ waves.
You can build towers and training camps to protect the village. You can put the towers on the green areas. Also we can upgrade with the resources and do more effective the statistics of towers.
The player will win once the time counter is over (30 minutes) and will lose if the enemy destroys the wall to the city.

## Resources: 

You will get an area to collect resources. In the university you can upgrade them to collect faster.
But you will need to do a good management with resources.

## Towers:

You will have 2 types of towers at the beginning of the game. Simple tower and bombard tower. On library you can investigate for upgrade our tower to different types of elementary towers.

 - Simple Tower: Got normal damage, normal hp, normal range and rate of fire.
 - Bombard Tower: Got a lot of damage, a lot of hp, less range than simple tower and heavy rate of fire.

### Elementary types of towers:

 - Fire Tower: More damage than a simple tower, more health but less range.
 - Ice Tower: A bit more damage than a simple tower, a little bit more health, they shoot a bit faster and shoots freeze the enemies.
 - Air Tower: Little bit less damage than simple tower, same health, more range and shoots faster than a simple tower.

Same from bombards towers with each statistics, however the Ice and Fire Bombard towers leave Ice/Fire in the floor for some time and the units that pass through are damaged (and slowed in the case of Ice).

## Trophies:

In order to make the player play more than once and make them feel that they are getting better and better in the game we will add some trophies to collect:

### SUPREME

- Get 74.625 points or more.

### GOLD

- Get 56.227 points or less.

### SILVER

- Get 38.820 points or less.

### BRONZE

- Get 21.900 points or less. 

### WOOD

- Get 6.975 points or less.

## Achievements

- Win the game without damage in your Townhall.
- Win the game using only Bombard Towers.
- Reach 100.000 points in one game (and win).
- Train a hundred soldiers in one game (and win).




## Team members:

| Role         | Name | Github          |
| :-------------:|:-------------:|:-----:|
| Team Leader   |Marc Latorre| https://github.com/marclafr |
| Code     | Dani Lopez|https://github.com/Danny0ner |
| Design |José Rodriguez| https://github.com/joserm45 |
|Art/Audio|Pau Serra| https://github.com/pau5erra|
|UI | Julià Mauri| https://github.com/juliamauri|
|Management|Martí Majó| https://github.com/martimyc|
|QA | Marc Samper|https://github.com/marcsamper|

Webpage: http://digitalwolves.webnode.es/

Github: https://github.com/marclafr/Digital-Wolves-Games

Twitter: https://twitter.com/DigitalWolvesG

Facebook: https://www.facebook.com/Digital-Wolves-Games-233798633695568/?ref=bookmarks

