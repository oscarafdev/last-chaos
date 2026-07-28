#include "stdh.h"

#include <vector>

#include <Engine/Graphics/DirectX12ResourceHandle.h>

namespace
{
	const UINT64 INDEX_MASK = 0x00000000FFFFFFFFULL;
	const UINT64 GENERATION_MASK = 0x00FFFFFF00000000ULL;
	const UINT64 KIND_MASK = 0xFF00000000000000ULL;

	UINT64 EncodeHandle(
		UINT index,
		UINT generation,
		DirectX12ResourceKind kind)
	{
		return (static_cast<UINT64>(kind) << 56)
			| ((static_cast<UINT64>(generation) & 0x00FFFFFFULL) << 32)
			| static_cast<UINT64>(index + 1);
	}

	UINT DecodeIndex(UINT64 value)
	{
		return static_cast<UINT>((value & INDEX_MASK) - 1);
	}

	bool HasEncodedIndex(UINT64 value)
	{
		return (value & INDEX_MASK) != 0;
	}

	UINT DecodeGeneration(UINT64 value)
	{
		return static_cast<UINT>((value & GENERATION_MASK) >> 32);
	}

	DirectX12ResourceKind DecodeKind(UINT64 value)
	{
		return static_cast<DirectX12ResourceKind>(
			(value & KIND_MASK) >> 56);
	}
}

struct CDirectX12ResourceRegistry::State
{
	struct Slot
	{
		void* pOwner;
		UINT generation;
		DirectX12ResourceKind kind;
		bool occupied;

		Slot()
			: pOwner(NULL)
			, generation(1)
			, kind(DX12_RESOURCE_UNKNOWN)
			, occupied(false)
		{
		}
	};

	struct LegacyAlias
	{
		const void* pIdentity;
		UINT64 handle;
		DirectX12ResourceKind kind;
	};

	std::vector<Slot> slots;
	std::vector<UINT> freeSlots;
	std::vector<LegacyAlias> aliases;
};

CDirectX12ResourceRegistry::CDirectX12ResourceRegistry()
	: m_pState(new State)
{
}

CDirectX12ResourceRegistry::~CDirectX12ResourceRegistry()
{
	delete m_pState;
	m_pState = NULL;
}

UINT64 CDirectX12ResourceRegistry::AllocateRaw(
	DirectX12ResourceKind kind,
	void* pOwner)
{
	if (m_pState == NULL || pOwner == NULL
		|| kind == DX12_RESOURCE_UNKNOWN)
		return 0;

	UINT index = 0;
	if (!m_pState->freeSlots.empty())
	{
		index = m_pState->freeSlots.back();
		m_pState->freeSlots.pop_back();
	}
	else
	{
		index = static_cast<UINT>(m_pState->slots.size());
		m_pState->slots.push_back(State::Slot());
	}

	State::Slot& slot = m_pState->slots[index];
	slot.pOwner = pOwner;
	slot.kind = kind;
	slot.occupied = true;
	return EncodeHandle(index, slot.generation, kind);
}

void CDirectX12ResourceRegistry::ReleaseRaw(
	UINT64 value,
	DirectX12ResourceKind kind)
{
	if (m_pState == NULL || value == 0 || !HasEncodedIndex(value)
		|| DecodeKind(value) != kind)
		return;
	const UINT index = DecodeIndex(value);
	if (index >= m_pState->slots.size())
		return;
	State::Slot& slot = m_pState->slots[index];
	if (!slot.occupied || slot.kind != kind
		|| slot.generation != DecodeGeneration(value))
		return;

	for (size_t iAlias = 0; iAlias < m_pState->aliases.size();)
	{
		if (m_pState->aliases[iAlias].handle == value)
			m_pState->aliases.erase(
				m_pState->aliases.begin() + iAlias);
		else
			++iAlias;
	}
	slot.pOwner = NULL;
	slot.kind = DX12_RESOURCE_UNKNOWN;
	slot.occupied = false;
	slot.generation = (slot.generation + 1) & 0x00FFFFFFU;
	if (slot.generation == 0)
		slot.generation = 1;
	m_pState->freeSlots.push_back(index);
}

