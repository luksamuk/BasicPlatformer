Sonic Engine v0.3.9b - Powered By Oficina Framework v1.3b
======================================================

Made by Lucas Vieira (luksamuk)

![alt text](Extras/Screenshots/ss03.png)

Changelog
---------
v0.3.9 (WIP)
- Added Parallax scrolling background;
- Added Basic Effects and Effect Spawners;
- Added WIP backgrounds for IIz, AAz, CCz (both levels 6 and 11), TFF (parallax only);
- Updated song for AAz;
- Added songs for CCz (both 6 and 11), DDz, SSz, WWz, JJz;
- Updated HUD;
- Improved usage of various sprites, which are now appropriately disposed;
- Various small changes;
- Changed Dynamic Dash Zone to Radiant Rush Zone;
- Added death/drowning;
- Updated Sonic and Super Sonic's sprites and palette;
- Updated Oficina version to v1.3 Snapshot 622;
- Added a new collision system (WIP);
- Added a debug util tool for drawing 2D primitives (WIP);
- Added Egg Walker boss (Test purposes only - WIP);
- Added a Level Editor subproject (OFLED - WIP).

v0.3.5
- Added Options menu;
- Added HUD (currently, only Timer is implemented);
- Added Title Card (WIP, might have positioning issues and has no ACT sign);
- Added various planned level names;
- Renamed TestScreen to LevelScreen;
- Recovered old test level and enabled it via Level Select (for Isolated Island Zone);
- Added WIP zones The Final Fight and Engine Test;
- Improved Level Select;
- Changed controls on some situations;
- Added a few more assets - full credits are planned for the Credits Screen to be implemented.

v0.3
- Added Main Menu;
- Added Level Select;
- Added Logo.

v0.2.9
- Started implementing collision system by region-based collisions
  (still inefficient in terms of memory. DO NOT run it on Debug target.
  You have been warned);
- You can now toggle hitboxes and region view regardless of debug;
- Added water! (only sound effects and value changes);
- Added Super Sonic's flying sprite.

v0.2.5
- Added Super Sonic animations;
- Music now speeds up on Speed Shoes mode;
- Music changes depending on day's hour.

Objective
---------
This engine is a collision and physics study, turned to the purpose of
replicating the original Sonic engine from Genesis games, as much as possible.
This software was developed with the sole purpose of study, and as
proof-of-concept for Oficina Framework itself.


A Little Note
-------------
As you might notice, there is plenty of unfinished stuff there. I still plan to
handle collisions in a better way, as it only supports very specific ramps and
rectangles for now, placed by hand on a hardcoded function.
Implementing real maps with a clever way to handle collision detection is my next
assignment.


License
-------
The code for this engine is completely Public Domain (check LICENSE for
details). You can do whatever you want with the code.

However, I do not own any of the libraries specified on the Dependencies section
(except for OficinaFramework) and, as of this current date (4/26/2016), I haven't
publically disclosed the source code for Oficina Framework, which is the actual
engine running underneath and managing everything, from screen to resolution to
music playback. So if I let you borrow the engine someday or if I disclose it,
I ask you to respect its license, whichever it is.

As for the binary release of this Sonic Engine, you can do whatever you want
with it; I also do not own Sonic nor anything related to it. All character
graphics, the debugger's font and music/sound effects are owned by SEGA.


Dependencies (Binary Release)
-----------------------------
Most dependencies are already included with the binary.
Software used is:

- OficinaFramework v1.3b-622
- OpenGL v3.1
  - GLEW v1.13.0
- SDL2 v2.0.4
- SDL2_image v2.0.1
  - libpng
  - libjpeg
  - zlib
- OpenAL v1.1
  - libogg
  - libvorbis
  - libvorbisfile
- PhysicsFS (libphysfs) v1.1
- Visual C++ 2015 Redistributable

It may be necessary to install both OpenAL 1.1 and VC++2015 redistributable,
which are included on the "redist" folder. For VC++2015, choose whichever fits
the binary's architecture.


Build
-----
I highly recommend you not to. Download a release instead, if you want to test.
You'll need OficinaFramework v1.3b dependencies, as long as the framework itself,
to build this. Or you can simply grab a snapshot from releases section.
Check OficinaFramework's dependencies at https://luksamuk.github.io/OficinaFramework.
They're roughly the same for the binary, though.
If you're using Visual Studio 2015, you may want to tweak the include and libs directories
for all targets and platforms (just because I did it on my computer and I didn't want to
release the code before, duh). A Linux Makefile is planned soon.


