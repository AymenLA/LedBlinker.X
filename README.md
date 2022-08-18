# Cmake exemple project for PIC18F cross compilation

## Introduction

This project was a learning example for using cmake files to cross compile code for other targets. Since I've never been very fan of using MplabX (Microchip solution to create projects and compile code for PIC MCUs), I've decided to create a small project for this purpose using a simple LEDBlinker example and PIC18F4550 as a target, and it was quite a journey !

I'm pleased to share with you what I've done hoping that some of you could help me figure out a solution for some of the limitations and also make this project more generic (PIC24, PIC32, dsPIC ..).

## What do you need to use this project

As far as I know, you'll need a Linux machine with cmake tool installed (version 3.18.6 and later) and install the xc8 compiler from Microchip website.

You'll also need to install Mplab IPE and have a PICKIT4 programmer if you want to run this example on a target.

In order to burn your code from command line, **ipecmd.jar** utility developed by Microchip needs to be executed by calling the proper java execution environment, normally this environment comes with Mplab IPE so no need for installing Java by your self.

## Cloning and compilation

```
git clone git@github.com:AymenLA/LedBlinker.X.git && cd LedBlinker.X/build
cmake ../application
make pcode // <- doesn't work for now, execute its command directly
make
make burn
```

## Known limitations
**1- target make pcode issue :**

Since the xc8 compiler is sophisticated proprietary Compiler developed by Microchip for performance, this compiler doesn't do things the same way as a normal compiler and generates .p1 intermediate files instead of **.obj** of **.o**, this is not configurable via compiler options and, unfortunately, cmake variable **CMAKE_C_OUTPUT_EXTENSION** doesn't seems helpful here.

![screen from xc8 Microchip documentation.](https://raw.githubusercontent.com/AymenLA/LedBlinker.X/master/media/xc8-documentation-p-code-files.png "screen from xc8 Microchip documentation.")

Read more about this in the xc8 user manual [MPLAB® XC8 C Compiler User’s Guide](https://ww1.microchip.com/downloads/en/devicedoc/50002053g.pdf).

So the only solution that I've found was to create a custom cmake target **pcode** in order to parse binary directory created by cmake and replace every **.obj** occurrence by **.p1**, but unfortunately this is not working properly : the used command on its own do the job, but for some reason when executed by cmake it does nothing, so the only solution is to run target pcode command manually after executing cmake script and before execution make, this should be done each time you run cmake script again.

## What should be improuved

**1- Hard coding paths, target name and project name:**

For this limitation, I just was too lazy to do things properly. feel free to experiment and change things !

**2- Burn command should be improuved:**

The burn command relies on the the existence of the command line tool **ipecmd.jar**, it would be nice to make this a cmake rule and stop the cmake script if this tool is not found.

**3- xml file, the elephant in the room**

The **memoryfile.xml** is a very important file for the link stage and should be provided, it would be great the be able to generate it at runtime from a *.json* file.

**4- build options, link options**

Many of the options used for build and link are redundant, it would be nice to replace them by local variables.

## what about a small demo ?

![gif file with compilation and burn steps.](https://github.com/AymenLA/LedBlinker.X/blob/master/media/cmake_demo-resized.gif?raw=true "gif file with compilation and burn steps.")


## Disclaimer

_Some parts of the C code used as example are reused AS IS form Microchip MLA® libraries examples, those codes are property of their respective owners and I DO NOT PRETEND TO BE THE AUTHOR OF ANY OF THEM_

