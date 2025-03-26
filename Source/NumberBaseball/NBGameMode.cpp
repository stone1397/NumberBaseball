#include "NBGameMode.h"
#include "NBPlayerController.h"
#include "NBPlayerState.h"
#include "NBGameState.h"

ANBGameMode::ANBGameMode()
{
	CurrentTurnIndex = 0;
	TurnCount = 0;
	TurnTime = 15.0f;
	bGameStarted = false;
}

void ANBGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("Number Baseball Started!"));
}

void ANBGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	ANBPlayerController* NBPlayer = Cast<ANBPlayerController>(NewPlayer);
	if (NBPlayer)
	{
		ANBPlayerState* NBPlayerState = NBPlayer->GetPlayerState<ANBPlayerState>();
		if (NBPlayerState)
		{
			// 플레이어 아이디 설정
			FString AssignedID = FString::Printf(TEXT("Player%d"), ConnectedPlayers.Num());
			NBPlayerState->PlayerName = AssignedID;
			UE_LOG(LogTemp, Log, TEXT("플레이어 추가됨: %s"), *AssignedID);
		}
		// 배열에 컨트롤러 추가
		ConnectedPlayers.Add(NBPlayer);
		UE_LOG(LogTemp, Warning, TEXT("플레이어 추가! 현재 접속자 수: %d"), ConnectedPlayers.Num());
	}
}

void ANBGameMode::StartGame()
{
	if (ConnectedPlayers.Num() < 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("아직 두 명의 플레이어가 들어오지 않았습니다."));
		return;
	}
	ANBGameState* GS = GetGameState<ANBGameState>();
	if (GS && ConnectedPlayers.Num() >= 2)
	{
		GS->bGameStarted = true;
		UE_LOG(LogTemp, Error, TEXT("bGameStarted = True!"))
		GS->TurnCount = 0;
		GS->CurrentTurnPlayerID = ConnectedPlayers[0]->GetPlayerState<ANBPlayerState>()->PlayerName;
	}

	/*bGameStarted = true;
	TurnCount = 0;
	CurrentTurnIndex = 0;*/

	// 랜덤 숫자를 생성한다.
	GenerateRandomNumber();

	// !게임이 시작되었음을 모든 클라이언트에 알림
	for (ANBPlayerController* Player : ConnectedPlayers)
	{
		Player->ClientGameStarted();
	}

	// 첫 번째 플레이어 턴 시작
	SwitchTurn();
}

// 랜덤 숫자 생성 로직
void ANBGameMode::GenerateRandomNumber()
{
	TArray<int32> Digits;
	while (Digits.Num() < 3)
	{
		int32 RandomDigit = FMath::RandRange(1, 9);
		if (!Digits.Contains(RandomDigit))
		{
			Digits.Add(RandomDigit);
		}
	}
	
	SecretNumber = FString::Printf(TEXT("%d%d%d"), Digits[0], Digits[1], Digits[2]);
	UE_LOG(LogTemp, Warning, TEXT("Generated Secret Number: %s"), *SecretNumber);
}
// 입력 숫자를 판단하는 로직
void ANBGameMode::CheckGuess(ANBPlayerController* Player, const FString& Guess)
{
	ANBGameState* GS = GetGameState<ANBGameState>();
	if (GS)
	{
		if (!GS->bGameStarted || Guess.Len() != 3)
		{
			UE_LOG(LogTemp, Warning, TEXT("아직 판단할 수 없습니다."))
				return;
		}
	}
	

	int32 Ball = 0;
	int32 Strike = 0;

	for (int32 i = 0; i < 3; i++)
	{
		if (Guess[i] == SecretNumber[i])
		{
			Strike++;
		}
		else if (SecretNumber.Contains(FString(1, &Guess[i])))
		{
			Ball++;
		}
	}

	// GameState에 Ball/Strike 정보 저장
	GS->LatestGuess = Guess;
	GS->LatestBall = Ball;
	GS->LatestStrike = Strike;

	// 서버에서도 OnRep_LatestFeedback()을 실행하여 모든 클라이언트에 반영
	GS->OnRep_LatestFeedback();

	//// 결과를 클라이언트에 전송 (Client RPC 호출)
	//Player->ClientReceiveFeedback(Guess, Ball, Strike);

	if (Strike == 3)
	{
		EndGame(Player);
	}
	else
	{
		// 승리 조건 X -> 턴 변경
		SwitchTurn();
	}
}

