#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NBPlayerState.generated.h"

UCLASS()
class NUMBERBASEBALL_API ANBPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	// 플레이어 ID
	UPROPERTY(Replicated)
	FString PlayerName;
	// 현재 턴인지 여부
	UPROPERTY(ReplicatedUsing = OnRep_IsMyTurn)
	bool bIsMyTurn;
	// 턴 변경 시 호출되는 함수
	UFUNCTION()
	void OnRep_IsMyTurn();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
