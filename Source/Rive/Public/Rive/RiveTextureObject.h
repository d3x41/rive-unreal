// Copyright Rive, Inc. All rights reserved.

#pragma once

#include "IRiveRenderTarget.h"
#include "RiveDescriptor.h"
#include "RiveTexture.h"
#include "RiveTypes.h"
#include "Tickable.h"
#include "RiveTextureObject.generated.h"

#if WITH_RIVE

class IRiveRenderer;
struct FAssetImportInfo;
class URiveArtboard;
class FRiveTextureResource;

namespace rive
{
	class File;
}

#endif // WITH_RIVE

class URiveAsset;
class UUserWidget;
class URiveFile;

/**
 * This class represents the logical side of a single RiveTexture / RenderTarget. It implements the logic to instantiate and tick an artboard passed into it with a RiveDescriptor.
 */
UCLASS(BlueprintType, Blueprintable, HideCategories=("ImportSettings", "Compression", "Adjustments", "LevelOfDetail", "Compositing"))
class RIVE_API URiveTextureObject : public URiveTexture, public FTickableGameObject
{
	GENERATED_BODY()
	DECLARE_MULTICAST_DELEGATE(FRiveReadyDelegate)


public:
	/**
	 * Structor(s)
	 */
	
	URiveTextureObject();
	
	virtual void BeginDestroy() override;
	void PostLoad() override;
	//~ BEGIN : FTickableGameObject Interface

public:
	virtual TStatId GetStatId() const override;

	virtual void Tick(float InDeltaSeconds) override;

	virtual bool IsTickable() const override
	{
		return !HasAnyFlags(RF_ClassDefaultObject) && bIsRendering;
	}

	virtual bool IsTickableInEditor() const override
	{
		return !HasAnyFlags(RF_ClassDefaultObject) && bIsRendering && bRenderInEditor;
	}

	virtual ETickableTickType GetTickableTickType() const override
	{
		return ETickableTickType::Conditional;
	}

	//~ END : FTickableGameObject Interface

	/**
	 * Implementation(s)
	 */

public:

	UFUNCTION(BlueprintPure, Category = Rive)
	FLinearColor GetClearColor() const;

	UFUNCTION(BlueprintCallable, Category = Rive)
	FVector2f GetLocalCoordinate(URiveArtboard* InArtboard, const FVector2f& InPosition);

	/**
	 * Returns the coordinates in the current Artboard space
	 * @param InExtents Extents of the RenderTarget, will be mapped to the RenderTarget size
	 */
	UFUNCTION(BlueprintCallable, Category = Rive)
	FVector2f GetLocalCoordinatesFromExtents(const FVector2f& InPosition, const FBox2f& InExtents) const;
	
	UFUNCTION(BlueprintCallable, Category=Rive)
	void Initialize(const FRiveDescriptor& InRiveDescriptor);

	UFUNCTION(BlueprintCallable, Category = Rive)
	URiveArtboard* GetArtboard() const;
	
	FRiveReadyDelegate OnRiveReady;
	
protected:

	void RiveReady(IRiveRenderer* InRiveRenderer);
	void OnResourceInitialized_RenderThread(FRHICommandListImmediate& RHICmdList, FTextureRHIRef& NewResource) const;

public:
	UPROPERTY(EditAnywhere, Transient, Category = Rive)
	bool bIsRendering = false;
	
#if WITH_EDITOR
	bool bRenderInEditor = false;
#endif
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Rive)
	FRiveDescriptor RiveDescriptor;
private:
	UFUNCTION()
	void OnArtboardTickRender(float DeltaTime, URiveArtboard* InArtboard);
	
	UPROPERTY(EditAnywhere, Category = Rive)
	FLinearColor ClearColor = FLinearColor::Transparent;
	
	TSharedPtr<IRiveRenderTarget> RiveRenderTarget;

	UPROPERTY(Transient, BlueprintReadOnly, Category=Rive, meta=(NoResetToDefault, AllowPrivateAccess, ShowInnerProperties))
	URiveArtboard* Artboard = nullptr;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category=Rive, meta=(AllowPrivateAccess))
	URiveAudioEngine* AudioEngine = nullptr;
	FDelegateHandle AudioEngineLambdaHandle; 
};
