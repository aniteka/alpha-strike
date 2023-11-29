

#include "Character/BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include <Kismet/GameplayStatics.h>
#include "UI/Widgets/HealthBarWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/WeaponComponent.h"
#include "Components/HealthComponent.h"


ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;	

	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar Widget Component"));
	HealthBarWidgetComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	MeshBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Body"));
	MeshBody->SetupAttachment(GetMesh());
	MeshBody->SetupAttachment(GetMesh(), FName(TEXT("Body_Soket")));	

	R_Hand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Hand"));
	R_Hand->SetupAttachment(MeshBody);

	L_Hand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Hand"));
	L_Hand->SetupAttachment(MeshBody);

	ArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	ArmComponent->bUsePawnControlRotation = false;
	ArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(ArmComponent);
}

void ABaseCharacter::UpdateCameraOffset()
{
	ArmComponent->SetRelativeRotation({ -R_Hand->GetRelativeRotation().Roll, MeshBody->GetComponentRotation().Pitch,0.f });
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializeHealthBar();
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateBody();
	UpdateCameraOffset();
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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
	UE_LOG(LogTemp, Warning, TEXT("Start initialize Widget from Character"));
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

