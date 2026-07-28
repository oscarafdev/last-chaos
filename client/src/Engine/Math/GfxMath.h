#ifndef SE_INCL_GFXMATH_H
#define SE_INCL_GFXMATH_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#pragma push_macro("O")
#pragma push_macro("Q")
#pragma push_macro("D")
#pragma push_macro("W")
#pragma push_macro("B")
#undef O
#undef Q
#undef D
#undef W
#undef B
#include <DirectXMath.h>
#pragma pop_macro("B")
#pragma pop_macro("W")
#pragma pop_macro("D")
#pragma pop_macro("Q")
#pragma pop_macro("O")
#include <d3d9types.h>

// Math PODs used at the renderer boundary. They deliberately keep the memory
// layout expected by the legacy state API while all operations are implemented
// with the header-only DirectXMath library.
struct GfxVector2
{
	FLOAT x;
	FLOAT y;

	GfxVector2() {}
	GfxVector2(FLOAT xValue, FLOAT yValue)
		: x(xValue), y(yValue) {}

	FLOAT& operator()(UINT index) { return (&x)[index]; }
	const FLOAT& operator()(UINT index) const { return (&x)[index]; }
};

struct GfxVector3
{
	FLOAT x;
	FLOAT y;
	FLOAT z;

	GfxVector3() {}
	GfxVector3(FLOAT xValue, FLOAT yValue, FLOAT zValue)
		: x(xValue), y(yValue), z(zValue) {}

	GfxVector3& operator+=(const GfxVector3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	FLOAT& operator()(UINT index) { return (&x)[index]; }
	const FLOAT& operator()(UINT index) const { return (&x)[index]; }
};

inline GfxVector3 operator+(
	const GfxVector3& left,
	const GfxVector3& right)
{
	return GfxVector3(
		left.x + right.x,
		left.y + right.y,
		left.z + right.z);
}

inline GfxVector3 operator*(
	const GfxVector3& vector,
	FLOAT scale)
{
	return GfxVector3(
		vector.x * scale,
		vector.y * scale,
		vector.z * scale);
}

struct GfxVector4
{
	FLOAT x;
	FLOAT y;
	FLOAT z;
	FLOAT w;

	GfxVector4() {}
	GfxVector4(
		FLOAT xValue,
		FLOAT yValue,
		FLOAT zValue,
		FLOAT wValue)
		: x(xValue), y(yValue), z(zValue), w(wValue) {}

	FLOAT& operator()(UINT index) { return (&x)[index]; }
	const FLOAT& operator()(UINT index) const { return (&x)[index]; }
};

struct GfxPlane
{
	FLOAT a;
	FLOAT b;
	FLOAT c;
	FLOAT d;

	GfxPlane() {}
	GfxPlane(FLOAT aValue, FLOAT bValue, FLOAT cValue, FLOAT dValue)
		: a(aValue), b(bValue), c(cValue), d(dValue) {}

	FLOAT& operator[](UINT index) { return (&a)[index]; }
	const FLOAT& operator[](UINT index) const { return (&a)[index]; }
};

struct GfxMatrix : public D3DMATRIX
{
	GfxMatrix() {}
	explicit GfxMatrix(const D3DMATRIX& matrix)
	{
		*static_cast<D3DMATRIX*>(this) = matrix;
	}

	FLOAT& operator()(UINT row, UINT column)
	{
		return m[row][column];
	}

	const FLOAT& operator()(UINT row, UINT column) const
	{
		return m[row][column];
	}
};

namespace GfxMathDetail
{
	inline DirectX::XMMATRIX LoadMatrix(const GfxMatrix* pMatrix)
	{
		return DirectX::XMLoadFloat4x4(
			reinterpret_cast<const DirectX::XMFLOAT4X4*>(pMatrix));
	}

	inline void StoreMatrix(
		GfxMatrix* pDestination,
		DirectX::FXMMATRIX matrix)
	{
		DirectX::XMStoreFloat4x4(
			reinterpret_cast<DirectX::XMFLOAT4X4*>(pDestination),
			matrix);
	}

	inline DirectX::XMVECTOR LoadVector3(
		const GfxVector3* pVector,
		FLOAT w)
	{
		return DirectX::XMVectorSet(
			pVector->x,
			pVector->y,
			pVector->z,
			w);
	}

	inline DirectX::XMVECTOR LoadVector4(const GfxVector4* pVector)
	{
		return DirectX::XMLoadFloat4(
			reinterpret_cast<const DirectX::XMFLOAT4*>(pVector));
	}

