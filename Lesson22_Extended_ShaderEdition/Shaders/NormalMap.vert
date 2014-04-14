//NormapMap Vertex Shader



varying vec3 vLight_tan; 
varying vec3 vEye_tan;
varying vec2 texCoord;
attribute vec3 vTangent; 
                     

void main(void)
{


    //Pass the texture coordinate to the fragment shader.
    texCoord = gl_MultiTexCoord0.xy;

    //Note: We multiply t and n with gl_NormalMatrix. Thus they are in eye space coordinates.
    vec3 t; 
    vec3 b; 
    vec3 n = normalize(gl_NormalMatrix * gl_Normal);
    t = normalize(gl_NormalMatrix * vec3(0.0, 1.0, 0.0));
    //Calculate the binomral vector b from n and t
    b = normalize(cross(n, t)); 


    //Why is the vertexPos in the eye space here ???
    vec3 vertexPos_Eye = vec3(gl_ModelViewMatrix * gl_Vertex);
    vec3 vLight_tantor_Eye = gl_LightSource[1].position.xyz - vertexPos_Eye;

    //This is the same as multiplying the vLight_tantor_Eye with the TBN matrix
    //vLight_tan is thus in tangentspace!
    vLight_tan.x = dot(vLight_tantor_Eye, t);
    vLight_tan.y = dot(vLight_tantor_Eye, b);
    vLight_tan.z = dot(vLight_tantor_Eye, n);

    //This is the same as multiplying the vEye_tantor_model with the TBN matrix
    //vEye_tan is thus in tangentspace!
    vec3 vEye_tantor_model = -gl_Vertex.xyz;
    vEye_tan.x = dot(vEye_tantor_model, t);
    vEye_tan.y = dot(vEye_tantor_model, b);
    vEye_tan.z = dot(vEye_tantor_model, n);

    //Perform standard vertex position transformation
    gl_Position = ftransform();
}
    
    


