#include "MECHicideCharacterMovementComponent.h"
#include "MECHicideCharacter.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"


UMECHicideCharacterMovementComponent::FSavedMove_MECHicide::FSavedMove_MECHicide()
{
	Saved_bWantsToSprint = 0;
}


bool UMECHicideCharacterMovementComponent::FSavedMove_MECHicide::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const
{
	FSavedMove_MECHicide* NewMECHicideMove = static_cast<FSavedMove_MECHicide*>(NewMove.Get());

	if (Saved_bWantsToSprint != NewMECHicideMove->Saved_bWantsToSprint)
	{
		return false;
	}

	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}


void UMECHicideCharacterMovementComponent::FSavedMove_MECHicide::Clear()
{
	FSavedMove_Character::Clear();

	Saved_bWantsToSprint = 0;
}


uint8 UMECHicideCharacterMovementComponent::FSavedMove_MECHicide::GetCompressedFlags() const
{
	//return FSavedMove_Character::GetCompressedFlags(); //1330

	uint8 Result = FSavedMove_Character::GetCompressedFlags();

	if (Saved_bWantsToSprint) Result |= FLAG_Custom_0;

	return Result;
}


void UMECHicideCharacterMovementComponent::FSavedMove_MECHicide::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	UMECHicideCharacterMovementComponent* CharacterMovement = Cast<UMECHicideCharacterMovementComponent>(C->GetCharacterMovement());

	Saved_bWantsToSprint = CharacterMovement->Safe_bWantsToSprint;
}


void UMECHicideCharacterMovementComponent::FSavedMove_MECHicide::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C); //1330
	//Super::PrepMoveFor(C);

	UMECHicideCharacterMovementComponent* CharacterMovement = Cast<UMECHicideCharacterMovementComponent>(C->GetCharacterMovement());

	CharacterMovement->Safe_bWantsToSprint = Saved_bWantsToSprint;
}


UMECHicideCharacterMovementComponent::FNetworkPredictionData_Client_MECHicide::FNetworkPredictionData_Client_MECHicide(const UCharacterMovementComponent& ClientMovement) 
: Super(ClientMovement)
{
}


FSavedMovePtr UMECHicideCharacterMovementComponent::FNetworkPredictionData_Client_MECHicide::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_MECHicide());
}


FNetworkPredictionData_Client* UMECHicideCharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr)

	if (ClientPredictionData == nullptr)
	{
		UMECHicideCharacterMovementComponent* MutableThis = const_cast<UMECHicideCharacterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_MECHicide(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}


void UMECHicideCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	Safe_bWantsToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}


void UMECHicideCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	if (MovementMode == MOVE_Walking)
	{
		if (Safe_bWantsToSprint)
		{
			MaxWalkSpeed = Sprint_MaxWalkSpeed;
		}
		else
		{
			MaxWalkSpeed = Walk_MaxWalkSpeed;
		}
	}
}


void UMECHicideCharacterMovementComponent::SprintPressed()
{
	Safe_bWantsToSprint = true;
}


void UMECHicideCharacterMovementComponent::SprintReleased()
{
	Safe_bWantsToSprint = false;
}


UMECHicideCharacterMovementComponent::UMECHicideCharacterMovementComponent()
{
}


