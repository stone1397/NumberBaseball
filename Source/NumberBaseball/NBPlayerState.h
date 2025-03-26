#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NBPlayerState.generated.h"

UCLASS()
class NUMBERBASEBALL_API ANBPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	// �÷��̾� ID
	UPROPERTY(Replicated)
	FString PlayerName;
	// ���� ������ ����
	UPROPERTY(ReplicatedUsing = OnRep_IsMyTurn)
	bool bIsMyTurn;
	// �� ���� �� ȣ��Ǵ� �Լ�
	UFUNCTION()
	void OnRep_IsMyTurn();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
