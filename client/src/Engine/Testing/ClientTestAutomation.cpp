#include "StdH.h"

#include <Engine/Testing/ClientTestAutomation.h>
#include <Engine/Testing/CameraTestCapture.h>

#include <Engine/Contents/Login/LoginNew.h>
#include <Engine/Contents/Login/ServerSelect.h>
#include <Engine/Brushes/Brush.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/GameStageManager/StageMgr.h>
#include <Engine/GameState.h>
#include <Engine/Graphics/Fog.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/TcpIpConnection.h>
#include <Engine/Entities/EntityClass.h>
#include <Engine/Ska/Mesh.h>
#include <Engine/Ska/ModelInstance.h>
#include <Engine/World/World.h>

namespace
{
	const ULONG NORMAL_MAP_SPECULAR_FLAG = 1UL << BASE_FLAG_OFFSET;

	BOOL IsNormalMapShader(const CShader* pShader)
	{
		if (pShader == NULL)
			return FALSE;

		const char* shaderFile = pShader->ser_FileName;
		return shaderFile != NULL
			&& strstr(shaderFile, "NormalMap.sha") != NULL;
	}

	INDEX EnableNormalMapSpecular(CModelInstance* pModel)
	{
		if (pModel == NULL)
			return 0;

		INDEX configuredSurfaceCount = 0;
		for (INDEX meshIndex = 0;
			meshIndex < pModel->mi_aMeshInst.Count();
			++meshIndex)
		{
			CMesh* pMesh = pModel->mi_aMeshInst[meshIndex].mi_pMesh;
			if (pMesh == NULL)
				continue;

			for (INDEX lodIndex = 0;
				lodIndex < pMesh->msh_aMeshLODs.Count();
				++lodIndex)
			{
				MeshLOD& lod = pMesh->msh_aMeshLODs[lodIndex];
				for (INDEX surfaceIndex = 0;
					surfaceIndex < lod.mlod_aSurfaces.Count();
					++surfaceIndex)
				{
					MeshSurface& surface =
						lod.mlod_aSurfaces[surfaceIndex];
					if (!IsNormalMapShader(surface.msrf_pShader))
						continue;

					surface.msrf_ShadingParams.sp_ulFlags |=
						NORMAL_MAP_SPECULAR_FLAG;
					++configuredSurfaceCount;
				}
			}
		}

		for (INDEX childIndex = 0;
			childIndex < pModel->mi_cmiChildren.Count();
			++childIndex)
		{
			configuredSurfaceCount += EnableNormalMapSpecular(
				&pModel->mi_cmiChildren[childIndex]);
		}
		return configuredSurfaceCount;
	}
}

CClientTestAutomation& CClientTestAutomation::Instance()
{
	static CClientTestAutomation automation;
	return automation;
}

CClientTestAutomation::CClientTestAutomation()
	: m_worldAnchorSector(NULL)
	, m_serverIndex(0)
	, m_channelIndex(0)
	, m_characterIndex(0)
	, m_characterHoldSeconds(0)
	, m_worldCommandDelaySeconds(5)
	, m_worldModelDelaySeconds(5)
	, m_worldModelLifetimeSeconds(30)
	, m_worldAnchorDelaySeconds(5)
	, m_worldModelAlpha(255)
	, m_worldViewDelaySeconds(5)
	, m_worldViewHoldSeconds(2)
	, m_worldNetworkCameraAngle(0.0f)
	, m_characterStageEnteredAt(0)
	, m_gameplayStageEnteredAt(0)
	, m_worldModelSpawnedAt(0)
	, m_worldModelFixture(NULL)
	, m_enabled(FALSE)
	, m_loginSubmitted(FALSE)
	, m_serverSubmitted(FALSE)
	, m_characterSubmitted(FALSE)
	, m_worldCommandSubmitted(FALSE)
	, m_worldAnchorApplied(FALSE)
	, m_worldModelSpawned(FALSE)
	, m_worldViewConfigured(FALSE)
	, m_worldViewApplied(FALSE)
	, m_worldCaptureRequested(FALSE)
	, m_worldModelNormalMapSpecular(FALSE)
	, m_forceBloom(FALSE)
	, m_bloomConfigured(FALSE)
	, m_lastReportedStage(-2)
{
}

