# Renderer
This is a small project I worked on during my studies at Full Sail University.

### Disclaimers

**Note: The project will build but will not run due to an error that was built into the initial code base that I was given from the school. My contribution to these files did not create the issue nor did it solve the issue. After asking and requesting assistance, no one was able to help me fix the issue.**

**In order to run the program, I have to open the .sln file and run it in the debugger.**

The file "dev-app.cpp" is kind of 3 seperate scripts in one. This is NOT how I would normally program. I did it like this so that the instructor could easily toggle the different desired functionality, since there is no shell program or interaciton built in. The purpose of uploading this is to show a piece of what I have done so far.

Each individual part (listed below) has it's own regions in the file with toggles at the top using Define statements.

### Description

The project is a rendering project. There are 3 parts to it. 
-  The first are particle systems. They generate colored particles that fire upwards and obey gravity, creating a fountain effect.
-  The second is a display of matrix functions. Each matrix is represented by a different colored unit line on each of the local axis'. The one in the center can be moved with the arrow keys, the other two represent two different Look-At functions. On is an instant Look-At and the other gradually follows the target (you) around.
-  The third is a combination of AABB and fustrum collision detection and a camera controller. You can move the "player" around with the arrow keys like before and boxes that are within the "players" point of view will light up green. You can hold the mouse down inside of the window and use W, A, S, and D to move the camera around to view how you would like.

### Usage

In order to run the program, sadly you will have to open it up in a debugger, probably visual studios. This is the only way I could get it to run despite the fact that it will build successfully, this is not due to any of my work or anything within my control on the project.

In order to view the different parts you will need to change the section (shown below) at the top of the "dev_app.cpp" file.
![StateSelect](https://github.com/tidekiller237/Renderer/assets/42755734/c01e5433-3166-4fb7-8c5b-120ff6468cbd)

For section one (The particle systems), you must set the "LAB_1" statement to a value of '1' and set the other two "#define" statements to a value of '0'.

For section two (The matrix behaviors), you must set the "LAB_2" statement to a value of '1' and set the other two "#define" statements to a value of '0'.

For section three (The fustrum detection), you must set "LAB_2" AND "LAB_3" statements to a value of '1' and set "LAB_1" statement to a value of '0'.

**Note: I know it's a little complicated and I would never have a file set up like this in a legitimate situation, this was for a specific situation and I apologize ahead of time.**

### Personal Contribution
I was supplied with the base line code, however I wrote the following sections:

main.cpp:
  -  The body of "WndProc" method; lines 134 - 193
  -  The body of "begin_main_loop" method; lines 226 - 250

The "dev_app.cpp" and "dev_app.h" files

pools.h:
  -  The bodies of the methods in the "sorted_pool_t" class; lines 15 - 62
  -  The bodies of the methods in the "pool_t" class; lines 73 - 122
