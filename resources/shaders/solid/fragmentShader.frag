#version 330 core

#define MAX_LIGHTS 5

in vec3 fNormal;
in vec3 fPos;
in vec2 fUV;

out vec4 FragColor;



struct Light {
   vec3 pos;
   vec3 color;
   samplerCube shadowMap;
};



uniform vec3 cameraPos;
uniform Light lightsVector[MAX_LIGHTS];
uniform int lightsCount;


vec3 lightLocation;
vec3 lightColour;

struct Material {
   sampler2D diffusionMap;
   sampler2D specularMap;
   sampler2D normalMap;
   float inverseRoughness;
};

uniform Material mat;



int shadowCalc(int i) {

   vec3 lightToFrag = fPos - lightsVector[i].pos;
   lightToFrag = vec3(lightToFrag.x*-1, lightToFrag.y*-1, lightToFrag.z);
 
   float depth = texture(lightsVector[i].shadowMap, lightToFrag).x;

   float bias = 0.05;

   if (depth-bias < length(lightToFrag)/100) {return 1;} else {return 0;}

}



void main() {

   vec3 lighting = vec3(0.0);


   for (int i=0; i<lightsCount; i++) {

      // Vars
      lightLocation = lightsVector[i].pos;
      lightColour = lightsVector[i].color;

      // TODO: NORMAL MAPPING BUT TANGENT SPACE IS CONFUSING 
      // vec3 normalFromMap = (vec3(texture(mat.normalMap, fUV)) * 2.0) - 1.0;
      vec3 unitNormal = normalize(fNormal);
      vec3 lightDir = normalize(lightLocation - fPos);
      vec3 cameraDir = normalize(cameraPos - fPos);
      vec3 reflectDir = reflect(-lightDir, unitNormal);

      // ATTENUATION
      float distance = length(lightLocation - fPos);
      float linearFalloff = 0.09;
      float quadraticFalloff = 0.032;
      float attenuation = 1.0 / (1.0 + linearFalloff * distance + quadraticFalloff * (distance * distance));

      // DIFFUSE
      float diff = max(dot(unitNormal, lightDir), 0.0);
      vec3 diffuse = diff * lightColour * attenuation;

      // SPECULAR
      float specularStrength = 0.4;

      vec4 specularMapValue = texture(mat.specularMap, fUV);
      float specularMapBrightness = (specularMapValue.x + specularMapValue.y + specularMapValue.z)/3;
      float spec = pow(max(dot(cameraDir, reflectDir), 0.0), mat.inverseRoughness * specularMapBrightness);
      vec3 specular = specularStrength * spec * lightColour * attenuation;

      // AMBIENT
      float ambientStrength = 0.05;
      vec3 ambient = ambientStrength * lightColour;

      // if (shadowCalc(i) == 1) {
         // lighting += vec3(diffuse + specular + ambient);
      // }
      // else {
         // lighting += vec3(ambient);
         // // lighting += vec3(10);
      // }

      lighting += vec3(diffuse + specular + ambient);

   }

   FragColor = texture(mat.diffusionMap, fUV) * vec4(lighting, 1.0);
   


   // vec3 lightToFrag = fPos - lightsVector[0].pos;
   // lightToFrag = vec3(lightToFrag.x*-1, lightToFrag.y*-1, lightToFrag.z);

   // float mapDepth = texture(lightsVector[0].shadowMap, lightToFrag).x;

   // float calcDepth = length(lightToFrag)/100;
   // FragColor = vec4(mapDepth*10, 0.0, calcDepth*10, 1.0);

};