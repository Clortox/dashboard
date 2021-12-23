Development
===========

All the boiler plate needed to show screens has been written. To develop your
own custom screens, however, you will need to know some SDL2. There is no real
drawing api, to allow you to have as much control over the display as possible.

dashboard uses a few objects for displaying things to the screen however, to
interact with the memory management. 

The default configuration is similar to web development "Carousels".
A series of **panels** are shown to the user by cycling through one by one.

A **board** is a singleton, and represents the physical display board. 
There is only one per instance of the program, and its has several **panels**

A **panel** is an object representing a slide on screen. This is a page with
weather, a news feed, etc. It is one of the scrolling pages shown to the user.
This is what you will need to write.

Each **panel** has a `draw()` function that you must implement. This will draw
everything to the screen using SDL2's API. If you want functions for getting
data, several threads, etc, **it is the panel's responsibility to implement
this**. The **board** provides a memory API, however you do not have to use
it.

The **panel** parent class has an `update()` and `update_texture()` function
you must implement. Use `update()` to update the data for the panel. Use
`udpate_texture()` to update the `SDL_Texture _texture` each **panel** has

Each **panel** also has an `initTexture()` function that you must implement. This
should initialize the texture and should be called in your draw function. It is
up to you to call this, to give you the option of lazy loading it. 
*This must be called in `draw()` or later, not in the constructor*. This is
because SDL will not be setup when your constructor is called, and therefore
will not be able to properly create the texture for the renderer.

The **_time_on_screen** value is in milliseconds, and will be the amount of time
the board will display the panel before transitioning. This value can be
modified at runtime if needed (ie displaying a dynamic length video).

The provided memory api stores textures (SDL_Texture), fonts (TTF_Font), and
strings (SDL_Texture). Both static values can be added that will exist for the
lifetime of the program, as well as dynamic versions that will be stored in a
cache (As of writing, only dynamically cached strings are implemented). All
functions related to looking up memory stored in these containers is performed
in O(1) time.

The SDL_Window and SDL_Renderer are provided as static globals, accessible via
the **board** object. 

Each **panel** is also recommended to add its own config.hpp file for
configuring possible settings. All static files that it needs should be placed
in the main config.hpp file

**See `src/panel/sample_panel*` for an example in code of a simple panel to
help you get started**

Things you need to implement as a panel creator
-----------------------------------------------

- `void mypanel::draw()`
- `mypanel::mypanel()`
- `mypanel::~mypanel()`
- `size_t mypanel::_time_on_screen` 
- `const_resources`

And be sure to inheret from dashboard::panel !
