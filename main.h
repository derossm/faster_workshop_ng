//==============================================================================
//
//==============================================================================
#pragma once

#include <RVA/RVA.h>

#include "Version.h"

#define PLUGIN_NAME_SHORT		"faster_workshop_ng"
#define PLUGIN_AUTHOR			"eventHandler, reg2k"

F4SE::PluginHandle g_pluginHandle{std::to_underlying(F4SE::LoadInterface::kInvalid)};

std::unordered_map<const RE::BGSKeyword*, std::vector<RE::BGSConstructibleObject*>> g_cobjMap;

using _GetKeywordByIndex = RE::BGSKeyword* (*)(RE::KeywordType a_type, std::uint16_t a_index);
using _WorkshopCanShowRecipe = bool (*)(RE::BGSConstructibleObject*, RE::BGSKeyword*);

//REL::Relocation<_GetKeywordByIndex>	GetKeywordByIndex(2206423);
//REL::Relocation<_WorkshopCanShowRecipe>	WorkshopCanShowRecipe(2194978);

RVA<_GetKeywordByIndex>		GetKeywordByIndex		("8D 41 FF 83 F8 11 77 2C"sv);
RVA<_WorkshopCanShowRecipe>	WorkshopCanShowRecipe	("48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 41 56 41 57 48 81 EC 90 00 00 00 33 FF"sv);

auto OnF4SEMessage(F4SE::MessagingInterface::Message* msg) noexcept -> void
{
	switch (msg->type)
	{
	case F4SE::MessagingInterface::kGameDataReady:
		logger::info("Clearing COBJ map.");
		g_cobjMap.clear();
		break;
	}
}

template<typename T, spdlog::level::level_enum logLevel, bool truncate = false>
inline static auto initLoggerImpl(std::string_view path = "") noexcept(false) -> void
{
	auto sink{[&]()
		{
			if constexpr (truncate)
			{
				return std::make_shared<T>(path.data(), truncate);
			}
			else
			{
				return std::make_shared<T>();
			}
		}()};

	auto log{std::make_shared<spdlog::logger>(Version::PROJECT.data(), std::move(sink))};

	log->set_level(logLevel);
	log->flush_on(logLevel);

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("[%^%l%$] %v"s);

	logger::info("{} {}.{}.{}.{}", Version::PROJECT, Version::MAJOR, Version::MINOR, Version::PATCH, Version::BUILD);
}

std::optional<std::filesystem::path> log_directory()
{
	wchar_t*                                                       buffer{ nullptr };
	const auto                                                     result = REX::W32::SHGetKnownFolderPath(REX::W32::FOLDERID_Documents, REX::W32::KF_FLAG_DEFAULT, nullptr, std::addressof(buffer));
	std::unique_ptr<wchar_t[], decltype(&REX::W32::CoTaskMemFree)> knownPath(buffer, REX::W32::CoTaskMemFree);
	if (!knownPath || result != 0) {
		//error("failed to get known folder path"sv);
		return std::nullopt;
	}

	std::filesystem::path path = knownPath.get();
	path /= std::format("My Games/Fallout4/F4SE");
	return path;
}

inline static auto InitializeLogger() noexcept -> void
{
	try
	{
#ifdef NDEBUG
		if (auto path{log_directory()}; path.has_value())
		{
			*path /= std::format("{}.log", Version::PROJECT);

			initLoggerImpl<spdlog::sinks::basic_file_sink_mt, spdlog::level::trace, true>(path.value().string());
		}
#else
		initLoggerImpl<spdlog::sinks::msvc_sink_mt, spdlog::level::trace>();
#endif
	}
	catch ([[maybe_unused]] const std::exception& ex)
	{
		// ignore exception if the logger can't be initialized
	}
}
