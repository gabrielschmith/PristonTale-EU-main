

#define APPLICATION_ID "614843823750774816" // API KEY DISCORD

namespace DiscordAPI
{
	// struct Nations
	struct NationStruct
	{
		std::string nation_name;
	};

	// List Nations
	const NationStruct NationsNames[] =
	{
		{ "capella" },
		{ "procyon" },
		{ "gm"}
	};

	// struct Class
	struct ClassStruct
	{
		std::string class_name;
	};

	// List Class
	const ClassStruct ClassNames[] =
	{
		{ "NA" },  // NA
		{ "as" }, // DU
		{ "fs" }, // MA
		{ "ms" }, // EA
		{ "prs" }, // GU
		{ "ks" }, // AA
		{ "assa" },  // GA
		{ "mgs" },
		{ "sha" },
		{ "ata" },
		{ "ps" }
	};

	// List Maps teste 2 cyt
	const std::string MapNames[] =
	{
		{ "Ric"},
		{ "pillay" },
		{ "Green Despair" },
		{ "Port Lux" },
		{ "Fort. Ruina" },
		{ "Lakeside" },
		{ "Undead Grounds" },
		{ "Forgotten Ruin" },
		{ "Mutant Forest" },
		{ "Pontus Ferrum" },
		{ "Forte Infernus" },
		{ "Vestigio Arcano" },
		{ "Frozen Tower of Undead" },
		{ "Ruina Station" },
		{ "Tierra Gloriosa" },
		{ "Tierra Gloriosa[Espera]" },
		{ "Ilha Desperta" },
		{ "T2 Desperto" },
		{ "Prisao" },
		{ "Arena Da Defesa" },
		{ "Senellinia" },
		{ "Dragona 3ss" },
		{ "Templo Esquecido 1SS" },
		{ "Cidade Vulcanica" },
		{ "Patren" },
		{ "Lago do Crepusculo" },
		{ "Dungeon" },
		{ "Dungeon" },
		{ "Dungeon" },
		{ "Central de Teleporte" },
		{ "Dungeon" },
		{ "Dungeon" },
		{ "Dungeon" },
		{ "Templo 2 ss" },
		{ "Ilha perdida" },
		{ "Siena 1 ss" },
		{ "Siena 2 ss" },
		{ "Dungeon" },
		{ "Dungeon" },
		{ "Dungeon" },
		{ "Dungeon" },
		{ "Dungeon" },
		{ "Dungeon" },
		{ "Dungeon" },
		{ "Dungeon" },
		{ "Dungeon" },
		{ "Castelo 1" },
		{ "Dungeon" },
		{ "Dungeon" },
		{ "Dungeon" },
		{ "Castelo 2" },
		{ "Dungeon" },
		{ "Dungeon" },
		{ "Sem nome" },
		{ "Sem nome" },
		{ "Sem nome" },
		{ "Sem nome" },
		{ "Sem nome" }
	};

	extern HMODULE module;

	namespace Functions
	{
		void Initialize();
		void Shutdown();
		void PluginThread();
	}
}