void CClientTestAutomation::ConfigureLogin(
	const CTString& userName,
	const CTString& password)
{
	m_userName = userName;
	m_password = password;
	m_enabled =
		m_userName.Length() > 0 && m_password.Length() > 0;
	m_loginSubmitted = FALSE;
	m_serverSubmitted = FALSE;
	m_characterSubmitted = FALSE;
	m_worldCommandSubmitted = FALSE;
	m_worldAnchorApplied = FALSE;
	m_worldModelSpawned = FALSE;
}

void CClientTestAutomation::ConfigureWorldSelection(
	INDEX serverIndex,
	INDEX channelIndex,
	INDEX characterIndex)
{
	m_serverIndex = (std::max)(serverIndex, static_cast<INDEX>(0));
	m_channelIndex = (std::max)(channelIndex, static_cast<INDEX>(0));
	m_characterIndex =
		(std::max)(characterIndex, static_cast<INDEX>(0));
}

void CClientTestAutomation::ConfigureCharacterHold(INDEX holdSeconds)
{
	m_characterHoldSeconds = (std::max)(
		static_cast<INDEX>(0),
		(std::min)(holdSeconds, static_cast<INDEX>(300)));
}

void CClientTestAutomation::ConfigureWorldCommand(
	const CTString& command,
	INDEX delaySeconds)
{
	m_worldCommand = command;
	m_worldCommandDelaySeconds = (std::max)(
		static_cast<INDEX>(0),
		(std::min)(delaySeconds, static_cast<INDEX>(300)));
	m_worldCommandSubmitted = FALSE;
}

void CClientTestAutomation::ConfigureWorldModel(
	const CTString& model,
	INDEX delaySeconds,
	INDEX lifetimeSeconds)
{
	m_worldModel = model;
	m_worldModelDelaySeconds = (std::max)(
		static_cast<INDEX>(0),
		(std::min)(delaySeconds, static_cast<INDEX>(300)));
	m_worldModelLifetimeSeconds = (std::max)(
		static_cast<INDEX>(1),
		(std::min)(lifetimeSeconds, static_cast<INDEX>(300)));
	m_worldModelFixture = NULL;
	m_worldModelSpawned = FALSE;
}

void CClientTestAutomation::ConfigureWorldAnchor(
	const CTString& entityClass,
	INDEX delaySeconds)
{
	m_worldAnchorClass = entityClass;
	m_worldAnchorSector = NULL;
	m_worldAnchorDelaySeconds = (std::max)(
		static_cast<INDEX>(0),
		(std::min)(delaySeconds, static_cast<INDEX>(300)));
	m_worldAnchorApplied = FALSE;
}

void CClientTestAutomation::ConfigureWorldModelRendering(
	INDEX alpha,
	BOOL enableNormalMapSpecular)
{
	m_worldModelAlpha = (std::max)(
		static_cast<INDEX>(0),
		(std::min)(alpha, static_cast<INDEX>(255)));
	m_worldModelNormalMapSpecular = enableNormalMapSpecular;
}

void CClientTestAutomation::ConfigureWorldView(
	const CPlacement3D& playerPlacement,
	const CPlacement3D& viewpointPlacement,
	FLOAT networkCameraAngle,
	INDEX delaySeconds,
	INDEX holdSeconds,
	const CTString& captureName)
{
	m_worldPlayerPlacement = playerPlacement;
	m_worldViewpointPlacement = viewpointPlacement;
	m_worldNetworkCameraAngle = networkCameraAngle;
	m_worldViewDelaySeconds = (std::max)(
		static_cast<INDEX>(0),
		(std::min)(delaySeconds, static_cast<INDEX>(300)));
	m_worldViewHoldSeconds = (std::max)(
		static_cast<INDEX>(1),
		(std::min)(holdSeconds, static_cast<INDEX>(30)));
	m_worldCaptureName = captureName;
	m_worldViewConfigured = TRUE;
	m_worldViewApplied = FALSE;
	m_worldCaptureRequested = FALSE;
}

void CClientTestAutomation::ConfigureBloomTest(BOOL forceEnabled)
{
	m_forceBloom = forceEnabled;
	m_bloomConfigured = FALSE;
}

