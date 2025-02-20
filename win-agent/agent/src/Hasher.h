#pragma once
#include "stdafx.h"
#include "SampleMessage.h"
class Hasher
{
public:
	Hasher(void);
	~Hasher(void);

	void fillHash(SampleMessage& sample);
private:
	bool getHash(const TString& path, TString& out);
	bool simpleHash(void* input, unsigned long length, unsigned char* md);
};

