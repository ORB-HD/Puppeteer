Puppeteer - A Motion Capture Mapping Tool
=========================================
Copyright (c) 2013-2018 Martin Felis <martin.felis@iwr.uni-heidelberg.de>
Copyright (c) 2018 Felix Richter <judge@felixrichter.tech>

Tip of the day: try the new scripting feature by running

    ./puppeteer subject3m.lua -s scripts/motionsickness.lua

Demo
=========================================

[![Puppeteer Demo](https://www.fysx.org/~martin/shared/puppeteer.png)](https://www.youtube.com/watch?v=-BVTGs8xCnQ)

Usage
=========================================

Puppeteer comes with an example model and motion capture data. To load them pass both model and motion capture data as arguments to the Puppeteer executable, e.g.:

    ./puppeteer model.lua data/testdata.c3d
    

Dependencies
=========================================

Puppeteer requires RBDL, the Rigid Body Dynamics Library that you can obtain from https://github.com/ORB-HD/rbdl-orb (open-source, zlib).

Additionally Puppeteer uses the following libraries and includes them in the `vendor/` folder:

  * c3dfile (c3d file parsing, MIT license)
  * LuaTables++ (LuaModel reading/writing, MIT License)
    * TCLAP (command line parsing, MIT license)
    * QtPropertyBrowser (Property widget, BSD license)

Building and Installation
=========================================

## Linux

1. Install RBDL, Lua, and the luamodel addon. See the readme file of [rbdl-orb](https://github.com/ORB-HD/rbdl-orb) for detailed installation instructions.
2. Install the graphics libraries that puppeteer depends on
    ```
    sudo apt install ffmpeg libavutil-dev libavcodec-dev libavutil-dev libavformat-dev libswscale-dev libvtk6-qt-dev
    ```
3.  Do a recursive clone of the puppeteer repository to also initialise all sub-repositories:
    >   git clone --recursive https://github.com/ORB-HD/puppeteer
    
    if not do you will have to initialise them yourself: 

    >   git submodule init
    >   git submodule update
4. Make separate build and installation directories *(optional)*
    ```
    mkdir puppeteer-build
    mkdir puppeteer-install
    ```
5. Run ccmake on puppeteer
    ```
    cd puppeteer-build
    ccmake ../puppeteer
    ```
6. Press 'c' to configure the CMakeCache.txt file.
    - If RBDL has been installed to a specific directory then you will have to set the CUSTOM_RBDL_PATH to RBDL's install folder.
    -  If all of the graphics libraries have been found and you want to install puppeteer to a specific location the puppeteer's cmake build settings will look like this (where the '...' will be a series of folders specific to your installation)
    ```
     CMAKE_BUILD_TYPE                 Release                                      
     CMAKE_INSTALL_PREFIX             /home/ ... /puppeteer-install
     CUSTOM_RBDL_PATH                 /home/ ... /rbdl-orb-release-install
     Qt5Core_DIR                      /usr/lib/x86_64-linux-gnu/cmake/Qt5Core      
     Qt5Gui_DIR                       /usr/lib/x86_64-linux-gnu/cmake/Qt5Gui       
     Qt5Network_DIR                   /usr/lib/x86_64-linux-gnu/cmake/Qt5Network   
     Qt5OpenGL_DIR                    /usr/lib/x86_64-linux-gnu/cmake/Qt5OpenGL    
     Qt5WebKit_DIR                    /usr/lib/x86_64-linux-gnu/cmake/Qt5WebKit    
     Qt5Widgets_DIR                   /usr/lib/x86_64-linux-gnu/cmake/Qt5Widgets   
     QtPropertyBrowser_BUILD_EXAMPL   OFF                                          
     RUN_AUTOMATIC_TESTS              OFF                                          
     VTK_DIR                          /usr/lib/cmake/vtk-6.3    
     ```
7. Build and install puppeteer using 
    ```
    make install
    ```
8. Modify your .bashrc file to include puppeteer's binary on the path (where the '...' will be a series of folders specific to your installation):
    ```
    export PUPPETEER=/home/.../puppeteer-install/bin
    export PATH=$PATH:$PUPPETEER
    ```
9. Open a new terminal in the puppeteer source directory and test the installtion by calling
    ```
     puppeteer model.lua data/testdata.c3d 
     ```
     A GUI should be launched that has a blocky humanoid model.
## Windows

At the moment we are unable to provide detailed installation instructions.

# Changelog

    * 2015-10-13: ensure that model markers are always sorted alphabetically in the GUI

# License

Puppeteer is published under the MIT license, which allows you to do pretty
much everything (including making changes and selling binaries). However
bug fixes, improvements, or other changes are most welcome!

Here is the full license text:

    Puppeteer - A Motion Capture Mapping Tool
    Copyright (c) 2013-2016 Martin Felis <martin.felis@iwr.uni-heidelberg.de>.
    All rights reserved.
    
    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the
    "Software"), to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to
    the following conditions:
    
    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.
    
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE

