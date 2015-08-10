#ifndef _K15_Math_Rectangle_h_
#define _K15_Math_Rectangle_h_

#include "K15_MathPrerequisites.h"
#include "K15_Vector2.h"

struct K15_Rectangle
{
	union 
	{
		K15_Vector2 v[2];

		struct  
		{
			real32 maxPosX;
			real32 maxPosY;

			real32 minPosX;
			real32 minPosY;
		};
	};
};

K15_Rectangle K15_CreateRectangle(real32 p_MaxPosX, real32 p_MaxPosY, real32 p_MinPosX, real32 p_MinPosY);
K15_Rectangle K15_CreateRectangle(K15_Vector2 p_MaxPos, K15_Vector2 p_MinPos);

K15_Vector2 K15_GetUpperLeftCorner(K15_Rectangle& p_Rectangle);
K15_Vector2 K15_GetUpperRightCorner(K15_Rectangle& p_Rectangle);
K15_Vector2 K15_GetBottomLeftCorner(K15_Rectangle& p_Rectangle);
K15_Vector2 K15_GetBottomRightCorner(K15_Rectangle& p_Rectangle);

real32 K15_CalculateRectangleArea(K15_Rectangle& p_Rectangle);

#endif //_K15_Math_Rectangle_h_