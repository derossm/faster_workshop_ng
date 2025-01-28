//==============================================================================
//
//==============================================================================
#include "main.h"

//#include "old_gen.h"
#include "next_gen.h"

bool InitPlugin(std::uint32_t runtimeVersion = 0)
{
	logger::info("{} load", PLUGIN_NAME_SHORT);
	logger::info("runtime version: 0x{:08X}", runtimeVersion);
	logger::info("Base address: 0x{:016X}", reinterpret_cast<std::uintptr_t>(GetModuleHandle(nullptr)));

	RVAManager::UpdateAddresses(runtimeVersion);

	auto& tramp{F4SE::GetTrampoline()};

	if (runtimeVersion > F4SE::RUNTIME_1_10_163)
	{
		return InitPluginNG(tramp);
	}

	//return InitPluginOG(tramp);
	return false;
}

//== F4SE Plugin API ==//
enum class kAddressIndependence : std::uint32_t
{
	// set this if you exclusively use signature matching to find your addresses and have NO HARDCODED ADDRESSES
	// the F4SE code does not use signature matching, so calling functions in the F4SE headers is not safe with this flag
	Signatures = 1 << 0,

	// set this if you are using a 1.10.980+ version of the Address Library
	AddressLibrary_1_10_980 = 1 << 1,
};

//== F4SE Plugin API ==//
enum class kStructureIndependence : std::uint32_t
{
	// set this if your plugin doesn't use any game structures
	NoStructs = 1 << 0,

	// works with the structure layout in 1.10.980+
	Layout_1_10_980 = 1 << 1,
};

F4SE_EXPORT constinit F4SE::PluginVersionData F4SEPlugin_Version
{
	.dataVersion{F4SE::PluginVersionData::kVersion},
	.pluginVersion{REL::Version(Version::MAJOR, Version::MINOR, Version::PATCH, Version::BUILD).pack()},
	.pluginName{PLUGIN_NAME_SHORT},
	.author{PLUGIN_AUTHOR},
	.addressIndependence{std::to_underlying(kAddressIndependence::AddressLibrary_1_10_980)},
	.structureIndependence{std::to_underlying(kStructureIndependence::Layout_1_10_980)},
	.compatibleVersions{F4SE::RUNTIME_1_10_984.pack(), 0},
	.xseMinimum{0},
	.reservedNonBreaking{0},
	.reservedBreaking{0},
	.reserved{0}
};

F4SE_EXPORT auto F4SEPlugin_Query(const F4SE::LoadInterface* f4se, F4SE::PluginInfo* info) -> bool
{
	if (!f4se || !info)
	{
		return false;
	}

	if (f4se->RuntimeVersion() <= F4SE::RUNTIME_1_10_163)
	{
		MessageBoxA(nullptr, "This plugin is not compatible with your game version. This is the Next-Gen version.", Version::Name.data(), MB_OK | MB_ICONEXCLAMATION);
	}
	else
	{
		MessageBoxA(nullptr, "This is the Next-Gen version in the Query Function; this shouldn't happen.", Version::Name.data(), MB_OK | MB_ICONEXCLAMATION);
	}

	info->infoVersion	= F4SE::PluginInfo::kVersion;
	info->name			= Version::PROJECT.data();
	info->version		= REL::Version(Version::MAJOR, Version::MINOR, Version::PATCH, Version::BUILD).pack();

	return false;
}

F4SE_EXPORT auto F4SEPlugin_Load(const F4SE::LoadInterface* f4se) -> bool
{
	if (!f4se)
	{
		return false;
	}

	if (f4se->RuntimeVersion() <= F4SE::RUNTIME_1_10_163)
	{
		MessageBoxA(nullptr, "This plugin is not compatible with your game version. This is the Next-Gen version.", Version::Name.data(), MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	F4SE::Init(f4se, false);

	if (f4se->IsEditor())
	{
		// don't replace the existing log file just to tell the user that the plugin is disabled in the CK
		return false;
	}

	InitializeLogger();

	g_pluginHandle = f4se->GetPluginHandle();

	F4SE::AllocTrampoline(1 << 12);

	if (const auto *const msgInterface{F4SE::GetMessagingInterface()}; msgInterface)
	{
		msgInterface->RegisterListener(OnF4SEMessage);
	
		return InitPlugin(f4se->RuntimeVersion().pack());
	}

	std::uint32_t runtimeVersion = f4se->RuntimeVersion().pack();
	logger::info("{} load", PLUGIN_NAME_SHORT);
	logger::info("runtime version: 0x{:08X}", runtimeVersion);
	logger::info("Base address: 0x{:016X}", reinterpret_cast<std::uintptr_t>(GetModuleHandle(nullptr)));

	return true;
}
