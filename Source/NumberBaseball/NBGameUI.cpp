#include "NBGameUI.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "NBPlayerController.h"
#include "NBPlayerState.h"
#include "Components/ProgressBar.h"

void UNBGameUI::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼 이벤트 바인딩
	if (SubmitButton && StartButton)
	{
		SubmitButton->OnClicked.AddDynamic(this, &UNBGameUI::OnSubmitButtonClicked);
		StartButton->OnClicked.AddDynamic(this, &UNBGameUI::OnStartButtonClicked);
	}
	// 처음에는 모든 UI가 보이지 않게 세팅
	UpdateUIForGameStart(false);
	bUIGameStart = true;
}

// 플레이어가 숫자를 입력하고 서버에 전송
void UNBGameUI::OnSubmitButtonClicked()
{
	if (!bUIGameStart)
	{
		return;
	}

	if (!NumberInputBox)
	{
		UE_LOG(LogTemp, Log, TEXT("No NumberInputBox"));
		return;
	}
	FString PlayerInput = NumberInputBox->GetText().ToString();

	// 숫자가 3자리인지 체크
	if (PlayerInput.Len() != 3)
	{
		ResultText->SetText(FText::FromString(TEXT("숫자는 3자리!")));
		// UI에 띄워주고 다음 턴으로 넘겨버리기

		return;
	}
	
	//PlayerController 가져오기
	ANBPlayerController* PlayerController = Cast<ANBPlayerController>(GetOwningPlayer());
	// PlayerState 가져오기
	ANBPlayerState* PlayerState = PlayerController ? PlayerController->GetPlayerState<ANBPlayerState>() : nullptr;


	// Authority 체크 (클라이언트에서 실행)
	if (PlayerController && PlayerState && PlayerState->bIsMyTurn) // && !PlayerController->HasAuthority() 제외하면??
	{
		PlayerController->SendGuessToServer(PlayerInput);
		NumberInputBox->SetText(FText::FromString(TEXT(""))); // 입력 필드를 초기화
	}
	else
	{
		ResultText->SetText(FText::FromString(TEXT(" 상대 턴입니다.")));
	}
}
// 시작 버튼을 눌렀을 때
void UNBGameUI::OnStartButtonClicked()
{
	ANBPlayerController* PlayerController = Cast<ANBPlayerController>(GetOwningPlayer());
	if (PlayerController && PlayerController->HasAuthority()) // 서버에서만 실행
	{
		PlayerController->ServerStartGame();
	}
}

// Ball/Strike 결과를 UI에 표시
void UNBGameUI::ShowBallStrikeResult(const FString& Guess, int32 Ball, int32 Strike)
{
	if (!ResultText)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Result Text!"));
		return;
	}
	if (Ball == -1 && Strike == -1)
	{
		FString ResultMessage = FString::Printf(TEXT("제한시간 초과"));
		ResultText->SetText(FText::FromString(ResultMessage));
		return;
	}

	FString ResultMessage = FString::Printf(TEXT("입력: %s | Ball : %d | Strike: %d"), *Guess, Ball, Strike);
	ResultText->SetText(FText::FromString(ResultMessage));
}
// 턴이 변경될 때 UI 업데이트
void UNBGameUI::UpdateTurn(ANBPlayerController* CurrentPlayer)
{
	if (!TurnStatusText)
	{
		UE_LOG(LogTemp, Warning, TEXT("No TurnStatus Text!"));
		return;
	}
	//FString TurnMessage = (CurrentPlayer->bIsMyTurn) ? TEXT("내 턴") : TEXT("상대 턴");
	ANBPlayerState* PS = CurrentPlayer ? CurrentPlayer->GetPlayerState<ANBPlayerState>() : nullptr;
	FString TurnMessage = (PS && PS->bIsMyTurn) ? TEXT("내 턴") : TEXT("상대 턴");
	TurnStatusText->SetText(FText::FromString(TurnMessage));
}
// 게임 종료 UI
void UNBGameUI::ShowGameOver(const FString& WinnerID)
{
	if (!GameOverText)
	{
		UE_LOG(LogTemp, Warning, TEXT("No GameOver Text!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("ShowGameOver 실행됨! WinnerID: %s"), *WinnerID);

	FString GameOverMessage;
	if (WinnerID != TEXT("무승부"))
	{
		GameOverMessage = FString::Printf(TEXT("승자: %s"), *WinnerID);
	}
	else
	{
		GameOverMessage = TEXT("무승부입니다.");
	}

	GameOverText->SetText(FText::FromString(GameOverMessage));
	bUIGameStart = false;
}


void UNBGameUI::UpdateUIForGameStart(bool bGameStarted)
{
	if (StartButton)
	{
		StartButton->SetIsEnabled(!bGameStarted);
	}

	if (SubmitButton)
	{
		SubmitButton->SetVisibility(bGameStarted ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	if (NumberInputBox)
	{
		NumberInputBox->SetVisibility(bGameStarted ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	if (TurnStatusText)
	{
		TurnStatusText->SetVisibility(bGameStarted ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	if (ResultText)
	{
		ResultText->SetVisibility(bGameStarted ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	if (GameOverText)
	{
		GameOverText->SetVisibility(bGameStarted ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}


}

void UNBGameUI::UpdateTimerProgress(float TimeLeft)
{
	if (!TimerBar) return;

	// 진행도를 0~1 사이 값으로 설정
	float Progress = FMath::Clamp(TimeLeft / 15.0f, 0.0f, 1.0f);
	TimerBar->SetPercent(Progress);
}
