// Project by Mahdi94x based on Stephen Ulibarri's create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS) Course.


#include "UI/Widget/Aura_UserWidget.h"

void UAura_UserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
