# Changelog 📝

## **0.2** ~ Light


### **[0.1.12]** ~ Platforms _2021-06-06_
#### Added
- Jump from bellow platform
- Fall when pressing down
#### Changed
- Collider overhaul


---

### **[0.1.11]** ~ Editor _2021-06-06_
#### Added
- Improved entities GUI
- Remove entities and components
- Get component name function
- Component GUI
- Display Vec2 in ImGui
- Gui Input
- Texture GUI
- Render collision boxes
- Window to scene coordinates
- Tilemap editor
- Save scenes
#### Changed
- Light now cleans every frame

---

### **[0.1.10]** ~ More light _2021-05-30_
#### Added
- Light cleaning function when switching scene
- Animation cleaning
- Player loses light toggle
- Player respawns when losing all the light
- Walk end animation
- Customizable border size for renderer (for smooth camera)
- Camera shake
- Checkpoints with light and fire (have to move instantiation from s_collider)
#### Changed
- Improved light processing and moved it to s_light
- Moved back flame offsets to player_controller, cleaner now
#### Fixed
- Lights didn't move and were permanent when switching scenes
- Animation crashing when scene changed
- Fixed lookahead and camera smooth follow
- Lights didn't take into account the scene border so they were displaced

---

### **[0.1.9]** ~ Web _2021-05-30_
#### Added
- Emscripten initial support
- Disable GUI though preprocessor directive and curr_state
- Makefile for web
- Web Support (WebGL 2, works in Firefox, not Safari)
- Noise texture is RGBA for web, Grayscale for PC
- Automatically update shaders for WebGL
- OpenGL error handling
#### Changed
- Renamed time to ftime

---

### **[0.1.8]** ~ State and Animation _2021-05-28_
#### Added
- Created player Move state machine
- Implemented Move SM in the controller
- Player Controller can return the name of the current states
- Player GUI
- Implemented Jump SM in the controller
- Timers, adapted to game speed and with no extra treads
- Jump squish animations
- Custom fps animations
- Queue animations
#### Changed
- Modified Jump state machine with new states: falling faster and crouching
- PlayerState is now a pointer inside Player Controller
- Greatly improved player animation, with state machines
- Flame offset modified from s_texture too (might change in the future)
#### Fixed
- Walk animation stopping when falling 1 block

---

### **[0.1.7]** ~ Water _2021-05-26_
#### Added
- Spawn points
- Choose closest spawn point
- Water collision layer
- Damage function to actor component, calls player respawn
#### Changed
- Cleanup conditional statements in fresa (game missing)
- Collision now returns a bitset with all the collision masks
#### Fixed
- Bug where the game crashed if hitting the transition from above without any input

---

### **[0.1.6]** ~ Scenes _2021-05-24_
#### Added
- Active scene
- GUI for changing scene
- Tilemaps with different tiles
- Collision masks
- Scene transition component
- Functional scene transition with player instantiation
- Load scene transitions
- Handle scene loading and unloading on change
#### Fixed
- Fire was a block for a few seconds when changing scene

---

### **[0.1.5]** ~ Fixes and State Machine _2021-05-15_
#### Added
- Light decreases with time
- Game speed controller
- Improved the menu
- State machine
- Array and static string changes (for SM)
- Transition tables
- Define state machines externally
#### Changed
- Started improving UI, changed to top menu
- Switched ImGui to docking
- Big refactoring and include cleanup
#### Fixed
- General polish and bug fixes
- Review all pointers and tried to fix EXEC_BAD_ACCESS
- Used a lot more pointer checks and error messages
- Finally fixed memory leaks!

---

### **[0.1.4]** ~ Procedural Animation _2021-05-12_
#### Added
- Fire component
- Procedural fire animation
- White and Perlin Noise generator
- Free camera
- Multiple textures
- Depth testing and layers
#### Changed
- Camera component and system
- Palettes with primary and secondary colors

---

### **[0.1.3]** ~ Serialization _2021-05-07_
#### Added
- YAML Serialization
- Load, write, destroy YAML files
- Edit/remove particular entries
- Append with specific typings
- Scene serialization
- Player serialization
#### Changed
- Different Vec2 and Vec2f, Rect2 and Rect2f (For precission and cleanness matters)
- Rect2 now uses x, y, w, h instead of pos and size
#### Fixed
- Now when you delete an entity, and create a new one, it has the propper name and not the previous one

---

