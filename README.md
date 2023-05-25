# football_penalty_kick_opengl_cpp
FootBall Penalty Shooter, as the name suggests is a game similar to football penalties in real life. It can be a 2-player game or an interactive game with the computer.

The Functionalities are:-
1. Set the level of game to be played(easy , medium, hard, move goalpost)
2. Move around the camera with the mouse/touchpad to get a better field of view.
3. Zoom in/out using the keyboard, again to get a better field of view.
4. Set the angle of the shot, and thus point it in the desired direction.
5. Set the spin of the ball, that is the angle by which you want the ball to turn and its direction as well.
6. Set the power of the shot, that is, the speed of the ball.
7. View the goal statistics and switch from one Category to the other using the keyboard.
8. The goal is defended by a moving and jumping Goalkeeper, which moves in the direction of the shot(in case of interactive game).
9. In 2-player Category , One player can be the attacker and can kick the ball in order to score a goal. The attacker has 5 chances, after which the game is over and the winner is declared.

<h2>Project Installation Guide</h2>

The main library used in our project is Open Graphics Library which is an API for rendering 2D and 3D Vector Graphics(use of polygons to represent image).

 - Clone the repo
 - Install the Opengl library
 - Move to required folder
 - Run using C++ compiler with help of command 

```
g++ main.cpp functionalities.cpp shapes.cpp constants.h -lGL -lGLU -lglut -lm -o run && ./run
```

 - The project will start working now

<h2>Important files and folder structure:-</h2>

 - constants.h - values of different constants are stored in it like dimensions
 - functionalities.h - declarations of different functions used
 - functionalities.cpp - definitions of different function used like collision between objects
 - shapes.h - declarations of different object used
 - shapes.cpp - definitions of rendering of different objects
 - main.cpp - it calls the sequential execution of different components used in project
 - textureloader.py - to convert image into RGB format to render into project
 - fontloader.py - to convert image into RGB format to render into project

<h2>Contributers:-</h2>

 - Amit Kumar Makkad
 - Atharva Bhagya
 - Nischay Shroff
 
This project is part of course cs352 Computer graphics Lab under guidance of Dr Somnath Dey.<br>
Reference: https://github.com/iamKunal/OpenGL-3D-Football-Penalty-Shooter
  
