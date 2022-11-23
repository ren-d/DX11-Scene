# DirectX 11 Scene
### Project Plan
To create a small Swiss inspired village with a lake surrounded by mountains. Including a sun that orbits the landscape and streetlamps about the village.
## Checklist
### Geometry
  - [ ] Height Mapped Plane
    - [x] added Mesh
    - [ ] Per Pixel normal map
    - [ ] Vertex Manipulation
    - [x] Textured
    - [ ] Multiple Noise Maps
    - [ ] UI editable
  - [ ] Imported models
    - [ ] Houses
      - [ ] Textured
    - [ ] Street Lamps
      - [ ] Textured
  - [ ] Other Geometry
    - [ ] Road
      - [ ] Textured
    - [ ] Grass
      - [ ] Generated
      - [ ] UI editable
    - [ ] Water
      - [ ] Waves
      - [ ] Textured
      - [ ] UI editable
### Lighting & Shadows
  - [ ] All objects cast shadows
  - [ ] Normal Recalculation
  - [ ] Directional lights
    - [ ] Sun
      - [ ] Orbits world
      - [ ] UI editable
    - [ ] Moon
      - [ ] Orbits world
      - [ ] UI editable
  - [ ] Point Lights
  - [ ] Spot Lights
    - [ ] Street lamps
      - [ ] UI editable
### Shaders
  - [ ] Tessellation
  - [ ] Water
  - [ ] Tilt shift
  - [ ] Depth of field
  - [ ] Edge Detection outline
  - [ ] Kernel
  - [ ] Texture
  - [ ] Depth
  - [ ] Shadow
### Stretch Goal
  - [ ] Cloud Shader using voronoi noise and raymarching
## Coding Standards

### Variables

**General**: camelCase

**Member**: m_camelCase

**Bool**: preface with "is" or "has"

**Const**: ALL_CAPS

**Enum**: ALL_CAPS


### Functions

**General**: camelCase

**Getters & Setters**: camelCase

### Classes

General: PascalCase
