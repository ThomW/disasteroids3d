#include <math.h>

double dblSinTable[360];
double dblCosTable[360];

const double ltconvertdegtorad	= 0.0174532925199432957692369076848861;

void InitLookupTables(void)
{
	int i = 0;
	double dblRadAngle = 0;

	for (i = 0; i < 360; i++)
	{
		dblRadAngle = ltconvertdegtorad * i;
		dblSinTable[i] = sin(dblRadAngle);
		dblCosTable[i] = cos(dblRadAngle);
	}
}

inline double CosLookup(const int angle) {
	return dblCosTable[abs(angle) % 360];
}

inline double CosLookup(const float angle) {
	return CosLookup(int(angle));
}

inline double SinLookup(const int angle)
{
	if (angle < 0)
		return -dblSinTable[abs(angle) % 360];
	else
		return dblSinTable[abs(angle) % 360];
}

inline double SinLookup(const float angle) {
	return SinLookup(int(angle));
}