Usage
-----
Simply open SonicEngine.exe.
Make sure you have OpenAL and Visual C++ 2015 Redistributable installed, as
pointed in the Dependencies section.


Controllers
------------
Oficina Framework supports both keyboard controls and game controllers.
To use a game controller, simply connect it before starting the game.

- Keyboard:

| Key       | Codename |
|-----------|----------|
| W         | Up       |
| A         | Left     |
| S         | Down     |
| D         | Right    |
| K         | Action 1 |
| L         | Action 2 |
| Enter     | Start    |
| Backspace | Select   |

- Xbox 360 Controller:

|  Button    | Codename     |
|------------|--------------|
| Left Stick | Directionals |
| A          | Action 1     |
| B          | Action 2     |
| X          | Action 3     |
| Start      | Start        |
| Back       | Select       |

Exit game at any time by manually closing the window.
If you close the window while loading, the game may hang until loading is
finished.


Controls
--------

- "Powered By" Screen

| Key      | Action       |
|----------|--------------|
| Action 1 | Next screen  |
| Start    | Next screen  |



- Main Menu / Level Select / Options

| Key           | Action                                     |
|---------------|--------------------------------------------|
| Directionals  | Change selection                           |
| Action 1      | Pick selection                             |
| Action 2      | Previous Screen (unavailable on Main Menu) |
| Start         | Pick selection                             |



- Game Controls

| Key                                | Action                                         |
|------------------------------------|------------------------------------------------|
| Directionals                       | Move                                           |
| Action 1   (while on ground)       | Jump                                           |
| Directional UP (if stopped)        | Look Up                                        |
| Directional DOWN (if stopped)      | Crouch Down                                    |
| Directional DOWN (if enough speed) | Roll                                           |
| Action 1 (while crouched)          | Spindash: Mash to rev up, release DOWN to roll |
| Start                              | Pause/Unpause Game                             |
| Select (while paused)              | Return to Level Select                         |



- Debug Controls

The following controls are related to the debugger:

| Key                            | Action                                             |
|--------------------------------|----------------------------------------------------|
| F1                             | Toggle debugger                                    |
| F2                             | Toggle debugger complexity (minimalistic debugger) |
| F11                            | Toggle fullscreen                                  |

Gamepad counterparts:

| Buttons                        | Action                                             |
|--------------------------------|----------------------------------------------------|
| (Press Left Stick) + RB        | Toggle debugger                                    |
| LB                             | Toggle debugger complexity (minimalistic debugger) |

- Debug Mode Controls

Debug Mode only works if debugger is visible.
The following controls and actions can only be performed while Debug Mode
is active:

| Key/Situation     | Action                                             |
|-------------------|----------------------------------------------------|
| 1 (non-numpad)    | Reset character states                             |
| 2 (non-numpad)    | Toggle Speed Shoes mode                            |
| 3 (non-numpad)    | Toggle Super Sonic                                 |
| 4 (non-numpad)    | Toggle collision, grids, AABBs and sensors         |
| 5 (non-numpad)    | Normal Shield (toggles if shield is active)        |
| 6 (non-numpad)    | Bubble Shield (toggles if shield is active)        |
| 7 (non-numpad)    | Kills Sonic instantly                              |
| 8 (non-numpad)    | Drowns Sonic instantly                             |
| F3                | Reposition Sonic at beginning of level             |
| Mouse Click       | Place Sonic on mouse position                      |

The same is valid for gamepad buttons:

| Button/Situation  | Action                                             |
|-------------------|----------------------------------------------------|
| Digital Hat Up    | Reset character states                             |
| Digital Hat Right | Toggle Speed Shoes mode                            |
| Digital Hat Down  | Toggle Super Sonic                                 |
| Digital Hat Left  | Toggle collision, grids, AABBs and sensors         |
| Action 3 (X)      | Normal Shield (toggles if shield is active)        |
| Action 4 (Y)      | Bubble Shield (toggles if shield is active)        |
| Tap Right Stick   | Reposition Sonic at beginning of level             |
