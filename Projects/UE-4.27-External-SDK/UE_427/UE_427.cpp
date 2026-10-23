
#include "pch.h"

// Globals
#include "UE_427/Globals/Globals.h"
#include "UE_427/Globals/Offsets.h"

#include "UE_427/UE_427.h"

namespace UE {
	UE_427::UE_427(std::wstring_view process_name) :
		m_PM (
			process_name, 
			Utils::WindowsProcessManager::Access::Read | Utils::WindowsProcessManager::Access::Write
		)
	{
		m_Base = Utils::GetModuleBaseAddress(process_name, m_PM.GetProcessID());
	}

	UE_427::~UE_427() {

	}

	bool UE_427::ResolvePointer(uintptr_t start, uintptr_t& store, std::initializer_list<uintptr_t> offsets) {
		uintptr_t current = start;
		for (auto offset : offsets) {
			if (!current)
				return false;
			current = m_PM.Read<uintptr_t>(current + offset);
		}

		store = current;
		return true;
	}

	bool UE_427::UpdateLocalPlayer() {
		// reading lp pawn
		bool pawn = ResolvePointer(
			m_EngineData.OwningGameInstance,
			m_EngineData.LocalPlayer.Pawn,
			{ Offsets::LocalPlayers, 0, Offsets::PlayerController, Offsets::AcknowledgedPawn }
		);

		if (!pawn)
			return false;

		// reading lp root component
		m_EngineData.LocalPlayer.RootComponent = m_PM.Read<uintptr_t>(
			m_EngineData.LocalPlayer.Pawn + Offsets::RootComponent
		);

		if (!m_EngineData.LocalPlayer.RootComponent)
			return false;

		// reading lp chracter movement component
		m_EngineData.LocalPlayer.CharacterMovementComponent = m_PM.Read<uintptr_t>(
			m_EngineData.LocalPlayer.Pawn + Offsets::CharacterMovement
		);

		if (!m_EngineData.LocalPlayer.CharacterMovementComponent)
			return false;

		return true;
	}

	bool UE_427::UpdateActors() {
		m_EngineData.Actors.clear();

		TArray<uintptr_t> actors_array = m_PM.Read<TArray<uintptr_t>>(m_EngineData.PersistentLevel + Offsets::Actors);
		if (!actors_array.Data || actors_array.Count <= 0 || actors_array.Count > 100000)
			return false;

		m_EngineData.Actors.reserve(actors_array.Count);

		for (int i = 0; i < actors_array.Count; i++) {
			auto actor_ptr = m_PM.Read<uintptr_t>(actors_array.Data + (i * sizeof(uintptr_t)));
			if(!actor_ptr)
				continue;

			// filters only players/enemies
			auto character_movement_component = m_PM.Read<uintptr_t>(actor_ptr + Offsets::CharacterMovement);
			if (!character_movement_component)
				continue;

			auto root_component = m_PM.Read<uintptr_t>(actor_ptr + Offsets::RootComponent);
			if (!root_component)
				continue;

			PawnData data;
			data.Pawn = actor_ptr;
			data.RootComponent = root_component;
			data.CharacterMovementComponent = character_movement_component;

			m_EngineData.Actors.push_back(std::move(data));
		}

		return true;
	}

	bool UE_427::UpdateEngineData() {
		// engine ptrs
		m_EngineData.GWorld = Read<uintptr_t>(m_Base + Offsets::GWorld);
		if (!m_EngineData.GWorld)
			printf("Failed to read 'EngineData::GWorld' address.\n");

		m_EngineData.PersistentLevel = Read<uintptr_t>(m_EngineData.GWorld + Offsets::PersistentLevel);
		if (!m_EngineData.PersistentLevel)
			printf("Failed to read 'EngineData::PersistentLevel' address.\n");

		m_EngineData.OwningGameInstance = Read<uintptr_t>(m_EngineData.GWorld + Offsets::OwningGameInstance);
		if (!m_EngineData.OwningGameInstance)
			printf("Failed to read 'EngineData::PersistentLevel' address.\n");
		
		if (!ValidPointers())
			return false;

		// local player
		UpdateLocalPlayer();

		// actors
		UpdateActors();

		return true;
	}

	void UE_427::ClearCache() {
		m_EngineData.GWorld = 0;
		m_EngineData.OwningGameInstance = 0;
		m_EngineData.PersistentLevel = 0;

		m_EngineData.LocalPlayer.Clear();
		m_EngineData.Actors.clear();
	}
}