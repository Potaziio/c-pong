# C-PONG

This is The basic Pong game programmed in pure C with my small OpenGL framework
It is meant to be a test of the framework 

## Table of Contents

- [Getting Started](#getting-started)
  - [Installation](#installation)
  - [Prerequisites](#prerequisites)

## Getting Started

GLFW and GLEW build libraries are required for this program to run, the Makefile looks for those under: include/GLFW/build/src/libglfw3.a and include/GL/lib/libGLEW.a

Follow build instructions of each library to build it properly for your system.


Tested build method on OSX/Linux: 

GLFW:

    go into include/GLFW

    make 'build' directory and cd into it

    do cmake .. and then make

    should end up with a src folder with the library 

    done.
GLEW:

    go into include/GL

    run make

    done.

## Installation

Create a 'build' directory on the root of the project, run 'make' while in the root project, app binary should be created in 'build', **go into 'build' and run 'app' from there**

### Prerequisites

glfw: https://github.com/glfw/glfw


glew: https://github.com/edoren/glew


cglm: https://github.com/recp/cglm


stb: https://github.com/nothings/stb

