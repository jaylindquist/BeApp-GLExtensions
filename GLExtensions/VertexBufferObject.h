#pragma once

#include <glee/glee.h>

class VertexBufferObject
{
public:
	enum Target 
	{
		ARRAY_BUFFER = GL_ARRAY_BUFFER,
		ELEMENT_ARRAY_BUFFER = GL_ELEMENT_ARRAY_BUFFER
	};

	enum Usage
	{
		STREAM_DRAW = GL_STREAM_DRAW,
		STREAM_READ = GL_STREAM_READ,
		STREAM_COPY = GL_STREAM_COPY,
		STATIC_DRAW = GL_STATIC_DRAW,
		STATIC_READ = GL_STATIC_READ,
		STATIC_COPY = GL_STATIC_COPY,
		DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
		DYNAMIC_READ = GL_DYNAMIC_READ,
		DYNAMIC_COPY = GL_DYNAMIC_COPY
	};

	enum Access
	{
		READ_ONLY = GL_READ_ONLY,
		WRITE_ONLY = GL_WRITE_ONLY,
		READ_WRITE = GL_READ_WRITE
	};

	// void GenBuffers(sizei n, uint *buffers);
	VertexBufferObject(void);
	
	// void DeleteBuffers(sizei n, const uint *buffers);
	~VertexBufferObject(void);

	//void BindBuffer(enum target, uint buffer);
	void Bind(VertexBufferObject::Target target);
	void Unbind(VertexBufferObject::Target target);

	//boolean IsBuffer(uint buffer);
	bool IsBuffer();

	//void BufferData(enum target, sizeiptr size, const void *data, enum usage);
	void SetData(VertexBufferObject::Target target, GLsizeiptr size, const void* data, VertexBufferObject::Usage usage);

	// void BufferSubData(enum target, intptr offset, sizeiptr size, const void *data);
	void SetSubData(VertexBufferObject::Target target, GLintptr offset, GLsizeiptr size, const void* data);

	//void GetBufferSubData(enum target, intptr offset, sizeiptr size, void *data);
	void GetSubData(VertexBufferObject::Target target, GLintptr offset, GLsizeiptr size, void* data);

	//void *MapBuffer(enum target, enum access);
	void* Map(VertexBufferObject::Target target, VertexBufferObject::Access access);

	//boolean UnmapBuffer(enum target);
	bool Unmap(VertexBufferObject::Target target);

	GLuint GetId() { return m_buffer; }

	//void GetBufferParameteriv(enum target, enum pname, int *params);
	//void GetBufferPointerv(enum target, enum pname, void **params);

private:
	GLuint m_buffer;
};
