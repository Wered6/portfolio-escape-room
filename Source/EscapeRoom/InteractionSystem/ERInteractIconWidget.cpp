#include "ERInteractIconWidget.h"
#include "ERInteractInterface.h"
#include "ERProgressCircle.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "EscapeRoom/Utility/WeredMacros.h"


void UERInteractIconWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	UVALID_LOG_DEBUG(IconSwitcher)
	UVALID_LOG_DEBUG(CollectIcon)
	UVALID_LOG_DEBUG(OpenIcon)
	UVALID_LOG_DEBUG(UseIcon)
	UVALID_LOG_DEBUG(UnlockIcon)
	UVALID_LOG_DEBUG(ProgressCircle)

	SetInteractCategory(InteractCategory);

	switch (InteractType)
	{
	case EERInteractType::Press:
		ProgressCircle->SetVisibility(ESlateVisibility::Collapsed);
		SetIconOpacity(1.f);
		break;
	case EERInteractType::Hold:
		ProgressCircle->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SetIconOpacity(0.f);
		SetProgressCircleOpacity(0.f);
		SetProgressCirclePercent(0.f);
		SetProgressCircleSize(ProgressCircleSize);
		break;
	}

	SetIconSize(IconSize);
}

void UERInteractIconWidget::Init(const EERInteractCategory NewInteractCategory,
                                 const EERInteractType NewInteractType,
                                 const float NewMinimalIconOpacity,
                                 const FVector2D NewIconSize,
                                 const float NewMinimalProgressCircleOpacity,
                                 const FVector2D NewProgressCircleSize)
{
	InteractCategory = NewInteractCategory;
	InteractType = NewInteractType;
	MinimalIconOpacity = NewMinimalIconOpacity;
	IconSize = NewIconSize;
	MinimalProgressCircleOpacity = NewMinimalProgressCircleOpacity;
	ProgressCircleSize = NewProgressCircleSize;
}

void UERInteractIconWidget::SetInteractCategory(const EERInteractCategory InInteractCategory)
{
	switch (InInteractCategory)
	{
	case EERInteractCategory::Use:
		IconSwitcher->SetActiveWidget(UseIcon);
		break;
	case EERInteractCategory::Collect:
		IconSwitcher->SetActiveWidget(CollectIcon);
		break;
	case EERInteractCategory::Open:
		IconSwitcher->SetActiveWidget(OpenIcon);
		break;
	case EERInteractCategory::Unlock:
		IconSwitcher->SetActiveWidget(UnlockIcon);
		break;
	}
}

void UERInteractIconWidget::SetIconSize(const FVector2D Size) const
{
	UVALID_LOG_DEBUG(IconSwitcher)

	for (UWidget* Icon : IconSwitcher->GetAllChildren())
	{
		if (UImage* ImageIcon{Cast<UImage>(Icon)})
		{
			ImageIcon->SetDesiredSizeOverride(Size);
		}
	}
}

void UERInteractIconWidget::SetIconOpacity(const float Opacity)
{
	UVALID_LOG_DEBUG(IconSwitcher)

	const float ClampedOpacity{FMath::Clamp(Opacity, 0.f, 1.f)};
	CurrentIconOpacity = FMath::Lerp(MinimalIconOpacity, 1.f, ClampedOpacity);

	IconSwitcher->SetRenderOpacity(CurrentIconOpacity);
}

void UERInteractIconWidget::SetProgressCircleSize(const FVector2D Size) const
{
	UVALID_LOG_DEBUG(ProgressCircle)

	ProgressCircle->SetImageSize(Size);
}

void UERInteractIconWidget::SetProgressCircleOpacity(const float Opacity)
{
	UVALID_LOG_DEBUG(ProgressCircle)

	const float ClampedOpacity{FMath::Clamp(Opacity, 0.f, 1.f)};
	CurrentProgressCircleOpacity = FMath::Lerp(MinimalProgressCircleOpacity, 1.f, ClampedOpacity);
	ProgressCircle->SetOpacity(CurrentProgressCircleOpacity);
}

void UERInteractIconWidget::SetProgressCirclePercent(const float Percent)
{
	UVALID_LOG_DEBUG(ProgressCircle)

	const float ClampedPercent{FMath::Clamp(Percent, 0.f, 1.f)};
	CurrentProgressCirclePercent = ClampedPercent;
	ProgressCircle->SetPercent(CurrentProgressCirclePercent);
}
