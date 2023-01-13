# DirectX 11 Scene
### Project Plan
To create a small scene with gerstner waves and objects, showcasing multiple lighting types, vertex manipulation, shadow mapping and post processing compute shaders using DirectX11

## Index
[Scene](#scene)

[Objects](#objects)

[Tessellation](#tessellation)

[Lighting & Shadows](#lighting--shadows)

[Post-Processing Bloom](#post-processing-bloom)

[Post-Processing Render Passes](#post-processing-render-passes)

[Coding Standards](#coding-standards)

## Screenshots
### Scene
<sub>[index](#index)</sub>
  
<sub> View mode GUI </sub>

![image](https://user-images.githubusercontent.com/49440612/212222918-f904ff2b-0c9b-48c4-bcdc-188559e813e4.png)

<sub> Wireframe </sub>

![image](https://user-images.githubusercontent.com/49440612/212222421-d0a2e8d6-2f69-47e1-9a99-75202b6f9f6a.png)

<sub> Scene rendered without post processing effects </sub>

![image](https://user-images.githubusercontent.com/49440612/212222487-9f88be78-e4aa-4cc5-988e-bb2fcff5926d.png)

<sub> Scene rendered with post processing bloom compute shaders</sub>

![image](https://user-images.githubusercontent.com/49440612/212222575-137d327a-4e04-4421-b1a4-43808a78b531.png)

<sub> Scene rendered with UVs visible </sub>

![image](https://user-images.githubusercontent.com/49440612/212222636-8bbadcbc-b985-403e-a91e-704782af97a6.png)

<sub> Scene rendered with normals visible</sub>

![image](https://user-images.githubusercontent.com/49440612/212222667-23f47a12-744e-4c94-8a6f-f22a7ba4c776.png)

### Objects
  
<sub>[index](#index)</sub>
  
<sub> Wave GUI </sub>

![image](https://user-images.githubusercontent.com/49440612/212223315-9bc2fb3d-8def-4918-bdb7-8d6ba0b76923.png)

<sub> Example of vertex manipulation using multiple Gerstner Waves. calm waters on (left). calm waters off(right). </sub>

![image](https://user-images.githubusercontent.com/49440612/212223104-3e79469f-b741-44f3-9d1a-cfde7514a068.png)![image](https://user-images.githubusercontent.com/49440612/212223119-71bb57ae-8dce-4819-b666-d9728f3a874a.png)


<sub> Normal and specular mapped barrel (left), Barrel with normals visible (right) </sub>

![image](https://user-images.githubusercontent.com/49440612/212223457-3d42f5e7-fa56-45d0-b372-18d92cea0e90.png)![image](https://user-images.githubusercontent.com/49440612/212223482-eafddf6f-4f23-4445-b783-631518b52ed6.png)

### Tessellation
  
<sub>[index](#index)</sub>
  
<sub> Tessellation GUI </sub>

![image](https://user-images.githubusercontent.com/49440612/212223361-f8f89304-8a45-4485-82a2-0cfe3ed6a9ff.png)

<sub> Wireframe view of tessellation value set to 1.0f (left), Wireframe view of Tessellation value set to 12 (right) </sub>

![image](https://user-images.githubusercontent.com/49440612/212223541-1f14b739-7f41-4db5-9854-37a9679e125c.png)![image](https://user-images.githubusercontent.com/49440612/212223549-810ac9ab-4bfb-4970-95f7-6015cfea37d5.png)


### Lighting & Shadows

<sub>[index](#index)</sub>
    
    
<sub>Showcasing multiple light types casting shadows: Point lights (blue & green), Spotlight (red), Directional light (pink)</sub>

![image](https://user-images.githubusercontent.com/49440612/212220864-5a9fee35-139a-415f-b5db-d6aee9603a7c.png)

<sub> Light type dropdown - all lights can be dynamically set at runtime</sub>

![image](https://user-images.githubusercontent.com/49440612/212221728-e7c4f590-fc8c-48e4-9404-3b504c88e0ac.png)


<sub> (left) Original shadow showcase scene: Point lights (blue & green), Spotlight (red), Directional light (pink). 
(right) same scene with changed light types: Point light (green), Spotlights (red, blue & pink)
</sub>

![image](https://user-images.githubusercontent.com/49440612/212221486-3b78b8b8-c862-4d68-8cd2-58d755bc9456.png) ![image](https://user-images.githubusercontent.com/49440612/212221498-a7c05ea6-af28-430a-b7bd-b54e10658909.png)

<sub> lighting GUI of point lights (top left) directional lights (top right) spotlights (bottom left)

![image](https://user-images.githubusercontent.com/49440612/212221992-aa461f0e-b365-46cc-8ff7-37269cdac01c.png)![image](https://user-images.githubusercontent.com/49440612/212222314-fea606c1-5baa-44c4-b2cc-97c50f7775ac.png)![image](https://user-images.githubusercontent.com/49440612/212222053-92d99152-5917-4d68-871c-6eff6cedee17.png)

<sub> Display shadow map GUI option </sub>

![image](https://user-images.githubusercontent.com/49440612/212223703-1f3346bc-3fa6-42b6-836b-ad7a09543252.png)

<sub> Point light cubemap depth maps displayed </sub>

![image](https://user-images.githubusercontent.com/49440612/212223765-792552a6-fe25-40e5-be6e-7fbe1eb8b015.png)

<sub> Spotlight depth map displayed </sub>

![image](https://user-images.githubusercontent.com/49440612/212223827-648cfd2f-c74d-4d43-9a99-a418ad65af95.png)

<sub> Directional light depth map displayed </sub>

![image](https://user-images.githubusercontent.com/49440612/212223892-7999b200-63a6-4484-a760-754393168aec.png)

### Post-Processing Bloom
  
  <sub>[index](#index)</sub>

<sub> Post processing GUI options </sub>

![image](https://user-images.githubusercontent.com/49440612/212224021-53f31880-f5e5-4928-8ff0-5e01880b0b20.png)

<sub>Bloom with low threshold</sub>

![image](https://user-images.githubusercontent.com/49440612/212221071-5cdea998-2ab7-4e41-8d1e-09ffb0d3fbda.png)

<sub>Bloom intensity set to 1.0f (left), Bloom intensity set to (right)</sub>

![image](https://user-images.githubusercontent.com/49440612/212221122-508adb52-61d3-4082-a1a9-94835b5d0b4e.png) ![image](https://user-images.githubusercontent.com/49440612/212221136-afde20a4-e2d1-4580-8367-f7f05a46a037.png)

<sub>Blending gamma set to 0.5f (left), Blending gamma set to 3.0f (right)</sub>

![image](https://user-images.githubusercontent.com/49440612/212221325-7f5208ea-013d-4da7-8a3a-16ecabaf59cc.png) ![image](https://user-images.githubusercontent.com/49440612/212221334-d52c753c-49c1-4222-9ede-7d6947f6d0a0.png)

### Post-Processing Render Passes
  
 <sub>[index](#index)</sub>
  

<sub> Original Scene </sub>

![image](https://user-images.githubusercontent.com/49440612/212224199-5bbb4ee6-4bab-4beb-bb5f-d10130990d72.png)

<sub> Brightness pass compute shader output</sub>
![image](https://user-images.githubusercontent.com/49440612/212224462-e96fb49c-bb37-4cd0-9b20-3345d7ad93eb.png)

<sub> Downsample compute shader pass 1 </sub>

![image](https://user-images.githubusercontent.com/49440612/212224639-c2e4c0b1-17e0-45cd-834c-dae85594fa53.png)

<sub> Downsample compute shader pass 2 </sub>

![image](https://user-images.githubusercontent.com/49440612/212224595-28439aa6-9802-4562-8d32-e31ac9ddc333.png)

<sub> Downsample compute shader pass 3 </sub>

![image](https://user-images.githubusercontent.com/49440612/212224615-0f5764d3-acfc-4d78-bdbc-5c085322a7a9.png)

<sub> Gaussian Blur pass 1 </sub>

![image](https://user-images.githubusercontent.com/49440612/212224711-fd0e7891-20cd-45b2-979b-12ff64d9a2a7.png)

<sub> Up sample compute shader pass 1 </sub>

![image](https://user-images.githubusercontent.com/49440612/212224732-2910553b-199c-4162-8b75-f522bbf50464.png)

<sub> Gaussian Blur pass 2 </sub>

![image](https://user-images.githubusercontent.com/49440612/212224751-59ae9181-20e5-4f37-96ad-313f6eb6252c.png)

<sub> Up sample compute shader pass 2 </sub>

![image](https://user-images.githubusercontent.com/49440612/212224768-fef1bc45-1b05-4ee0-966e-44a678dbdeaf.png)

<sub> Gaussian Blur pass 3 </sub>

![image](https://user-images.githubusercontent.com/49440612/212224783-48646d0b-3ea2-47dd-a434-d3ccb23dc8a0.png)

<sub> Up sample compute shader pass 3 </sub>

![image](https://user-images.githubusercontent.com/49440612/212224794-fcab89bf-6162-4e90-badc-195ae489f58c.png)

<sub> Gaussian Blur pass 4 </sub>

![image](https://user-images.githubusercontent.com/49440612/212224813-d1df02fe-9842-4288-b045-57a14354b968.png)

<sub> Final blending pass </sub>

![image](https://user-images.githubusercontent.com/49440612/212224828-6ce779ce-d8cc-4974-b03d-d73a1d26b566.png)






## Coding Standards
  
<sub>[index](#index)</sub>
  
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