void ANBGameMode::SwitchTurn()
{
	ANBGameState* GS = GetGameState<ANBGameState>();
	if (!GS || !GS->bGameStarted)
	{
		UE_LOG(LogTemp, Log, TEXT("게임이 이미 끝났습니다."));
		return;
	}
	/*if (!bGameStarted)
	{
		UE_LOG(LogTemp, Log, TEXT("게임이 이미 끝났습니다."));
		return;
	}*/
	//if (TurnCount >= 6)
	//{
	//	// 6턴이 지날 경우 무승부 처리
	//	EndGame(nullptr);
	//	return;
	//}
	if (GS->TurnCount >= 6)
	{
		EndGame(nullptr);
		return;
	}

	GS->TurnCount++;

	int32 NextTurnIndex = (GS->TurnCount % ConnectedPlayers.Num());
	
	// 서버에서 직접 CurrentTurnPlayerID 변경 (클라이언트의 OnRep_CurrentTurn() 실행 트리거)
	if (HasAuthority())
	{
		ANBPlayerState* NextTurnState = ConnectedPlayers[NextTurnIndex]->GetPlayerState<ANBPlayerState>();
		if (NextTurnState)
		{
			GS->CurrentTurnPlayerID = NextTurnState->PlayerName;
			UE_LOG(LogTemp, Log, TEXT("턴 변경! 다음 플레이어: %s"), *GS->CurrentTurnPlayerID);

			// 서버에서도 OnRep_CurrentTurn()을 명시적으로 호출 (클라이언트와 동기화)
			GS->OnRep_CurrentTurn();
		}
	}
	/*ANBPlayerState* NextTurnState = ConnectedPlayers[NextTurnIndex]->GetPlayerState<ANBPlayerState>();
	if (NextTurnState)
	{
		GS->CurrentTurnPlayerID = NextTurnState->PlayerName;
	}*/

	//// 턴 변경
	//CurrentTurnIndex = (CurrentTurnIndex + 1) % 2;
	//TurnCount++;

	//// 다음 플레이어에게 턴 시작을 알림
	//if (ConnectedPlayers.IsValidIndex(CurrentTurnIndex))
	//{
	//	ANBPlayerController* TargetPlayer = ConnectedPlayers[CurrentTurnIndex];

	//	// 모든 클라이언트에서 bIsMyTurn을 false로 동기화
	//	for (ANBPlayerController* Player : ConnectedPlayers)
	//	{
	//		if (Player)
	//		{
	//			/*Player->MulticastResetTurns();*/

	//			// 모든 플레이어의 bIsMyTurn을 false로 동기화
	//			ANBPlayerState* PS = Player->GetPlayerState<ANBPlayerState>();
	//			if (PS) PS->bIsMyTurn = false;
	//		}
	//	}
	//	//// 리슨 서버의 경우, 자기 자신에게 Client RPC를 보내지 않도록 방지!
	//	//if (!TargetPlayer->IsLocalController())
	//	//{
	//	//	TargetPlayer->ClientStartTurn();
	//	//	UE_LOG(LogTemp, Log, TEXT("CLientStart"));
	//	//}
	//	//else
	//	//{
	//	//	// 리슨 서버는 직접 실행 (이미 서버에서 실행 중이므로, RPC 필요 없음)
	//	//	TargetPlayer->StartTurnLocally();
	//	//	UE_LOG(LogTemp, Log, TEXT("ListenStart"));
	//	//}
	//	
	//	// 현재 턴인 플레이어의 bIsMyTurn을 true로 동기화
	//	ANBPlayerState* TurnPS = ConnectedPlayers[CurrentTurnIndex]->GetPlayerState<ANBPlayerState>();
	//	if (TurnPS) TurnPS->bIsMyTurn = true;
	//}

	// 제한시간 타이머 시작
	StartTurnTimer();
}

void ANBGameMode::StartTurnTimer()
{
	// 남은 시간 초기화
	ANBGameState* GS = GetGameState<ANBGameState>();
	if (GS)
	{
		GS->RemainingTurnTime = 15.0f;
		GS->OnRep_RemainingTime(); // 서버에서도 즉시 업데이트 필요
	}

	// 매초마다 UpdateTurnTimer() 실행
	GetWorldTimerManager().SetTimer(
		CountdownTimerHandle,
		this,
		&ANBGameMode::UpdateTurnTimer,
		1.0f,
		true
	);
	GetWorldTimerManager().SetTimer(
		TurnTimerHandle,
		this,
		&ANBGameMode::OnTurnTimeExpired,
		TurnTime,
		false
	);
}

void ANBGameMode::UpdateTurnTimer()
{
	ANBGameState* GS = GetGameState<ANBGameState>();
	if (GS)
	{
		GS->RemainingTurnTime -= 1.0f;
		GS->OnRep_RemainingTime(); // 서버에서도 즉시 실행
	}
}

void ANBGameMode::OnTurnTimeExpired()
{
	GetWorldTimerManager().ClearTimer(CountdownTimerHandle); // 카운트다운 중지
	
	UE_LOG(LogTemp, Warning, TEXT("제한시간 끝"));

	// 모든 클라이언트에 제한시간 초과 메시지 전송
	for (ANBPlayerController* Player : ConnectedPlayers)
	{
		Player->MulticastShowTimeoutMessage();
	}
	SwitchTurn();
}

void ANBGameMode::EndGame(ANBPlayerController* Winner)
{
	ANBGameState* GS = GetGameState<ANBGameState>();
	if (GS)
	{
		GS->bGameStarted = false;
	}
//    bGameStarted = false;

	FString WinnerID = Winner ? Winner->GetPlayerState<ANBPlayerState>()->PlayerName : TEXT("무승부");

	UE_LOG(LogTemp, Warning, TEXT("게임 끝! WinnerID: %s"), *WinnerID);

	// 모든 클라이언트에게 승자 ID를 전달하여 UI 업데이트
	for (ANBPlayerController* Player : ConnectedPlayers)
	{
		Player->ClientShowGameOver(WinnerID);
	}
 //   if (Winner) // `nullptr` 체크 있음
 //   {
 //       UE_LOG(LogTemp, Warning, TEXT("게임 끝! 승자 : %s"), *Winner->PlayerID));
	//	// 서버에서 클라이언트에게 게임 종료 UI출력 지시
	//	for (ANBPlayerController* Player : ConnectedPlayers)
	//	{
	//		Player->ClientShowGameOver(Winner);
	//	}
	//	return;
 //   }
	//UE_LOG(LogTemp, Warning, TEXT("비겼습니다!"));
	//
	//// 서버에서 클라이언트에게 게임 종료 UI출력 지시
	//for (ANBPlayerController* Player : ConnectedPlayers)
	//{
	//	Player->ClientShowGameOver(Winner);
	//}
	GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
	GetWorldTimerManager().ClearTimer(TurnTimerHandle);
}

