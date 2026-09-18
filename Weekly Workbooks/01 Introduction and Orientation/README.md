## Introduction and Orientation
### <a href="https://mediasite.bris.ac.uk/Mediasite/Channel/d2f2d6b0f5184177b73cc931217fa1af5f" target="_blank"> Weekly Briefing ![](../../resources/icons/briefing.png) </a>
### Task 1: Introduction


The aim of this introductory workbook is to ease you gently into the first week of term. The main objective is to make sure you are able to build and run the code template project. This is of crucial importance because this template is the starting point for **ALL** of the work you will undertake on this unit. Getting the template project working _might_ take some time and perseverance (depending on the operating system you intend to use for this unit). It is **ESSENTIAL** that you achieve everything in this workbook during the first week of term. If you are unable to build and run applications, you will quickly find yourself falling behind on this unit.  


# 
### Task 2: Language and Libraries


The implementation language for this unit will be C++. Although everybody may have their own preferred language for writing code, C++ is still very much a standard for low-level graphics rendering applications. The aim of this unit is to develop a fundamental understanding of graphics rendering and as such, we will NOT be making use of any existing rendering frameworks (such as OpenGL or DirectX). Instead, we will building our own rending engines ! That said, the practical exercises will make use of the following two external libraries:
- <a href="https://github.com/g-truc/glm" target="_blank">GLM</a> for general purpose mathematical operations (mostly for manipulating vectors and matrices)
- <a href="https://www.libsdl.org" target="_blank">SDL2</a> for drawing to the screen (mostly setting the colour of individual pixels !)

Remember: The aim of this unit is to learn to build things from the ground-up, so don't use anything other than SDL2 and GLM when implementing the practical exercises (and in the coursework itself).

In order to make things a little tidier, we have provided a "wrapper" object that encapsulates the SDL2 library which we have called `DrawingWindow`. This wrapper initialises SDL, opens up a window for drawing to the screen and provides various methods for drawing and interacting with the user. The `DrawingWindow` object has the following methods:  

- `DrawingWindow` constructor for the drawing window class that takes 3 parameters:
    - Horizontal resolution for the window (integer width)
    - Vertical resolution for the window (integer height)
    - Whether or not to scale the window into full-screen mode (boolean)
- `setPixelColour` sets an individual pixel (indicated by [x,y] coordinates) to a specified ARGB colour
- `getPixelColour` returns the ARGB colour of the pixel indicated by [x,y] coordinates
- `renderFrame` draws all pixels to the screen (until you call this, all pixel changes are just in a memory buffer)
- `savePPM` saves the currently rendered content of the window to a .ppm image file
- `saveBMP` saves the currently rendered content of the window to a .bmp image file
- `clearPixels` clears all pixel colours (from the whole window)
- `pollForInputEvents` checks to see if there are any events waiting to be processed (returns true if there are !)

Examples of how to call the methods of the `DrawingWindow` class are included in the 
<a href="extras/RenderProject/src/MainApplication.cpp" target="_blank">template project</a>. 


  


# 
### Task 3: Using Your Own Computer (Native OS)


We have tried to use a programming language and set of libraries that are agnostic to the underlying architecture and operating system. As a result, it should be _fairly_ straight-forward to get things running on most machines. Note however that we will test and mark the coursework assignment on the lab machines, so you should avoid using any platform-specific features that aren't available on the lab machines.

The template project we have provided assumes that you have the `clang` complier installed (which many of you may already have !). If you do not currently have `clang`, you can either install it now or alternatively (if you have a different compiler already installed) update the `COMPILER` variable in the project Makefile to reflect the compiler that you do have installed (so that it uses `g++` or `cl` for example).

You can use an IDE for development if you wish (CLion, Visual Studio, Eclipse, XCode etc). We have provided a CMake file inside the project root folder to help you configure and build your project. If you do use an IDE then please make sure that your project can still be compiled outside of the IDE (i.e. by providing a Makefile that works on the lab machines).

CLion works well as an IDE and is able to open the `CMakeLists.txt` file as a project. If you prefer to use VSCode instead then you will need to install some extensions - see <a href="https://code.visualstudio.com/docs/cpp/cmake-linux" target="_blank">this tutorial</a> for a step-by-step guide. You are free to use whichever IDE you wish, however please remember that we are unlikely to able to offer assistance using all IDEs - any support that we can provide will depend on the prior experiences of the lab teaching team ! 

Installation of SDL2 can sometimes be a non-trivial task (due to the fact that it must link with native graphics libraries). For this reason, the recommended way to install SDL2 is using a package manager (e.g. apt, rpm, yum, brew, ports, pacman etc). The name of the required SDL2 package will vary depending on your platform. Here are some likely candidates for a range of different package managers:

