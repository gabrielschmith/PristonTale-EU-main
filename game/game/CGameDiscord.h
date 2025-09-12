#pragma once

#include <memory>
#include <string>
#include <functional>
#include "discord.h"

class CGameDiscord
{
public:
	CGameDiscord();
	virtual ~CGameDiscord();

	BOOL					Init();
	void					Shutdown();
	void					Update(float fTime);

	// Rich Presence / Activity Management
	void					SetActivity(const std::string& state, const std::string& details);
	void					SetActivityMap(const std::string& mapName);
	void					SetActivityCharacter(const std::string& characterName, int level, const std::string& className);
	void					SetActivityParty(int currentSize, int maxSize, const std::string& partyId = "");
	void					SetActivityTimestamp(int64_t startTime, int64_t endTime = 0);
	void					SetActivityImages(const std::string& largeImage, const std::string& largeText, 
											 const std::string& smallImage = "", const std::string& smallText = "");
	void					SetActivitySecrets(const std::string& joinSecret = "", const std::string& spectateSecret = "");
	
	void					ClearActivity();
	void					UpdateActivity();

	// Game state callbacks
	void					OnCharacterLogin(const std::string& characterName, int level, const std::string& className);
	void					OnCharacterLogout();
	void					OnMapChange(const std::string& mapName);
	void					OnPartyChange(int currentSize, int maxSize, const std::string& partyId = "");
	void					OnGameStateChange(const std::string& state, const std::string& details = "");

	// Discord Game SDK Event Handlers
	void					OnReady();
	void					OnDisconnected(int errorCode, const char* message);
	void					OnError(int errorCode, const char* message);

	// Utility functions
	BOOL					IsConnected() const { return m_bConnected; }
	BOOL					IsInitialized() const { return m_bInitialized; }
	
	// Reset login state (for character logout)
	static void				ResetCharacterLoginFlag();
	
private:
	// Discord Game SDK Core
	std::unique_ptr<discord::Core>	m_pDiscordCore;

	// State tracking
	BOOL					m_bInitialized;
	BOOL					m_bConnected;
	float					m_fUpdateTimer;
	float					m_fUpdateInterval;
	
	// Current game state
	std::string				m_sCurrentState;
	std::string				m_sCurrentDetails;
	std::string				m_sCurrentMap;
	std::string				m_sCharacterName;
	std::string				m_sClassName;
	int						m_iCharacterLevel;
	int						m_iPartyCurrentSize;
	int						m_iPartyMaxSize;
	std::string				m_sPartyId;
	int64_t					m_iStartTimestamp;
	
	// Activity images
	std::string				m_sLargeImageKey;
	std::string				m_sLargeImageText;
	std::string				m_sSmallImageKey;
	std::string				m_sSmallImageText;

	// Internal helper methods
	void					InitializeActivity();
	void					UpdateActivityInternal();
	std::string				GetClassImageKey(const std::string& className);
	std::string				GetMapImageKey(const std::string& mapName);
	void					LogDiscordError(discord::Result result, const std::string& operation);
};

