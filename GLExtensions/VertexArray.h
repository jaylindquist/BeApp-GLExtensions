#pragma once

#include <glee/glee.h>

class VertexArray
{
public:
	typedef enum 
	{
		POINTS = GL_POINTS,
		LINE_STRIP,
		LINE_LOOP,
		LINES,
		TRIANGLE_STRIP,
		TRIANGLE_FAN,
		TRIANGLES,
		QUAD_STRIP,
		QUADS,
		POLYGON
	} Mode;


	VertexArray(void);
	~VertexArray(void);

	//void ArrayElementEXT(int i);
	void ArrayElement(int i);

	//void DrawArraysEXT(enum mode, int first, sizei count);
	void DrawArrays(

	void VertexPointerEXT(int size, enum type, sizei stride, sizei count, const void* pointer);

	void NormalPointerEXT(enum type, sizei stride, sizei count, const void* pointer);

	void ColorPointerEXT(int size, enum type, sizei stride, sizei count, const void* pointer);

	void IndexPointerEXT(enum type, sizei stride, sizei count, const void* pointer);

	void TexCoordPointerEXT(int size, enum type, sizei stride, sizei count, const void* pointer);

	void EdgeFlagPointerEXT(sizei stride, sizei count, const Boolean* pointer);

	void GetPointervEXT(enum pname, void** params);
};