```
apt install libsdl2-dev
rpm -i SDL2-devel
yum install SDL2-devel
brew install sdl2
port install libsdl2
pacman -S sdl2
```

If you can't get your package manager to install SDL2 (or if you don't have a package manager at all !) then you will need to install SDL2 manually. Various resources are available to help you, including <a href="extras/clion.pdf" target="_blank">this guide for installing SDL2 for CLion under Windows</a> or alternatively <a href="https://lazyfoo.net/tutorials/SDL/01_hello_SDL/" target="_blank">this general guidance for installing SDL2 on all platforms</a>. If you are still really struggling to install SDL2 on your existing operating system, then the following sections provide some alternatives you might like to consider.  


**Hints & Tips:**  
The code template (that we will introduce later in this workbook) comes bundled with the GLM library built-in, so there is no need for you to install this yourselves !
  


# 
### Task 4: Using Your Own Computer (Guest OS)


If you are having difficulties installing SDL2 on your existing operating system, you might find things easier using a fresh operating system install. We have created a <a href="extras/Vagrantfile" target="_blank">Vagrant configuration file</a> that can be used to bring up a virtual machine with all of the required tools and libraries installed. See the comments at the top of the configuration file for instructions on how to get started with Vagrant.

Alternatively (if you are confident to do so) it might be worth considering a "fresh" install of your preferred linux distribution. It should be relatively straight-forward to install SDL2 on a clean linux install using a decent package manager. You might even like to consider dual booting your computer (if you are brave enough to do so !)

It is possible to build and compile the template project using Windows Subsystem for Linux (WSL). Recent versions of Windows _should_ be able to run graphical linux applications seamlessly (i.e. without having to install and configure XWindows/X11). Note that if you have an older version of Windows 10, you might need to do a software update in order to allow graphical applications to work out-of-the-box.  


**Hints & Tips:**  
If using a virtual machine, you should avoid using display scaling (i.e. use VirtualBox in 100% "Unscaled Output" mode) otherwise you may experience a reduction in window manager performance.  


# 
### Task 5: Using the Lab Machines


As an alternative to getting SDL2 installed on your own computer, you might choose to compile and run your code on the lab machines. The lab machines should have all of the required software installed - including gcc, SDL and CLion (which should be installed somewhere within the `/opt/clion` folder structure). The lab machines can either be used physically (whilst you are actually in the lab) or remotely (via remote desktop services). In order to gain remote access you will need to use the _X2Go_ client. Full instructions for installing and running the _X2Go_ client can be found in <a href="https://uob.sharepoint.com/sites/itservices/SitePages/fits-engineering-linux-x2go.aspx" target="_blank">this how-to guide</a>. It is worth noting however that the quality of your experience using this approach will vary depending on the quality of your internet connection.


   


**Hints & Tips:**  
For help resolving problems with remote access to the lab machines, check out the
<a href="https://uob.sharepoint.com/sites/itservices/SitePages/contacts.aspx" target="_blank">IT services help page</a> in particular the Student Teaching Helpline and the Service Desk Helpline.

If you get a warning that `GLX is not supported` when remote logging in to the lab machines using X2Go then you will need to disable hardware acceleration in the template project. Open the `libs/sdw/DrawingWindow.cpp` file and change the value of the `flags` variable from `SDL_WINDOW_OPENGL` so that it now reads `SDL_WINDOW_FOREIGN`. This will disable hardware acceleration and _should_ solve the problem - it may run more slowly, but at least it will run !  


# 
### Task 6: The Template Project


In order to help get you started, we have provided a template project called <a href="extras/RenderProject" target="_blank">RenderProject</a>. This project provides the structure that you will need for the weekly workbooks, as well as illustrating the use of the `DrawingWindow` methods. 

In order to help you compile, link and run the <a href="extras/RenderProject" target="_blank">RenderProject</a>, a Makefile has been provided for you inside the project root folder. This makefile contains multiple rules to build code for different purposes:

- `debug` a development build rule that will compile and link your project for use with a debugger (gdb)
- `diagnostic` a development build rule that includes extra memory checking and diagnosis flags.  
Note: this rule requires <a href="https://clang.llvm.org/docs/AddressSanitizer.html" target="_blank">Address Sanitizer</a> to be installed (comes built-in with some C++ compilers)
- `speedy` a rule that builds an optimised "high performance" executable (making interaction testing quicker)
- `production` a build rule to make an executable without debug hooks (for release or distribution)

Just typing `make` on its own will build the project using the _debug_ rule and run the resultant executable.

Alternatively you can build and run the project from inside an IDE (such as CLion). Typically IDEs will use `cmake` to compile the project - if you need to change any build options, you will need to edit the `CMakeLists.txt` file. 

