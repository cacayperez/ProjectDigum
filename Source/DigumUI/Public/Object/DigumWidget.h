// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DigumWidget.generated.h"

class SDigumWidget;

USTRUCT(BlueprintType)
struct FDigumWidgetParameters
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Digum Widget")
	FText TitleName;
	
	UPROPERTY(EditAnywhere, Category = "Digum Widget")
	FVector2D Size;
};

/**
 * 
 */
UCLASS(Blueprintable)
class DIGUMUI_API UDigumWidget : public UObject
{
	GENERATED_BODY()
	
	TSharedPtr<SDigumWidget> Widget;
	
public:
	UDigumWidget();
	~UDigumWidget();
	
protected:
	UPROPERTY(EditAnywhere, Category = "Digum Widget")
	UMaterialInterface* BackgroundMaterial;
	
	UPROPERTY(EditAnywhere, Category = "Digum Widget")
	FDigumWidgetParameters ConstructParameters;
	
public:
	virtual void OnCreateWidget();
	virtual TSharedPtr<SDigumWidget> GetWidget() const;

	template<typename T>
	static T* Create(UClass* ClassType = nullptr);
	
	template<typename T>
	static T* Create(UObject* Outer, UClass* ClassType = nullptr);
};

template <typename T>
T* UDigumWidget::Create(UClass* ClassType)
{
	static_assert(std::is_base_of<UDigumWidget, T>::value, "T must be a derived class of UDigumWidget");
	T* NewWidget = nullptr;
	
	if(ClassType)
	{
		NewWidget = NewObject<T>(ClassType);
	}
	else
	{
		NewWidget = NewObject<T>();
	}

	if(NewWidget)
		NewWidget->OnCreateWidget();

	return NewWidget;
}

template <typename T>
T* UDigumWidget::Create(UObject* Outer, UClass* ClassType)
{
	static_assert(std::is_base_of<UDigumWidget, T>::value, "T must be a derived class of UDigumWidget");
	T* NewWidget = NewObject<T>(Outer);
	NewWidget->OnCreateWidget();

	if(ClassType)
	{
		NewWidget = NewObject<T>(Outer, ClassType);
	}
	else
	{
		NewWidget = NewObject<T>(Outer);
	}

	if(NewWidget)
		NewWidget->OnCreateWidget();

	return NewWidget;
}
