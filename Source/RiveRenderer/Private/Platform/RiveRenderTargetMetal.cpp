// Copyright Rive, Inc. All rights reserved.

#include "Platform/RiveRenderTargetMetal.h"

#if PLATFORM_APPLE

#include "DynamicRHI.h"
#include "Engine/Texture2DDynamic.h"
#include "Logs/RiveRendererLog.h"
#include "RiveRenderer.h"
#include <Metal/Metal.h>

#if WITH_RIVE
#include "RiveCore/Public/PreRiveHeaders.h"
THIRD_PARTY_INCLUDES_START
#include "rive/artboard.hpp"
#include "rive/pls/pls_renderer.hpp"
#include "rive/pls/metal/pls_render_context_metal_impl.h"
THIRD_PARTY_INCLUDES_END
#endif // WITH_RIVE

UE::Rive::Renderer::Private::FRiveRenderTargetMetal::FRiveRenderTargetMetal(const TSharedRef<FRiveRenderer>& InRiveRenderer, const FName& InRiveName, UTexture2DDynamic* InRenderTarget)
    : FRiveRenderTarget(InRiveRenderer, InRiveName, InRenderTarget)
{
}

UE::Rive::Renderer::Private::FRiveRenderTargetMetal::~FRiveRenderTargetMetal()
{
    RIVE_DEBUG_FUNCTION_INDENT
    CachedPLSRenderTargetMetal.release();
}

DECLARE_GPU_STAT_NAMED(CacheTextureTarget, TEXT("FRiveRenderTargetMetal::CacheTextureTarget_RenderThread"));
void UE::Rive::Renderer::Private::FRiveRenderTargetMetal::CacheTextureTarget_RenderThread(FRHICommandListImmediate& RHICmdList, const FTexture2DRHIRef& InTexture)
{
    check(IsInRenderingThread());
    
    FScopeLock Lock(&RiveRenderer->GetThreadDataCS());
    
#if WITH_RIVE
    
    rive::pls::PLSRenderContext* PLSRenderContext = RiveRenderer->GetPLSRenderContextPtr();
    
    if (PLSRenderContext == nullptr)
    {
        return;
    }
    
#endif // WITH_RIVE
    
    // TODO, make sure we have correct texture DXGI_FORMAT_R8G8B8A8_UNORM
    EPixelFormat PixelFormat = InTexture->GetFormat();
    
    if (PixelFormat != PF_R8G8B8A8)
    {
        return;
    }
    
    SCOPED_GPU_STAT(RHICmdList, CacheTextureTarget);

    const bool bIsValidRHI = GDynamicRHI != nullptr && GDynamicRHI->GetInterfaceType() == ERHIInterfaceType::Metal;

    if (bIsValidRHI && InTexture.IsValid())
    {        
        // Get the underlying metal texture so we can render to it.
        id<MTLTexture> MetalTexture = (id<MTLTexture>)InTexture->GetNativeResource();
        
        check(MetalTexture);
        
        UE_LOG(LogRiveRenderer, Warning, TEXT("MetalTexture texture %dx%d"), MetalTexture.width, MetalTexture.height);

#if WITH_RIVE
        if (CachedPLSRenderTargetMetal)
        {
            CachedPLSRenderTargetMetal.release();
        }
        
        // For now we just set one renderer and one texture
        rive::pls::PLSRenderContextMetalImpl* const PLSRenderContextMetalImpl = PLSRenderContext->static_impl_cast<rive::pls::PLSRenderContextMetalImpl>();
        
        CachedPLSRenderTargetMetal = PLSRenderContextMetalImpl->makeRenderTarget(MetalTexture.pixelFormat, MetalTexture.width, MetalTexture.height);
        
        CachedPLSRenderTargetMetal->setTargetTexture(MetalTexture);
        
#endif // WITH_RIVE
    }
}

#if WITH_RIVE
rive::rcp<rive::pls::PLSRenderTarget> UE::Rive::Renderer::Private::FRiveRenderTargetMetal::GetRenderTarget() const
{
    return CachedPLSRenderTargetMetal;
}
#endif // WITH_RIVE


#endif // PLATFORM_APPLE
