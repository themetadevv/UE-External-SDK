#pragma once

#include <Utils/Common.h>

#include "UE_427/Types.h"

namespace UE::Offsets {
	// Engine Pointers
	inline uintptr_t GWorld = 0x4A00FB0;
	inline uintptr_t GUObjectArray = 0x48B8C10;
	inline uintptr_t FNamePool = 0x487C8C0;

	// Extracted Using Dumper-7

	// UWorld 
	inline uintptr_t PersistentLevel    = 0x30;
	inline uintptr_t OwningGameInstance = 0x180;

	// ULevel
	inline uintptr_t Actors = 0x98; // class TArray<class AActor*>

	// UGameInstance
	inline uintptr_t LocalPlayers = 0x38;

	// UPlayer
	inline uintptr_t PlayerController = 0x30;

	// APlayerController
	inline uintptr_t AcknowledgedPawn    = 0x2A0;
	inline uintptr_t PlayerCameraManager = 0x2B8;

	// AActor
	inline uintptr_t RootComponent = 0x130;

	// ACharacter
	inline uintptr_t CharacterMovement = 0x288;

	// UCharacterMovementComponent
	inline uintptr_t JumpZVelocity = 0x158;
	inline uintptr_t MaxWalkSpeed  = 0x18C;
}


//// SceneComponent
//inline uintptr_t RelativeLocation = 0x11C;