void CClientTestAutomation::Tick()
{
	CCameraTestCapture::PollRequest();
	if (STAGEMGR() == NULL)
		return;

	const INDEX stage = STAGEMGR()->GetCurStage();
	if (stage != m_lastReportedStage)
	{
		CPrintF(
			"Diagnostico de arranque: etapa %d detectada%s.\n",
			stage,
			m_enabled ? " con automatizacion" : "");
		m_lastReportedStage = stage;
	}

	if (!m_enabled)
		return;

	if (m_forceBloom)
	{
		extern INDEX g_iUseBloom;
		if (g_iUseBloom <= 0)
			g_iUseBloom = 1;
		if (!m_bloomConfigured)
		{
			m_bloomConfigured = TRUE;
			CPrintF("Prueba automatizada: bloom habilitado.\n");
		}
	}

	static INDEX configuredStage = -2;
	if (stage != configuredStage)
	{
		CPrintF(
			"Prueba automatizada: etapa %d detectada.\n",
			stage);
		if (stage == eSTAGE_SELCHAR)
		{
			m_characterStageEnteredAt = GetTickCount();
			if (m_characterHoldSeconds > 0)
			{
				CPrintF(
					"Prueba automatizada: seleccion de personaje "
					"retenida %d segundos.\n",
					m_characterHoldSeconds);
			}
		}
		else if (stage == eSTAGE_GAMEPLAY)
		{
			m_gameplayStageEnteredAt = GetTickCount();
			m_worldModelFixture = NULL;
			m_worldAnchorSector = NULL;
			m_worldAnchorApplied = FALSE;
			m_worldModelSpawned = FALSE;
			m_worldViewApplied = FALSE;
			m_worldCaptureRequested = FALSE;
		}
		else
		{
			// El mundo elimina sus entidades al cambiar de etapa.
			m_worldModelFixture = NULL;
		}
		configuredStage = stage;
	}
	switch (stage)
	{
	case eSTAGE_LOGIN:
		if (!m_loginSubmitted)
			m_loginSubmitted = TrySubmitLogin();
		break;
	case eSTAGE_SELSERVER:
		if (!m_serverSubmitted)
			m_serverSubmitted = TrySubmitServer();
		break;
	case eSTAGE_SELCHAR:
		if (!m_characterSubmitted)
			m_characterSubmitted = TrySubmitCharacter();
		break;
	case eSTAGE_GAMEPLAY:
		if (m_worldViewConfigured)
			m_worldViewApplied = TryApplyWorldView();
		if (!m_worldCommandSubmitted && m_worldCommand.Length() > 0)
			m_worldCommandSubmitted = TrySubmitWorldCommand();
		if (!m_worldAnchorApplied && m_worldAnchorClass.Length() > 0)
			m_worldAnchorApplied = TryApplyWorldAnchor();
		else if (m_worldAnchorApplied)
		{
			CEntity* pPlayer = CEntity::GetPlayerEntity(0);
			if (pPlayer != NULL)
				pPlayer->SetPlacement(m_worldAnchorPlacement);
		}
		if (!m_worldModelSpawned && m_worldModel.Length() > 0)
			m_worldModelSpawned = TrySpawnWorldModel();
		else if (m_worldModelFixture != NULL)
			TryRemoveWorldModel();
		break;
	default:
		break;
	}
}

BOOL CClientTestAutomation::TryApplyWorldView()
{
	const ULONG elapsed = GetTickCount() - m_gameplayStageEnteredAt;
	const ULONG delay =
		static_cast<ULONG>(m_worldViewDelaySeconds) * 1000UL;
	if (elapsed < delay || _pNetwork == NULL)
		return FALSE;

	CPlayerEntity* player =
		static_cast<CPlayerEntity*>(CEntity::GetPlayerEntity(0));
	if (player == NULL)
		return FALSE;

	const ULONG hold =
		static_cast<ULONG>(m_worldViewHoldSeconds) * 1000UL;
	if (!m_worldViewApplied || elapsed < delay + hold)
	{
		player->SetPlacement(m_worldPlayerPlacement);
		player->en_plViewpoint = m_worldViewpointPlacement;
		player->en_plLastViewpoint = m_worldViewpointPlacement;
		_pNetwork->SetMyPosition(
			m_worldPlayerPlacement,
			m_worldNetworkCameraAngle);
	}

	if (!m_worldViewApplied)
	{
		CPrintF(
			"Prueba automatizada: posición y cámara restauradas en "
			"(%.3f, %.3f, %.3f).\n",
			m_worldPlayerPlacement.pl_PositionVector(1),
			m_worldPlayerPlacement.pl_PositionVector(2),
			m_worldPlayerPlacement.pl_PositionVector(3));
	}

	if (!m_worldCaptureRequested
		&& elapsed >= delay + hold
		&& m_worldCaptureName.Length() > 0)
	{
		CCameraTestCapture::Request(m_worldCaptureName);
		m_worldCaptureRequested = TRUE;
	}
	return TRUE;
}