### **[0.1.2]** ~ New rendering pipeline _2021-05-04_
#### Changed
- Completely redesigned rendering pipeline. Now it renders in low res, applies post processing and camera movement, then upscales it and add the remaining camera move (smooth cam but keeping it pixel perfect). Finally it places it in the middle of the screen and adds the background
- New and more clear mvp
- Separated the shaders into several pieces

---

### **[0.1.1]** ~ Fresa _2021-05-04_
#### Added
- 3D preliminary rendering
#### Changed
- Separated engine for more games

---

###  **[0.1.0]** ~ Brainstorm _2021-03-21_

The core of the app is done! We got most of the essential systems working, a player, a camera, a level, collissions, rendering everything... Now we can start focusing on features of the game itself.

The first step to take will be to organize all the thoughts and put them into paper, deciding on the main movement and interaction mechanics to be implemented. We then will start creating those abilities and the ligth mechanics, while also making levels happen. Art needs to be a priority as well, and we need at least placeholders for the animations and the levels.

---
---
---

## **0.1** ~ Basics

#### 2021-03-21

---

###  **[0.0.12]** ~ Camera _2021-03-13_
#### Added
- Basic camera
- Barebones camera follow
- Lookahead X
- Camera bounds
### Changed
- Fixed dithering
- Fixed light position when resizing
- Tilemap Performance Optimization!

---

###  **[0.0.11]** ~ Dithering _2021-03-13_
#### Added
- Light dithering
- Background
- Toggle dithering

---

### **[0.0.10]** ~ Renderer _2021-03-08_
#### Added
- Basic light variation
- Conditional compiling for SDL or OpenGL
- Basic OpenGL Renderer, VAO and VBO
- Version in the title
- Texture rendering
- Animation spritesheet support
- imGui OpenGL
- Render lights again
- Multiple lights
- Smooth min light blending
- Flip sprite
#### Changed
- Code cleanup
- Moved imgui to deps
- Moved controllers to ecs
- Moved all rendering to r_pipeline
- Separeted rendering between SDL and OpenGL
- Improved FPS count
#### Deprecated
- SDL_Renderer

---

### **[0.0.9]** ~ Light _2021-03-03_
#### Added
- Variable jump height
- LIGHT SHADER
- Grayscale render mode
- Light follows the player
#### Fixed
- When hitting a block's downside on_ground was set so you sticked to the ceiling

---

### **[0.0.8]** ~ Levels _2021-03-01_
#### Added
- Barebones animation
- Texture flip
- Load levels from PNGs

---

### **[0.0.7]** ~ Screen _2021-02-27_
#### Added
- Log levels
- Palette transitions
- FPS counter
- Always pixel perfect resolution
- Window resizing and padding
- Background padding color matches palette
#### Changed
- Cleaned up rendering code
- Fix variable names
- Changed "platform" to "events"
- Window title
- ImGUI mouse handling (for window padding)

---

### **[0.0.6]** ~ Color _2021-02-25_
#### Added
- Texture Loading
    - Offset
    - Load Surface, then Texture
- Shader Support
- Multi Textured GL Shader Renderer
- PALETTE SHADER
- (Other) Create Versioning
- (Other) Move to Lume
#### Changed
- Revamped Graphics Renderer

---

### **[0.0.5]** ~ Actor _2021-02-22_
#### Added
- Free Controller
- Player Controller
    - Acceleration
    - Friction
- Jump
    - Jump Grace
    - Coyote Time
- Respawn
- Actor List (GUI)

---

### **[0.0.4]** ~ Collisions _2021-02-21_
#### Added
- Create Basic Components
    - Collider
    - Texture
    - Tilemap
    - Actor
- Collision System
- Tilemap Collision System
- Render Collision Boxes

---

### **[0.0.3]** ~ GUI _2020-12-05_
#### Added
- Add imGUI
- SDL + imGUI
- Display the GUI
- Entity List

---

### **[0.0.2]** ~ ECS _2020-12-05_
#### Added
- Entity Basics
- Component
- Component Pool
- Scene
- Add, Get, Remove Components
- Scene View

---

### **[0.0.1]** ~ Core App _2020-12-04_
#### Added
- Graphic Rendering
- Time
- Game Loop
- Events
- Input
- Logging

---

---

---

# Format 💾

## **0.0** ~ Name of the update 🌟

### **[0.0.0]** ~ Name _date_
#### Added ✨ _(new features)_
#### Changed 🌿 _(changes in existing functionality)_
#### Deprecated 🥀 _(soon to be removed)_
#### Removed 🗑 _(deleted entirely)_
#### Fixed 🔨 _(bug fixes)_
#### Security 💻 _(in case of vulnerabilities)_

---

