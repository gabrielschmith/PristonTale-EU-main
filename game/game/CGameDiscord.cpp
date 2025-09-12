#include "stdafx.h"
#include "CGameDiscord.h"
#include "Discord/discord.h"
#include "Logger.h"

#define DISCORD_APPLICATION_ID 1416047082652958730

CGameDiscord::CGameDiscord()
	: m_bInitialized(FALSE)
	, m_bConnected(FALSE)
	, m_fUpdateTimer(0.0f)
	, m_fUpdateInterval(5.0f) // Update every 5 seconds
	, m_iCharacterLevel(0)
	, m_iPartyCurrentSize(0)
	, m_iPartyMaxSize(0)
	, m_iStartTimestamp(0)
{
	// Discord Core will be initialized in Init()
}

CGameDiscord::~CGameDiscord()
{
}

BOOL CGameDiscord::Init()
{
	if (m_bInitialized)
		return TRUE;

	try
	{
		// Initialize Discord Game SDK Core (following official example)
		discord::Core* core{};
		auto result = discord::Core::Create(DISCORD_APPLICATION_ID, DiscordCreateFlags_Default, &core);

		// Check result before setting the core
		if (result != discord::Result::Ok)
		{
			// Don't fail the entire application, just disable Discord integration
			LogDiscordWarn(result, "Discord Core Creation");
			
			m_bInitialized = FALSE;
			m_bConnected = FALSE;
			return TRUE; // Return TRUE to allow game to continue without Discord
		}

		m_pDiscordCore.reset(core);

		if (!m_pDiscordCore)
		{
			LogDiscordWarn(result, "Discord Core Creation - Null Pointer");

			// Don't fail the entire application, just disable Discord integration
			m_bInitialized = FALSE;
			m_bConnected = FALSE;

			return TRUE; // Return TRUE to allow game to continue without Discord
		}

		// Set up log hook (optional, for debugging)
		m_pDiscordCore->SetLogHook(discord::LogLevel::Debug, [](discord::LogLevel level, const char* message) {
			// You can add logging here if needed
			// For example: printf("Discord Log(%d): %s\n", static_cast<int>(level), message);
			});

		// Set initial timestamp
		m_iStartTimestamp = time(nullptr);

		// Initialize default activity
		InitializeActivity();

		m_bInitialized = TRUE;
		m_bConnected = TRUE;

		// Set initial game state
		OnGameStateChange("Main Menu", "Starting PristonTale EU");

		// Log successful initialization
		DEBUG("Discord integration initialized successfully");

		return TRUE;
	}
	catch (const std::exception& e)
	{
		// Log error but don't crash the application
		LOGERROR("Exception during Discord initialization: %s", e.what());
		WARN("Discord integration disabled due to exception - game will continue without Discord Rich Presence");
		
		// Reset state and allow game to continue without Discord
		m_bInitialized = FALSE;
		m_bConnected = FALSE;
		m_pDiscordCore.reset();
		
		return TRUE; // Return TRUE to allow game to continue without Discord
	}
	catch (...)
	{
		// Catch any other exceptions that might occur
		WARN("Discord integration disabled due to unknown exception - game will continue without Discord Rich Presence");
		
		// Reset state and allow game to continue without Discord
		m_bInitialized = FALSE;
		m_bConnected = FALSE;
		m_pDiscordCore.reset();
		
		return TRUE; // Return TRUE to allow game to continue without Discord
	}
}

void CGameDiscord::Shutdown()
{
	if (!m_bInitialized)
		return;

	try
	{
		// Clear activity before shutdown
		ClearActivity();

		// Reset state
		m_bInitialized = FALSE;
		m_bConnected = FALSE;
		m_pDiscordCore.reset();
	}
	catch (const std::exception& e)
	{
		// Log error if needed
		WARN("Exception during Discord Shutdown: %s", e.what());
	}
}

