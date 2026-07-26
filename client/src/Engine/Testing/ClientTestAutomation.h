#ifndef SE_INCL_CLIENTTESTAUTOMATION_H
#define SE_INCL_CLIENTTESTAUTOMATION_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <Engine/Base/CTString.h>
#include <Engine/Math/Placement.h>

class CEntity;
class CBrushSector;

// Automatización opcional para pruebas integrales del cliente. No se activa
// durante una ejecución normal y nunca expone la contraseña en el registro.
class ENGINE_API CClientTestAutomation
{
public:
	static CClientTestAutomation& Instance();

	void ConfigureLogin(
		const CTString& userName,
		const CTString& password);
	void ConfigureWorldSelection(
		INDEX serverIndex,
		INDEX channelIndex,
		INDEX characterIndex);
	void ConfigureCharacterHold(INDEX holdSeconds);
	void ConfigureWorldCommand(
		const CTString& command,
		INDEX delaySeconds);
	void ConfigureWorldModel(
		const CTString& model,
		INDEX delaySeconds,
		INDEX lifetimeSeconds);
	void ConfigureWorldAnchor(
		const CTString& entityClass,
		INDEX delaySeconds);
	void ConfigureWorldModelRendering(
		INDEX alpha,
		BOOL enableNormalMapSpecular);
	void ConfigureBloomTest(BOOL forceEnabled);
	void Tick();

	BOOL IsEnabled() const;
	const CTString& GetUserName() const;
	const CTString& GetPassword() const;
	INDEX GetServerIndex() const;
	INDEX GetChannelIndex() const;
	INDEX GetCharacterIndex() const;

private:
	CClientTestAutomation();
	CClientTestAutomation(const CClientTestAutomation&);
	CClientTestAutomation& operator=(const CClientTestAutomation&);

	BOOL TrySubmitLogin();
	BOOL TrySubmitServer();
	BOOL TrySubmitCharacter();
	BOOL TrySubmitWorldCommand();
	BOOL TryApplyWorldAnchor();
	BOOL TrySpawnWorldModel();
	void TryRemoveWorldModel();
	void ClearPassword();

	CTString m_userName;
	CTString m_password;
	CTString m_worldCommand;
	CTString m_worldModel;
	CTString m_worldAnchorClass;
	CPlacement3D m_worldAnchorPlacement;
	CBrushSector* m_worldAnchorSector;
	INDEX m_serverIndex;
	INDEX m_channelIndex;
	INDEX m_characterIndex;
	INDEX m_characterHoldSeconds;
	INDEX m_worldCommandDelaySeconds;
	INDEX m_worldModelDelaySeconds;
	INDEX m_worldModelLifetimeSeconds;
	INDEX m_worldAnchorDelaySeconds;
	INDEX m_worldModelAlpha;
	ULONG m_characterStageEnteredAt;
	ULONG m_gameplayStageEnteredAt;
	ULONG m_worldModelSpawnedAt;
	CEntity* m_worldModelFixture;
	BOOL m_enabled;
	BOOL m_loginSubmitted;
	BOOL m_serverSubmitted;
	BOOL m_characterSubmitted;
	BOOL m_worldCommandSubmitted;
	BOOL m_worldAnchorApplied;
	BOOL m_worldModelSpawned;
	BOOL m_worldModelNormalMapSpecular;
	BOOL m_forceBloom;
	BOOL m_bloomConfigured;
	INDEX m_lastReportedStage;
};

#endif
