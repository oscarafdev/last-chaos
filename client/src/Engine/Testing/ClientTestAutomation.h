#ifndef SE_INCL_CLIENTTESTAUTOMATION_H
#define SE_INCL_CLIENTTESTAUTOMATION_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <Engine/Base/CTString.h>

// Automatización opt-in para pruebas integrales del cliente. No se activa
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
	void ClearPassword();

	CTString m_userName;
	CTString m_password;
	INDEX m_serverIndex;
	INDEX m_channelIndex;
	INDEX m_characterIndex;
	BOOL m_enabled;
	BOOL m_loginSubmitted;
	BOOL m_serverSubmitted;
	BOOL m_characterSubmitted;
	INDEX m_lastReportedStage;
};

#endif
