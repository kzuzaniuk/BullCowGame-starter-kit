// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
// #include "Math/UnrealMathUtility.h" jest w CoreMinimal
// #include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    HighScore = 0;
    DifficultyLevel = 1;
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    // FFileHelper::LoadFileToStringArray(Words, *WordListPath); //Stara metoda, gdzie najpierw ładujemy wszystkie linijki do tablicy Words, a potem używam GetValidWords, by stworzyć nową tablicę z isogramami
    // Isograms = GetValidWords(Words);
    FFileHelper::LoadFileToStringArrayWithPredicate(Isograms, *WordListPath, [](const FString& Word){ //ostatni argument to obiekt funkcji, który przyjmuje po kolei każdą linijkę z pliku, sprawdza warunek i dodaje ją do tablicy jeśli true
        return Word.Len() > 3 && Word.Len() < 12 && IsIsogram(Word);
    });

    // FBullCowCount Count;
    SetupGame();

    // PrintLine(TEXT("The number of valid words is: %i\n"), GetValidWords(Words).Num()); 
    
    //Prompt for a guess
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if(bGameOver){ 
        ClearScreen();  //if game is over then ClearScreen() and SetupGame()
                        //else Check PlayerGuess again
        SetupGame();
    }
    else {  //Checking the player guess
        ProcessGuess(Input);
   
    }
    

}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)]; //ValidWords[rand() % ValidWords.Num()];
    if(DifficultyLevel <= 2)
    {
        do
        {
            HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)]; //ValidWords[rand() % ValidWords.Num()];
        } while (HiddenWord.Len() > 4);
        Lives = HiddenWord.Len();
    }
    else if (DifficultyLevel <= 4)
    {
        do
        {
            HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)]; //ValidWords[rand() % ValidWords.Num()];
        } while (HiddenWord.Len() < 6 && HiddenWord.Len() <= 8);
        Lives = Lives = HiddenWord.Len() - DifficultyLevel;
    }
    else
    {
        do
        {
            HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)]; //ValidWords[rand() % ValidWords.Num()];
        } while (HiddenWord.Len() < 10);
        Lives = HiddenWord.Len() - DifficultyLevel;
    }
    
    FString output;
    output.AppendChar(HiddenWord[0]);
    // Lives = HiddenWord.Len();
    PrintLine(TEXT("Welcome to Bull Cow Game!\n"));
    PrintLine(TEXT("You have %i tries."), Lives);
    PrintLine(TEXT("You are on %i level"), DifficultyLevel);
    PrintLine(FString::Printf(TEXT("Guess the %i letter word.\n"), HiddenWord.Len()));
    PrintLine(TEXT("Type in your guess and \npress enter to continue...\n"));
    PrintLine(FString::Printf(TEXT("The hidden word is: %s\n"), *HiddenWord)); //DEBUG
    PrintLine(FString::Printf(TEXT("The hidden word starts with the letter: %s\n"), *output)); //DEBUG
    bGameOver = false;
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press enter to play again or ESC to stop\n"));
    PrintLine(TEXT("Highscore...%i\n"), HighScore);
    PrintLine(TEXT("You reached level...%i\n"), DifficultyLevel);
    
    //number of playthroughs
    //press something to not play again
    
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
        /* if(Guess == HiddenWord){
        ClearScreen();
        PrintLine(TEXT("Good job, you win!\n"));
        EndGame();
            //Try again?
            //Or next level
        }
        //if(!isogram){ --lives return}
        //if 
        else {
            --Lives;
            if (Lives > 0)
            {
                if (Guess.Len() != HiddenWord.Len())
                {
                    PrintLine(FString::Printf(TEXT("The hidden word is %i letters long.\nYou have %i lives left!\n"), HiddenWord.Len(), Lives));
                }
                else {
                    PrintLine(TEXT("Bad guess!\nYou have %i lives left!\n"), Lives);
                }
            }
            else {
                ClearScreen();
                PrintLine(TEXT("No lives left...\n"));
                PrintLine(FString::Printf(TEXT("The hidden word was: %s\n"), *HiddenWord));
                EndGame();
            }
        }  */
            //Check if isogram
            //Check if right number of characters
            //Display cows and bulls
            //Remove the life
                //Check if lives > 0 
                //If yes, loop again
                //Show lives left
                //If no, display game over
                //Prompt to play again
                //Check user input
                //Play agian or quit


        if(Guess == HiddenWord){
            ClearScreen();
            PrintLine(TEXT("Good job, you win!\n"));
            if(Lives == 5){
                HighScore += Lives * 12;
            }
            else if (Lives >= 3)
            {
                HighScore += Lives * 6; 
            }
            else
            {
                HighScore += Lives * 2;
            }
            ++DifficultyLevel;
            EndGame();
            return;
        }

        --Lives;
        if(Lives <= 0) {
            ClearScreen();
            PrintLine(TEXT("No lives left...\n"));
            PrintLine(FString::Printf(TEXT("The hidden word was: %s\n"), *HiddenWord));
            EndGame();
            return;
        }
        
        if (Guess.Len() != HiddenWord.Len())
        {
            
            PrintLine(FString::Printf(TEXT("The hidden word is %i letters long.\nYou have %i lives left!\n"), HiddenWord.Len(), Lives));
            return;
        }

        if (!IsIsogram(Guess))
        {
            PrintLine(TEXT("Not an isogram...\nYou have %i lives left!\n"), Lives);
            return;
        }

        FBullCowCount Count = GetBullCows(Guess);
        PrintLine(TEXT("You have %i Bulls and %i Cows\n"), Count.Bulls, Count.Cows);
        PrintLine(TEXT("Good letters right place %s\n"), *Count.coMasz);
        PrintLine(TEXT("Good letters bad place %s\n"), *Count.coPrawieMasz);
        PrintLine(TEXT("Bad guess!\nYou have %i lives left!\n"), Lives);
        // PrintLine(FString::Printf(TEXT("Guess again, you have %i lives left.\n"), Lives));
            //Check if isogram
            //Check if right number of characters
            //Display cows and bulls
            //Remove the life
                //Check if lives > 0 
                //If yes, loop again
                //Show lives left
                //If no, display game over
                //Prompt to play again
                //Check user input
                //Play agian or quit
        
}

