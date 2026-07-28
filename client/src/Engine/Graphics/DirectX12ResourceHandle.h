#ifndef SE_INCL_DIRECTX12RESOURCEHANDLE_H
#define SE_INCL_DIRECTX12RESOURCEHANDLE_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <windows.h>

enum DirectX12ResourceKind
{
	DX12_RESOURCE_UNKNOWN,
	DX12_RESOURCE_SAMPLED_TEXTURE,
	DX12_RESOURCE_RENDER_TEXTURE,
	DX12_RESOURCE_VERTEX_BUFFER,
	DX12_RESOURCE_INDEX_BUFFER
};

template<DirectX12ResourceKind Kind>
class DirectX12TypedResourceHandle
{
public:
	DirectX12TypedResourceHandle()
		: m_value(0)
	{
	}

	bool IsValid() const
	{
		return m_value != 0;
	}

	UINT64 GetValue() const
	{
		return m_value;
	}

	static DirectX12TypedResourceHandle FromValue(UINT64 value)
	{
		const UINT64 encodedKind = value >> 56;
		return encodedKind == static_cast<UINT64>(Kind)
			? DirectX12TypedResourceHandle(value)
			: DirectX12TypedResourceHandle();
	}

	bool operator==(const DirectX12TypedResourceHandle& other) const
	{
		return m_value == other.m_value;
	}

	bool operator!=(const DirectX12TypedResourceHandle& other) const
	{
		return m_value != other.m_value;
	}

private:
	explicit DirectX12TypedResourceHandle(UINT64 value)
		: m_value(value)
	{
	}

	UINT64 m_value;

	friend class CDirectX12ResourceRegistry;
};

typedef DirectX12TypedResourceHandle<DX12_RESOURCE_SAMPLED_TEXTURE>
	DirectX12TextureHandle;
typedef DirectX12TypedResourceHandle<DX12_RESOURCE_RENDER_TEXTURE>
	DirectX12RenderTextureHandle;
typedef DirectX12TypedResourceHandle<DX12_RESOURCE_VERTEX_BUFFER>
	DirectX12VertexBufferHandle;
typedef DirectX12TypedResourceHandle<DX12_RESOURCE_INDEX_BUFFER>
	DirectX12IndexBufferHandle;

static const DirectX12TextureHandle DX12_INVALID_TEXTURE;
static const DirectX12RenderTextureHandle DX12_INVALID_RENDER_TEXTURE;
static const DirectX12VertexBufferHandle DX12_INVALID_VERTEX_BUFFER;
static const DirectX12IndexBufferHandle DX12_INVALID_INDEX_BUFFER;

// Registro generacional compartido por todas las familias de recursos.
// Los aliases legacy son adaptadores temporales: la identidad canónica es el
// handle y sobrevive aunque el puntero D3D9 sea reemplazado durante un upload.
class CDirectX12ResourceRegistry
{
public:
	CDirectX12ResourceRegistry();
	~CDirectX12ResourceRegistry();

	template<DirectX12ResourceKind Kind>
	DirectX12TypedResourceHandle<Kind> Allocate(void* pOwner)
	{
		return DirectX12TypedResourceHandle<Kind>(
			AllocateRaw(Kind, pOwner));
	}

	template<DirectX12ResourceKind Kind>
	void Release(DirectX12TypedResourceHandle<Kind> handle)
	{
		ReleaseRaw(handle.GetValue(), Kind);
	}

	template<DirectX12ResourceKind Kind>
	void* Resolve(DirectX12TypedResourceHandle<Kind> handle) const
	{
		return ResolveRaw(handle.GetValue(), Kind);
	}

	template<DirectX12ResourceKind Kind>
	bool BindLegacyAlias(
		const void* pLegacyIdentity,
		DirectX12TypedResourceHandle<Kind> handle)
	{
		return BindLegacyAliasRaw(
			pLegacyIdentity,
			handle.GetValue(),
			Kind);
	}

	template<DirectX12ResourceKind Kind>
	void UnbindLegacyAlias(
		const void* pLegacyIdentity,
		DirectX12TypedResourceHandle<Kind> handle)
	{
		UnbindLegacyAliasRaw(
			pLegacyIdentity,
			handle.GetValue(),
			Kind);
	}

	template<DirectX12ResourceKind Kind>
	DirectX12TypedResourceHandle<Kind> ResolveLegacyAlias(
		const void* pLegacyIdentity) const
	{
		return DirectX12TypedResourceHandle<Kind>(
			ResolveLegacyAliasRaw(pLegacyIdentity, Kind));
	}

private:
	CDirectX12ResourceRegistry(const CDirectX12ResourceRegistry&);
	CDirectX12ResourceRegistry& operator=(
		const CDirectX12ResourceRegistry&);

	UINT64 AllocateRaw(DirectX12ResourceKind kind, void* pOwner);
	void ReleaseRaw(UINT64 value, DirectX12ResourceKind kind);
	void* ResolveRaw(UINT64 value, DirectX12ResourceKind kind) const;
	bool BindLegacyAliasRaw(
		const void* pLegacyIdentity,
		UINT64 value,
		DirectX12ResourceKind kind);
	void UnbindLegacyAliasRaw(
		const void* pLegacyIdentity,
		UINT64 value,
		DirectX12ResourceKind kind);
	UINT64 ResolveLegacyAliasRaw(
		const void* pLegacyIdentity,
		DirectX12ResourceKind kind) const;

	struct State;
	State* m_pState;
};

CDirectX12ResourceRegistry& GetDirectX12ResourceRegistry();
bool ValidateDirectX12ResourceRegistry();

#endif
