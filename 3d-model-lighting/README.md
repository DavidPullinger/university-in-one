# OpenGL Assignment 1

## Setup

A Makefile is provided to compile the program. The program requires the
following libraries:

- OpenGL (this is linked differently on MacOS than on Linux - please refer to the Makefile if compiling for MacOS)
- SDL
- GLEW (included in the `include/` directory for convenience)
- GLM (included in the `include/` directory for convenience)

To compile the program, run `make` in the root directory. The executable will be
placed in the `build/` directory. To run the program, run `make run` in the root
directory. You are able to specify three parameters when running the program:

- `MODEL` - the model to load (default: `watermelon`)
- `ALBEDO` - the albedo texture to load (default: `watermelon_albedo.jpg`)
- `NORMAL` - the normal texture to load (default: `watermelon_normal.jpg`)

A few models and textures are provided in the `build/objs` and `build/textures` directories, respectively (these inflated the project size, I apologize - I included them for interest sake). Notable/useful ones include:

### Models

- `watermelon`
- `sphere`
- `skull`

### Textures

- `watermelon_albedo.jpg` and `watermelon_normal.jpg`
- `couch_albedo.jpg` and `couch_normal.jpg`
- `leather_albedo.jpg` and `leather_normal.jpg`
- `skull_albedo.jpg`
- `skybox/*` (see below)

## Transformation Controls

The following controls are available for transformation of the model and view (and manipulation of lighting):

### Target Change

- `m` - change to model transformation
- `v` - change to view transformation
- `l` - change to lighting manipulation

### Mode Change (model and view only)

- `r` - change to rotate mode
- `s` - change to scale mode

### Axis Change (model and view only)

- `x` - change to x-axis
- `y` - change to y-axis
- `z` - change to z-axis

### Lighting Change (lighting only)

- `1` - change to ambient lighting
- `2` - change to diffuse lighting
- `3` - change to specular lighting
- `4` - change to shininess

### Transform

- `up arrow` (target = model/view) - apply positive transformation along the current axis (based on
  current mode)
- `down arrow` (target = model/view) - apply negative transformation along the current axis (based on
  current mode)
- `up arrow` (target = lighting) - increase the current lighting value
- `down arrow` (target = lighting) - decrease the current lighting value

### Reset

- `space` - reset model to original position and orientation

## Extra Credit

Both of the following features were implemented (after much trial and error) by reading through the tutorials on [learnopengl.com](https://learnopengl.com/).

### Normal Mapping

The normal map is loaded in as a texture and the normal is calculated in the fragment shader. This is done by getting the corresponding normal from the normal map (using the fragment texture coordinates) and transforming it from tangent space to view space (using the tangent, bitangent, and normal vectors of the fragment). The normal is then used in the lighting calculations as usual. The normal can be toggled on and off using the `n` key.

### Skybox

The skybox is loaded in as six textures (one for each face of the cube) and is rendered using a cube with the camera at the center. The skybox is rendered last to avoid continuously overwriting the z-buffer (the skybox will always be at the back of models). In the vertex shader, the skybox is rendered with its z coordinate set to 1.0 to ensure that it is rendered at the far plane (this is done by setting position = position.xyww). The textures for the skybox are stored in the `build/textures/skybox` directory.

### Lighting Manipulation

While this wasn't implemented for extra credit, it may help to clearly see the difference between the different lighting types. The lighting can be manipulated by first entering lighting mode using the `l` key. The lighting type can be changed using the `1`, `2`, `3`, and `4` keys. The lighting value can be changed using the `up arrow` and `down arrow` keys, which will increase and decrease the corresponding lighting value (coefficient), respectively. For good results when loading in the couch/leather textures, I dropped the specular lighting because it made the texture unrealistically shiny.
