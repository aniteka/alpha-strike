

#include "Character/BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include <Kismet/GameplayStatics.h>

#include "DelayAction.h"
#include "UI/Widgets/HealthBarWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/WeaponComponent.h"
#include "Components/HealthComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"


ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;	

	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar Widget Component"));
	HealthBarWidgetComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidgetComponent->SetDrawSize(FVector2D(200, 18));

	MeshBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Body"));
	//MeshBody->SetupAttachment(GetMesh());
	MeshBody->SetupAttachment(GetMesh(), FName(TEXT("Body_Soket")));	

	R_Hand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Hand"));
	R_Hand->SetupAttachment(MeshBody);

	R_Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("R_Weapon"));
	R_Weapon->SetupAttachment(R_Hand, FName(TEXT("DeafaultRobotWeapon")));

	L_Hand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Hand"));
	L_Hand->SetupAttachment(MeshBody);	

	L_Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("L_Weapon"));
	L_Weapon->SetupAttachment(L_Hand, FName(TEXT("DeafaultRobotWeapon")));

	ArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	ArmComponent->bUsePawnControlRotation = false;
	ArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(ArmComponent);

	PerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(
		"PerceptionStimuliSourceComponent");
	PerceptionStimuliSourceComponent->bAutoRegister;
}

void ABaseCharacter::UpdateCameraOffset()
{
	ArmComponent->SetRelativeRotation({ -R_Hand->GetRelativeRotation().Roll, MeshBody->GetComponentRotation().Pitch,0.f });
}

void ABaseCharacter::OnDeathCallback(AController* Damaged, AController* Causer)
{
	PerceptionStimuliSourceComponent->UnregisterFromPerceptionSystem();

	WeaponComponent->StopFire();
	
	const auto OldRotation = MeshBody->GetComponentRotation();
	const auto OldLocation = MeshBody->GetComponentLocation();
	MeshBody->SetAbsolute(true, true);
	MeshBody->SetWorldRotation(OldRotation);
	MeshBody->SetWorldLocation(OldLocation);
	MeshBody->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	MeshBody->SetSimulatePhysics(true);

	MeshBody->AddImpulse(DeathExplosionHeadImpulse);

	if(ExplosionParticle)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetTransform());
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	
	if (!IsPlayerControlled())
	{
		InitializeHealthBar();
	}
	else
	{	
		Tags.Add(FName("Player"));		
	}
	HealthBarWidgetComponent->SetVisibility(false);

	HealthComponent->OnDeathDelegate.AddUObject(this, &ABaseCharacter::OnDeathCallback);

	PerceptionStimuliSourceComponent->RegisterForSense(UAISense_Sight::StaticClass());
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateBody();
	UpdateCameraOffset();
	UpdateHealthBarVisibility();
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseCharacter::InitTeamsVisualSigns(UMaterialInterface* Material)
{
	MeshBody->SetMaterial(1, Material);
}

void ABaseCharacter::RotateBody()
{
	FRotator BodyRotation = { 0.f, GetControlRotation().Yaw -90, 0.f}; // -90 this is the difference in angle due to the incorrect positioning of the model in Blender.
	MeshBody->SetWorldRotation(BodyRotation);
}

bool ABaseCharacter::HandsRotationInRange(const float& LookAtTarget)
{
	double CurrentRotation = R_Hand->GetRelativeRotation().Roll;
	if ((CurrentRotation >= -HandsRotationAngle && LookAtTarget < 0.f) ||
		(CurrentRotation <= HandsRotationAngle && LookAtTarget > 0.f))
	{
		return true;
	}
	return false;
}

void ABaseCharacter::RotateHands(float LookAtTarget)
{
	if (HandsRotationInRange(LookAtTarget))
	{
		FRotator HandsRotation = { 0.f, 0.f,  LookAtTarget };
		R_Hand->AddLocalRotation(HandsRotation);
		L_Hand->AddLocalRotation(HandsRotation);
	}
}

void ABaseCharacter::InitializeHealthBar()
{		
	if (HealthBarWidgetClass == nullptr || HealthBarWidgetComponent == nullptr)
	{	
		return;
	}
	UUserWidget* Widget = CreateWidget<UHealthBarWidget>(UGameplayStatics::GetPlayerController(this, 0), HealthBarWidgetClass);
	HealthBarWidgetComponent->SetWidget(Widget);
	if (UHealthBarWidget* Health = Cast<UHealthBarWidget>(HealthBarWidgetComponent->GetWidget()))
	{
		Health->InitializeWidget(HealthComponent);
	}
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (EventInstigator->ActorHasTag(FName("Player_Controller")) && !ActorHasTag(FName("Player")))
	{
		ShowHealthBarOnDamageTaken();
	}
	
	return 0.0f;
}

void ABaseCharacter::ShowHealthBarOnDamageTaken()
{
	LastTimeDamageTaken = GetWorld()->GetTimeSeconds();
	HealthBarWidgetComponent->SetVisibility(true);
	TakenDamageRecently = true;
}

void ABaseCharacter::UpdateHealthBarVisibility()
{
	if (TakenDamageRecently)
	{
		float CurrentTime = GetWorld()->GetTimeSeconds();
		if (CurrentTime - LastTimeDamageTaken >= TimeToShowHealthOnDamage)
		{
			TakenDamageRecently = false;
			HealthBarWidgetComponent->SetVisibility(false);
		}
		else
		{
			HealthBarWidgetComponent->SetVisibility(true);
		}
	}
}

void ABaseCharacter::SwapWeapon(USkeletalMesh* NewWeaponSkeletalMesh)
{
	R_Weapon->SetSkeletalMesh(NewWeaponSkeletalMesh);
	L_Weapon->SetSkeletalMesh(NewWeaponSkeletalMesh);
}

