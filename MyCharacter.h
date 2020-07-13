// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Inventory.h"
#include "MyCharacter.generated.h"

UCLASS()
class VALH_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	//handles e press for interacting with game world
	void Interact();

	void AimDownSights();
	void RelaxAim();

	bool isADS;

	

	//float RateOfFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		class AInventory* Inventory;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> InventoryDefault;

	/** Sound to play each time we pick up an item */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* PickupSound;

	//Active Body Mesh ; should not include arms ; should be armless
	
	UPROPERTY(EditAnywhere, Category = Mesh)
		class USkeletalMeshComponent* MeshBody;

	//Active Torso Clothing
	UPROPERTY(EditAnywhere, Category = Mesh)
		class USkeletalMeshComponent* MeshTorso;

	//Arms
	UPROPERTY(EditAnywhere, Category = Mesh)
		class USkeletalMeshComponent* Arms;

	//Arms Actual
	UPROPERTY(EditAnywhere, Category = Mesh)
		class USkeletalMeshComponent* LeftArm;

	UPROPERTY(EditAnywhere, Category = Mesh)
		class USkeletalMeshComponent* RightArm;

	//Active Firearm
	UPROPERTY(EditAnywhere, Category = Mesh)
		class USkeletalMeshComponent* Gun;


	//Active Magazine
	UPROPERTY(EditAnywhere, Category = Mesh)
		class USkeletalMeshComponent* MeshMagazine;

	UPROPERTY(EditAnywhere, Category = Mesh)
		class UStaticMeshComponent* MeshMag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* FireAnimation;

	

	// TOOLS, WEAPONS, EQUIPMENT

	/////////////////////////////////////////////////////////////////////////////////       "Whitewater Special" (revolver)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class USkeletalMesh* Gun0MeshReference;					//gun 0 revolver

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class UAnimBlueprint* Gun0AnimReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* Gun0FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* Gun0FireSound;

	
	/////////////////////////////////////////////////////////////////////////////////       "Freedom Fighter" (ak 103)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class USkeletalMesh* Gun1MeshReference;        

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class USkeletalMesh* Gun1MagazineMeshReference;         // rifle

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class UAnimBlueprint* Gun1AnimReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* Gun1FireAnimation;

	//////////////////////////////////////////////////////////////////////////////////       "Old English" (sten mk5)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class USkeletalMesh* Gun3MeshReference;                 // smg

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class UAnimBlueprint* Gun3AnimReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* Gun3FireAnimation;

	/////////////////////////////////////////////////////////////////////////////////        "Short Stack" (aks74u draco)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class USkeletalMesh* Gun7MeshReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class USkeletalMesh* Gun7WoodenParts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class UAnimBlueprint* Gun7AnimReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* Gun7FireAnimation;

	////////////////////////////////////////////////////////////////////////////////         "Plastic Container"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class USkeletalMesh* Gun8MeshReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		class UAnimBlueprint* Gun8AnimReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* Gun8FireAnimation;




	////////// /////////// /////////// /////////// //////////// ////////// /////////
	//Gun Flash
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		TSubclassOf<AActor> GunFlash;


	//This is the base function called, from here, a seconday "Hashmap" functions will be called to grab locations for guns 
	//These secondary functions will set scale andf location/ rotation global variables that will be accessed by an ADS trigger
	UFUNCTION(BlueprintCallable, Category = "Guns")
		void ChangeGunEquipped(int gunNumber);

	//CEW is repeated item identifier. 3 items may have the same CEW. "WHAT" is it
	int CurrentEquippedWeapon;

	UFUNCTION(BlueprintCallable, Category = "Guns")
		void SetGunVariables(int gunNumber);

	FVector RelativeGunlocation;
	FVector AdsRelativeGunLocation;
	FRotator GunRotation;
	FRotator AdsGunRotation;
	FVector GunScale;

	FVector RelativeMagazineLocation;
	FVector AdsRelativeMagazineLocation;
	FRotator MagazineRotation;
	FRotator AdsMagazineRotation;
	FVector MagazineScale;

	void FireWeaponOrTool();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		float gunBaseDamage;

	//spawns bulletsmoke at point of impact when firing
	UFUNCTION()
		void SpawnBulletImpact(FVector Loc, FRotator Rot);

	UFUNCTION()
		void DamageTarget(int damage);

	//bullet smoke 
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> BulletImpact;

	//this is what is expelled from the guns barrel upon firing as smoke
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<AActor> GunSmoke;

	//timer to be used by automatic weapon firing functions
	float firearmTimer;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Guns")
		float rateOfFire;

	void SemiAutomaticFire();
	void FullyAutomaticFire();

	//let the computer know that the player has stopped pressing the fire button so that it does not continue automatic fire
	void ReleaseTrigger();

	FTimerHandle firingPin;

	//spawns gunsmoke upon firing
	UFUNCTION()
		void SpawnGunSmoke();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		int ammoInMagazine;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* FP_MuzzleLocation;

	void OnFire();
	bool isFiring;
	void GatherWater();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		int currentFireType;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector GunOffset;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = time)
		int Hour;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = time)
		int Minutes;

	void SetHoursMinutes(int H, int M);

	void GenerateClockText();
	void GenerateWeaponText();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = time)
		FString ClockText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = guns)
		FString WeaponText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		float HEALTH;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		float STAMINA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		float WATER;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		float HUNGER;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		float FATIGUE;

	void UpdateStats();

	
	///////////////////////////////////////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paused")
		bool Paused;

		int pausecheck;

		//press E, run check, "Paused" grabbed by UI widget
		void PauseCheck();
		
		UFUNCTION(BlueprintCallable, Category = Guns)
			void SetAmmoInMag(int itemID);

		
		//When we change weapons or tools, it is helpful to know which slot in the inventory it occupies
		UFUNCTION(BlueprintCallable, Category = "Guns")
		void ChangeItemIndexEquipped(int IndexNumber);
		//IIE refers to element # in inventory array system, as this number can be used to manipulate multiple arrays and corresponding values. "WHERE" is it
		int InventoryIndexEquipped;

		

};
