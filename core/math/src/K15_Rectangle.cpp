#include "K15_Rectangle.h"

/*********************************************************************************/
K15_Rectangle K15_CreateRectangle(real32 p_LeftPos, real32 p_TopPos, real32 p_RightPos, real32 p_BottomPos)
{
	return K15_CreateRectangle(K15_CreateVector(p_LeftPos, p_TopPos), K15_CreateVector(p_RightPos, p_BottomPos));
}
/*********************************************************************************/
K15_Rectangle K15_CreateRectangle(K15_Vector2 p_LeftTop, K15_Vector2 p_RightBottom)
{
	K15_Rectangle rectangle = {};

	rectangle.leftTop = p_LeftTop;
	rectangle.rightBottom = p_RightBottom;

	return rectangle;
}
/*********************************************************************************/