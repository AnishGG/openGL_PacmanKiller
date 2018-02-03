Opengl 2D Game
=========================

**Anish Gulati: 20161213**

This game is inspired by the game [Pacman](http://www.pacman4u.com/pacman-killer/). The Objective of this game is to have more fun and gain as much points as possible. This game is divided into three levels and some instructions to reach to the next level is displayed on the title bar of the game window.

### Installation
This game needs three major development packages from the mentioned libraries as GLFW3, GLM, and GLEW. CMAKE is also needed to run this project.

**Following are the instructions for installing these packages**

```bash
sudo apt install cmake pkg-config libglfw3-dev libglm-dev libglew-dev
```

#### Run the code

```bash
cd path/to/project
mkdir build
cd build
cmake ../
make
./pacman
```

### Controls:

------

- a/d - Move Left/Right
- Space - Jump
- Mousewheel - Zoom control


- Arrow keys - Panning
- Player control using mouse drag - Try it!

#### Extra Features

------

- 3 Levels with different difficulties
- Countdown-timer using BCD display
- Moving player using mouse
- Automatic panning when user tries to go out of the screen
- Modular codebase

### World

-----

1. **Random Enemies/Balls without slopes** - Have random velocity, color and radius. Every time player hits these, it's score increases by 3.
2. **Random Enemies/Balls with slopes** - Have properties of enemy mentioned above + slabs placed at random angles on the balls. When the player hits these balls, it is reflected away using the laws of reflection. And the score is increased by 5.
3. **Water** - The speed of player is slow due to more deaccelaration faced by the player in the water. Hence jumping height is also affected. Also when the player touches the any point in the pond other than bottom most point, it follows physics and the ellipse trajectory to reach the bottom most point of the pond(until disturbed by user input).
4. **Magnet** - Attracts the player towards it selves.
5. **Porcupines** - Hitting this would lead to reduction in score of 1 and disappearing of this enemy for some time.
6. **Trampoline** - Jump with more velocity than usual jump.

### Scoring

------

Collision with normal ball = +3

Collision with flying ball with inclined slope = +5

Collision with ground porcupines =  -1
