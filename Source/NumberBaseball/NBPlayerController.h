#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NBPlayerController.generated.h"

class ANBGameMode;

UCLASS()
class NUMBERBASEBALL_API ANBPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Replicated)
	FString PlayerID;
	UPROPERTY()
	class UNBGameUI* NBGameUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UNBGameUI> NBGameUIClass;

	//// 현재 내 턴인지 여부
	//bool bIsMyTurn;

	// 숫자 입력을 서버로 전송
	UFUNCTION(Server, Reliable, WithValidation)
	void SendGuessToServer(const FString& Guess);
	// 서버에서 게임 시작
	UFUNCTION(Server, Reliable)
	void ServerStartGame();

	// 서버로부터 피드백을 받아 클라이언트에서 실행
	//UFUNCTION(Client, Reliable)
	//void ClientReceiveFeedback(const FString& Guess, int32 Ball, int32 Strike);
	
	//// 서버에서 턴에 변경될 때 클라이언트에서 턴 시작
	//UFUNCTION(Client, Reliable)
	//void ClientStartTurn();

	// 클라이언트에 게임이 시작했음을 알려줌
	UFUNCTION(Client, Reliable)
	void ClientGameStarted();
	// 게임이 끝났음을 알려줌
	UFUNCTION(Client, Reliable)
	void ClientShowGameOver(const FString& WinnerID);

	//// 턴을 초기화하는 Multicast RPC
	//UFUNCTION(NetMulticast, Reliable)
	//void MulticastResetTurns();
	
	// 제한시간이 초과되었음을 알려주는 Multicast RPC
	UFUNCTION(NetMulticast, Reliable)
	void MulticastShowTimeoutMessage();

	//UFUNCTION()
	//void StartTurnLocally();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	

};
