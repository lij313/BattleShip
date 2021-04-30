
# BattleShip

3GC3 Final Project `Group 12`:  
Haoyang Tao  MacID:`taoh4`  
Lin Yang     MacID:`yangl78`  
Jin Hao Li   MacID:`Lij313`  

Instruction to run:  
Open terminal and navigate to `Code`. Then type `make` then the program will run. If the islands is covering the ship, please quit the program, and re-run it. Mouse is automatically hidden.

Controls:  
Hold up mouse left/right button and move the mouse to move around the camera.  
Scroll the mouse wheel to zoom in or zoom out the camera.  
Press `Insert` to enter "SnipeMode". In this mode, you are allowed to aim and fire.  
In "SnipeMode", use arrow key to move around the scene. You can use `PageDown` or `PageUp` to zoom in or zoom out.  
In "SnipeMode", press `Space` to fire.  Once the shell is fired, 'p' can be pressed to change the perspective to the shell. (since the shell is going in an upward motion at first, the camera is facing the sky. it will result in a black screen.)
Use `w` or `s` to set your crusing speed. Once the speed is set, the ship will adjust speed automatically keep that cruising speed.  
Use `a` or `d` to turn the ship. Keep in mind that a ship must maintain a certain speed in order to turn fast!  
`q`, or `Esc` will quit the program.  


Game Procedures:
The battleship will be spawned in a randomly generated map which consists of island and ocean.
Three obstacles are also going to be randomly distributed across the map, these are the target for players.
Follow the instructions in the Controls section to move the ship around and aim.
Destory all obstacles before time runs out. 

Notes: 
The number right beside the ship is the current crusing speed.
There are two numbers above the mini map, the first one is the current health of the ship, 
and the other one is the gear of the ship, a postive number means going forward and a negative number means going backword.

Tips:
Try to aviod the island while crusing around the map.
The closer the ship to the target, the greater accuracy.l

Implemented functions:  
1. Load and render obj files.  
2. Extract and store normal vectors from obj file.  
3. Generate and render stable water surface.  
4. Generate and render islands with circle algorithm.  
5. Basic camera control: move along an axis.  
6. Lighting and shading of an obj file.  
7. Calculate and apply the normal of terrain and water surface.  
8. Read mtl files to extract the material properites of a ship.  
9. Read `.PPM` files. (but texturing is not ready yet)  


Required External Library:  
None.  

Reference:  
1. https://www.youtube.com/watch?v=PoxDDZmctnU
2. https://blog.csdn.net/u010223072/article/details/53519865