BOOL CClientTestAutomation::TrySubmitLogin()
{
	if (_pNetwork == NULL || _pNetwork->m_bSendMessage)
		return FALSE;

	GameDataManager* pGameData = GameDataManager::getSingleton();
	CUIManager* pUiManager = CUIManager::getSingleton();
	if (pGameData == NULL || pUiManager == NULL
		|| pUiManager->GetGame() == NULL)
		return FALSE;

	LoginNew* pLogin = pGameData->GetLoginData();
	if (pLogin == NULL)
		return FALSE;

	pLogin->SetUserId(m_userName);
	pLogin->SetPassword(m_password);
	if (!pUiManager->GetGame()->PreNewGame()
		|| !pLogin->ConnectToLoginServer())
		return FALSE;

	CPrintF("Prueba automatizada: inicio de sesion enviado.\n");
	ClearPassword();
	return TRUE;
}

BOOL CClientTestAutomation::TrySubmitServer()
{
	if (_pNetwork == NULL || _pNetwork->m_bSendMessage)
		return FALSE;

	GameDataManager* pGameData = GameDataManager::getSingleton();
	if (pGameData == NULL)
		return FALSE;
	CServerSelect* pSelection = pGameData->GetServerData();
	if (pSelection == NULL)
		return FALSE;

	sServerInfo* pServer = pSelection->ServerListAt(m_serverIndex);
	if (pServer == NULL
		|| m_channelIndex < 0
		|| m_channelIndex
			>= static_cast<INDEX>(pServer->vecChannelInfo.size()))
		return FALSE;
	sChannelInfo& channel = pServer->vecChannelInfo[m_channelIndex];

	pSelection->ConnectToServer(channel.strAddress, channel.iPortNum);
	_pNetwork->m_iServerGroup = pServer->iServerNo;
	_pNetwork->m_iServerCh = channel.iSubNum;
	_pNetwork->m_iServerType = pServer->ubServerType;
	_pGameState->ClearCharacterSlot();
	CPrintF(
		"Prueba automatizada: servidor %d, canal %d enviados.\n",
		m_serverIndex,
		m_channelIndex);
	return TRUE;
}

BOOL CClientTestAutomation::TrySubmitCharacter()
{
	const ULONG elapsed =
		GetTickCount() - m_characterStageEnteredAt;
	if (m_characterHoldSeconds > 0
		&& elapsed
			< static_cast<ULONG>(m_characterHoldSeconds) * 1000UL)
		return FALSE;
	if (_pNetwork == NULL || _pNetwork->m_bSendMessage
		|| _pGameState == NULL
		|| m_characterIndex < 0
		|| m_characterIndex >= _pGameState->GetExistCharNum())
		return FALSE;

	const ULONG characterId =
		_pGameState->m_SlotInfo[m_characterIndex].index;
	_pNetwork->SendGameStart(characterId);
	CPrintF(
		"Prueba automatizada: personaje %d enviado.\n",
		m_characterIndex);
	return TRUE;
}

BOOL CClientTestAutomation::TrySubmitWorldCommand()
{
	const ULONG elapsed =
		GetTickCount() - m_gameplayStageEnteredAt;
	if (elapsed
		< static_cast<ULONG>(m_worldCommandDelaySeconds) * 1000UL)
		return FALSE;
	if (_pNetwork == NULL || _pNetwork->m_bSendMessage)
		return FALSE;

	_pNetwork->SendGMCommand(m_worldCommand);
	CPrintF("Prueba automatizada: comando de mundo enviado.\n");
	return TRUE;
}

