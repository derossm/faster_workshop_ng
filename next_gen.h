//==============================================================================
//
//==============================================================================
#pragma once

#include "main.h"

RVA<std::uintptr_t> HookLeaf		("E8 F8 FE FD FF 84 C0 0F 84 AE 00 00 00 49 8B 47 08"sv);
RVA<std::uintptr_t> HookNonLeaf		("E8 B1 C2 FF FF 84 C0 45 0F B6 F6 45 0F 45 F4"sv);
RVA<std::uintptr_t> HookIconLoadLag	("B3 01 44 38 B6 34 03 00 00 75 36 48 8B 05 E7 56 63 02"sv);

auto TryBuildMapNG() noexcept -> void
{
	if (g_cobjMap.empty())
	{
		logger::info("Building COBJ map...");

		const auto& objectArray{RE::TESDataHandler::GetSingleton()->GetFormArray<RE::BGSConstructibleObject>()};

		for (const auto& obj : objectArray)
		{
			if (!obj)
			{
				logger::warn("Null COBJ found.");
				continue;
			}

			auto recipeFiltered{std::span{obj->filterKeywords.array, obj->filterKeywords.size}};
			for (auto& keywordValue : recipeFiltered)
			{
				if (RE::BGSKeyword* keyword{GetKeywordByIndex(RE::KeywordType::kRecipeFilter, keywordValue.keywordIndex)}; keyword)
				{
					if (WorkshopCanShowRecipe(obj, keyword))
					{
						g_cobjMap[keyword].push_back(obj);
					}
				}
			}
		}

		logger::info("Map built. {} map elements. {} cobjs processed.", g_cobjMap.size(), objectArray.size());
	}
}

auto HandlerMatches(RE::BGSConstructibleObject* obj, const RE::BGSKeyword* keyword) noexcept -> bool
{
	TryBuildMapNG();

	if (g_cobjMap.contains(keyword))
	{
		for (auto* cobj : g_cobjMap[keyword])
		{
			if (obj == cobj)
			{
				return true;
			}
		}
	}

	return false;
}

inline static auto InitPluginNG(F4SE::Trampoline& tramp) noexcept -> bool
{
	// 1: replace calls to WorkshopCanShowRecipe
	tramp.write_call<5>(HookLeaf.GetUIntPtr(), (std::uintptr_t)HandlerMatches);
	tramp.write_call<5>(HookNonLeaf.GetUIntPtr(), (std::uintptr_t)HandlerMatches);

	logger::info("Patched HookLeaf 0x{:016X}", HookLeaf.GetUIntPtr());
	logger::info("Patched HookNonLeaf 0x{:016X}", HookNonLeaf.GetUIntPtr());

	// 2: Fix icon load delay by ensuring vertical animation does not count towards preventing icon load
	std::array payload{ std::uint8_t{0x66}, std::uint8_t{0x90} };
	REL::safe_write(HookIconLoadLag.GetUIntPtr(), payload.data(), payload.size());
	logger::info("Patched HookIconLoadLag 0x{:016X}", HookIconLoadLag.GetUIntPtr());

	return true;
}
