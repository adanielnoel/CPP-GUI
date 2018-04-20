see the following :

http://plib.sourceforge.net/pui/index.html


This folder contains everything you will need to get a funky user interface
into your C/C++ programs. 

Pui (picoscopic-user-interface) is a user interface that is drawn using OpenGL
and works nicely with glut (although it can be used independantly). The library
is unfortunately C++ so all files in your project should have the *.cpp extension
rather than the *.c extension. 

The directory "includes" contains all of the include files for Glut and Pui. 
The directory "lib" contains the pre-compiled pui libs for windows and irix.
All other directories are example files for how to use pui.


Creating a new pui project
==========================

If you want to create a new project of your own, create a folder within the pui
directory, ie "myProject". Next copy the Makefile into that folder. 

All files you add to the project should go into that folder and should have either
the *.h or *.cpp extensions. Due to the way that I've set the makefile up you
will have to manually delete the executable file before calling 'make' again.
This is so that you don't have to understand or edit the Makefile if you don't
want to. 

Changing the output executable name
===================================

One point worth noting, if you want to change the name of the output
executable, change the following line in the makefile :

EXE_NAME= program
 

using *.c and *.cpp files
=========================
 
if you do want to use *.c files as well as *.cpp files. You will have to change
a line in the Makefile to the following ...

FILES= *.cpp *.c


*.vcproj and *.sln files
========================

These file types are Visual Studio .NET project files. Delete them if you don't
intend to ever use Visual C++ .NET. These are like Makefiles for Visual Studio.


Rob.