void* CDirectX12ResourceRegistry::ResolveRaw(
	UINT64 value,
	DirectX12ResourceKind kind) const
{
	if (m_pState == NULL || value == 0 || !HasEncodedIndex(value)
		|| DecodeKind(value) != kind)
		return NULL;
	const UINT index = DecodeIndex(value);
	if (index >= m_pState->slots.size())
		return NULL;
	const State::Slot& slot = m_pState->slots[index];
	return slot.occupied && slot.kind == kind
		&& slot.generation == DecodeGeneration(value)
		? slot.pOwner
		: NULL;
}

bool CDirectX12ResourceRegistry::BindLegacyAliasRaw(
	const void* pLegacyIdentity,
	UINT64 value,
	DirectX12ResourceKind kind)
{
	if (pLegacyIdentity == NULL || ResolveRaw(value, kind) == NULL)
		return false;
	for (size_t iAlias = 0;
		iAlias < m_pState->aliases.size();
		++iAlias)
	{
		State::LegacyAlias& alias = m_pState->aliases[iAlias];
		if (alias.pIdentity == pLegacyIdentity && alias.kind == kind)
		{
			alias.handle = value;
			return true;
		}
	}
	State::LegacyAlias alias;
	alias.pIdentity = pLegacyIdentity;
	alias.handle = value;
	alias.kind = kind;
	m_pState->aliases.push_back(alias);
	return true;
}

void CDirectX12ResourceRegistry::UnbindLegacyAliasRaw(
	const void* pLegacyIdentity,
	UINT64 value,
	DirectX12ResourceKind kind)
{
	if (m_pState == NULL || pLegacyIdentity == NULL)
		return;
	for (size_t iAlias = 0;
		iAlias < m_pState->aliases.size();
		++iAlias)
	{
		const State::LegacyAlias& alias = m_pState->aliases[iAlias];
		if (alias.pIdentity == pLegacyIdentity && alias.handle == value
			&& alias.kind == kind)
		{
			m_pState->aliases.erase(
				m_pState->aliases.begin() + iAlias);
			return;
		}
	}
}

UINT64 CDirectX12ResourceRegistry::ResolveLegacyAliasRaw(
	const void* pLegacyIdentity,
	DirectX12ResourceKind kind) const
{
	if (m_pState == NULL || pLegacyIdentity == NULL)
		return 0;
	for (size_t iAlias = 0;
		iAlias < m_pState->aliases.size();
		++iAlias)
	{
		const State::LegacyAlias& alias = m_pState->aliases[iAlias];
		if (alias.pIdentity == pLegacyIdentity && alias.kind == kind
			&& ResolveRaw(alias.handle, kind) != NULL)
			return alias.handle;
	}
	return 0;
}

CDirectX12ResourceRegistry& GetDirectX12ResourceRegistry()
{
	// Se mantiene vivo hasta finalizar el proceso para evitar dependencias de
	// orden entre destructores globales del motor y recursos gráficos.
	static CDirectX12ResourceRegistry* pRegistry =
		new CDirectX12ResourceRegistry;
	return *pRegistry;
}

bool ValidateDirectX12ResourceRegistry()
{
	CDirectX12ResourceRegistry registry;
	int firstOwner = 1;
	int secondOwner = 2;
	const DirectX12TextureHandle first =
		registry.Allocate<DX12_RESOURCE_SAMPLED_TEXTURE>(&firstOwner);
	if (!first.IsValid() || registry.Resolve(first) != &firstOwner
		|| !registry.BindLegacyAlias(&firstOwner, first)
		|| registry.ResolveLegacyAlias<DX12_RESOURCE_SAMPLED_TEXTURE>(
			&firstOwner) != first)
		return false;

	registry.Release(first);
	if (registry.Resolve(first) != NULL
		|| registry.ResolveLegacyAlias<DX12_RESOURCE_SAMPLED_TEXTURE>(
			&firstOwner).IsValid())
		return false;

	const DirectX12TextureHandle second =
		registry.Allocate<DX12_RESOURCE_SAMPLED_TEXTURE>(&secondOwner);
	const bool validGeneration =
		second.IsValid() && second != first
		&& registry.Resolve(second) == &secondOwner;
	registry.Release(second);
	return validGeneration;
}
