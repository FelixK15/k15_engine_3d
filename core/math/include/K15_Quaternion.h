#ifndef _K15_Math_Quaternion_h_
#define _K15_Math_Quaternion_h_

#include "K15_MathPrerequisites.h"

struct K15_Quaternion
{
	union
	{
		struct  
		{
			real32 w;
			real32 x;
			real32 y;
			real32 z;
		};
		real32 v[4]; //w,x,y,z
	};
};

K15_Quaternion operator*(K15_Quaternion& p_Quaternion1, K15_Quaternion& p_Quaternion2);

#endif //_K15_Math_Quaternion_h_