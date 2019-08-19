#pragma once

#include "../../pEntryPoint/pch.h"

// dx
#include "dx_utility/d3dx12.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;

// pEngine
#include "../../pUtils/putils.h"
#include "lib_define.h"
#include "../pgeneralrender.h"
#include "../../pEngine/pengine.h"

class DYNLIB_PRENDER_DIRECT3D CoreDirect3D: public QWidget, public IRender
{
    Q_OBJECT

protected:
    std::array<D3D_FEATURE_LEVEL, 9> dx_featureLevels;
    D3D_FEATURE_LEVEL dx_supportLevel;
    DXGI_FORMAT dx_BackBufferFormat;
    DXGI_FORMAT dx_DepthStencilFormat;

    ComPtr<IDXGIFactory4> dx_dxgiFactory;
    std::map<int, ComPtr<IDXGIAdapter1> > dx_graphicAdapters;
    std::map<int, std::vector<ComPtr<IDXGIOutput>> > dx_adaptersOutputs;
    ComPtr<ID3D12Device1> dx_Device;
    ComPtr<ID3D12Fence> dx_Fence;
    UINT dx_RtvDescriptorSize;
    UINT dx_DsvDescriptorSize;
    UINT dx_CbvSrvUavDescriptorSize;
    std::vector<uint> dx_msaaQuality;
    bool dx_msaaState;
    ComPtr<ID3D12CommandQueue> dx_CommandQueue;
    ComPtr<ID3D12CommandAllocator> dx_DirectCmdListAlloc;
    ComPtr<ID3D12GraphicsCommandList> dx_CommandList;
    ComPtr<IDXGISwapChain> dx_SwapChain;
    static const int dx_swapChainBufferCount = 2;
    ComPtr<ID3D12DescriptorHeap> dx_RtvHeap;
    ComPtr<ID3D12DescriptorHeap> dx_DsvHeap;
    int dx_width;
    int dx_height;
    ComPtr<ID3D12Resource> dx_SwapChainBuffer[dx_swapChainBufferCount];
    ComPtr<ID3D12Resource> dx_DepthStencilBuffer;
    UINT64 dx_CurrentFence;
    D3D12_VIEWPORT dx_ScreenViewport;
    D3D12_RECT dx_ScissorRect;
    int dx_CurrBackBuffer;

    std::shared_ptr<QTimer> updater;
    std::shared_ptr<FPSCounter> fpsCounter;

public:
    CoreDirect3D(QWidget* obj = nullptr);

    ComPtr<ID3D12Resource> currentBackBuffer()const;
    D3D12_CPU_DESCRIPTOR_HANDLE currentBackBufferView()const;
    D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView()const;


protected:
    void enableDebugLayer();
    void getAllGraphicAdapters();
    std::vector<ComPtr<IDXGIOutput>> getAllOutputsForGraphicAdapter(ComPtr<IDXGIAdapter1>);
    void tryCreateRealDevice();
    void tryCreateWARPDevice();
    void createFenceCPUAndGPU();
    void createDescriptorsBuffersGPU();
    void checkSupportMSAAQuality();
    void createCommandObjects();
    void createSwapChain();
    std::pair<uint, uint> getFrequencyOutput(std::vector<ComPtr<IDXGIOutput>> outputs);
    void createRtvAndDsvDescriptorHeaps();
    void createRenderTargetView();
    void createDepthAndStencilBufferAndView();
    void createViewportAndSrissorRect();
    void flushCommandQueue();
    void startRender();
    void startFpsCounting();
    void printUsingDirect3D(const D3D_FEATURE_LEVEL& l) const;

private:
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

public slots:
    void updaterRender();

signals:
    void closedChildrenWindows();

    // IRender interface
public:
    void initializeRender(int w, int h) override;
    void resizeRender(int w, int h) override;
    void updateRender() override;
    void drawRender() override;
    void showRender() override;
    void updateFps() override;
    ~CoreDirect3D() override;
};