void CGameDiscord::Update(float fTime)
{
	if (!m_bInitialized || !m_pDiscordCore)
		return;

	try
	{
		// Run Discord callbacks
		auto result = m_pDiscordCore->RunCallbacks();

		if (result != discord::Result::Ok)
		{
			LogDiscordWarn(result, "RunCallbacks");
		}

		// Update timer
		m_fUpdateTimer += fTime;

		// Update activity at regular intervals
		if (m_fUpdateTimer >= m_fUpdateInterval * 1000.0f) // fTime is in milliseconds
		{
			UpdateActivityInternal();
			m_fUpdateTimer = 0.0f;
		}
	}
	catch (const std::exception& e)
	{
		// Log error if needed
		WARN("Exception during Discord Shutdown: %s", e.what());
	}
}

void CGameDiscord::SetActivity(const std::string& state, const std::string& details)
{
	if (!m_bInitialized)
		return;

	m_sCurrentState = state;
	m_sCurrentDetails = details;

	UpdateActivity();
}

void CGameDiscord::SetActivityMap(const std::string& mapName)
{
	if (!m_bInitialized)
		return;

	m_sCurrentMap = mapName;

	// Update details to show current map
	m_sCurrentDetails = "Exploring " + mapName;

	// Set map-specific image
	m_sLargeImageKey = GetMapImageKey(mapName);
	m_sLargeImageText = mapName;

	UpdateActivity();
}

void CGameDiscord::SetActivityCharacter(const std::string& characterName, int level, const std::string& className)
{
	if (!m_bInitialized)
		return;

	m_sCharacterName = characterName;
	m_iCharacterLevel = level;
	m_sClassName = className;

	// Update state to show character info
	char characterInfo[256];
	snprintf(characterInfo, sizeof(characterInfo), "%s | Level %d", characterName.c_str(), level);
	m_sCurrentState = characterInfo;

	// Set class-specific small image
	m_sSmallImageKey = GetClassImageKey(className);
	m_sSmallImageText = className + " - Level " + std::to_string(level);

	UpdateActivity();
}

void CGameDiscord::SetActivityParty(int currentSize, int maxSize, const std::string& partyId)
{
	if (!m_bInitialized)
		return;

	m_iPartyCurrentSize = currentSize;
	m_iPartyMaxSize = maxSize;
	m_sPartyId = partyId;

	UpdateActivity();
}

void CGameDiscord::SetActivityTimestamp(int64_t startTime, int64_t endTime)
{
	if (!m_bInitialized)
		return;

	m_iStartTimestamp = startTime;

	UpdateActivity();
}

void CGameDiscord::SetActivityImages(const std::string& largeImage, const std::string& largeText,
	const std::string& smallImage, const std::string& smallText)
{
	if (!m_bInitialized)
		return;

	m_sLargeImageKey = largeImage;
	m_sLargeImageText = largeText;
	m_sSmallImageKey = smallImage;
	m_sSmallImageText = smallText;

	UpdateActivity();
}

void CGameDiscord::SetActivitySecrets(const std::string& joinSecret, const std::string& spectateSecret)
{
	if (!m_bInitialized)
		return;

	// Store secrets for use in UpdateActivity
	// Note: We'd need to add member variables for secrets if we want to persist them
	// For now, we'll create the activity with secrets directly in UpdateActivity when needed

	UpdateActivity();
}

void CGameDiscord::ClearActivity()
{
	if (!m_bInitialized)
		return;

	// Reset activity state variables
	m_sCurrentState.clear();
	m_sCurrentDetails.clear();
	m_sLargeImageKey.clear();
	m_sLargeImageText.clear();
	m_sSmallImageKey.clear();
	m_sSmallImageText.clear();
	m_iPartyCurrentSize = 0;
	m_iPartyMaxSize = 0;
	m_sPartyId.clear();

	// Update Discord with cleared activity
	UpdateActivity();
}

