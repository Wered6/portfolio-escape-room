# EscapeRoom

FPP Escape Room game made with Unreal Engine 5.5

# Introduction

Escape room is a project primarily made for class, but it is evolving into my first portfolio entry.

## 🛠️ Key Mechanics Implemented

| Mechanic                                                                          | Description                                                     |
|-----------------------------------------------------------------------------------|-----------------------------------------------------------------|
| [Interact System](#interact-system) ([code](Source/EscapeRoom/InteractionSystem)) | Core interaction system                                         |
| Lock-Key System (to be documented)                                                | Lock and key mechanic with keys and matching locks              |
| UV Flashlight (to be documented)                                                  | Reveals hidden clues or writings when aimed at certain surfaces |
| Interactive Keypads (to be documented)                                            | Custom keypads for puzzles with code input and validation logic |
| TV System (to be documented)                                                      | Plays movies or switches to interactive widgets                 |

# Interact System

I implemented a designer-friendly customizable FPP interaction system based on line trace, fully usable in both C++ and Blueprints.

<details>
<summary>More</summary>

# Video

[![Watch the video](https://img.youtube.com/vi/LXiYMMlcryQ/maxresdefault.jpg)](https://www.youtube.com/watch?v=LXiYMMlcryQ)

![Interact1](https://github.com/user-attachments/assets/770a5737-f053-4986-9b25-15f35b4e0b66)
![Interact2](https://github.com/user-attachments/assets/77eefc8d-95aa-45d8-9f51-31de5e766125)
![Interact3](https://github.com/user-attachments/assets/dbfd446c-01d0-44d5-a0e0-ca8ef3ca07a4)

# Implementations

- [**Interact component**](#interact-component-code) ([code](Source/EscapeRoom/InteractionSystem/ERInteractComponent.h))
    - [Enhanced input](#enhanced-input)
- [**Interactable base classes**](#interactable-base-classes) (actor [code](Source/EscapeRoom/InteractionSystem/ERInteractableActorBase.h),
  pawn [code](Source/EscapeRoom/InteractionSystem/ERInteractablePawnBase.h),
  character [code](Source/EscapeRoom/InteractionSystem/ERInteractableCharacterBase.h)) - their implementations are basically the same thanks
  to wrapping logic into component
    - [Interactable component](#interactable-component-code) ([code](Source/EscapeRoom/InteractionSystem/ERInteractableComponent.h))
        - [Outline](#outline-logic)
        - [Interact area](#interact-area-logic)
        - [Widget attachment](#widget-attachment-logic)
    - [Interact interface](#interact-interface-code) ([code](Source/EscapeRoom/InteractionSystem/ERInteractInterface.h))
- [**Interact icon widget**](#interact-icon-widget-code) ([code](Source/EscapeRoom/InteractionSystem/ERInteractIconWidget.h))
    - [Progress circle](#progress-circle-code)

# Interact component ([code](Source/EscapeRoom/InteractionSystem/ERInteractComponent.h))

It's a component that allows the player to interact. All the necessary logic is encapsulated within it.  
It's easy to add to C++ class or Blueprint class.

<details>
<summary>How to use</summary>

***C++***  
.h

```cpp
UPROPERTY(VisibleAnywhere)
TObjectPtr<UERInteractComponent> InteractComponent;
```

constructor

```cpp
InteractComponent = CreateDefaultSubobject<UERInteractComponent>(TEXT("InteractComponent"));
```

***Blueprints***  
![image](https://github.com/user-attachments/assets/ed71782c-2208-4374-b5a4-bb7e9a327dc1)

In both scenarios its works after setting up IMC and Input Actions.  
![image](https://github.com/user-attachments/assets/dce343fc-4d72-459d-80c9-4657aeb85685)

</details>

## Enhanced input

I made this system with enhanced input system's capabilities in mind. So there are two types of interaction: press and hold.

<details>
<summary>More</summary>

The "Press" input action has a *Pressed* trigger, so it triggers only once.

![image](https://github.com/user-attachments/assets/eec78740-9d52-469f-b4e0-5092c3d70ddc)

Hold input action has *Hold* trigger and `OneShot` marked, so after `HoldTimeThreshold` it triggers only once.

![image](https://github.com/user-attachments/assets/682a9e47-b7b8-4e3a-b24f-d5982b3dc4bf)

Thanks to this trigger we can visualize it using [Progress circle](#progress-circle-code).

</details>

# Interactable base classes

I combined interface, component, and inheritance to create three base classes for actors, pawns, and characters. I'm aware it's not a
perfect solution, mainly due to repetitive code in those classes and the existence of two ways to check if an object is interactable - via
the base class and via the interface. However, it's the best idea I came up with. Thanks to wrapping the logic into
the [interactable component](#interactable-component-code), repetitive code is reduced, and any modifications can be made directly in the
interactable component.

<details>
<summary>How to use</summary>

***C++***  
.h

```cpp
UCLASS()
class ESCAPEROOM_API AKey : public AERInteractableActorBase

...

UCLASS()
class ESCAPEROOM_API AKeypad : public AERInteractablePawnBase

...

UCLASS()
class ESCAPEROOM_API ANPC : public AERInteractableCharacterBase
```

***Blueprints***  
![image](https://github.com/user-attachments/assets/b1c97b89-6c7d-410a-8277-f43fa91dbb37)

</details>

## Interactable component ([code](Source/EscapeRoom/InteractionSystem/ERInteractableComponent.h))

It's tightly connected with [interact interface](#interact-interface-code). In blueprint you have to select Interact Widget Class. Thanks to
this component we can easily change properties via C++ or inside blueprint details.

<details>
<summary>How to use</summary>

***C++***

constructor

```cpp
InteractableComp->InteractCategory = EERInteractCategory::Collect;
InteractableComp->InteractType = EERInteractType::Hold;
InteractableComp->IconSize = FVector2D(25.f, 25.f);
InteractableComp->InitialIconOpacity = 0.3f;
InteractableComp->InitialProgressCircleOpacity = 0.f;
InteractableComp->ProgressCircleSize = FVector2D(50.f, 50.f);
InteractableComp->HoldTimeThreshold = 1.f;
InteractableComp->bCanInteract = true;
InteractableComp->bUseCustomInteractArea = true;
```

***Blueprints***  
![image](https://github.com/user-attachments/assets/2b9d2281-8d45-4e90-9883-88fbc9544998)

In both scenarios you have to set Interact Widget Class.

</details>

### Outline ([logic](Source/EscapeRoom/InteractionSystem/ERInteractableComponent.cpp#L50))

To outline meshes I used Outline material, which I added to Post Process Volume materials and change `Custom Depth-Stencil Pass` in Project
Settings to `Enabled with Stencil`.  
To select which mesh should outline we have to add them to array `OutlineMeshComps` (we can add many).

<details>
<summary>Screens</summary>

![image](https://github.com/user-attachments/assets/83057750-b73d-4b13-ac54-3cc2dceed0f4)  
![image](https://github.com/user-attachments/assets/ffaa3e7c-72c8-4841-8622-336e2f999770)  
![image](https://github.com/user-attachments/assets/1ebf220d-6579-487f-a011-a86e9cd3b102)

</details>

<details>
<summary>How to use</summary>

***C++***  
constructor or begin play

```cpp
InteractableComp->AddOutlineMeshComponent(KeyMesh);

...

InteractableComp->SetOutlineMeshComponents(OutlineMeshesArray);
```

***Blueprints***  
<img src="https://github.com/user-attachments/assets/9ae56ca0-2b4b-4082-b8fe-779e59d85b76" width="800">  
<img src="https://github.com/user-attachments/assets/1028a9e1-74c9-4eca-85ca-f4abda771bfb" width="800">

</details>

### Interact area ([logic](Source/EscapeRoom/InteractionSystem/ERInteractComponent.cpp#L249))

By default interact area is whole object. We can adjust that by setting `bUseCustomInteractArea` to `true`. After this we have to add
collision with collision preset `InteractArea` and we can adjust its attachment, size and transform.

<details>
<summary>How to use</summary>

***C++***  
.h

```cpp
UPROPERTY(VisibleAnywhere)
TObjectPtr<UBoxComponent> InteractBox;
```

constructor

```cpp
InteractableComp->bUseCustomInteractArea = true;
InteractBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractBox"));
InteractBox->SetCollisionProfileName(TEXT("InteractArea"));
```

***Blueprints***

<img src="https://github.com/user-attachments/assets/1162e3b6-85ea-4bfa-9d88-35f999d0aa18" width="600">
<img src="https://github.com/user-attachments/assets/2447c27d-534b-4dfe-a0d9-14a003e4e0f2" width="600">
<img src="https://github.com/user-attachments/assets/07495b2e-671a-4708-821e-56e286fe626a" width="600">

</details>

### Widget attachment ([logic](Source/EscapeRoom/InteractionSystem/ERInteractableComponent.cpp#L216))

By default widget is attached to the root of object. We can adjust that by overriding [interact interface](#interact-interface-code)
function - `GetWidgetAttachmentComponent`. Thanks to this we can reattach and retransform widget attachment as we wish.

<details>
<summary>How to use</summary>

***C++***  
.h

```cpp
UPROPERTY(VisibleAnywhere)
TObjectPtr<USceneComponent> WidgetAttachment;
```

constructor

```cpp
WidgetAttachment = CreateDefaultSubobject<USceneComponent>(TEXT("WidgetAttachment"));
WidgetAttachment->SetupAttachment(KeyMesh);
```

***Blueprints***

<img src="https://github.com/user-attachments/assets/88df66d3-6728-4430-bd24-57718d0f86a9" width="600">
<img src="https://github.com/user-attachments/assets/ab6a6f0a-0e21-425b-a157-cb1b20d7c62b" width="600">
<img src="https://github.com/user-attachments/assets/2db82a32-360b-4012-a116-463b440ac888" width="600">

</details>

## Interact interface ([code](Source/EscapeRoom/InteractionSystem/ERInteractInterface.h))

The `Interact interface` is present in all base classes, so we don't have to implement it ourselves.
There are several functions, but some are only meant to be overridden, while others have their basic implementation
in [Interactable Component](#interactable-component-code).

<details>
<summary>Can be overriden</summary>

| Function                                                                                      | Description                                                                                                                                                                   |
|-----------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| [DisplayInteractionUI](Source/EscapeRoom/InteractionSystem/ERInteractableComponent.cpp#L38)   | has basic logic for showing widget and outline correct meshes, called when looking at interactable object                                                                     |
| GetWidgetAttachment                                                                           | has NO basic logic, override it to provide custom widget location/attachment                                                                                                  |
| [InteractPressStarted](Source/EscapeRoom/InteractionSystem/ERInteractableComponent.cpp#L60)   | has basic logic for setting interact instigator, called as soon as we press interaction key                                                                                   |
| InteractPressTriggered                                                                        | has NO basic logic, override it to provide interaction logic, called after InteractPressStarted                                                                               |
| [InteractPressCompleted](Source/EscapeRoom/InteractionSystem/ERInteractableComponent.cpp#L71) | has basic logic for resetting interact instigator, called after InteractPressTriggered                                                                                        |
| [InteractHoldStarted](Source/EscapeRoom/InteractionSystem/ERInteractableComponent.cpp#L77)    | has basic logic for setting interact instigator and setting holding logic, called as soon as we press interaction key                                                         |
| [InteractHoldOngoing](Source/EscapeRoom/InteractionSystem/ERInteractableComponent.cpp#L94)    | has basic logic for interpolating widget visuals, called every frame for [HoldTimeTimeThreshold](Source/EscapeRoom/InteractionSystem/ERInteractableComponent.h#L83)           |
| InteractHoldTriggered                                                                         | has NO basic logic, override it to provide interaction logic, called after [HoldTimeTimeThreshold](Source/EscapeRoom/InteractionSystem/ERInteractableComponent.h#L83)         |
| [InteractHoldCanceled](Source/EscapeRoom/InteractionSystem/ERInteractableComponent.cpp#L116)  | has basic logic for resetting interact instigator and enables widget visuals to fade down, called when we stop holding key and InteractHoldTriggered was never getting called |
| [InteractHoldCompleted](Source/EscapeRoom/InteractionSystem/ERInteractableComponent.cpp#L131) | has basic logic for resetting interact instigator and enables widget visuals to fade down, called after successful hold after InteractHoldTriggered                           |

We can override them in C++ and Blueprints.  
*Note:* Do **NOT** forget to call parent/super to preserve the base class logic.

***C++***  
.h

```cpp
virtual void InteractHoldTriggered_Implementation() override;
```

.cpp

```cpp
void AERKey::InteractHoldTriggered_Implementation()
{
  Super::InteractHoldTriggered_Implementation();
  
  // logic
}
```

***Blueprints***  
![image](https://github.com/user-attachments/assets/71406912-cdd8-495e-8533-5d5bbaf2e8f2)

</details>

<details>
<summary>Meant only to be called</summary>

| Function                                                                                          | Description                                                                                                            |
|---------------------------------------------------------------------------------------------------|------------------------------------------------------------------------------------------------------------------------|
| [DoesUseCustomInteractArea](Source/EscapeRoom/InteractionSystem/ERInteractableComponent.cpp#L146) | determines usage of custom interact area ([usage](Source/EscapeRoom/InteractionSystem/ERInteractComponent.cpp#L250))   |
| [SetCanInteract](Source/EscapeRoom/InteractionSystem/ERInteractableComponent.cpp#L151)            | determines intractability, can be used to prevent further interaction or enable/disable interaction in specific moment |
| [GetCanInteract](Source/EscapeRoom/InteractionSystem/ERInteractableComponent.cpp#L156)            | returns if can interact ([usage](Source/EscapeRoom/InteractionSystem/ERInteractComponent.cpp#L243))                    |
| [GetInteractType](Source/EscapeRoom/InteractionSystem/ERInteractableComponent.cpp#L161)           | returns interact type ([usage](Source/EscapeRoom/InteractionSystem/ERInteractComponent.cpp#L101))                      |

</details>

# Interact icon widget ([code](Source/EscapeRoom/InteractionSystem/ERInteractIconWidget.h))

Widget that is attached to every interactable object. Based on `InteractCategory` and `InteractType` it switches icons and
shows/hides [progress circle](#progress-circle-code) accordingly.  
We setting initial values thanks to [Init](Source/EscapeRoom/InteractionSystem/ERInteractIconWidget.cpp#L81)
function - [usage](Source/EscapeRoom/InteractionSystem/ERInteractableComponent.cpp#L203).

<details>
<summary>Screens</summary>

<img src="https://github.com/user-attachments/assets/926617e0-929a-4ee4-9410-863f7832a56c" width="150">
<img src="https://github.com/user-attachments/assets/5e9be686-01f7-462d-83ce-8ce98bd0573a" width="600"><br>
<img src="https://github.com/user-attachments/assets/54e41b5d-e39a-420a-9a7e-11e6f2b81b2d" width="150">
<img src="https://github.com/user-attachments/assets/0550f40a-ede3-4dd9-8ede-4a3fb1e5cd7f" width="600"><br>

</details>

## Progress circle ([code](Source/EscapeRoom/InteractionSystem/ERProgressCircle.h))

<details>
<summary>More</summary>

![image](https://github.com/user-attachments/assets/08de9d83-2dc7-413e-84c0-19057ccffb4a)  
Thanks to [this](https://www.youtube.com/watch?v=BgOAbAdi8f0) tutorial I made progress circle material and then widget with some adjustment
to my preferences.
![image](https://github.com/user-attachments/assets/7e8476f6-1beb-4437-923a-b571eb0d112d)  
In case screen is not readable enough - [here](https://blueprintue.com/render/n14dzb06/) is link to bluprintue.

</details>

</details>
