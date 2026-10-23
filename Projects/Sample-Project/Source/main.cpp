
#include <UE_427/UE_427.h>
#include <UE_427/Globals/Offsets.h>

int main() {
	UE::UE_427 TestGame(L"UnrealTestProject-Win64-Shipping.exe");

	const auto& lp = TestGame.GetLocalPlayerData();

	while (true) {
		if (!TestGame.UpdateEngineData())
			break;

		auto actors = TestGame.GetActors();
		printf("Total Actors Found: %zu\n", actors.size());

		for (const auto& actor : actors) {

			float max_speed = TestGame.Read<float>(
				actor.CharacterMovementComponent + UE::Offsets::MaxWalkSpeed
			);

			TestGame.Write<float>(actor.CharacterMovementComponent + UE::Offsets::MaxWalkSpeed, 20000.0f);
		}

		/*float max_speed = TestGame.Read<float>(
			lp.CharacterMovementComponent + UE::Offsets::MaxWalkSpeed
		);

		printf("Max Speed: %f\n", max_speed);

		TestGame.Write<float>(lp.CharacterMovementComponent + UE::Offsets::MaxWalkSpeed, 20000.0f);*/

		Sleep(16);
	}
}