#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NBGameState.generated.h"


UCLASS()
class NUMBERBASEBALL_API ANBGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	// 현재 게임이 시작되었는지
	UPROPERTY(ReplicatedUsing = OnRep_CurrentTurn)
	bool bGameStarted;
	// 현재 턴인 플레이어의 아이디
	UPROPERTY(ReplicatedUsing = OnRep_CurrentTurn)
	FString CurrentTurnPlayerID;
	// 현재 진행 중인 턴 수
	UPROPERTY(Replicated)
	int32 TurnCount;
	// 마지막 추론
	UPROPERTY(ReplicatedUsing = OnRep_LatestFeedback)
	FString LatestGuess;

	UPROPERTY(ReplicatedUsing = OnRep_LatestFeedback)
	int32 LatestBall;

	UPROPERTY(ReplicatedUsing = OnRep_LatestFeedback)
	int32 LatestStrike;
	//남은 시간
	UPROPERTY(ReplicatedUsing = OnRep_RemainingTime)
	float RemainingTurnTime;
	
	UFUNCTION()
	void OnRep_RemainingTime();
	UFUNCTION()
	void OnRep_GameStarted();
	UFUNCTION()
	void OnRep_CurrentTurn();
	UFUNCTION()
	void OnRep_LatestFeedback();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
