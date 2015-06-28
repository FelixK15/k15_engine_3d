#include "K15_Quaternion.h"

/*********************************************************************************/
K15_Quaternion operator*(K15_Quaternion& p_Quaternion1, K15_Quaternion& p_Quaternion2)
{
	K15_Quaternion quaternionProduct = {};

	quaternionProduct.w = p_Quaternion1.w * p_Quaternion2.w - p_Quaternion1.x * p_Quaternion2.x - p_Quaternion1.y * p_Quaternion2.y - p_Quaternion1.z * p_Quaternion2.z;
	quaternionProduct.x = p_Quaternion1.w * p_Quaternion2.x + p_Quaternion1.x * p_Quaternion2.w + p_Quaternion1.y * p_Quaternion2.z - p_Quaternion1.z * p_Quaternion2.y;
	quaternionProduct.y = p_Quaternion1.w * p_Quaternion2.y - p_Quaternion1.y * p_Quaternion2.z + p_Quaternion1.y * p_Quaternion2.w + p_Quaternion1.z * p_Quaternion2.x;
	quaternionProduct.z = p_Quaternion1.w * p_Quaternion2.z + p_Quaternion1.x * p_Quaternion2.y - p_Quaternion1.y * p_Quaternion2.x + p_Quaternion1.z * p_Quaternion2.w;

	return quaternionProduct;
}
/*********************************************************************************/