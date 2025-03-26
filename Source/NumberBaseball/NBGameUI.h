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
	// Ball/Strike ����� UI�� ǥ��
	UFUNCTION()
	void ShowBallStrikeResult(const FString& Guess, int32 Ball, int32 Strike);
	// ���� ����� �� UI ������Ʈ
	UFUNCTION()
	void UpdateTurn(class ANBPlayerController* CurrentPlayer);
	// ���� ���� ȭ�� ǥ��
	UFUNCTION()
	void ShowGameOver(const FString& WinnerID);
	UFUNCTION()
	void UpdateUIForGameStart(bool bGameStarted);
	// ���ѽð� ProgressBar ������Ʈ
	void UpdateTimerProgress(float TimeLeft);

protected:
	virtual void NativeConstruct() override;

private:
	// UI ���
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


	// ��ư Ŭ�� �̺�Ʈ
	UFUNCTION()
	void OnSubmitButtonClicked();
	UFUNCTION()
	void OnStartButtonClicked();

	bool bUIGameStart;
};
