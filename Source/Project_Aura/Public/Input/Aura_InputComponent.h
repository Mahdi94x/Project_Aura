// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.

#pragma once

#include "CoreMinimal.h"
#include "Aura_InputConfig.h"
#include "EnhancedInputComponent.h"
#include "Aura_InputComponent.generated.h"

UCLASS()
class PROJECT_AURA_API UAura_InputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UAura_InputConfig* InputConfig, UserClass* Object,PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);	
};

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void UAura_InputComponent::BindAbilityActions(const UAura_InputConfig* InputConfig, UserClass* Object,
		PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	checkf(InputConfig, TEXT("Check the Input Config Data Asset in the blueprint."))
	
	for (const auto& ActionStruct : InputConfig->AbilityInputActionsArray)
	{
		if (ActionStruct.InputAction && ActionStruct.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(ActionStruct.InputAction, ETriggerEvent::Started , Object, PressedFunc, ActionStruct.InputTag);
			}
			if (ReleasedFunc)
			{
				BindAction(ActionStruct.InputAction, ETriggerEvent::Completed , Object, ReleasedFunc, ActionStruct.InputTag);
			}
			if (HeldFunc)
			{
				BindAction(ActionStruct.InputAction, ETriggerEvent::Triggered , Object, HeldFunc, ActionStruct.InputTag);
			}
		}
	}
};