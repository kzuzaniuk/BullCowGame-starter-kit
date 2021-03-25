// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

struct FBullCowCount 
{
	int32 Bulls = 0;
	int32 Cows = 0;
	FString coMasz;
	FString coPrawieMasz;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString& Input) override;
	void SetupGame();
	void EndGame();
	void ProcessGuess(const FString& Guess);
	static bool IsIsogram(const FString& Word); //static, bo jest powiązana z klasą, nie z konkretnym obiektem (nie używa żadnych member variables, więc nie potrzebuje instancji). Nie musi być też const, bo nie zmienia żadnego obiektu
	TArray<FString> GetValidWords(const TArray<FString>& List) const;
	FBullCowCount GetBullCows(const FString& Guess) const;

	// Your declarations go below!
	private:
	FString HiddenWord;
	int32 Lives;
	bool bGameOver;
	int32 DifficultyLevel;
	int32 HighScore;
	TArray<FString> Words;
	TArray<FString> Isograms;
};