	inline DirectX::XMVECTOR LoadPlane(const GfxPlane* pPlane)
	{
		return DirectX::XMLoadFloat4(
			reinterpret_cast<const DirectX::XMFLOAT4*>(pPlane));
	}
}

inline GfxMatrix* GfxMatrixIdentity(GfxMatrix* pDestination)
{
	if (pDestination != NULL)
		GfxMathDetail::StoreMatrix(
			pDestination,
			DirectX::XMMatrixIdentity());
	return pDestination;
}

inline GfxMatrix* GfxMatrixMultiply(
	GfxMatrix* pDestination,
	const GfxMatrix* pLeft,
	const GfxMatrix* pRight)
{
	if (pDestination == NULL || pLeft == NULL || pRight == NULL)
		return NULL;
	const DirectX::XMMATRIX result = DirectX::XMMatrixMultiply(
		GfxMathDetail::LoadMatrix(pLeft),
		GfxMathDetail::LoadMatrix(pRight));
	GfxMathDetail::StoreMatrix(pDestination, result);
	return pDestination;
}

inline GfxMatrix operator*(
	const GfxMatrix& left,
	const GfxMatrix& right)
{
	GfxMatrix result;
	GfxMatrixMultiply(&result, &left, &right);
	return result;
}

inline GfxMatrix* GfxMatrixTranspose(
	GfxMatrix* pDestination,
	const GfxMatrix* pSource)
{
	if (pDestination == NULL || pSource == NULL)
		return NULL;
	const DirectX::XMMATRIX source =
		GfxMathDetail::LoadMatrix(pSource);
	GfxMathDetail::StoreMatrix(
		pDestination,
		DirectX::XMMatrixTranspose(source));
	return pDestination;
}

inline GfxMatrix* GfxMatrixInverse(
	GfxMatrix* pDestination,
	FLOAT* pDeterminant,
	const GfxMatrix* pSource)
{
	if (pDestination == NULL || pSource == NULL)
		return NULL;
	DirectX::XMVECTOR determinant;
	const DirectX::XMMATRIX inverse = DirectX::XMMatrixInverse(
		&determinant,
		GfxMathDetail::LoadMatrix(pSource));
	if (pDeterminant != NULL)
		*pDeterminant = DirectX::XMVectorGetX(determinant);
	GfxMathDetail::StoreMatrix(pDestination, inverse);
	return pDestination;
}

inline GfxMatrix* GfxMatrixTranslation(
	GfxMatrix* pDestination,
	FLOAT x,
	FLOAT y,
	FLOAT z)
{
	if (pDestination != NULL)
		GfxMathDetail::StoreMatrix(
			pDestination,
			DirectX::XMMatrixTranslation(x, y, z));
	return pDestination;
}

inline GfxMatrix* GfxMatrixScaling(
	GfxMatrix* pDestination,
	FLOAT x,
	FLOAT y,
	FLOAT z)
{
	if (pDestination != NULL)
		GfxMathDetail::StoreMatrix(
			pDestination,
			DirectX::XMMatrixScaling(x, y, z));
	return pDestination;
}

inline GfxMatrix* GfxMatrixLookAtLH(
	GfxMatrix* pDestination,
	const GfxVector3* pEye,
	const GfxVector3* pTarget,
	const GfxVector3* pUp)
{
	if (pDestination == NULL || pEye == NULL
		|| pTarget == NULL || pUp == NULL)
		return NULL;
	GfxMathDetail::StoreMatrix(
		pDestination,
		DirectX::XMMatrixLookAtLH(
			GfxMathDetail::LoadVector3(pEye, 1.0f),
			GfxMathDetail::LoadVector3(pTarget, 1.0f),
			GfxMathDetail::LoadVector3(pUp, 0.0f)));
	return pDestination;
}

inline GfxMatrix* GfxMatrixOrthoLH(
	GfxMatrix* pDestination,
	FLOAT width,
	FLOAT height,
	FLOAT nearZ,
	FLOAT farZ)
{
	if (pDestination != NULL)
		GfxMathDetail::StoreMatrix(
			pDestination,
			DirectX::XMMatrixOrthographicLH(
				width,
				height,
				nearZ,
				farZ));
	return pDestination;
}

inline GfxMatrix* GfxMatrixOrthoRH(
	GfxMatrix* pDestination,
	FLOAT width,
	FLOAT height,
	FLOAT nearZ,
	FLOAT farZ)
{
	if (pDestination != NULL)
		GfxMathDetail::StoreMatrix(
			pDestination,
			DirectX::XMMatrixOrthographicRH(
				width,
				height,
				nearZ,
				farZ));
	return pDestination;
}

inline GfxMatrix* GfxMatrixOrthoOffCenterRH(
	GfxMatrix* pDestination,
	FLOAT left,
	FLOAT right,
	FLOAT bottom,
	FLOAT top,
	FLOAT nearZ,
	FLOAT farZ)
{
	if (pDestination != NULL)
		GfxMathDetail::StoreMatrix(
			pDestination,
			DirectX::XMMatrixOrthographicOffCenterRH(
				left,
				right,
				bottom,
				top,
				nearZ,
				farZ));
	return pDestination;
}

inline GfxVector4* GfxVec3Transform(
	GfxVector4* pDestination,
	const GfxVector3* pSource,
	const GfxMatrix* pMatrix)
{
	if (pDestination == NULL || pSource == NULL || pMatrix == NULL)
		return NULL;
	DirectX::XMStoreFloat4(
		reinterpret_cast<DirectX::XMFLOAT4*>(pDestination),
		DirectX::XMVector4Transform(
			GfxMathDetail::LoadVector3(pSource, 1.0f),
			GfxMathDetail::LoadMatrix(pMatrix)));
	return pDestination;
}

inline GfxVector4* GfxVec4Transform(
	GfxVector4* pDestination,
	const GfxVector4* pSource,
	const GfxMatrix* pMatrix)
{
	if (pDestination == NULL || pSource == NULL || pMatrix == NULL)
		return NULL;
	DirectX::XMStoreFloat4(
		reinterpret_cast<DirectX::XMFLOAT4*>(pDestination),
		DirectX::XMVector4Transform(
			GfxMathDetail::LoadVector4(pSource),
			GfxMathDetail::LoadMatrix(pMatrix)));
	return pDestination;
}

inline GfxVector3* GfxVec3TransformNormal(
	GfxVector3* pDestination,
	const GfxVector3* pSource,
	const GfxMatrix* pMatrix)
{
	if (pDestination == NULL || pSource == NULL || pMatrix == NULL)
		return NULL;
	DirectX::XMStoreFloat3(
		reinterpret_cast<DirectX::XMFLOAT3*>(pDestination),
		DirectX::XMVector3TransformNormal(
			GfxMathDetail::LoadVector3(pSource, 0.0f),
			GfxMathDetail::LoadMatrix(pMatrix)));
	return pDestination;
}

inline FLOAT GfxVec3Dot(
	const GfxVector3* pLeft,
	const GfxVector3* pRight)
{
	return DirectX::XMVectorGetX(
		DirectX::XMVector3Dot(
			GfxMathDetail::LoadVector3(pLeft, 0.0f),
			GfxMathDetail::LoadVector3(pRight, 0.0f)));
}

inline GfxVector3* GfxVec3Cross(
	GfxVector3* pDestination,
	const GfxVector3* pLeft,
	const GfxVector3* pRight)
{
	if (pDestination == NULL || pLeft == NULL || pRight == NULL)
		return NULL;
	DirectX::XMStoreFloat3(
		reinterpret_cast<DirectX::XMFLOAT3*>(pDestination),
		DirectX::XMVector3Cross(
			GfxMathDetail::LoadVector3(pLeft, 0.0f),
			GfxMathDetail::LoadVector3(pRight, 0.0f)));
	return pDestination;
}

inline GfxVector3* GfxVec3Normalize(
	GfxVector3* pDestination,
	const GfxVector3* pSource)
{
	if (pDestination == NULL || pSource == NULL)
		return NULL;
	DirectX::XMStoreFloat3(
		reinterpret_cast<DirectX::XMFLOAT3*>(pDestination),
		DirectX::XMVector3Normalize(
			GfxMathDetail::LoadVector3(pSource, 0.0f)));
	return pDestination;
}

inline GfxPlane* GfxPlaneFromPointNormal(
	GfxPlane* pDestination,
	const GfxVector3* pPoint,
	const GfxVector3* pNormal)
{
	if (pDestination == NULL || pPoint == NULL || pNormal == NULL)
		return NULL;
	DirectX::XMStoreFloat4(
		reinterpret_cast<DirectX::XMFLOAT4*>(pDestination),
		DirectX::XMPlaneFromPointNormal(
			GfxMathDetail::LoadVector3(pPoint, 1.0f),
			GfxMathDetail::LoadVector3(pNormal, 0.0f)));
	return pDestination;
}

inline GfxPlane* GfxPlaneFromPoints(
	GfxPlane* pDestination,
	const GfxVector3* pPoint0,
	const GfxVector3* pPoint1,
	const GfxVector3* pPoint2)
{
	if (pDestination == NULL || pPoint0 == NULL
		|| pPoint1 == NULL || pPoint2 == NULL)
		return NULL;
	DirectX::XMStoreFloat4(
		reinterpret_cast<DirectX::XMFLOAT4*>(pDestination),
		DirectX::XMPlaneFromPoints(
			GfxMathDetail::LoadVector3(pPoint0, 1.0f),
			GfxMathDetail::LoadVector3(pPoint1, 1.0f),
			GfxMathDetail::LoadVector3(pPoint2, 1.0f)));
	return pDestination;
}

inline GfxPlane* GfxPlaneTransform(
	GfxPlane* pDestination,
	const GfxPlane* pSource,
	const GfxMatrix* pMatrix)
{
	if (pDestination == NULL || pSource == NULL || pMatrix == NULL)
		return NULL;
	DirectX::XMStoreFloat4(
		reinterpret_cast<DirectX::XMFLOAT4*>(pDestination),
		DirectX::XMPlaneTransform(
			GfxMathDetail::LoadPlane(pSource),
			GfxMathDetail::LoadMatrix(pMatrix)));
	return pDestination;
}

const FLOAT GFX_PI = DirectX::XM_PI;

#endif
