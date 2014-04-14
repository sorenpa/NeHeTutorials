//Normal map  Fragment Shader

varying vec3 vLight_tan;
varying vec3 vEye_tan;
varying vec2 texCoord;
uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform float invRadius;

void main (void)
{
   
    vec4 colorMapBase = texture2D(colorMap, texCoord);
    
    vec3 fragmentNormal_tan = normalize( texture2D(normalMap, texCoord).xyz * 2.0 - 1.0);

    vec4 vAmbient = gl_LightSource[1].ambient * gl_FrontMaterial.ambient;

    float diffuse = max( dot(normalize(vLight_tan), fragmentNormal_tan), 0.0 );
    
    vec4 vDiffuse = gl_LightSource[1].diffuse * gl_FrontMaterial.diffuse * 
                    diffuse;	
    
    gl_FragColor =  (vAmbient*colorMapBase + vDiffuse*colorMapBase);
}