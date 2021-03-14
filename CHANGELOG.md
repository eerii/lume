# Changelog 📝

## **0.1** ~ Basics

###  **[0.0.12]** ~ Camera _2021-03-13_
#### Added
- Basic camera
- Barebones camera follow
- Lookahead X
### Changed
- Fixed dithering
- Fixed light position when resizing

###  **[0.0.11]** ~ Dithering _2021-03-13_
#### Added
- Light dithering
- Background
- Toggle dithering

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

