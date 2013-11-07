/*

functions.cpp

author: TheKK <thumbd03803@gmail.com>

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

bool loadOBJ ( string filePath, vector<GLfloat> &vertex, vector<GLuint> &element, vector<GLfloat> &normal )
{	
	//Create a file stream and check if there is an error
	ifstream OBJsoruce( filePath.c_str(), ios::in );
	if( !OBJsoruce.good() ){
		cout << "OBJ file load failed: " << filePath << endl;
		return false;
	} 

	//Load line by line and search keyword to identify the data type( vertex or normal or ETC )
	string strTmp;
	vector <GLfloat> normalTable;
	vector <GLuint> normalIndice;
	float vx, vy, vz;
	float nx, ny, nz;
	int vertex1, vertex2, vertex3;
	int normalIndex1, normalIndex2, normalIndex3;
	while( getline( OBJsoruce, strTmp ) ){
		if( sscanf( strTmp.c_str(), "v %f %f %f\n", &vx, &vy, &vz ) > 0 ){
			vertex.push_back( vx );
			vertex.push_back( vy );
			vertex.push_back( vz );
		}
	
		else if( sscanf( strTmp.c_str(), "vn %f %f %f", &nx, &ny, &nz ) > 0 ){
			normalTable.push_back( nx );
			normalTable.push_back( ny );
			normalTable.push_back( nz );
		}
		
		else if( sscanf( strTmp.c_str(), "%*s %d//%d %d//%d %d//%d", &vertex1, &normalIndex1, &vertex2, &normalIndex2, &vertex3, &normalIndex3 ) > 0 ){
			element.push_back( vertex1 - 1 );			
			element.push_back( vertex2 - 1 );			
			element.push_back( vertex3 - 1 );			

			normalIndice.push_back( normalIndex1 - 1 );
			normalIndice.push_back( normalIndex2 - 1 );
			normalIndice.push_back( normalIndex3 - 1 );	
		}
	}
	//Match normal to each vertex
	normal.resize( vertex.size(), 0 );
	for( int i = 0; i < element.size(); i++ ){
		normal[ element[i] * 3 + 0 ] += normalTable[ normalIndice[i] * 3 + 0 ];	
		normal[ element[i] * 3 + 1 ] += normalTable[ normalIndice[i] * 3 + 1 ];	
		normal[ element[i] * 3 + 2 ] += normalTable[ normalIndice[i] * 3 + 2 ];	
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
