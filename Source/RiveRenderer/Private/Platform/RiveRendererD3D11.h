// Copyright Rive, Inc. All rights reserved.

#pragma once

#include "RiveRenderer.h"

#if PLATFORM_WINDOWS

#if WITH_RIVE
THIRD_PARTY_INCLUDES_START
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/MinWindows.h"
#include "rive/pls/d3d/pls_render_context_d3d_impl.hpp" // This ends up including windows.h 
#include "Windows/HideWindowsPlatformTypes.h"
THIRD_PARTY_INCLUDES_END

struct ID3D11DynamicRHI;

namespace rive::pls
{
	class PLSRenderTargetD3D;
	class PLSRenderContextD3DImpl;
}

#endif // WITH_RIVE

namespace UE::Rive::Renderer::Private
{
	class FRiveRendererD3D11GPUAdapter
	{
	public:
		FRiveRendererD3D11GPUAdapter() {}

#if WITH_RIVE
		void Initialize(rive::pls::PLSRenderContextD3DImpl::ContextOptions& OutContextOptions);
#endif // WITH_RIVE
		void ResetBlendState();
		
		ID3D11DynamicRHI* GetD3D11RHI() const { return D3D11RHI; }
		ID3D11Device* GetD3D11DevicePtr() const { return D3D11DevicePtr; }
		ID3D11DeviceContext* GetD3D11DeviceContext() const { return D3D11DeviceContext; }
		IDXGIDevice* GetDXGIDevice() const { return DXGIDevice; }

	private:
		void InitBlendState();
	private:
		ID3D11DynamicRHI* D3D11RHI = nullptr;
		ID3D11Device* D3D11DevicePtr = nullptr;
		ID3D11DeviceContext* D3D11DeviceContext = nullptr;
		IDXGIDevice* DXGIDevice = nullptr;
	
		TRefCountPtr<ID3D11BlendState> AlphaBlendState;
		TRefCountPtr<ID3D11RasterizerState> NormalRasterState;
	};

	class RIVERENDERER_API FRiveRendererD3D11 : public FRiveRenderer
	{
		/**
		 * Structor(s)
		 */

	public:

		FRiveRendererD3D11();

		virtual ~FRiveRendererD3D11() override = default;

		//~ BEGIN : IRiveRenderer Interface

	public:

		virtual IRiveRenderTargetPtr CreateTextureTarget_GameThread(const FName& InRiveName, UTexture2DDynamic* InRenderTarget) override;

		virtual void CreatePLSContext_RenderThread(FRHICommandListImmediate& RHICmdList) override;

		virtual void CreatePLSRenderer_RenderThread(FRHICommandListImmediate& RHICmdList) override;

		//~ END : IRiveRenderer Interface

		void ResetBlendState() const;

	private:
		TUniquePtr<UE::Rive::Renderer::Private::FRiveRendererD3D11GPUAdapter> D3D11GPUAdapter;
	};
}

#endif // PLATFORM_WINDOWS