When you have successfully built and run `RenderProject` you should see a window that looks like the below animation - this shows a test render called `RedNoise`. Remember that the cleanest way to quit an SDL application is by pressing the `ESC` key ! If you have a window that you can't close (which does happen from time to time), you may need to kill off the process manually using Task Manager, Activity Monitor or the kill/killall command (depending on your platform).  


![](06%20The%20Template%20Project/images/RedNoise.gif)

**Hints & Tips:**  
On some platforms you may need to #include `<cstring>` in the `DrawingWindow` class (if the compiler complains that it can't find `memset`).

On some platforms you might also need to alter the #include statement in the `DrawingWindow.h` file from `"SDL.h"` to `"SDL2/SDL.h"` to allow the compiler to find the SDL library.

If you are using CMake (either from the command line or inside an IDE) you might get a compile error referring to `asan`. This is mostly likely due to not having the Address Sanitizer library installed. You can either try to install this library, or just edit the `CMakeLists.txt` file to remove reference to `asan` (it isn't essential to the project - it's just sometimes helpful for tracking down segfaults and memory leaks).  


# 
### Task 7: Understanding the Template


It is useful to spend a little time getting to know the structure of the <a href="extras/RenderProject/src/MainApplication.cpp" target="_blank">main application class</a> in the template project. We are going to be using it a lot over the next few weeks, so it is going to be important. The main function in the template contains a "forever" loop which:

1. Polls the event queue for incoming mouse and keyboard events
2. Draws the scene to the screen memory buffer (using the `RedNoiseRenderer`)
3. Renders the memory buffer onto the SDL window

Drawing the scene onto the screen buffer basically involves looping through a pixel at a time, deciding what colour it should be, packing that colour into an integer and setting the relevant pixel to that colour. The `RedNoiseRenderer` example just creates a random pattern of red pixels of varying brightness. This may seem a bit pointless, but it does allow us to make sure that the code in actually running. In later weeks we are going to do more sophisticated (and more useful) things with the pixels !

You will note from the `handleEvent` method that it is possible to respond to both mouse and keyboard events. Currently key presses simply print out messages to the terminal (by directing strings to the `cout` object). In later weeks we are going to use these event handlers to allow the user to navigate around a scene and switch between different rendering modes. For the time being however, just printing out messages is fine.

You may also notice the pressing the `TAB` key with switch on and off frame saving. This feature will be useful later on in the unit, when you are going to need to export rendered frames as images. When frame saving is in operation, screenshots of the current content of the window are saved to the filesystem. You will notice that both PPM and BMP files can be generated. The reason for this is that some platforms and some image viewers can be fussy about the format of image files. Pick whichever file format works best on your platform and work with that.

Finally, you will notice the hitting the `ENTER` key will toggle fullscreen mode of the application. This will be really useful later in the unit, when you wish to zoom in on a render in order to check that is it being drawn correctly.  


# 
### Task 8: Add New Classes


The final task in this week's workbook is to check that you have a good grasp of the project template and are able to effectively use the development tools and platforms you have chosen. At some point over the next few weeks, your code is going to become complex enough to justify creating additional classes (in addition to the existing classes in the project). It is a good idea to check that you have the knowledge and understanding to add new C++ classes to the project _now_ (before the workload on the unit starts to ramp up).

At some point soon you will need to change the drawing behaviour of the project, so that it draws something more useful (such as 3D renders). It is useful to delegate drawing to separate classes - this modularity will make it a lot simpler to switch between rendering modes when the time comes (later on in the unit).

Open the `RenderProject` folder and create a new class using the development tools that you have chosen to use on this unit. This might be as simple as creating `.ccp` and `.h` files in the filesystem (if you are using the command line to compile your code) or this might involve using the features of an IDE to introduce a new class into the project. The new class that you create should be called `BlueNoiseRenderer` and should do a similar job to `RedNoiseRenderer` (except this time the pixels should be blue in hue). In the `MainApplication` file, instantiate your `BlueNoiseRenderer` and point the `currentRenderer` at that instance (so that this is used to render graphics in the main window).

It is essential that you create all of your new classes in the `src` folder (this is where all your code should be placed). Don't create new classes in the `libs` folder - this folder is just for existing code that we have given to you. During the marking process we will only consider code in the `src` folder - anything in `libs` will be ignored !  


**Hints & Tips:**  
If you are struggling to understand how classes are structured in C++, take a look in the `sdw` folder (this can be found in the `libs` directory). This folder contains various different classes that we will be using later on in the unit. These provide some useful examples of how to define classes in C++. Note that due to the flexibility of the language, there is more that one way to create classes, however the samples in the `sdw` folder illustrate a fairly good approach to achieve this.  


# 
### End of workbook
