#pragma once

#include <string>
#include <vector>

// Externals
#include <Utils/Process/WindowsProcessManager.h>

// Types
#include "UE_427/Structs/FVector.h"
#include "UE_427/Structs/TArray.h"

namespace UE {
	using UnrealOffset = uintptr_t;

	class UE_427 {
	private:
		Utils::WindowsProcessManager m_PM;
		uintptr_t m_Base = 0;

		struct PawnData {
			uintptr_t Pawn = 0;

			// evaluated using pawn during runtime
			uintptr_t RootComponent = 0;
			uintptr_t CharacterMovementComponent = 0;

			void Clear() {
				Pawn = 0;
				RootComponent = 0;
				CharacterMovementComponent = 0;
			}
		};

		struct EngineData {
			uintptr_t GWorld = 0;
			uintptr_t OwningGameInstance = 0;
			uintptr_t PersistentLevel = 0;

			PawnData LocalPlayer;
			std::vector<PawnData> Actors;
		};

		EngineData m_EngineData;

		bool ValidPointers() const {
			return m_EngineData.GWorld != 0 
				&& m_EngineData.OwningGameInstance != 0 
				&& m_EngineData.PersistentLevel != 0;
		}

	public:
		// constructors
		explicit UE_427(std::wstring_view process_name);
		~UE_427();

		UE_427(const UE_427&) = delete;
		UE_427& operator=(const UE_427&) = delete;

	private:
		// private functions
		bool ResolvePointer(uintptr_t start, uintptr_t& store, std::initializer_list<uintptr_t> offsets);

		template <typename T>
		bool ResolvePointerT(uintptr_t start, T& store, std::initializer_list<uintptr_t> offsets) {
			uintptr_t current = start;
			
			auto it  = offsets.begin();
			auto end = offsets.end();

			if (it == end)
				return false;

			while (!(it == end - 1)) {
				if (!current)
					return false;

				current = m_PM.Read<uintptr_t>(current + *it);
				++it; // increases the count of offsets in the list ignoring the last one
			}

			if (!current)
				return false;

			// reads the last offset in the list to return a valid usable type
			store = m_PM.Read<T>(current + *it); 
			return true;
		}

		bool UpdateLocalPlayer();
		bool UpdateActors();

	public:
		// methods
		template <typename T>
		T Read(uintptr_t address) {
			return m_PM.Read<T>(address);
		}

		template <typename T>
		bool Write(uintptr_t address, const T& value) {
			if (!m_PM.Write<T>(address, value))
				return false;

			return true;
		}

		bool UpdateEngineData();
		void ClearCache();

	public:
		// getters
		const PawnData& GetLocalPlayerData() const { return m_EngineData.LocalPlayer; }
		const std::vector<PawnData>& GetActors() const { return m_EngineData.Actors; }
	};
}