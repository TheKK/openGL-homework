/*

vertexShader

author: TheKK <thumbd03803@gmail.com>
date: 10/16/2013

*/
#version 400 core

in vec3 position_modelspace;
in vec3 normal_modelspace;

out vec3 position_worldspace;
flat out vec3 normal_cameraspace;
out vec3 eyeDirection_cameraspace;
out vec3 lightDirection_cameraspace;
out vec3 lightPosition_worldspace;

uniform mat4 rota; 
uniform mat4 revoX;
uniform mat4 revoY;
uniform mat4 revoZ;
uniform mat4 model;	//Model matrix
uniform mat4 view;	//View( camera ) matrix
uniform mat4 proj;	//Projection matrix
uniform mat4 viewport;	//Viewport matrix

uniform vec3 lightPosition_space;

void main () {
	gl_Position = viewport * proj * view * revoX * revoY * revoZ * model * rota * vec4( ( position_modelspace ), 1.0 );
	
	//Object in the world space
	position_worldspace = ( revoX * revoY * revoZ * model * rota * vec4( position_modelspace, 1.0 ) ).xyz;

	//In camera space
	vec3 position_cameraspace = ( view * revoX * revoY * revoZ * model * rota * vec4( position_modelspace, 1.0 ) ).xyz;
	eyeDirection_cameraspace = vec3( 0.0, 0.0, 0.0) - position_cameraspace;	

	//Vector that goes from vertex to light source, in camera space
	lightPosition_worldspace = lightPosition_space;
	vec3 lightPosition_cameraspace = ( view * vec4( lightPosition_worldspace, 1 ) ).xyz; 	
	lightDirection_cameraspace = lightPosition_cameraspace - position_cameraspace ;

	//Normal of the surface, in camera space
	normal_cameraspace = ( view * revoX * revoY * revoZ * model * rota * vec4( normal_modelspace, 0 ) ).xyz;
};

