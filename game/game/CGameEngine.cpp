#include "stdafx.h"
#include "CGameEngine.h"

bool DetectCheatEngineByHWND();
uint64_t DetectCooldown = 0;
extern bool g_CheatEngineDetected;

CGameEngine::CGameEngine()
{
	pcGameGraphic = new CGameGraphic();
	pcGameAudio = new CGameAudio();
}

CGameEngine::~CGameEngine()
{
	SAFE_DELETE(pcGameGraphic);
	SAFE_DELETE(pcGameAudio);
}

BOOL CGameEngine::Init()
{
	INIT(pcGameGraphic);
	INIT(pcGameAudio);
	return TRUE;
}

void CGameEngine::Shutdown()
{
	SHUTDOWN(pcGameGraphic);
	SHUTDOWN(pcGameAudio);
}

void CGameEngine::Frame()
{
	pcGameGraphic->Frame();
}

void CGameEngine::Update(float fTime)
{
	pcGameGraphic->Update(fTime);
	pcGameAudio->Update(fTime);

	if (time(nullptr) > DetectCooldown)
	{
		if (DetectCheatEngineByHWND())
		{
			g_CheatEngineDetected = true;
		}

		DetectCooldown = time(nullptr) + 15; //check every 15s 
	}

	SOCKETGAME->PingConnections(true);
}

void CGameEngine::Render3D()
{
	pcGameGraphic->Render3D();
}

void CGameEngine::Render2D()
{
	pcGameGraphic->Render2D();
}

#define IMAGE_DIRECTORY_ENTRY_EXPORT            0   // Export Directory
#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES        16

namespace Structures
{
	struct PEB
	{
		uint8_t         InheritedAddressSpace;
		uint8_t         ReadImageFileExecOptions;
		uint8_t         BeingDebugged;

		union
		{
			uint8_t     BitField;
			struct
			{
				uint8_t ImageUsesLargePages : 1;
				uint8_t IsProtectedProcess : 1;
				uint8_t IsImageDynamicallyRelocated : 1;
				uint8_t SkipPatchingUser32Forwarders : 1;
				uint8_t IsPackagedProcess : 1;
				uint8_t IsAppContainer : 1;
				uint8_t IsProtectedProcessLight : 1;
				uint8_t SpareBits : 1;
			};
		};

		uintptr_t       Mutant;
		uintptr_t       ImageBaseAddress;
		uintptr_t       Ldr;
	};

	struct UNICODE_STRING
	{
		uint16_t  Length;
		uint16_t  MaximumLength;
		wchar_t* Buffer;
	};

	struct cLIST_ENTRY
	{
		uintptr_t Flink;
		uintptr_t Blink;
	};

	struct LdrData
	{
		uint32_t        Length;
		uint8_t         Initialized;
		uintptr_t       SsHandle;
		cLIST_ENTRY     InLoadOrderModuleList;
		cLIST_ENTRY     InMemoryOrderModuleList;
		cLIST_ENTRY     InInitializationOrderModuleList;
		uintptr_t       EntryInProgress;
		uint8_t         ShutdownInProgress;
		uintptr_t       ShutdownThreadId;
	};

	struct LdrDataTableEntry
	{
		cLIST_ENTRY     InLoadOrderLinks;
		cLIST_ENTRY     InMemoryOrderLinks;
		cLIST_ENTRY     InInitializationOrderLinks;
		uintptr_t       DllBaseAddress;
		uintptr_t       EntryPoint;
		uint32_t        SizeOfImage;
		UNICODE_STRING  FullDllName;
		UNICODE_STRING  BaseDllName;
		uint32_t        Flags;
		uint16_t        LoadCount;
		uint16_t        TlsIndex;
		cLIST_ENTRY     HashLinks;
		uint32_t        TimeDateStamp;
		uintptr_t       EntryPointActivationContext;
		uintptr_t       PatchInformation;
	};

	struct IMAGE_DOS_HEADER
	{
		uint16_t   e_magic;                     // Magic number
		uint16_t   e_cblp;                      // Bytes on last page of file
		uint16_t   e_cp;                        // Pages in file
		uint16_t   e_crlc;                      // Relocations
		uint16_t   e_cparhdr;                   // Size of header in paragraphs
		uint16_t   e_minalloc;                  // Minimum extra paragraphs needed
		uint16_t   e_maxalloc;                  // Maximum extra paragraphs needed
		uint16_t   e_ss;                        // Initial (relative) SS value
		uint16_t   e_sp;                        // Initial SP value
		uint16_t   e_csum;                      // Checksum
		uint16_t   e_ip;                        // Initial IP value
		uint16_t   e_cs;                        // Initial (relative) CS value
		uint16_t   e_lfarlc;                    // File address of relocation table
		uint16_t   e_ovno;                      // Overlay number
		uint16_t   e_res[4];                    // Reserved uint16_ts
		uint16_t   e_oemid;                     // OEM identifier (for e_oeminfo)
		uint16_t   e_oeminfo;                   // OEM information; e_oemid specific
		uint16_t   e_res2[10];                  // Reserved uint16_ts
		uint32_t   e_lfanew;                    // File address of new exe header
	};

