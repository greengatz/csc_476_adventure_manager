#version 120
//Texture
uniform sampler2D uTexUnit;

//Switch toggle for leafs shading
uniform int leafToggle;

varying vec4 pos;
varying vec3 normal;
varying vec2 vTexCoord;

void main()
{
   vec4 texColor = texture2D(uTexUnit, vTexCoord);
   if (leafToggle == 1 && texColor.y > 0.7)
   {
      discard;
   }

   gl_FragColor = texColor;
}
