#include "NBGameState.h"
#include "Net/UnrealNetwork.h"
#include "NBGameUI.h"
#include "NBPlayerController.h"
#include "NBPlayerState.h"

void ANBGameState::OnRep_RemainingTime()
{
	// 모든 로컬 플레이어의 UI 업데이트
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ANBPlayerController* PC = Cast<ANBPlayerController>(It->Get());
		if (PC && PC->NBGameUI)
		{
			PC->NBGameUI->UpdateTimerProgress(RemainingTurnTime);
		}
	}
}

void ANBGameState::OnRep_GameStarted()
{
	UE_LOG(LogTemp, Log, TEXT("게임 상태 변경! bGameStarted : %s"), bGameStarted ? TEXT("TRUE") : TEXT("FALSE"));
	
	// 모든 로컬 플레이어의 UI 업데이트
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ANBPlayerController* PC = Cast<ANBPlayerController>(It->Get());
		if (PC && PC->NBGameUI)
		{
			PC->NBGameUI->UpdateUIForGameStart(bGameStarted);
		}
	}
}

void ANBGameState::OnRep_CurrentTurn()
{
	UE_LOG(LogTemp, Log, TEXT("턴 변경! 현재 턴 플레이어: %s"), *CurrentTurnPlayerID);

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ANBPlayerController* PC = Cast<ANBPlayerController>(It->Get());
		if (PC)
		{
			ANBPlayerState* PS = PC->GetPlayerState<ANBPlayerState>();
			if (PS)
			{
				PS->bIsMyTurn = (PS->PlayerName == CurrentTurnPlayerID);
			}

			if (PC->NBGameUI)
			{
				PC->NBGameUI->UpdateTurn(PC);
			}
		}
	}
}

void ANBGameState::OnRep_LatestFeedback()
{
	UE_LOG(LogTemp, Log, TEXT("모든 클라이언트에서 피드백 업데이트 입력: %s | Ball: %d | Strike: %d"), *LatestGuess, LatestBall, LatestStrike);

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ANBPlayerController* PC = Cast<ANBPlayerController>(It->Get());
		if (PC && PC->NBGameUI)
		{
			PC->NBGameUI->ShowBallStrikeResult(LatestGuess, LatestBall, LatestStrike);
		}
	}
}

void ANBGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANBGameState, bGameStarted);
	DOREPLIFETIME(ANBGameState, CurrentTurnPlayerID);
	DOREPLIFETIME(ANBGameState, TurnCount);

	DOREPLIFETIME(ANBGameState, LatestGuess);
	DOREPLIFETIME(ANBGameState, LatestBall);
	DOREPLIFETIME(ANBGameState, LatestStrike);
	DOREPLIFETIME(ANBGameState, RemainingTurnTime);
}