	struct IMAGE_DATA_DIRECTORY
	{
		uint32_t   VirtualAddress;
		uint32_t   Size;
	};

	struct IMAGE_OPTIONAL_HEADER
	{
		struct _BaseType32
		{
			uint32_t   BaseOfCode;
			uint32_t   BaseOfData;
		};
		using _BaseType = _BaseType32;

		uint16_t                Magic;
		uint8_t                 MajorLinkerVersion;
		uint8_t                 MinorLinkerVersion;
		uint32_t                SizeOfCode;
		uint32_t                SizeOfInitializedData;
		uint32_t                SizeOfUninitializedData;
		uint32_t                AddressOfEntryPoint;
		_BaseType               BaseOfCode;
		uintptr_t               ImageBase;
		uint32_t                SectionAlignment;
		uint32_t                FileAlignment;
		uint16_t                MajorOperatingSystemVersion;
		uint16_t                MinorOperatingSystemVersion;
		uint16_t                MajorImageVersion;
		uint16_t                MinorImageVersion;
		uint16_t                MajorSubsystemVersion;
		uint16_t                MinorSubsystemVersion;
		uint32_t                Win32VersionValue;
		uint32_t                SizeOfImage;
		uint32_t                SizeOfHeaders;
		uint32_t                CheckSum;
		uint16_t                Subsystem;
		uint16_t                DllCharacteristics;
		uintptr_t               SizeOfStackReserve;
		uintptr_t               SizeOfStackCommit;
		uintptr_t               SizeOfHeapReserve;
		uintptr_t               SizeOfHeapCommit;
		uint32_t                LoaderFlags;
		uint32_t                NumberOfRvaAndSizes;
		IMAGE_DATA_DIRECTORY    DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
	};

	struct IMAGE_FILE_HEADER
	{
		uint16_t    Machine;
		uint16_t    NumberOfSections;
		uint32_t    TimeDateStamp;
		uint32_t    PointerToSymbolTable;
		uint32_t    NumberOfSymbols;
		uint16_t    SizeOfOptionalHeader;
		uint16_t    Characteristics;
	};

	struct IMAGE_NT_HEADERS
	{
		uint32_t                Signature;
		IMAGE_FILE_HEADER       FileHeader;
		IMAGE_OPTIONAL_HEADER   OptionalHeader;
	};

	struct IMAGE_EXPORT_DIRECTORY
	{
		uint32_t    Characteristics;
		uint32_t    TimeDateStamp;
		uint16_t    MajorVersion;
		uint16_t    MinorVersion;
		uint32_t    Name;
		uint32_t    Base;
		uint32_t    NumberOfFunctions;
		uint32_t    NumberOfNames;
		uint32_t    AddressOfFunctions;     ///< RVA from base of image
		uint32_t    AddressOfNames;         ///< RVA from base of image
		uint32_t    AddressOfNameOrdinals;  ///< RVA from base of image
	};
}
class Structure
{
public:
	/// Get field offset
	/// @p_Field : Field pointer
	template<typename t_Class, typename t_FType> static constexpr size_t OffsetOf(t_FType t_Class::* p_Field)
	{
		return reinterpret_cast<size_t>(&(((t_Class*)(nullptr))->*p_Field));
	}

};

using tfn_IsWindow = decltype(IsWindow);
using tfn_GetWindowTextA = decltype(GetWindowTextA);
using tfn_GetClassNameA = decltype(GetClassNameA);
using tfn_GetWindow = decltype(GetWindow);
using tfn_EnumWindows = decltype(EnumWindows);

struct Context
{
	tfn_IsWindow* fn_IsWindow;
	tfn_GetWindowTextA* fn_GetWindowTextA;
	tfn_GetClassNameA* fn_GetClassNameA;
	tfn_GetWindow* fn_GetWindow;
	tfn_EnumWindows* fn_EnumWindows;
	bool                          CheatEngineFound;
};


