#ifndef __LIGHTOBJECT_H__
#define __LIGHTOBJECT_H__

#include "AIGlibMacros.h"

namespace AIGlib
{
	const int lightNVertex = 24;
	const int lightNTriangleIndex = 12;
	const unsigned int lightTriangleIndex[] = 
	{
		0,1,2,			1,3,2,
		4,6,5,			5,6,7,
		8,10,9,			9,10,11,
		12,13,14,		13,15,14,
		16,17,18,		17,19,18,
		20,22,21,		21,22,23
	};
	const float lightVertexPos[] = 
	{
		-1.0f,	-1.0f,	 1.0f, //0
		1.0f,	-1.0f,	 1.0f, //1
		-1.0f,	 1.0f,	 1.0f, //2
		1.0f,	 1.0f,	 1.0f, //3	   
		-1.0f,	-1.0f,	-1.0f, //4
		1.0f,	-1.0f,	-1.0f, //5
		-1.0f,	 1.0f,	-1.0f, //6
		1.0f,	 1.0f,	-1.0f, //7		   
		1.0f,	-1.0f,	-1.0f, //8
		1.0f,	-1.0f,	 1.0f, //9
		1.0f,	 1.0f,	-1.0f, //10
		1.0f,	 1.0f,	 1.0f, //11	   
		-1.0f,	-1.0f,	-1.0f, //12
		-1.0f,	-1.0f,	 1.0f, //13
		-1.0f,	 1.0f,	-1.0f, //14
		-1.0f,	 1.0f,	 1.0f, //15		   
		-1.0f,	 1.0f,	-1.0f, //16
		-1.0f,	 1.0f,	 1.0f, //17
		1.0f,	 1.0f,	-1.0f, //18
		1.0f,	 1.0f,	 1.0f, //19	   
		-1.0f,	-1.0f,	-1.0f, //20
		-1.0f,	-1.0f,	 1.0f, //21
		1.0f,	-1.0f,	-1.0f, //22
		1.0f,	-1.0f,	 1.0f  //23
	};

	const float lightVertexTexCoord[] = 
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f,	1.0f,
		1.0f,	1.0f,
		0.0f,	0.0f,
		1.0f,	0.0f,
		0.0f,	0.0f,
		1.0f,	0.0f,
		0.0f,	1.0f,
		1.0f,	1.0f,	
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};
}
const float lightVertexNormal[] =
{
	0.0f,	0.0f,	 1.0f,
	0.0f,	0.0f,	 1.0f,
	0.0f,	0.0f,	 1.0f,
	0.0f,	0.0f,	 1.0f,	   
	0.0f,	0.0f,	-1.0f,
	0.0f,	0.0f,	-1.0f,
	0.0f,	0.0f,	-1.0f,
	0.0f,	0.0f,	-1.0f,	   
	1.0f,	0.0f,	 0.0f,
	1.0f,	0.0f,	 0.0f,
	1.0f,	0.0f,	 0.0f,
	1.0f,	0.0f,	 0.0f,		   
	-1.0f,	0.0f,	 0.0f,
	-1.0f,	0.0f,	 0.0f,
	-1.0f,	0.0f,	 0.0f,
	-1.0f,	0.0f,	 0.0f,	   
	0.0f,	1.0f,	0.0f,
	0.0f,	1.0f,	0.0f,
	0.0f,	1.0f,	0.0f,
	0.0f,	1.0f,	0.0f,	   
	0.0f,	-1.0f,	0.0f,
	0.0f,	-1.0f,	0.0f,
	0.0f,	-1.0f,	0.0f,
	0.0f,	-1.0f,	0.0f
};

const float lightVertexColor[] = 
{
	0.0f,	0.0f,	 1.0f,	1.0f,
	0.0f,	0.0f,	 1.0f,	1.0f,
	0.0f,	0.0f,	 1.0f,	1.0f,
	0.0f,	0.0f,	 1.0f,	1.0f,	   
	0.6f,	0.6f,	 1.0f,	1.0f,
	0.6f,	0.6f,	 1.0f,	1.0f,
	0.6f,	0.6f,	 1.0f,	1.0f,
	0.6f,	0.6f,	 1.0f,	1.0f,	   
	1.0f,	0.0f,	 0.0f,	1.0f,
	1.0f,	0.0f,	 0.0f,	1.0f,
	1.0f,	0.0f,	 0.0f,	1.0f,
	1.0f,	0.0f,	 0.0f,	1.0f,		   
	1.0f,	0.6f,	 0.6f,	1.0f,
	1.0f,	0.6f,	 0.6f,	1.0f,
	1.0f,	0.6f,	 0.6f,	1.0f,
	1.0f,	0.6f,	 0.6f,	1.0f,	   
	0.0f,	1.0f,	0.0f,	1.0f,
	0.0f,	1.0f,	0.0f,	1.0f,
	0.0f,	1.0f,	0.0f,	1.0f,
	0.0f,	1.0f,	0.0f,	1.0f,		   
	0.6f,	1.0f,	0.6f,	1.0f,
	0.6f,	1.0f,	0.6f,	1.0f,
	0.6f,	1.0f,	0.6f,	1.0f,
	0.6f,	1.0f,	0.6f,	1.0f
};

const float lightVertexTangent[] = 
{
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,				
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,				
	0.0f,	0.0f,	-1.0f,
	0.0f,	0.0f,	-1.0f,
	0.0f,	0.0f,	-1.0f,
	0.0f,	0.0f,	-1.0f,			
	0.0f,	0.0f,	1.0f,
	0.0f,	0.0f,	1.0f,
	0.0f,	0.0f,	1.0f,
	0.0f,	0.0f,	1.0f,   
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,					   
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f
};

#endif