void CGameDiscord::UpdateActivity()
{
	if (!m_bInitialized || !m_pDiscordCore)
		return;

	// Create a fresh activity object (following official example pattern)
	discord::Activity activity{};

	// Set basic activity information
	activity.SetState(m_sCurrentState.c_str());
	activity.SetDetails(m_sCurrentDetails.c_str());
	activity.SetType(discord::ActivityType::Playing);

	// Set timestamps
	if (m_iStartTimestamp > 0)
	{
		activity.GetTimestamps().SetStart(m_iStartTimestamp);
	}

	// Set images
	if (!m_sLargeImageKey.empty())
	{
		activity.GetAssets().SetLargeImage(m_sLargeImageKey.c_str());
		if (!m_sLargeImageText.empty())
		{
			activity.GetAssets().SetLargeText(m_sLargeImageText.c_str());
		}
	}

	if (!m_sSmallImageKey.empty())
	{
		activity.GetAssets().SetSmallImage(m_sSmallImageKey.c_str());
		if (!m_sSmallImageText.empty())
		{
			activity.GetAssets().SetSmallText(m_sSmallImageText.c_str());
		}
	}

	// Set party information if available
	if (m_iPartyCurrentSize > 0 && m_iPartyMaxSize > 0)
	{
		activity.GetParty().GetSize().SetCurrentSize(m_iPartyCurrentSize);
		activity.GetParty().GetSize().SetMaxSize(m_iPartyMaxSize);
		if (!m_sPartyId.empty())
		{
			activity.GetParty().SetId(m_sPartyId.c_str());
		}
		activity.GetParty().SetPrivacy(discord::ActivityPartyPrivacy::Public);
	}

	// Update the activity (following official example)
	m_pDiscordCore->ActivityManager().UpdateActivity(activity, [this](discord::Result result) {
		if (result == discord::Result::Ok)
		{
			// Success - activity updated
		}
		else
		{
			LogDiscordWarn(result, "UpdateActivity");
		}
		});
}

// Game state callback methods
void CGameDiscord::OnCharacterLogin(const std::string& characterName, int level, const std::string& className)
{
	SetActivityCharacter(characterName, level, className);
	OnGameStateChange("In Game", "Playing as " + characterName);
}

void CGameDiscord::OnCharacterLogout()
{
	OnGameStateChange("Character Selection", "Choosing Character");

	// Clear character-specific information
	m_sCharacterName.clear();
	m_sClassName.clear();
	m_iCharacterLevel = 0;
	m_sSmallImageKey.clear();
	m_sSmallImageText.clear();

	// Reset character login flag so next character login will be detected
	ResetCharacterLoginFlag();

	UpdateActivity();
}

void CGameDiscord::OnMapChange(const std::string& mapName)
{
	SetActivityMap(mapName);
}

void CGameDiscord::OnPartyChange(int currentSize, int maxSize, const std::string& partyId)
{
	SetActivityParty(currentSize, maxSize, partyId);
}

void CGameDiscord::OnGameStateChange(const std::string& state, const std::string& details)
{
	SetActivity(state, details);
}

// Discord Event Handlers
void CGameDiscord::OnReady()
{
	m_bConnected = TRUE;
	// Log successful connection
}

void CGameDiscord::OnDisconnected(int errorCode, const char* message)
{
	m_bConnected = FALSE;
	// Log disconnection
}

void CGameDiscord::OnError(int errorCode, const char* message)
{
	// Log error
}

// Private helper methods
void CGameDiscord::InitializeActivity()
{
	// Set default activity values
	m_sLargeImageKey = "priston_logo";
	m_sLargeImageText = "PristonTale EU";
	m_sCurrentState = "Main Menu";
	m_sCurrentDetails = "Starting Adventure";

	// Update Discord with initial activity
	UpdateActivity();
}

void CGameDiscord::UpdateActivityInternal()
{
	// This method is called periodically to refresh the activity
	// You can add game-specific logic here to update based on current game state

	UpdateActivity();
}