BOOL CClientTestAutomation::TryApplyWorldAnchor()
{
	const ULONG elapsed =
		GetTickCount() - m_gameplayStageEnteredAt;
	if (elapsed
		< static_cast<ULONG>(m_worldAnchorDelaySeconds) * 1000UL)
		return FALSE;
	if (_pNetwork == NULL)
		return FALSE;

	CEntity* pPlayer = CEntity::GetPlayerEntity(0);
	if (pPlayer == NULL)
		return FALSE;

	if (m_worldAnchorClass == "Fog Marker")
	{
		CBrushSector* pSector = pPlayer->GetFirstSector();
		if (pSector != NULL
			&& pSector->bsc_pbmBrushMip != NULL
			&& pSector->bsc_pbmBrushMip->bm_pbrBrush != NULL)
		{
			CEntity* pWorldEntity =
				pSector->bsc_pbmBrushMip->bm_pbrBrush->br_penEntity;
			for (INDEX fogIndex = 0; fogIndex < 9; ++fogIndex)
			{
				CFogParameters fog;
				if (pWorldEntity == NULL
					|| !pWorldEntity->GetFog(fogIndex, fog))
					continue;

				pSector->SetFogType(fogIndex);
				m_worldAnchorSector = pSector;
				m_worldAnchorPlacement = pPlayer->GetPlacement();
				CPrintF(
					"Prueba automatizada: niebla %d aplicada al "
					"sector actual.\n",
					fogIndex);
				return TRUE;
			}
		}
	}

	CDynamicContainer<CEntity>& entities =
		_pNetwork->ga_World.wo_cenAllEntities;
	if (m_worldAnchorClass == "Fog Marker")
	{
		for (INDEX entityIndex = 0;
			entityIndex < entities.Count();
			++entityIndex)
		{
			CEntity* pEntity = &entities[entityIndex];
			if (pEntity->GetRenderType() != CEntity::RT_BRUSH
				|| pEntity->GetBrush() == NULL
				|| !(pEntity->GetFlags() & ENF_ZONING))
				continue;

			CBrushMip* pMip = pEntity->GetBrush()->GetFirstMip();
			if (pMip == NULL)
				continue;

			for (INDEX sectorIndex = 0;
				sectorIndex < pMip->bm_abscSectors.Count();
				++sectorIndex)
			{
				CBrushSector& sector =
					pMip->bm_abscSectors[sectorIndex];
				const INDEX fogIndex = sector.GetFogType();
				CFogParameters fog;
				if (!pEntity->GetFog(fogIndex, fog))
					continue;

				m_worldAnchorPlacement = pPlayer->GetPlacement();
				m_worldAnchorSector = &sector;
				// El centro geométrico del sector no representa una superficie
				// caminable y puede dejar al jugador flotando. Conservamos el
				// punto válido de entrada y usamos la relación únicamente para
				// aportar el contexto de niebla a la escena controlada.
				pPlayer->en_rdSectors.Clear();
				AddRelationPairTailTail(
					sector.bsc_rsEntities,
					pPlayer->en_rdSectors);
				CPrintF(
					"Prueba automatizada: sector con niebla %d "
					"relacionado desde (%.2f, %.2f, %.2f).\n",
					fogIndex,
					m_worldAnchorPlacement.pl_PositionVector(1),
					m_worldAnchorPlacement.pl_PositionVector(2),
					m_worldAnchorPlacement.pl_PositionVector(3));
				return TRUE;
			}
		}
	}

	for (INDEX entityIndex = 0;
		entityIndex < entities.Count();
		++entityIndex)
	{
		CEntity* pEntity = &entities[entityIndex];
		if (!IsOfClass(pEntity, m_worldAnchorClass))
			continue;

		m_worldAnchorPlacement = pEntity->GetPlacement();
		pPlayer->SetPlacement(m_worldAnchorPlacement);
		CPrintF(
			"Prueba automatizada: ancla %s aplicada en "
			"(%.2f, %.2f, %.2f).\n",
			(const char*)m_worldAnchorClass,
			m_worldAnchorPlacement.pl_PositionVector(1),
			m_worldAnchorPlacement.pl_PositionVector(2),
			m_worldAnchorPlacement.pl_PositionVector(3));
		return TRUE;
	}

	return FALSE;
}

