
#include "includes.h"
#define PI 3.1415927

// Fast arctan2, fixed point.
// In : dx et dy avec 8 bits de virgule fixe. Out : Angle entre les deux points [-127; 128].
s32 fatan2(s32 y, s32 x)
{
	if (y == 0 && x == 0) return (0);

	s32 angle, r;
	s32 coeff_1 = (PI * 256) / 4;
	s32 coeff_2 = 3 * coeff_1;
	//float abs_y = fabs(y)+1e-10;      // kludge to prevent 0/0 condition
	//x *= 256;
	//y *= 256;
	s32 abs_y = abs(y);//+1e-10;      // kludge to prevent 0/0 condition
	if (x >= 0)
	{
		r = ((x - abs_y) * 256) / (x + abs_y);
		angle = coeff_1 - ((coeff_1 * r) / 256);
	}
	else
	{
		r = ((x + abs_y) * 256) / (abs_y - x);
		angle = coeff_2 - ((coeff_1 * r) / 256);
	}
	if (y < 0)
		//return (-angle);     // negate if in quad III or IV
		return ((-angle * 128 / PI) / 256);     // negate if in quad III or IV
	else
		return ((angle * 128 / PI) / 256);
}

/*
// Précalcul des tables de sinus-cosinus.
// 256 angles, val *256 (=> varie de -256 à 256).
void PrecaSinCos(void)
{
	u32	i;

	for (i = 0; i < 256 + 64; i++)
	{
		gVar.pSinCos[i] = (s16) (cos(i * 2 * PI / 256) * 256);
		//printf("i = %d : %f\n", i, cos(i * 2 * PI / 256) * 256);
		//printf("i = %d : %d\n", i, gVar.pSinCos[i]);
	}
	gVar.pSin = gVar.pSinCos + 64;
	gVar.pCos = gVar.pSinCos;

	// Correction, seul pCos[0] est égal à 256. Tout le reste est symétrique. => Tout entre -255 et 255.
//	gVar.pCos[0] = 255;

	// Correction, on remet les angles droits à la valeur 256. => Tout entre -256 et 256.
	gVar.pSinCos[0] = 256;
	gVar.pSinCos[128] = -256;
	gVar.pSinCos[256] = 256;


	/ *
	for (i = 0; i < 256; i++)
	{
		printf("i = %d : sin = %d - cos = %d\n", i, gVar.pSin[i], gVar.pCos[i]);
	}
	* /
	/ *
	for (i = 0; i < 256+64; i++)
	{
		if ((i & 63) == 0) printf("*** i=%d\n", i);
		printf("sincos = %d\n", gVar.pSinCos[i]);
	}
	* /

}
*/

s16 gpCosSin[256+64] = {
256, 255, 255, 255, 254, 254, 253, 252, 251, 249, 248, 246, 244, 243, 241, 238,
236, 234, 231, 228, 225, 222, 219, 216, 212, 209, 205, 201, 197, 193, 189, 185,
181, 176, 171, 167, 162, 157, 152, 147, 142, 136, 131, 126, 120, 115, 109, 103,
97, 92, 86, 80, 74, 68, 62, 56, 49, 43, 37, 31, 25, 18, 12, 6,
0, -6, -12, -18, -25, -31, -37, -43, -49, -56, -62, -68, -74, -80, -86, -92,
-97, -103, -109, -115, -120, -126, -131, -136, -142, -147, -152, -157, -162, -167, -171, -176,
-181, -185, -189, -193, -197, -201, -205, -209, -212, -216, -219, -222, -225, -228, -231, -234,
-236, -238, -241, -243, -244, -246, -248, -249, -251, -252, -253, -254, -254, -255, -255, -255,
-256, -255, -255, -255, -254, -254, -253, -252, -251, -249, -248, -246, -244, -243, -241, -238,
-236, -234, -231, -228, -225, -222, -219, -216, -212, -209, -205, -201, -197, -193, -189, -185,
-181, -176, -171, -167, -162, -157, -152, -147, -142, -136, -131, -126, -120, -115, -109, -103,
-97, -92, -86, -80, -74, -68, -62, -56, -49, -43, -37, -31, -25, -18, -12, -6,
0, 6, 12, 18, 25, 31, 37, 43, 49, 56, 62, 68, 74, 80, 86, 92,
97, 103, 109, 115, 120, 126, 131, 136,142, 147, 152, 157, 162, 167, 171, 176,
181, 185, 189, 193, 197, 201, 205, 209, 212, 216, 219, 222, 225, 228, 231, 234,
236, 238, 241, 243, 244, 246, 248, 249, 251, 252, 253, 254, 254, 255, 255, 255,
256, 255, 255, 255, 254, 254, 253, 252, 251, 249, 248, 246, 244, 243, 241, 238,
236, 234, 231, 228, 225, 222, 219, 216, 212, 209, 205, 201, 197, 193, 189, 185,
181, 176, 171, 167, 162, 157, 152, 147, 142, 136, 131, 126, 120, 115, 109, 103,
97, 92, 86, 80, 74, 68, 62, 56, 49, 43, 37, 31, 25, 18, 12, 6 };

void PrecaSinCos(void)
{
	gVar.pCos = gpCosSin;
	gVar.pSin = gpCosSin + 64;
}