__forceinline size_t StringTools_Length(const char* p_String)
{
	const char* l_Ptr = p_String;
	size_t l_Length = 0;

	while (*l_Ptr)
	{
		l_Ptr++;
		l_Length++;
	}

	return l_Length;
}
__forceinline size_t GetChilds(HWND p_Parent, HWND* p_Buffer, size_t p_Max, Context* p_Context)
{
	size_t l_Count = 0;

	HWND  hChildWnd = p_Context->fn_GetWindow(p_Parent, GW_CHILD);
	if (p_Context->fn_IsWindow(hChildWnd))
	{
		p_Buffer[l_Count] = hChildWnd;
		l_Count++;

		while (hChildWnd && l_Count < p_Max)
		{
			hChildWnd = p_Context->fn_GetWindow(hChildWnd, GW_HWNDNEXT);
			if (p_Context->fn_IsWindow(hChildWnd))
			{
				p_Buffer[l_Count] = hChildWnd;
				l_Count++;
			}
		}
	}

	return l_Count;
}
__forceinline char* GetWindowTitle(HWND p_HWND, char* p_Buffer, size_t p_MaxSize, Context* p_Context)
{
	memset(p_Buffer, 0, p_MaxSize);
	p_Context->fn_GetWindowTextA(p_HWND, p_Buffer, p_MaxSize);
	return p_Buffer;
}