std::string CGameDiscord::GetClassImageKey(const std::string& className)
{
	// Map class names to Discord image keys
	// These should match the images uploaded to your Discord application

	if (className == "Fighter" || className == "fs") return "class_fighter";
	if (className == "Archer" || className == "as") return "class_archer";
	if (className == "Pikeman" || className == "ps") return "class_pikeman";
	if (className == "Atalanta" || className == "ata") return "class_atalanta";
	if (className == "Knight" || className == "ks") return "class_knight";
	if (className == "Magician" || className == "ms") return "class_magician";
	if (className == "Priestess" || className == "prs") return "class_priestess";
	if (className == "Assassin" || className == "assa") return "class_assassin";
	if (className == "Shaman" || className == "sha") return "class_shaman";

	return "class_unknown";
}

std::string CGameDiscord::GetMapImageKey(const std::string& mapName)
{
	// Map location names to Discord image keys
	// These should match the images uploaded to your Discord application

	if (mapName.find("Ricarten") != std::string::npos || mapName == "Ric") return "map_ricarten";
	if (mapName.find("Pillai") != std::string::npos || mapName == "pillay") return "map_pillai";
	if (mapName.find("Green Despair") != std::string::npos) return "map_green_despair";
	if (mapName.find("Port Lux") != std::string::npos) return "map_port_lux";
	if (mapName.find("Ruina") != std::string::npos) return "map_ruina";
	if (mapName.find("Lakeside") != std::string::npos) return "map_lakeside";
	if (mapName.find("Undead") != std::string::npos) return "map_undead";
	if (mapName.find("Mutant Forest") != std::string::npos) return "map_mutant_forest";
	if (mapName.find("Bellatra") != std::string::npos) return "map_bellatra";
	if (mapName.find("Bless Castle") != std::string::npos) return "map_bless_castle";

	return "map_unknown";
}

void CGameDiscord::LogDiscordWarn(discord::Result result, const std::string& operation)
{
	const char* errorMsg = "Unknown Error";

	switch (result)
	{
	case discord::Result::ServiceUnavailable: errorMsg = "Service Unavailable"; break;
	case discord::Result::InvalidVersion: errorMsg = "Invalid Version"; break;
	case discord::Result::LockFailed: errorMsg = "Lock Failed"; break;
	case discord::Result::InternalError: errorMsg = "Internal Error"; break;
	case discord::Result::InvalidPayload: errorMsg = "Invalid Payload"; break;
	case discord::Result::InvalidCommand: errorMsg = "Invalid Command"; break;
	case discord::Result::InvalidPermissions: errorMsg = "Invalid Permissions"; break;
	case discord::Result::NotAuthenticated: errorMsg = "Not Authenticated"; break;
	case discord::Result::NotRunning: errorMsg = "Discord Not Running"; break;
	case discord::Result::NotInstalled: errorMsg = "Discord Not Installed"; break;
	case discord::Result::OAuth2Error: errorMsg = "OAuth2 Error"; break;
	case discord::Result::InvalidAccessToken: errorMsg = "Invalid Access Token"; break;
	case discord::Result::ApplicationMismatch: errorMsg = "Application Mismatch"; break;
	case discord::Result::RateLimited: errorMsg = "Rate Limited"; break;
	case discord::Result::PurchaseCanceled: errorMsg = "Purchase Canceled"; break;
	case discord::Result::InvalidGuild: errorMsg = "Invalid Guild"; break;
	case discord::Result::InvalidEvent: errorMsg = "Invalid Event"; break;
	case discord::Result::InvalidChannel: errorMsg = "Invalid Channel"; break;
	case discord::Result::InvalidOrigin: errorMsg = "Invalid Origin"; break;
	case discord::Result::NotFetched: errorMsg = "Not Fetched"; break;
	case discord::Result::NotFound: errorMsg = "Not Found"; break;
	case discord::Result::Conflict: errorMsg = "Conflict"; break;
	default: break;
	}

	// Use the game's logging system
	WARN("Discord %s failed: %s (Code: %d)", operation.c_str(), errorMsg, static_cast<int>(result));
}

// Static function to reset character login flag (defined in CharacterGame.cpp)
extern void ResetDiscordCharacterLoginFlag();

void CGameDiscord::ResetCharacterLoginFlag()
{
	ResetDiscordCharacterLoginFlag();
}
