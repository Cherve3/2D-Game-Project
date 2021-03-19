# 2D Beat'em Up Game
***
## Description
This is my semester project for IT 276 - 2D Game Development. I started out using the professors seed project using SDL2 with a blank slate described below or here: [gameframework2d](https://github.com/engineerOfLies/gameframework2d "EngineerOfLies")

## About the game
Beat 'em up game with action role-playing elements Inspired by River City Ransom and River City Girls but with more of a mystery twist with hints of Yakuza gameplay and cheekiness.

***
## How to Play
You start off in the locker room of the school inside there are other jocks that are inside you leave anyhere on the right side of the level. You will enter the main entrance of the school. A big open area where the preppy students hang out and chat. Moving forward to the right of the main entrance exits the school to the front lawn and the start of the city. There is a shop on the corner where health conumables will be available and items to power up your character. The alleyways are filled with shady characters beware.

You will be able to move around these areas freely after what needs to be completed in that area. Fight any hostile npcs to earn cash to spend at the corner shop. 
### Controls
|Button|Description|
|------|-----------|
|W|Move up|
|A|Move Left|
|S|Move Down|
|D|Move Right|
|Q|Pick up or drop items|
|E|Interaction|
|I|Inventory|
|Shift|Sprint(Hold)|
|Tab|Character sheet|
***
## Deliverables

These are the common deliverables of all students and my personal deliverables for my game idea required for the midterm and final.
### Common Midterm
* Functioning Entity System
* Fully interactable environment
* User Interface (HUD, Stats, Messages, menus)
* File read / write access (high score list, saved games, saved songs, etc.)
* README for how to play and see all game features(this should be in the git repo)
### Individual Midterm
* Open connected world for a beat-em-up ( at least 3 areas that can be revisited)
* Enemy drops and Shops
* Players equip dropped / purchased items
* combat mechanics (at least 3 basic attacks, +5 additional that can be unlocked)
* 5 different enemies to fight (feel free to have endless repaints, with different stats)
### Common Final
* TBA
### Individual Final
* TBA
***

# gameframework2d (GF2D)
a collection of utlitity functions designed to facilitate creating 2D games with SDL2
This project is specifically intended to function as an educational tool for my students taking 2D Game Programming.

Currently the project is in a WIP state, however it is functional with sample projects showcasing rudimentary scrolling shooters,
platformers, and adventure games.

The master branch is kept deliberately minimal to allow students to build out systems as they are learning.
Other branches have more complete system: Collisions, menus, input abstraction.

# Build Process

Before you can build the example code we are providing for you, you will need to obtain the libraries required
by the source code
 - SDL2
 - SDL2_image
 - SDL2_mixer
 - SDL2_ttf
There are additional sub modules that are needed for this project to work as well, but they can be pulled right from within the project.
Performable from the following steps from the root of the cloned git repository within a terminal. 

Make sure you fetch submodules: `git submodule update --init --recursive`
Go into each submodule's src directory and type:
`make`
`make static`

Once each submodule has been made you can go into the base project src folder anre simply type:
`make`

You should now have a `gf2d` binary within the root of your git repository. Executing this will start your game.