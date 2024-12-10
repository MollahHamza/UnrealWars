#include "YourAICharacter.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

void AYourAICharacter::BeginPlay()
{
    Super::BeginPlay();

    // Start the search when the game begins
    SearchAround();
}

void AYourAICharacter::SearchAround()
{
    // Get the actor's current location
    FVector CurrentLocation = GetActorLocation();

    // Prepare to store the random point in the world
    FVector RandomLocation;

    // Get the navigation system to find a random reachable point
    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    if (NavSystem)
    {
        // Generate a random point within a radius of 2000 units
        NavSystem->GetRandomReachablePointInRadius(CurrentLocation, 2000.0f, RandomLocation);

        // Get the AI Controller
        AAIController* AIController = Cast<AAIController>(GetController());
        if (AIController)
        {
            // Make the AI move to the random location with a 300 acceptance radius
            AIController->MoveToLocation(RandomLocation, 300.0f);
        }
    }

    // Set a delay to call SearchAround again (using a timer)
    GetWorld()->GetTimerManager().SetTimer(SearchHandle, this, &AYourAICharacter::SearchAround, 0.1f, false);
}
// Constructor
AYourCharacter::AYourCharacter()
{
    // Initialize default health
    Health = 100.0f;

    // Setup components (optional depending on your project setup)
    // If using physics on a Mesh or similar component
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);

    CharacterMovement = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("CharacterMovement"));
    CharacterMovement->SetupAttachment(RootComponent);
}



// Function to handle any damage
void AYourCharacter::ReceiveDamage(float DamageAmount)
{
    // Subtract the damage from health
    Health -= DamageAmount;

    // Log the health value (equivalent to the Print String in the Blueprint)
    UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);

    // Check if the health is below or equal to 0
    if (Health <= 0.0f)
    {
        // Health is zero or less, do something (e.g., simulate physics and change movement mode)
        HandleZeroHealth();
    }
}

void AYourCharacter::HandleZeroHealth()
{
    // Simulate physics on the Mesh component
    if (Mesh)
    {
        Mesh->SetSimulatePhysics(true);
    }

    // Log a message for debugging purposes (equivalent to Print String in the Blueprint)
    UE_LOG(LogTemp, Warning, TEXT("Character died, simulating physics and disabling movement."));

    // Set the movement mode to None (disabling movement)
    if (CharacterMovement)
    {
        CharacterMovement->SetMovementMode(EMovementMode::MOVE_None);
    }
}

// Constructor
AYourAICharacter::AYourAICharacter()
{
    // Set up Pawn Sensing component
    PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
    PawnSensingComponent->OnSeePawn.AddDynamic(this, &AYourAICharacter::OnSeePawn);

    // Initialize variables (if any)
}


// Function to handle seeing a Pawn (called when the AI sees another pawn)
void AYourAICharacter::OnSeePawn(APawn* Pawn)
{
    // Cast to the character (similar to Cast to BP_ThirdPersonCharacter in Blueprint)
    ACharacter* PlayerCharacter = Cast<ACharacter>(Pawn);
    if (PlayerCharacter)
    {
        // Log a message (similar to Print String "Hello")
        UE_LOG(LogTemp, Warning, TEXT("Hello, Player Seen!"));

        // Call the chase function to start moving toward the player
        ChasePlayer(PlayerCharacter);
    }
}

// Custom event "Chase" - Starts chasing the detected player
void AYourAICharacter::ChasePlayer(ACharacter* TargetActor)
{
    // Get the AI controller for movement
    AAIController* AIController = Cast<AAIController>(GetController());
    if (AIController && TargetActor)
    {
        // Use AI MoveTo function to move toward the player
        AIController->MoveToActor(TargetActor, 300.0f);  // Acceptance radius of 300 units

        // After moving, perform the next action (like the Fire event in Blueprint)
        FireAtPlayer(TargetActor);
    }
}

// Custom function "Fire" - Can be an attack or interaction
void AYourAICharacter::FireAtPlayer(ACharacter* TargetActor)
{
    if (TargetActor)
    {
        // Log a message to simulate the "Fire" event
        UE_LOG(LogTemp, Warning, TEXT("Firing at Player!"));

        // Additional logic to perform any attack or other interaction goes here
        // (e.g., dealing damage, playing animations, etc.)
    }
}