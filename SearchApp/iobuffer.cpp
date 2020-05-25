// iobuffer.cc

#include "pch.h"
#include "iobuffer.h"
#include <cstring>

#define IS_VALID_HEADER 0

IOBuffer::IOBuffer(int maxBytes)
// construct with a maximum of maxFields
{
	Init(maxBytes);
}

IOBuffer& IOBuffer :: operator = (const IOBuffer& buffer)
{
	if (MaxBytes < buffer.BufferSize) return *this; // fails
	Initialized = buffer.Initialized;
	BufferSize = buffer.BufferSize;
	memcpy(Buffer, buffer.Buffer, buffer.BufferSize);
	NextByte = buffer.NextByte;
	Packing = Packing;
	return *this;
}

void IOBuffer::Clear()
// clear field values from buffer
{
	NextByte = 0;
	Packing = TRUE;
}

void IOBuffer::Print(ostream& stream) const
{
	stream << "MaxBytes " << MaxBytes << " BufferSize " << BufferSize;
}

int IOBuffer::Init(int maxBytes)
{
	Initialized = FALSE;
	if (maxBytes < 0) maxBytes = 0;
	MaxBytes = maxBytes;
	Buffer = new char[MaxBytes];
	BufferSize = 0;
	Clear();
	return 1;
}
int IOBuffer::DRead(istream& stream, int recref)
// read specified record
{
	stream.seekg(recref, ios::beg);
	int pos = static_cast<int>(stream.tellg());
	if (pos != recref) return -1;
	return Read(stream);
}

int IOBuffer::DWrite(ostream& stream, int recref) const
// write specified record
{
	stream.seekp(recref, ios::beg);
	int pos = static_cast<int>(stream.tellp());
	if (pos != recref) return -1;
	return Write(stream);
}

static const char* headerStr = "IOBuffer";
static const int headerSize = strlen(headerStr);

int IOBuffer::ReadHeader(istream& stream)
{
	char* str = new char[headerSize + 1];
	stream.seekg(0, ios::beg);
	stream.read(str, headerSize);
	if (!stream.good()) return -1;
	int isHeader = strncmp(str, headerStr, headerSize);
	delete[] str;
	if (isHeader == IS_VALID_HEADER) return headerSize;
	else return -1;
}

int IOBuffer::WriteHeader(ostream& stream) const
{
	stream.seekp(0, ios::beg);
	stream.write(headerStr, headerSize);
	if (!stream.good()) return -1;
	return headerSize;
}

