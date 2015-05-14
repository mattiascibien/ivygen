# An Ivy Generator [![Build Status](https://travis-ci.org/mattiascibien/ivygen.svg?branch=master)](https://travis-ci.org/mattiascibien/ivygen) [![Build status](https://ci.appveyor.com/api/projects/status/y2rpe8f1fc8s4reh?svg=true)](https://ci.appveyor.com/project/mattiascibien/ivygen)

[![Join the chat at https://gitter.im/mattiascibien/ivygen](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/mattiascibien/ivygen?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)


Original code Copyright (c) 2007 Thomas Luft. All rights reserved.

This fork is maintained by [Mattias Cibien](http://mattiascibien.net)

## Changes since the original code

 - Port to Qt5
 - Improvements on rendering code
 - Usability improvements
 - No more need to triangulate models on import

## Usage

Import your obj+mtl scene, double-click to place the ivy root,
press "grow", and enjoy watching the ivy growing! 

Press "birth" to generate the full geometry. 

Use the export function to save your ivy geometry as obj+mtl for usage in your 3d world.
Please try to keep your scenes simple, since the growing process 
slows down in complex scenes heavily. 

In case your ivy is growing inside your
objects, use the "flip normals" button to flip the surface
normals of your objects. You can customize your ivy using the
provided settings - tool-tips will help you to understand
the settings.


## Camera controls

 - rotate - left mouse button
 - track - left mouse button + control
 - dolly - right mouse button
 - orbit - left mouse button + shift


## Licence

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
version 2 as long as this file is included.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
