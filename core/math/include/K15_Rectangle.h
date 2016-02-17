#ifndef _K15_Math_Rectangle_h_
#define _K15_Math_Rectangle_h_

#include "K15_MathPrerequisites.h"
#include "K15_Vector2.h"

struct K15_Rectangle
{
	union 
	{
		struct 
		{
			K15_Vector2 leftTop;
			K15_Vector2 rightBottom;
		};

		struct  
		{
			real32 leftPos;
			real32 topPos;

			real32 rightPos;
			real32 bottomPos;
		};
	};
};

K15_Rectangle K15_CreateRectangle(real32 p_LeftPos, real32 p_TopPos, real32 p_RightPos, real32 p_BottomPos);
K15_Rectangle K15_CreateRectangle(K15_Vector2 p_LeftTop, K15_Vector2 p_RightBottom);

#endif //_K15_Math_Rectangle_h_