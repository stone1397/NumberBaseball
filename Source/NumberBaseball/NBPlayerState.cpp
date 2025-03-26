#include "NBPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "NBPlayerController.h"
#include "NBGameUI.h"

void ANBPlayerState::OnRep_IsMyTurn()
{
	// 클라이언트에서 UI 업데이트
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
