


// *****
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlaybackCtrlInterface.h"
#include "Common/OscDataElemStruct.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "CueActor.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FComponentCueRxSignature, const FName &, Address, const TArray<FOscDataElemStruct> &, Data, const FString &, SenderIp);
DECLARE_DELEGATE(OnCueStateEndDelegate)

UENUM(BlueprintType)
enum class CueActorState : uint8 {
    None UMETA(DisplayName = "None"),
    FadeIn UMETA(DisplayName = "Fade In"),
    Run UMETA(DisplayName = "Run"),
    FadeOut UMETA(DisplayName = "Fade Out"),
    Finished UMETA(DisplayName = "Finished")
};

UCLASS(ClassGroup=PlaybackCtrl)
class PLAYBACKCTRL_API ACueActor : public AActor
{
    GENERATED_BODY()
    
public:
//    UPROPERTY(EditAnywhere, Category=PlaybackCtrl)
    FString DepartmentFilter;
//
//    UPROPERTY(EditAnywhere, Category=PlaybackCtrl)
    FString BuildFilter;
    

//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Enum)
//    EActionEnum ActionEnum;
    
    /** Add your fade in sequence here, if applicable */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=PlaybackCtrl)
    ULevelSequence* FadeInSeq;
    
    /** Add your run sequence here, if applicable */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=PlaybackCtrl)
    ULevelSequence* RunSeq;
    
    /** Add your fade out sequence here, if applicable */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=PlaybackCtrl)
    ULevelSequence* FadeOutSeq;
    
    UPROPERTY(BlueprintAssignable, Category=PlaybackCrtl)
    FComponentCueRxSignature OnCueRx;
    
    UPROPERTY(EditAnywhere, Category=PlaybackCtrl)
    ULevelSequencePlayer* SequencePlayer;


    
    // EVENTS
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category=PlaybackCtrl)
    void OnFadeInStart();
    
    UFUNCTION()
    void OnFadeInStart_Implementation();
    
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category=PlaybackCtrl)
    void OnFadeInEnd();
    
    UFUNCTION()
    void OnFadeInEnd_Implementation();
    
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category=PlaybackCtrl)
    void OnRunStart();
    
    UFUNCTION()
    void OnRunStart_Implementation();
    
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category=PlaybackCtrl)
    void OnRunEnd();
    
    UFUNCTION()
    void OnRunEnd_Implementation();
    
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category=PlaybackCtrl)
    void OnFadeOutStart();
    
    UFUNCTION()
    void OnFadeOutStart_Implementation();
    
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category=PlaybackCtrl)
    void OnFadeOutEnd();
    
    UFUNCTION()
    void OnFadeOutEnd_Implementation();
    
    UFUNCTION(BlueprintCallable)
    TMap<FString, FString> getCueParameters() const { return DataDict_; }

    TMap<FString, FString> DataDict_;
    TArray<TAssetSubclassOf<UObject>> ToSpawn;
        

public:
    // Sets default values for this actor's properties
    ACueActor();
    
    /// Hot reload constructor
    ACueActor(FVTableHelper & helper);
    
    const FString & GetDepartmentFilter() const
    {
        return DepartmentFilter;
    }
    
    const FString & GetBuildFilter() const
    {
        return BuildFilter;
    }
    
    ULevelSequence* & GetFadeInSeq()
    {
        return FadeInSeq;
    }
    
    ULevelSequence* & GetRunSeq()
    {
        return RunSeq;
    }
    
    ULevelSequence* & GetFadeOutSeq()
    {
        return FadeOutSeq;
    }
    
    void SendEvent(const FName & Address, const TArray<FOscDataElemStruct> & Data, const FString & SenderIp)
    {
        OnCueRx.Broadcast(Address, Data, SenderIp);
    }
    
    UFUNCTION()
    void OnCueReceived(const FName & Address, const TArray<FOscDataElemStruct> & Data, const FString & SenderIp);
    
    void CueStateStart(ULevelSequence* Seq, FString CueStateLength, FName EndCueState);
    
    void ResetCue();
    
    UFUNCTION(BlueprintCallable)
    float getCueProgress() const { return cueProgress_;  }

    UFUNCTION(BlueprintCallable)
    float getCueStateProgress() const { return cueStateProgress_; }

    UFUNCTION(BlueprintCallable, Category = "PlaybackCtrl")
    float GetFloatParam(FString ParamName);

    UFUNCTION(BlueprintCallable, Category = "PlaybackCtrl")
    FString GetStringParam(FString ParamName);
    
    UFUNCTION(BlueprintCallable)
    CueActorState getCueState() const { return cueState_; }

    UFUNCTION(BlueprintCallable)
    FString getCueStateString() const;

    UFUNCTION(BlueprintCallable)
    float getStateLength(CueActorState state);

    void setState(CueActorState state);
    float getSequenceDurationSeconds(ULevelSequence* seq) const;

protected:
    void BeginDestroy() override;
    virtual void BeginPlay() override;
    void Tick(float DeltaTime) override;
    
private:
    BasicCueReceiver<ACueActor> _listener;
    CueActorState cueState_;
    float cueStateProgress_, cueProgress_;
    float fadeInLen_, fadeOutLen_, runLen_, cueTotalLen_;
    OnCueStateEndDelegate onStateEndDelegate_;
};