bool UBullCowCartridge::IsIsogram(const FString& Word)
{
    // int32 Counter;
    for (int32 i = 0; i < Word.Len() - 1; i++)
    {
        // Counter = 0; 
        for (int32 j = i + 1; j < Word.Len(); j++)
        {
            if(Word[i] == Word[j])
            {
                // Counter += 1;
                return false;
            }
        }
        // if(Counter > 1){
        //     return false;
        // }
        
    }
    
             /*    loop guess letters (iterator i until guess.len()-1)
                through all the guess letters to find match (iterator j hiddenword.len -1 )
                i.e. x loops through the whole a->l->m->o->s->t and then loops again with next letter
                    if it matches more than once then it's not isogram and return false
                    there must be a counter for every letter 
                    
                    
                    if it's a match and i == j then it's a bull
                    if there is a match but i != j then it's a cow */
    return true;
}

/* TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& List) const
{
    TArray<FString> ValidWords;
                                                                                   for (int32 Index = 0; Index < List.Num(); Index++)
                                                                                    {
                                                                                     if(List[Index].Len() >= 4 && List[Index].Len() <= 8 && IsIsogram(List[Index])){
                                                                                        ValidWords.Emplace(List[Index]);
                                                                                     }
        
                                                                                 } 

    for (FString TempVar : List)
    {
        if(TempVar.Len() >= 4 && TempVar.Len() <= 8 && IsIsogram(TempVar)){
            ValidWords.Emplace(TempVar);
        }
    }
    

                                                                                 
                                                                                for (int32 word = HiddenWordList.Num()-1; word >= 0; --word){
                                                                                    if (HiddenWordList[word].Len() < 4 || 
                                                                                       HiddenWordList[word].Len() > 8 ||                                   WERSJA BEZ DRUGIEJ TABLICY
                                                                                       !IsIsogram(HiddenWordList[word]))
                                                                                    {
                                                                                      UE_LOG(LogTemp, Warning,TEXT("Removed: %s"), *HiddenWordList[word]);
                                                                                      HiddenWordList.RemoveAt(word);
                                                                                 }    
                                                                                 }
                                                                                }

    return ValidWords;
} */

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            Count.coMasz.AppendChar(Guess[GuessIndex]);
            continue;
        }

        for (int32 HiddenWordIndex = 0; HiddenWordIndex < HiddenWord.Len(); HiddenWordIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenWordIndex])
            {
                Count.Cows++;
                Count.coPrawieMasz.AppendChar(Guess[GuessIndex]);
                break;
            }
            
        }
        
        
    }

    return Count;
    
}


//Highscore
