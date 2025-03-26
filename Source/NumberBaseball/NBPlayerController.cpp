#include "NBPlayerController.h"
#include "NBGameMode.h"
#include "NBGameUI.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"

// 숫자 입력을 서버로 전송 (클라 -> 서버)
void ANBPlayerController::SendGuessToServer_Implementation(const FString& Guess)
{
	// 서버에서 실행됨 (GameMode에서 처리)
	if (HasAuthority())
	{
		ANBGameMode* GameMode = GetWorld()->GetAuthGameMode<ANBGameMode>();
		if (GameMode)
		{
			GameMode->CheckGuess(this, Guess);
		}
	}
	
}
// 서버 유효성 검증 ( 보안 )
bool ANBPlayerController::SendGuessToServer_Validate(const FString& Guess)
{
	return Guess.Len() == 3;
}

// 서버에서 게임 실행
void ANBPlayerController::ServerStartGame_Implementation()
{
	ANBGameMode* GameMode = GetWorld()->GetAuthGameMode<ANBGameMode>();
	if (GameMode && !GameMode->bGameStarted)
	{
		GameMode->StartGame();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("No GameMode"));
	}
}


//// 클라이언트가 서버로부터 피드백 받음 (서버 -> 클라이언트)
//void ANBPlayerController::ClientReceiveFeedback_Implementation(const FString& Guess, int32 Ball, int32 Strike)
//{
//	UE_LOG(LogTemp, Warning, TEXT("Client: Received Feedback - Guess : %s, Ball: %d, Strike: %d"), *Guess, Ball, Strike);
//	
//	if (NBGameUI)
//	{
//		NBGameUI->ShowBallStrikeResult(Guess, Ball, Strike);
//	}
//}
//// 클라이언트에서 턴 시작 (서버에서 호출 -> 클라이언트 실행)
//void ANBPlayerController::ClientStartTurn_Implementation()
//{
//	UE_LOG(LogTemp, Warning, TEXT("클라이언트 턴 시작"));
//	bIsMyTurn = true;
//
//	if (NBGameUI)
//	{
//		NBGameUI->UpdateTurn(this);
//	}
//}
// 클라이언트에 게임이 시작했음을 알려줌
void ANBPlayerController::ClientGameStarted_Implementation()
{
	if (NBGameUI)
	{
		NBGameUI->UpdateUIForGameStart(true);
	}
}

// 게임 종료를 클라이언트들에게 알려줌 (서버에서 호출 -> 클라이언트 실행)
void ANBPlayerController::ClientShowGameOver_Implementation(const FString& WinnerID)
{
	if (NBGameUI)
	{
		NBGameUI->ShowGameOver(WinnerID);
	}
}

//void ANBPlayerController::MulticastResetTurns_Implementation()
//{
//	bIsMyTurn = false; // 모든 클라이언트에서 bIsMyTurn을 false로 설정
//
//	if (NBGameUI)
//	{
//		NBGameUI->UpdateTurn(this);
//	}
//}

void ANBPlayerController::MulticastShowTimeoutMessage_Implementation()
{
	if (NBGameUI)
	{
		NBGameUI->ShowBallStrikeResult(TEXT(""), -1, -1); // 제한시간 초과 메시지 출력
	}
}

//void ANBPlayerController::StartTurnLocally()
//{
//	// 리슨 서버의 경우, 직접 실행
//	UE_LOG(LogTemp, Warning, TEXT("Server (Listen): Directly executing StartTurnLocally"));
//
//	bIsMyTurn = true;
//
//	if (NBGameUI)
//	{
//		NBGameUI->UpdateTurn(this);
//	}
//}

void ANBPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
//	bIsMyTurn = false;

	if (IsLocalController()) // 로컬 컨트롤러에서만 UI 추가
	{
		if (NBGameUIClass)
		{
			NBGameUI = CreateWidget<UNBGameUI>(this, NBGameUIClass);
			if (NBGameUI)
			{
				NBGameUI->AddToViewport();
				NBGameUI->SetOwningPlayer(this);
			}
		}
	}
}

void ANBPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 프로퍼티 추가
	DOREPLIFETIME(ANBPlayerController, PlayerID);
}


