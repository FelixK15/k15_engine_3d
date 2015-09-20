#include "K15_Rectangle.h"

/*********************************************************************************/
K15_Rectangle K15_CreateRectangle(real32 p_MaxPosX, real32 p_MaxPosY, real32 p_MinPosX, real32 p_MinPosY)
{
	return K15_CreateRectangle(K15_CreateVector(p_MaxPosX, p_MaxPosY), K15_CreateVector(p_MinPosX, p_MinPosY));
}
/*********************************************************************************/
K15_Rectangle K15_CreateRectangle(K15_Vector2 p_MaxPos, K15_Vector2 p_MinPos)
{
	K15_Rectangle rectangle = {};

	rectangle.v[0] = p_MaxPos;
	rectangle.v[1] = p_MinPos;

	return rectangle;
}
/*********************************************************************************/
K15_Vector2 K15_GetUpperLeftCorner(K15_Rectangle& p_Rectangle)
{
	real32 x = K15_MIN(p_Rectangle.maxPosX, p_Rectangle.minPosX);
	real32 y = K15_MIN(p_Rectangle.maxPosY, p_Rectangle.minPosY);

	return K15_CreateVector(x, y);
}
/*********************************************************************************/
K15_Vector2 K15_GetUpperRightCorner(K15_Rectangle& p_Rectangle)
{
	real32 x = K15_MAX(p_Rectangle.maxPosX, p_Rectangle.minPosX);
	real32 y = K15_MIN(p_Rectangle.maxPosY, p_Rectangle.minPosY);

	return K15_CreateVector(x, y);
}
/*********************************************************************************/
K15_Vector2 K15_GetBottomLeftCorner(K15_Rectangle& p_Rectangle)
{
	real32 x = K15_MIN(p_Rectangle.maxPosX, p_Rectangle.minPosX);
	real32 y = K15_MAX(p_Rectangle.maxPosY, p_Rectangle.minPosY);

	return K15_CreateVector(x, y);
}
/*********************************************************************************/
K15_Vector2 K15_GetBottomRightCorner(K15_Rectangle& p_Rectangle)
{
	real32 x = K15_MAX(p_Rectangle.maxPosX, p_Rectangle.minPosX);
	real32 y = K15_MAX(p_Rectangle.maxPosY, p_Rectangle.minPosY);

	return K15_CreateVector(x, y);
}
/*********************************************************************************/
real32 K15_CalculateRectangleArea(K15_Rectangle& p_Rectangle)
{
	K15_Vector2 ur = K15_GetUpperRightCorner(p_Rectangle);
	K15_Vector2 ul = K15_GetUpperLeftCorner(p_Rectangle);
	K15_Vector2 bl = K15_GetBottomLeftCorner(p_Rectangle);

	real32 a = ur.x - ul.x;
	real32 b = bl.y - ul.y;

	return a * b;
}
/*********************************************************************************/