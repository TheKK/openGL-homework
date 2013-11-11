/*

object.cpp
-Class for OBJ object

author: TheKK <thumbd03803@gmail.com>

*/

#include "object.h"

Object::Object( char* OBJFilePath )
{
	mObjectName = NULL;
	mFilePath = OBJFilePath;
	
	mVertex = 0;
	mNormal = 0;
	mTexture = 0;	
	mElementNumber = 0;
}

bool
Object::loadOBJ()
{	
	//Create a file stream and check if there is any error
	ifstream OBJsoruce( mFilePath, ios::in );
	if( !OBJsoruce.good() ){
		cout << "OBJ file load failed: " << mFilePath << endl;
		return false;
	} 

	string strTmp;
	vector<GLfloat> vertex;
	vector<GLfloat> normal;
	vector<GLuint> element;
	vector<GLfloat> normalTable;		//Recored the normal vector table
	vector<GLuint> normalIndice;		//Recored the indice of normal vectors
	float vx, vy, vz;			//X, y and z coordinate of vertex
	float nx, ny, nz;			//X, y and z coordinate of normal
	int vertexIndex1, vertexIndex2, vertexIndex3;
	int normalIndex1, normalIndex2, normalIndex3;

	while( getline( OBJsoruce, strTmp ) ){
		if( sscanf( strTmp.c_str(), "%*s %d//%d %d//%d %d//%d", &vertexIndex1, &normalIndex1, &vertexIndex2, &normalIndex2, &vertexIndex3, &normalIndex3 ) > 0 ){
			//Recored the index of vertices( according to vertex table )
			element.push_back( vertexIndex1 - 1 );			
			element.push_back( vertexIndex2 - 1 );			
			element.push_back( vertexIndex3 - 1 );			

			//Recored the index of normals( according to normal table )
			normalIndice.push_back( normalIndex1 - 1 );
			normalIndice.push_back( normalIndex2 - 1 );
			normalIndice.push_back( normalIndex3 - 1 );	
		}
		else if( sscanf( strTmp.c_str(), "v %f %f %f\n", &vx, &vy, &vz ) > 0 ){
			//Make a vertex table just like in the OBJ file
			vertex.push_back( vx );
			vertex.push_back( vy );
			vertex.push_back( vz );
		}
		else if( sscanf( strTmp.c_str(), "vn %f %f %f", &nx, &ny, &nz ) > 0 ){
			//Make a normal table just like in the OBJ file
			normalTable.push_back( nx );
			normalTable.push_back( ny );
			normalTable.push_back( nz );
		}
	}
	OBJsoruce.close();

	//Check for error
	if( ( vertex.size() == 0 ) || ( normalTable.size() == 0 ) || ( element.size() == 0 ) || ( normalIndice.size() == 0 ) ){
		cout << "OBJ file broken!" << endl;
		return false;
	}

	//Match normal to each vertex
	normal.resize( vertex.size(), 0 );
	for( unsigned int i = 0; i < element.size(); i++ ){
		normal[ element[i] * 3 + 0 ] += normalTable[ normalIndice[i] * 3 + 0 ];	
		normal[ element[i] * 3 + 1 ] += normalTable[ normalIndice[i] * 3 + 1 ];	
		normal[ element[i] * 3 + 2 ] += normalTable[ normalIndice[i] * 3 + 2 ];	
	}	

	//Generate a VertexBufferObject and store the vertice data into it
	glGenBuffers( 1, &mVertex );
	glBindBuffer( GL_ARRAY_BUFFER, mVertex );
	glBufferData( GL_ARRAY_BUFFER,
			vertex.size() * sizeof(GLfloat),
			&vertex[ 0 ],
			GL_STATIC_DRAW
			);

	//Generate a NormalBufferObject and store the normal data into it
	glGenBuffers( 1, &mNormal );
	glBindBuffer( GL_ARRAY_BUFFER, mNormal );
	glBufferData( GL_ARRAY_BUFFER,
			normal.size() * sizeof(GLfloat),
			&normal[ 0 ],
			GL_STATIC_DRAW
			);

	//Generate a ElementBufferObject and store the element data into it
	glGenBuffers( 1, &mEbo );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mEbo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER,
			element.size() * sizeof(GLuint),
			&element[ 0 ],
			GL_STATIC_DRAW
			);	
	mElementNumber = element.size();

	return true;
}

void
Object::free()
{
	
	glDeleteBuffers( 1, &mVertex );
	glDeleteBuffers( 1, &normalBuffer );
	glDeleteVertexArrays( 1, &vao );
}
