#define TO_INT(x,format) \
{ \
	int iOffset = 0; \
	if(x < 0){ \
		iOffset = 1; \
	} \
	unsigned int iSizeBuffer = Mathematic::GetDigits(x); \
	iSizeBuffer += iOffset; \
	char *bBuffer = new char[iSizeBuffer]; \
	if(iOffset > 0){ \
		bBuffer[0] = '-'; \
		iSizeBuffer -= iOffset; \
	} \
	sprintf_s(bBuffer + iOffset,iSizeBuffer,#format,x); \
	_Append(bBuffer,iSizeBuffer); \
	delete[] bBuffer; \
}