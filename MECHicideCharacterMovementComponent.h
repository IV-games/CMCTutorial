#pragma once

#include "CoreMinimal.h"
#include "MECHicide.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MECHicideCharacterMovementComponent.generated.h"

UCLASS()
class MECHICIDE_API UMECHicideCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class FSavedMove_MECHicide : public FSavedMove_Character
	{
	public:

		//typedef FSavedMove_Character Super;
		
		uint8 Saved_bWantsToSprint:1;
		
		FSavedMove_MECHicide();

		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};

	class FNetworkPredictionData_Client_MECHicide : public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionData_Client_MECHicide(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;

		virtual FSavedMovePtr AllocateNewMove() override;
	};

	UPROPERTY(EditDefaultsOnly) float Sprint_MaxWalkSpeed;
	UPROPERTY(EditDefaultsOnly) float Walk_MaxWalkSpeed;

	bool Safe_bWantsToSprint;

public:
	UMECHicideCharacterMovementComponent();
public:
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
protected:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

public:
	UFUNCTION(BlueprintCallable) void SprintPressed();
	UFUNCTION(BlueprintCallable) void SprintReleased();


};
