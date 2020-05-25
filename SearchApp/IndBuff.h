//indbuff.h
#ifndef INDBUFF_H
#define INDBUFF_H

#include "simpind.h"
#include "fixfld.h"

using namespace std;

template <class keyType>
class IndexBuffer : public FixedFieldBuffer
{
public:
	IndexBuffer(int maxKeys = 100, int keySize = sizeof(keyType));
	int Pack(const SimpleIndex<keyType>&);
	int Unpack(SimpleIndex<keyType>&);
	void Print(ostream&) const;
protected:
	int MaxKeys;
	int KeySize;
};


#endif