BOOL CClientTestAutomation::TrySpawnWorldModel()
{
	const ULONG elapsed =
		GetTickCount() - m_gameplayStageEnteredAt;
	if (elapsed
		< static_cast<ULONG>(m_worldModelDelaySeconds) * 1000UL)
		return FALSE;
	if (_pNetwork == NULL)
		return FALSE;

	CEntity* pPlayer = CEntity::GetPlayerEntity(0);
	if (pPlayer == NULL)
		return FALSE;
	if (m_worldAnchorClass.Length() > 0 && !m_worldAnchorApplied)
		return FALSE;
	CPlacement3D placement = m_worldAnchorApplied
		? m_worldAnchorPlacement
		: pPlayer->GetPlacement();
	// Separa el fixture del personaje para que la comparación visual permita
	// distinguir ambas geometrías y sus materiales, incluso con un ancla.
	placement.pl_PositionVector(1) += 2.0f;

	CEntity* pFixture = NULL;
	try
	{
		pFixture = _pNetwork->ga_World.CreateEntity_t(
			placement,
			CTFILENAME("Classes\\ModelHolder3.ecl"),
			WLD_AUTO_ENTITY_ID,
			FALSE);
		pFixture->Initialize(_eeVoid, FALSE);
		pFixture->SetSkaModel_t(m_worldModel);
		if (m_worldAnchorClass == "Fog Marker"
			&& m_worldAnchorSector != NULL)
		{
			// Un fixture grande puede tocar sectores vecinos y quedar
			// encolado primero sin fog. La prueba lo relaciona únicamente
			// con el sector controlado para reproducir la pareja exacta.
			pFixture->en_rdSectors.Clear();
			AddRelationPairTailTail(
				m_worldAnchorSector->bsc_rsEntities,
				pFixture->en_rdSectors);
		}
		pFixture->SetModelColor(
			0xFFFFFF00UL | static_cast<UBYTE>(m_worldModelAlpha));
		INDEX configuredSurfaceCount = 0;
		if (m_worldModelNormalMapSpecular)
		{
			configuredSurfaceCount = EnableNormalMapSpecular(
				pFixture->GetModelInstance());
		}
		CPrintF(
			"Prueba automatizada: modelo configurado con alfa %d "
			"y %d superficies NormalMap specular.\n",
			m_worldModelAlpha,
			configuredSurfaceCount);
		m_worldModelFixture = pFixture;
		m_worldModelSpawnedAt = GetTickCount();
	}
	catch (char* error)
	{
		if (pFixture != NULL)
			pFixture->Destroy();
		CPrintF(
			"Prueba automatizada: no se pudo crear el modelo: %s\n",
			error);
		return TRUE;
	}

	CPrintF("Prueba automatizada: modelo de mundo creado.\n");
	return TRUE;
}

void CClientTestAutomation::TryRemoveWorldModel()
{
	if (m_worldModelFixture == NULL || _pNetwork == NULL)
		return;

	const ULONG elapsed = GetTickCount() - m_worldModelSpawnedAt;
	if (elapsed
		< static_cast<ULONG>(m_worldModelLifetimeSeconds) * 1000UL)
		return;

	_pNetwork->ga_World.DestroyOneEntity(m_worldModelFixture);
	m_worldModelFixture = NULL;
	CPrintF(
		"Prueba automatizada: modelo de mundo retirado tras %d segundos.\n",
		m_worldModelLifetimeSeconds);
}

void CClientTestAutomation::ClearPassword()
{
	m_password = "";
}

BOOL CClientTestAutomation::IsEnabled() const
{
	return m_enabled;
}

const CTString& CClientTestAutomation::GetUserName() const
{
	return m_userName;
}

const CTString& CClientTestAutomation::GetPassword() const
{
	return m_password;
}

INDEX CClientTestAutomation::GetServerIndex() const
{
	return m_serverIndex;
}

INDEX CClientTestAutomation::GetChannelIndex() const
{
	return m_channelIndex;
}

INDEX CClientTestAutomation::GetCharacterIndex() const
{
	return m_characterIndex;
}
