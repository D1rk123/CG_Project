# Laser Bird
Laser Bird is an single player arcade game inspired by Flappy Bird. Hop through space and shoot lasers to make a way through the asteroid field. Most of the game was developed in two weeks as a group assignment for the course Computer Graphics at the Delft University of Technology.

## Gameplay
Press the space bar to fly up, press P to shoot a laser. If your spaceship gets hit by a meteor or hits the botom of the screen you're game over. You can remove meteors by hitting them with a laser.

## Play the game
Windows users can just download the [Laser-Bird-Game.zip](https://github.com/D1rk123/Laser-Bird/raw/master/Laser-Bird-Game.zip) file from this repository to play the game.

![Screenshot](https://raw.githubusercontent.com/D1rk123/Laser-Bird/master/GithubImages/Screenshot720p.png)

## Technology
The game is build in C++ and OpenGL without using a game engine. Some libraries were used: DevIL (loading images), FreeGLUT (interaction with the OS), GLEW (loading OpenGL extensions), GLM (vector math).

### Technical Features
  * .obj 3D model loading and displaying
  * Ellipsoid based collision detection (press L while playing to see the collision shapes)
  * Procedurally generated meteors
  * Procedurally generated infinite levels
  * Phong based per pixel lighting on the GPU with a moving light source on every laser

## Building the game
In this repository Code::Blocks project files and precompiled libraries for windows are provided. If you have Code::Blocks installed with the GCC/MINGW compiler on Windows you can just open the Project/Project.cpb file to get started. The code and the used libraries are multiplatform, so if you want to work on a different OS or IDE it should be possible, but you would have to set that up yourself.

## Origin
The original version of the game was made for a group assignment for the course Computer Graphics at the Delft University of Technology. The goal of the assignment was to make a flappy bird inspired game with a gun and any form of procedurally generated content in C++ and OpenGL. The intent of the assignment was to use the fixed function pipeline of OpenGL 1, but we used OpenGL 3 with shaders to get per pixel lighting. As a result of this we got a 10/10 grade. All core features were completed as part of this assignment, but the game was a bit unpolished because we only had two weeks. In this fork I've aimed to bring the game to a playable state by adding clear game states(intro, playing or game over), tweaking some of the game behavior and cleaning up the code a bit.
