#include "VertexBufferObject.h"

// void GenBuffers(sizei n, uint *buffers);
VertexBufferObject::VertexBufferObject(void)
{
	glGenBuffers(1, &m_buffer);
}

// void DeleteBuffers(sizei n, const uint *buffers);
VertexBufferObject::~VertexBufferObject(void)
{
	glDeleteBuffers(1, &m_buffer);
}

//void BindBuffer(enum target, uint buffer);
void 
VertexBufferObject::Bind(VertexBufferObject::Target target)
{
	glBindBuffer(target, m_buffer);
}

void 
VertexBufferObject::Unbind(VertexBufferObject::Target target)
{
	glBindBuffer(target, 0);
}

//boolean IsBuffer(uint buffer);
bool 
VertexBufferObject::IsBuffer()
{
	return glIsBuffer(m_buffer) == GL_TRUE? true:false;
}

//void BufferData(enum target, sizeiptr size, const void *data, enum usage);
void 
VertexBufferObject::SetData(VertexBufferObject::Target target, GLsizeiptr size, const void* data, VertexBufferObject::Usage usage)
{
	glBufferData(target, size, data, usage);
}

// void BufferSubData(enum target, intptr offset, sizeiptr size, const void *data);
void 
VertexBufferObject::SetSubData(VertexBufferObject::Target target, GLintptr offset, GLsizeiptr size, const void* data)
{
	glBufferSubData(target, offset, size, data);
}

//void GetBufferSubData(enum target, intptr offset, sizeiptr size, void *data);
void
VertexBufferObject::GetSubData(VertexBufferObject::Target target, GLintptr offset, GLsizeiptr size, void* data)
{
	glGetBufferSubData(target, offset, size, data);
}

//void *MapBuffer(enum target, enum access);
void* 
VertexBufferObject::Map(VertexBufferObject::Target target, VertexBufferObject::Access access)
{
	return glMapBuffer(target, access);
}

//boolean UnmapBuffer(enum target);
bool 
VertexBufferObject::Unmap(VertexBufferObject::Target target)
{
	return glUnmapBuffer(target) == GL_TRUE? true:false;
}