#include "NBPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "NBPlayerController.h"
#include "NBGameUI.h"

void ANBPlayerState::OnRep_IsMyTurn()
{
	// Ŭ���̾�Ʈ���� UI ������Ʈ
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (PC)
	{
		ANBPlayerController* NBPC = Cast<ANBPlayerController>(PC);
		if (NBPC && NBPC->NBGameUI)
		{
			NBPC->NBGameUI->UpdateTurn(NBPC);
		}
	}
}

void ANBPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANBPlayerState, PlayerName);
	DOREPLIFETIME(ANBPlayerState, bIsMyTurn);
}
