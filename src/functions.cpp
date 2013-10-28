/*

functions.cpp

author: TheKK <thumbd03803@gmail.com>
date: 10/16/2013

*/
#include "functions.h"

void setOrtho ( float projMat[4][4], float left, float right, float bottom, float top, float near, float far )
{	
	projMat[0][0] = 2.0 / ( right - left );
	projMat[1][1] = 2.0 / ( top - bottom );
	projMat[2][2] = 2.0 / ( near - far );
	
	projMat[0][3] =-1.0 * ( right + left ) / ( right - left );
	projMat[1][3] =-1.0 * ( top + bottom ) /	( top - bottom );
	projMat[2][3] = 1.0 *( near + far ) / ( near - far );	
}

void rotationX ( float matrix[4][4], double degree )
{	
	float sum = 0;
	float tmp[4][4];
	double rota[4][4] = {
		{ 1,             0,               0, 0 }, 
		{ 0, cos( degree ),-1*sin( degree ), 0 },
		{ 0, sin( degree ),   cos( degree ), 0 },
		{ 0,             0,               0, 1 }
	};

	for( int i = 0; i < 4; i++ ){
		for( int j = 0; j < 4; j++ ){
			sum = 0;
			for( int k = 0; k < 4; k++ ){
				sum += rota[i][k] * matrix[k][j];
			}
			tmp[i][j] = sum;	
		}	
	}

	//Get the value
	for( int i = 0; i < 4; i++ )
		for( int j = 0; j < 4; j++ )
			matrix[i][j] = tmp[i][j];
}

void rotationY ( float matrix[4][4], double degree )
{	
	float sum = 0;
	float tmp[4][4];
	double rota[4][4] = {
		{   cos( degree ), 0, sin( degree ), 0 }, 
		{               0, 1,             0, 0 },
		{-1*sin( degree ), 0, cos( degree ), 0 },
		{               0, 0,             0, 1 }
	};

	for( int i = 0; i < 4; i++ ){
		for( int j = 0; j < 4; j++ ){
			sum = 0;
			for( int k = 0; k < 4; k++ ){
				sum += rota[i][k] * matrix[k][j];
			}
			tmp[i][j] = sum;	
		}	
	}

	//Get the value
	for( int i = 0; i < 4; i++ )
		for( int j = 0; j < 4; j++ )
			matrix[i][j] = tmp[i][j];
}

void rotationZ ( float matrix[4][4], double degree )
{	
	float sum = 0;
	float tmp[4][4];
	double rota[4][4] = {
		{ cos( degree ),-1*sin( degree ), 0, 0 },
		{ sin( degree ),   cos( degree ), 0, 0 },
		{             0,               0, 1, 0 },
		{             0,               0, 0, 1 }
	};

	for( int i = 0; i < 4; i++ ){
		for( int j = 0; j < 4; j++ ){
			sum = 0;
			for( int k = 0; k < 4; k++ ){
				sum += rota[i][k] * matrix[k][j];
			}
			tmp[i][j] = sum;	
		}	
	}

	//Get the value
	for( int i = 0; i < 4; i++ )
		for( int j = 0; j < 4; j++ )
			matrix[i][j] = tmp[i][j];
}

bool loadOBJ ( string filePath, vector <GLfloat> *vertex, vector <GLuint> *element )
{	
	//Create a file stream and check if there is an error
	ifstream OBJsoruce( filePath.c_str(), ios::in );
	if( !OBJsoruce.good() ){
		cout << "OBJ file load failed: " << filePath << endl;
		return false;
	} 

	//Load line by line and search keyword to identify the data type( vertex or normal or ETC )
	string strTmp;
	while( getline( OBJsoruce, strTmp ) ){
		if( strTmp.find( "v" ) == 0 ){
			float x, y, z;	
			sscanf( strTmp.c_str(), "%*s %f %f %f", &x, &y, &z );

			vertex->push_back( x );
			vertex->push_back( y );
			vertex->push_back( z );
		}
		
		else if( strTmp.find( "f" ) == 0 ){
			int vertex1, vertex2, vertex3;
			int normal1, normal2, normal3;
			sscanf( strTmp.c_str(), "%*s %d//%d %d//%d %d//%d", &vertex1, &normal1, &vertex2, &normal2, &vertex3, &normal3 );

			element->push_back( vertex1 - 1 );			
			element->push_back( vertex2 - 1 );			
			element->push_back( vertex3 - 1 );			

			//normal.push_back( normal1 );
			//normal.push_back( normal2 );
			//normal.push_back( normal3 );
		}	
	}

	return true;
}

string loadShaderSource ( string filePath )
{	
	//Create file stream and check if there is an error
	ifstream source( filePath.c_str(), ios::in );
	if( !source.good() ){
		cout << "File open failed: " << filePath << endl;
		return "";
	}

	//Stream the file date into stringstream buffer	
	stringstream buffer;
	buffer << source.rdbuf();
	source.close();

	//Return the pointer of the string
	return buffer.str();
}
