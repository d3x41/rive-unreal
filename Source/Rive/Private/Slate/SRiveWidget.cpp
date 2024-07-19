// Copyright Rive, Inc. All rights reserved.

#include "Slate/SRiveWidget.h"

#include "ImageUtils.h"
#include "Rive/RiveTextureObject.h"

#if WITH_EDITOR
#include "TextureEditorSettings.h"
#endif

namespace UE::Private::SRiveWidget
{
	FSlateBrush* CreateTransparentBrush()
	{
		FSlateBrush* TransparentBrush = new FSlateBrush();
		TransparentBrush->DrawAs = ESlateBrushDrawType::NoDrawType;
		return TransparentBrush;
	}
}

void SRiveWidget::Construct(const FArguments& InArgs)
{
#if WITH_EDITOR
	ModifyCheckerboardTextureColors();
#endif
	
	ChildSlot
	[
		SNew(SOverlay)
#if WITH_EDITOR
		+ SOverlay::Slot() [
			SNew( SImage )
			.Visibility( InArgs._bDrawCheckerboardInEditor ? EVisibility::Visible : EVisibility::Collapsed )
			.Image(CheckerboardBrush)
		]
#endif
		+ SOverlay::Slot() [
			SAssignNew(RiveImageView, SImage)
			.Image(UE::Private::SRiveWidget::CreateTransparentBrush()) 	// Start with a transparent brush, which will be swapped out later
		]
	];
}

void SRiveWidget::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
	SCompoundWidget::OnArrangeChildren(AllottedGeometry, ArrangedChildren);

	FVector2D CurrentSize = AllottedGeometry.GetLocalSize();
	if (PreviousSize != CurrentSize)
	{
		PreviousSize = CurrentSize;

		UWorld* World = GetWorld();

		if (World)
		{
			World->GetTimerManager().ClearTimer(TimerHandle);
			World->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateSP(this, &SRiveWidget::OnResize), 0.025f, false);
		}
	}
}

void SRiveWidget::SetRiveTexture(URiveTexture* InRiveTexture)
{
	if (RiveImageView)
	{
		RiveTexture = InRiveTexture;

		if (InRiveTexture == nullptr)
		{
			RiveImageView->SetImage(UE::Private::SRiveWidget::CreateTransparentBrush());
			RiveTextureBrush.Reset();
		} else
		{
			RiveTextureBrush = MakeShareable(new FSlateBrush());
			RiveTextureBrush->DrawAs = ESlateBrushDrawType::Image;
			RiveTextureBrush->TintColor = FSlateColor(FLinearColor::White);
			RiveTextureBrush->SetResourceObject(RiveTexture);
			RiveImageView->SetImage(RiveTextureBrush.Get());
		}
	}
}

void SRiveWidget::OnResize() const
{
	if (RiveTextureBrush && RiveTexture)
	{
		RiveTexture->ResizeRenderTargets(FIntPoint(PreviousSize.X, PreviousSize.Y));
	}
}

UWorld* SRiveWidget::GetWorld() const
{
#if WITH_EDITOR
	if (GEditor && GEditor->PlayWorld)
	{
		return GEditor->PlayWorld;
	}
	
	if (GEditor && GEditor->GetEditorWorldContext().World())
	{
		return GEditor->GetEditorWorldContext().World();
	}
#endif

	if (GEngine && GEngine->GetWorld())
	{
		return GEngine->GetWorld();
	}

	return nullptr;
}

#if WITH_EDITOR
void SRiveWidget::ModifyCheckerboardTextureColors()
{
	DestroyCheckerboardTexture();

	const UTextureEditorSettings& Settings = *GetDefault<UTextureEditorSettings>();
	CheckerboardTexture = FImageUtils::CreateCheckerboardTexture(Settings.CheckerColorOne, Settings.CheckerColorTwo, Settings.CheckerSize);
	CheckerboardTexture->AddToRoot();
	
	CheckerboardBrush = new FSlateBrush();
	CheckerboardBrush->DrawAs = ESlateBrushDrawType::Image;
	CheckerboardBrush->Tiling = ESlateBrushTileType::Both;
	CheckerboardBrush->SetResourceObject(CheckerboardTexture);
}

void SRiveWidget::DestroyCheckerboardTexture()
{
	if (IsValid(CheckerboardTexture))
	{
		CheckerboardBrush = nullptr;
		CheckerboardTexture->ReleaseResource();
		CheckerboardTexture->RemoveFromRoot();
		CheckerboardTexture->MarkAsGarbage();
		CheckerboardTexture = nullptr;
	}
}
#endif
