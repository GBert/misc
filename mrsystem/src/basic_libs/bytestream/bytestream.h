#ifndef BYTESTREAM_H
#define BYTESTREAM_H

unsigned long GetLongFromByteArray(char *Data);
unsigned int GetIntFromByteArray(char *Data);
void SetLongToByteArray(char *Data, unsigned long Value);
void SetIntToByteArray(char *Data, unsigned int Value);

#endif
