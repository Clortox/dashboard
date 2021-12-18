dashboard
=========

My dashboard, for my house. Display RSS feeds and other custom bits of info via
direct SDL2 calls. 

Features/TODO
=============

- Write straight to framebuffer (sdl2)
- Display Weather rss feed
- Display camera feed
- Display who is home
- Display network speed
- Display plex currently playing
- Play audio files (background music/tracks)?

Depends on
==========

- sdl2
- stdlib

optional dependencies
---------------------

- Weather/RSS
  - rapidxml
  - libcurl

Building sdl2 on rpi
--------------------

Run in this order
```
sudo apt-get install libdrm-dev libgdm-dev
```

Next downlad the SDL source tarball

```
cd sdl-src
./configure --enable-video-kmsdrm
make -j4
sudo make install
```

compile the test.cpp file provided in ./tests . There should be output on the
screen as flashing. Feel free to change the desired graphics driver in the
raspberry pi config

Developing your own pannels
===========================

Refer to the
[development]
(DEVELOPMENT.md)
documentation
Usage
=====

To compile:

`make all`

To install:

`make install`

To uninstall:

`make uninstall`
