// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceSpawner.h"
#include "FileHelper.h"
#include "Paths.h"
#include "Engine.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/FileHelper.h"

//Functions along the same basis as the FoliageGod and EnemyHandler

// Sets default values
AResourceSpawner::AResourceSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AResourceSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AResourceSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<FString> AResourceSpawner::GetBlockCoords(int BlockNumber)
{
	FString BlockCoordFileName = "/BlockCoordinatesSpreadsheet.txt"; FString BlockCoordFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + BlockCoordFileName;
	TArray<FString> CoordFile; FFileHelper::LoadANSITextFileToStrings(*BlockCoordFilePath, NULL, CoordFile);
	TArray<FString> CoordData; CoordFile[BlockNumber + 1].ParseIntoArray(CoordData, TEXT(","), 1);

	return CoordData;
}

TArray<FString> AResourceSpawner::GetSeedData(int BlockNumber)
{
	FString SeedFileName = "/ItemBlockSeedData" + FString::FromInt(BlockNumber) + ".txt";
	FString SeedFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + SeedFileName;
	TArray<FString> SeedFile; FFileHelper::LoadANSITextFileToStrings(*SeedFilePath, NULL, SeedFile);

	return SeedFile;
}

void AResourceSpawner::AppendItemToDataFile(int BlockNumber, TArray<FString> ItemInfo)
{
	FString ItemDataFileName = "/ItemBlockData" + FString::FromInt(BlockNumber) + ".txt";
	FString ItemDataFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + ItemDataFileName;
	FString LineToSave = ItemInfo[0] + "," + ItemInfo[1] + "," + ItemInfo[2] + "," + ItemInfo[3] + "," + ItemInfo[4] + "," + "\n";
	FFileHelper::SaveStringToFile(LineToSave, *ItemDataFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
}

void AResourceSpawner::GenerateItemData(int BlockNumber)
{
	//produce a file name for the save data referring to persistent world items which we will create as data in the following lines
	//we create a fie path for the previosuly imagined file
	//We wipe anything that might already be in the file; This is mainly for testing purposes
	//Get Coordinates for block | A "block" is an abstract data representation of a landscape tile's bounds and is used to refer to its contents
	//Get integer values for coords (x y bounds)
	//Get  csv line of metadata for each item type delivered in an array
	//Iterate through each item type, by iterating through each line of the seed data file, where each file holds information for a type of item to spawn
	//Generate a location
	FString SaveDataFileName = "/ItemBlockSaveData" + FString::FromInt(BlockNumber) + ".txt";
	FString SaveDataFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + SaveDataFileName;
	FString ClearingText = TEXT(""); FFileHelper::SaveStringToFile(ClearingText, *SaveDataFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get());
	FVector Locs;
	TArray<FString> CoordData = GetBlockCoords(BlockNumber);
	int xMin = FCString::Atoi(*CoordData[1]); int xMax = FCString::Atoi(*CoordData[2]);
	int yMin = FCString::Atoi(*CoordData[3]); int yMax = FCString::Atoi(*CoordData[4]);
	TArray<FString> SeedFile = GetSeedData(BlockNumber);

	for (int i = 0; i < SeedFile.Num(); )
	{
		TArray<FString> ItemData;
		SeedFile[i].ParseIntoArray(ItemData, TEXT(","), 1);              //for each item type, we construct an array to hold its data from the file, during a for loop
		TArray<int> ItemInts;                                            //we convert the text data from the file's lines into an int array for each line
					ItemInts.Init(1, 4);                                 //initiate the array
		for (int dat = 0; dat < ItemData.Num(); dat++)
		{ItemInts[dat] = FCString::Atoi(*ItemData[dat]);}                //the conversion

		int NumberOfItems = floor(FMath::FRandRange(ItemInts[2], ItemInts[3]));            //we determine the number of items of each item to spawn 

		for (int j = 0; j < NumberOfItems; j++)                          //We generate data for each item to be spawned
		{
			Locs.X = FMath::FRandRange(xMin, xMax);
			Locs.Y = FMath::FRandRange(yMin, yMax);
			Locs.Z = 8000.0f;

			TArray<FString> ItemInfo; ItemInfo.Init("", 5);            //We initialize an array of strings to save to the file
			ItemInfo[0] = ItemData[0];                                 //we grab the name to save
			ItemInfo[1] = ItemData[1];                                 //We grab the item ID to save
			ItemInfo[2] = FString::SanitizeFloat(Locs.X);
			ItemInfo[3] = FString::SanitizeFloat(Locs.Y);
			ItemInfo[4] = FString::SanitizeFloat(Locs.Z);              //We sanitize the floats to strings and add to the array

			AppendItemToDataFile(BlockNumber, ItemInfo);               //We save the item to the data file

		}
	}
}

//the values of the data being pulled in from text and put into arrays can be found in the corresponding blueprint text files
//TODO: Add Documentation; Add an Item Defaults file and use it to pass to the spawned items, which will set their initial values (isWep, isRes, etc);
void AResourceSpawner::SpawnItemsFromData(int BlockNumber)
{
	FVector Locs; FRotator Rotacione; FActorSpawnParameters SpawnParams;
	TArray<FString> ItemData = GetItemData(BlockNumber);
	for (int i = 0; i < ItemData.Num(); i++)
	{
		TArray<FString> ItemDataStr; ItemData[i].ParseIntoArray(ItemDataStr, TEXT(","), 1);
		TArray<int> ItemIntegers; ItemIntegers.Init(1, 5); for (int j = 1; j < 5; j++) { ItemIntegers[j] = FCString::Atoi(*ItemDataStr[j]); }
		Locs.X = ItemIntegers[2]; Locs.Y = ItemIntegers[3]; Locs.Z = ItemIntegers[4]; Rotacione.Roll = 0;  Rotacione.Pitch = 0; Rotacione.Yaw = 0;

		AActor* SpawnedActorRef;

		switch (ItemIntegers[1])
		{
		case 8: SpawnedActorRef = GetWorld()->SpawnActor<AActor>(ItemNumber8, Locs, Rotacione, SpawnParams); break;
		}
	}
}

TArray<FString> AResourceSpawner::GetItemData(int BlockNumber)
{
	FString ItemDataFileName = "/ItemBlockData" + FString::FromInt(BlockNumber) + ".txt";
	FString ItemDataFilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + ItemDataFileName;
	TArray<FString> ItemData; FFileHelper::LoadANSITextFileToStrings(*ItemDataFilePath, NULL, ItemData);

	return ItemData;
}


