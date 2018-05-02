# CS3113-Game-Programming
Homework for 3113

# Special

Use gitmodules to include glm;

Please clone with ```git clone --recursive https://github.com/Skycocoo/CS3113-Game-Programming.git```

# Dependencies

Please make sure you have these libraries installed before cmake

```
- OpenGL (find packages with cmake)
- GLEW (find packages with cmake)
- glm (included in the folder)

- SDL_mixer (find packages with cmake modules)
- SDL2 (find packages with cmake modules)
- SDL2_Image (find packages with cmake modules)
```


# Installation with cmake

Clone this project: (Note that I use .gitmodules to include glm, the clone should be in recursive mode)

```bash
$ git clone git@github.com:Skycocoo/CS3113-Game-Programming.git --recursive
$ cd ./final\ project
```

<!-- Open the folder and cd into the folder Checker-Game in shell -->

Make build directory for cmake:
```bash
$ mkdir build
$ cd ./build/
```

Run cmake & build executable for this project:

```bash
$ cmake ..
$ make
```
Run the executable of this project:

```bash
$ ./main
```



# Assignments

1. create simple 2D scene with textured & untextured polygons; include simple animation

2. PONG!

3. Space Invaders

> bugs: when the enemy is destroyed, its bullets also disappear

4. simple Platformer

5. Separated-Axis-Collision demo

6. add sound effects to one of the previous games


# Project Requirement

1. title screen and proper mode for game over

2. music and sound effects.

3. at least 3 different levels or be procedurally generated

4. local multiplayer or have AI (or both)

5. some animation or particle effects.

## Extra Credits

1. running on phone

2. 3D elements

3. shader effects
