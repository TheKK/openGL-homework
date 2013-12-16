#version 400 core

in vec3 position_worldspace;
flat in vec3 normal_cameraspace;
in vec3 eyeDirection_cameraspace;
in vec3 lightDirection_cameraspace;
in vec3 lightPosition_worldspace;

out vec4 outColor;


void main() {
	//Make a light soruce
	vec3 lightColor = vec3( 1, 1, 1 );
	float lightPower = 900.f;	
	
	//Object properties
	vec3 objectDiffuseColor = vec3( 0.9, 0.9, 0.9 ); 
	vec3 objectAmbientColor = vec3( 0.2, 0.2, 0.2 ) * objectDiffuseColor;
	vec3 objectSpecularColor = vec3( 0.2, 0.2, 0.2 );

	//Calculate the distance between light and vertex	
	float distance = length( lightPosition_worldspace - position_worldspace );	

	//Normal of the computed fragment, in camera space
	vec3 n = normalize( normal_cameraspace );
	//Direction of the light to the vertex
	vec3 l = normalize( lightDirection_cameraspace );
	//Calculate the cosine value between l and n
	float cosTheta = clamp( dot( l, n ), 0, 1 );	//Get the value of dot with range form 0 to 1

	//Eye direction
	vec3 E = normalize( eyeDirection_cameraspace );
	//Direction of reflection of light
	vec3 R = reflect( -l, n );
	//Calculate the cosine value between E and R
	float cosAlpha = clamp( dot( E, R ), 0, 1 );

	outColor = vec4(
			objectAmbientColor +
			objectDiffuseColor * lightColor * lightPower * cosTheta / ( distance * distance ) +
			objectSpecularColor * lightColor * lightPower * pow( cosAlpha, 5 ) / ( distance * distance ),
			0	
			);
}
