#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NBGameUI.generated.h"

class ANBPlayerControler;

UCLASS()
class NUMBERBASEBALL_API UNBGameUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Ball/Strike 결과를 UI에 표시
	UFUNCTION()
	void ShowBallStrikeResult(const FString& Guess, int32 Ball, int32 Strike);
	// 턴이 변경될 때 UI 업데이트
	UFUNCTION()
	void UpdateTurn(class ANBPlayerController* CurrentPlayer);
	// 게임 종료 화면 표시
	UFUNCTION()
	void ShowGameOver(const FString& WinnerID);
	UFUNCTION()
	void UpdateUIForGameStart(bool bGameStarted);
	// 제한시간 ProgressBar 업데이트
	void UpdateTimerProgress(float TimeLeft);

protected:
	virtual void NativeConstruct() override;

private:
	// UI 요소
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* TimerBar;
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* NumberInputBox;
	UPROPERTY(meta = (BindWidget))
	class UButton* SubmitButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* StartButton;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TurnStatusText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ResultText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GameOverText;


	// 버튼 클릭 이벤트
	UFUNCTION()
	void OnSubmitButtonClicked();
	UFUNCTION()
	void OnStartButtonClicked();

	bool bUIGameStart;
};