bool DetectCheatEngineByHWND()
{

	Context l_Context;
	l_Context.CheatEngineFound = false;

	auto l_PEB = (Structures::PEB*)__readfsdword(0x30);
	auto l_Ldr = reinterpret_cast<Structures::LdrData*>(l_PEB->Ldr);
	auto l_JumpOffset = Structure::OffsetOf(&Structures::LdrDataTableEntry::InMemoryOrderLinks);
	auto l_FirstEntryPtr = l_Ldr->InMemoryOrderModuleList.Flink - l_JumpOffset;
	auto l_CurrentEntryPtr = l_FirstEntryPtr;

	do
	{
		auto l_CurrentEntry = reinterpret_cast<Structures::LdrDataTableEntry*>(l_CurrentEntryPtr);

		/// Skip the last null module
		if (!l_CurrentEntry->DllBaseAddress)
			break;

		const wchar_t* l_CurrentModuleName = reinterpret_cast<const wchar_t*>(l_CurrentEntry->BaseDllName.Buffer);

		bool l_IsUser32 = towlower(l_CurrentModuleName[0]) == L'u'
			&& towlower(l_CurrentModuleName[1]) == L's'
			&& towlower(l_CurrentModuleName[2]) == L'e'
			&& towlower(l_CurrentModuleName[3]) == L'r'
			&& towlower(l_CurrentModuleName[4]) == L'3'
			&& towlower(l_CurrentModuleName[5]) == L'2';

		if (l_IsUser32)
		{
			auto l_DOSHeader = reinterpret_cast<Structures::IMAGE_DOS_HEADER*>(l_CurrentEntry->DllBaseAddress);
			auto l_NTHeader = reinterpret_cast<Structures::IMAGE_NT_HEADERS*>(l_CurrentEntry->DllBaseAddress + l_DOSHeader->e_lfanew);
			auto l_ExportAddr = l_NTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;

			if (l_ExportAddr)
			{
				auto l_ExportDirectory = reinterpret_cast<Structures::IMAGE_EXPORT_DIRECTORY*>(l_NTHeader->OptionalHeader.ImageBase + l_ExportAddr);

				if (l_ExportDirectory->NumberOfFunctions < 1)
					break;

				const uint32_t* l_NamesAddr = reinterpret_cast<uint32_t*>(l_NTHeader->OptionalHeader.ImageBase + l_ExportDirectory->AddressOfNames);
				const uint32_t* l_FunctAddr = reinterpret_cast<uint32_t*>(l_NTHeader->OptionalHeader.ImageBase + l_ExportDirectory->AddressOfFunctions);
				const uint16_t* l_OrdinAddr = reinterpret_cast<uint16_t*>(l_NTHeader->OptionalHeader.ImageBase + l_ExportDirectory->AddressOfNameOrdinals);

				for (size_t l_I = 0; l_I < l_ExportDirectory->NumberOfNames; ++l_I)
				{
					auto l_Ordinal = l_OrdinAddr[l_I];
					auto l_StringStartAddr = l_NTHeader->OptionalHeader.ImageBase + l_NamesAddr[l_I];
					auto l_FunctionAddress = l_NTHeader->OptionalHeader.ImageBase + l_FunctAddr[l_Ordinal];

					const char* l_ExportName = reinterpret_cast<const char*>(l_StringStartAddr);
					size_t       l_ExportNameLength = StringTools_Length(l_ExportName);

					if (l_ExportNameLength == 8 && l_ExportName[0] == 'I' && l_ExportName[1] == 's' && l_ExportName[7] == 'w')
					{
						l_Context.fn_IsWindow = (tfn_IsWindow*)(l_FunctionAddress);
						//printf("IsWindow %d - %s\n",l_I, l_ExportName);
					}
					if (l_ExportNameLength == 14 && l_ExportName[0] == 'G' && l_ExportName[1] == 'e' && l_ExportName[9] == 'T' && l_ExportName[13] == 'A')
					{
						l_Context.fn_GetWindowTextA = (tfn_GetWindowTextA*)(l_FunctionAddress);
						//printf("GetWindowTextA %d - %s\n", l_I, l_ExportName);
					}
					if (l_ExportNameLength == 13 && l_ExportName[0] == 'G' && l_ExportName[1] == 'e' && l_ExportName[8] == 'N' && l_ExportName[12] == 'A')
					{
						l_Context.fn_GetClassNameA = (tfn_GetClassNameA*)(l_FunctionAddress);
						//printf("GetClassNameA %d - %s\n", l_I, l_ExportName);
					}
					if (l_ExportNameLength == 9 && l_ExportName[0] == 'G' && l_ExportName[1] == 'e' && l_ExportName[7] == 'o' && l_ExportName[8] == 'w')
					{
						l_Context.fn_GetWindow = (tfn_GetWindow*)(l_FunctionAddress);
						//printf("GetWindow %d - %s\n", l_I, l_ExportName);
					}
					if (l_ExportNameLength == 11 && l_ExportName[0] == 'E' && l_ExportName[1] == 'n' && l_ExportName[9] == 'w' && l_ExportName[10] == 's')
					{
						l_Context.fn_EnumWindows = (tfn_EnumWindows*)(l_FunctionAddress);
						//printf("EnumWindows %d - %s\n", l_I, l_ExportName);
					}

				}
			}
		}

		l_CurrentEntryPtr = l_CurrentEntry->InMemoryOrderLinks.Flink - l_JumpOffset;
	} while (l_CurrentEntryPtr != 0 && l_CurrentEntryPtr != l_FirstEntryPtr);


	l_Context.fn_EnumWindows([](HWND p_HWND, LPARAM p_LParam) {
		Context* l_Context = (Context*)p_LParam;
		const int CHILD_BUFFER_SIZE = 100;

		char l_StringBuffer[MAX_PATH];
		HWND l_ChildBuffer[CHILD_BUFFER_SIZE];

		l_Context->fn_GetClassNameA(p_HWND, l_StringBuffer, _countof(l_StringBuffer));

		/// "Window"
		if ((*((uint32_t*)(l_StringBuffer + 0)) == 0x646e6957 && *((uint16_t*)(l_StringBuffer + 4)) == 0x776F)
			|| GetChilds(p_HWND, l_ChildBuffer, CHILD_BUFFER_SIZE, l_Context) == 4)
		{
			GetWindowTitle(p_HWND, l_StringBuffer, MAX_PATH, l_Context);

			/// "Cheat engine"
			if (*((uint32_t*)(l_StringBuffer + 0)) == 0x61656843
				&& *((uint32_t*)(l_StringBuffer + 4)) == 0x6E452074
				&& *((uint32_t*)(l_StringBuffer + 8)) == 0x656e6967)
				l_Context->CheatEngineFound = true;
			else
			{
				auto l_ChildCount = GetChilds(p_HWND, l_ChildBuffer, CHILD_BUFFER_SIZE, l_Context);

				if (l_ChildCount == 4)
				{
					l_ChildCount = GetChilds(l_ChildBuffer[0], l_ChildBuffer, CHILD_BUFFER_SIZE, l_Context);
					if (l_ChildCount > 10)
					{
						for (unsigned int l_ChildI = 0; l_ChildI < l_ChildCount; ++l_ChildI)
						{
							GetWindowTitle(l_ChildBuffer[l_ChildI], l_StringBuffer, MAX_PATH, l_Context);

							for (unsigned int l_CharI = 0; l_CharI < StringTools_Length(l_StringBuffer); ++l_CharI)
							{
								l_StringBuffer[l_CharI] = (l_StringBuffer[l_CharI] >= 'A' && l_StringBuffer[l_CharI] <= 'Z') ? l_StringBuffer[l_CharI] - 'A' + 'a' : l_StringBuffer[l_CharI];
							}

							/// "Memory view"
							if (*((uint32_t*)(l_StringBuffer + 0)) == 0x6f6d656d
								&& *((uint32_t*)(l_StringBuffer + 4)) == 0x76207972
								&& *((uint16_t*)(l_StringBuffer + 8)) == 0x6569
								&& *((uint8_t*)(l_StringBuffer + 10)) == 0x77)
								l_Context->CheatEngineFound = true;
						}
					}
				}
			}
		}

		return TRUE;
		}, (LPARAM)&l_Context);

	return l_Context.CheatEngineFound;
}
