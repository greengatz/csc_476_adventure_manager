#version 120
//Texture
uniform sampler2D uTexUnit;

//We should only have one light eventually (the sun) or many smaller lights for the sun.
//Maybe do an array here sometime if we can.
uniform vec3 lightPos;
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;

//Switch toggle for leafs shading
uniform int leafToggle;

varying vec4 pos;
varying vec3 normal;
varying vec2 vTexCoord;

varying float fogDist;

void main()
{
   vec3 n,e,l1,h1,color,light1;
   float intensity = 1.0, result;
   n = normalize(normal);
   e = normalize((vec3(0.0,0.0,0.0) - pos.xyz));
   l1 = normalize(lightPos - pos.xyz);
   h1 = normalize(l1 + e);
   light1 = (ka + (kd * max(dot(l1,n), 0.0)) + (ks * pow(max(dot(h1,n), 0.0), s))) * intensity;
   color = light1;

   vec4 texColor = texture2D(uTexUnit, vTexCoord);
   if (leafToggle == 1 && texColor.y > 0.7)
   {
      discard;
   }

   vec4 fogColor = vec4(0.4, 0.4, 0.4, 1.0);
   // float fogFactor = (5 - fogDist)/(5 - 8); //linear fog equation
   float fogFactor = 1.0 - 1.0 / exp(fogDist * 0.15); //fog density is 0.15
   fogFactor = clamp(fogFactor, 0.0, 1.0);

   vec4 targetCol = texColor * vec4(color.x, color.y, color.z, 1.0);
   gl_FragColor = mix(targetCol, fogColor, fogFactor);
}
