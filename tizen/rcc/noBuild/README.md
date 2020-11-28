Resources in this 'exception' folder need extra modules.


You should follow the sequence like below to use these resource codes.

First, Go to the craftroom to make custom platform image.(https://craftroom.tizen.org)
       And add certain module what you want to use to the basic platform image.

Second, move resource C file you want to use to src/resource/ folder
        move resource h file matched with above C file to inc/resource/ folder

Third, add module name to spec file and CMake file.
#example) in spec file - BuildRequires:  pkgconfig(capi-media-camera)
#         in CMake file - capi-media-camera

Fourth, include header file to the resource.h file.
