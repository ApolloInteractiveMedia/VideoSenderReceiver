# Video Sender and Receiver

This project contains a simple API that can be used to stream video
and audio from an OpenGL 3.x+ application over the network. We are
using hardware accelerated video encoding and decoding, making this
library super fast. We've build this library for our need to stream
video from one OpenGL application and to receive it in another one
for playback.

<img src="https://c2.staticflickr.com/4/3940/33881827475_8c86be4af5_o.png" alt="">
<img src="https://c2.staticflickr.com/4/3856/33724935252_2b9e934fe5_o.png" alt="">

## Prerequisites

### Windows

We have tested this library on Windows 8.1 with Visual Studio 2013 and
Visual Studio 2015. Make sure to install at least:

- Visual Studio 2013 or
- Visual Studio 2015
- Git Bash
- CMake 3.7+
- Intel CPU for hardware acceleration support

### Mac

We have tested this library on macOS Sierra 10.12.1, i7 with XCode 8.3.
Make sure to install:

- XCode 8.3+
- CMake 3.7+

## Compiling

We provide an opaque API with a static library for the most popular
compilers on Windows and Mac. To use this library you first have to
execute the _release.sh_ script that we provide. This will download
and setup all the dependencies for the examples.

Make sure you have installed the prequisites. On Windows open a Git
Bash terminal and on Mac just open a terminal and execute

    cd build
    ./release.sh

## Running the examples

We provide a video sender and receiver example. Run both to see the
output of the *example_sender* in the *example_receiver* application.
Note, we don't have echo cancellation build in, so you'll need to turn
down your speakers.

- Browse into `install/bin/`
- Run `example_sender`
- Run `example_receiver`
    

