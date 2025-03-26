#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NBGameMode.generated.h"

class ANBPlayerController;

UCLASS()
class NUMBERBASEBALL_API ANBGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    ANBGameMode();

    // 현재 게임에 접속한 플레이어 리스트
    TArray<class ANBPlayerController*> ConnectedPlayers;

    // 현재 턴의 플레이어 인덱스 (0 또는 1)
    int32 CurrentTurnIndex;

    // 정답 숫자 (랜덤 생성)
    FString SecretNumber;

    // 현재 턴 카운트 (최대 6턴)
    int32 TurnCount;

    // 한 턴 제한 시간
    float TurnTime;

    // 게임이 시작되었는지 여부
    bool bGameStarted;

    // 턴 제한시간 타이머 핸들
    FTimerHandle TurnTimerHandle;
    FTimerHandle CountdownTimerHandle; 

    // 게임 시작 함수
    void StartGame();

    // 랜덤한 숫자 생성
    void GenerateRandomNumber();

    // 입력된 숫자를 검증 (Ball, Strike 계산)
    void CheckGuess(class ANBPlayerController* Player, const FString& Guess);

    // 턴 전환
    void SwitchTurn();

    // 제한시간 타이머 시작
    void StartTurnTimer();
    
    // 타이머바 업데이트 함수
    void UpdateTurnTimer();

    // 제한시간 초과 시 처리
    void OnTurnTimeExpired();

    // 게임 종료
    void EndGame(class ANBPlayerController* Winner);

protected:
    virtual void BeginPlay() override;
    virtual void PostLogin(APlayerController* NewPlayer) override;
	
private:
	
};
