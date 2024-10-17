

typedef unsigned short Port;

unsigned char INP(Port port);
void OUTP(Port port, unsigned char data);
void memcopy(char *sorce, char *dest, int nbytes);
