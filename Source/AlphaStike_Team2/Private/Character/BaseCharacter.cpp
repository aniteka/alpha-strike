

#include "Character/BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/WeaponComponent.h"


ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	ArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	ArmComponent->bUsePawnControlRotation = true;
	ArmComponent->SetupAttachment(GetRootComponent());
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(ArmComponent);

	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));

	MeshBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Body"));
	MeshBody->SetupAttachment(GetMesh());
	MeshBody->SetupAttachment(GetMesh(), FName(TEXT("Body_Soket")));

	R_Hand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Hand"));
	R_Hand->SetupAttachment(MeshBody);

	L_Hand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Hand"));
	L_Hand->SetupAttachment(MeshBody);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	//bUseControllerRotationYaw = false;
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateBody();
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseCharacter::RotateBody()
{
	FRotator BodyRotation = { 0.f, ArmComponent->GetComponentRotation().Yaw - 90,0.f }; // -90 this is the difference in angle due to the incorrect positioning of the model in Blender.
	MeshBody->SetWorldRotation(BodyRotation);
}

void ABaseCharacter::RotateHands(float LookAtTarget)
{
	/*double deltaAngle = LookAtTarget - R_Hand->GetRelativeRotation().Pitch;
	
	R_Hand->AddRelativeRotation({ deltaAngle,0,0 });
	L_Hand->AddRelativeRotation({ deltaAngle,0,0 });*/

	FRotator HandsRotation = { ArmComponent->GetComponentRotation().Pitch, 0.f, 0.f };
	R_Hand->SetRelativeRotation(HandsRotation);
	L_Hand->SetRelativeRotation(HandsRotation);
}

