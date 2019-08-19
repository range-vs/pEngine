#include "coredirect3d.h"

CoreDirect3D::CoreDirect3D(QWidget *obj):QWidget(obj)
{
    dx_featureLevels =
    {
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
//        D3D_FEATURE_LEVEL_9_3,
//        D3D_FEATURE_LEVEL_9_2,
//        D3D_FEATURE_LEVEL_9_1
    };
    dx_BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    dx_msaaState = false;
    dx_DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dx_CurrentFence = 0;
    dx_CurrBackBuffer = 0;
    fpsCounter.reset(new FPSCounter);
}

CoreDirect3D::~CoreDirect3D()
{

}

ComPtr<ID3D12Resource> CoreDirect3D::currentBackBuffer() const
{
    return dx_SwapChainBuffer[dx_CurrBackBuffer];
}

D3D12_CPU_DESCRIPTOR_HANDLE CoreDirect3D::currentBackBufferView()const
{
    return CD3DX12_CPU_DESCRIPTOR_HANDLE(
        dx_RtvHeap->GetCPUDescriptorHandleForHeapStart(),
        dx_CurrBackBuffer,
                dx_RtvDescriptorSize);
}

D3D12_CPU_DESCRIPTOR_HANDLE CoreDirect3D::depthStencilView() const
{
    return dx_DsvHeap->GetCPUDescriptorHandleForHeapStart();
}

void CoreDirect3D::enableDebugLayer()
{
    ComPtr<ID3D12Debug> debugController;
    DebugHR::debugging(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
    debugController->EnableDebugLayer();
}

void CoreDirect3D::getAllGraphicAdapters()
{
    uint i = 0;
    ComPtr<IDXGIAdapter1> adapter(nullptr);
    while(dx_dxgiFactory->EnumAdapters1(i, adapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND)
    {
//        DXGI_ADAPTER_DESC desc;
//        adapter->GetDesc(&desc);
//        std::wstring text = L"***Adapter: "; text += desc.Description;
//        text += L"\n"; OutputDebugString(text.c_str()); adapterList.push_back(adapter);
        dx_graphicAdapters.insert({i, adapter});
        dx_adaptersOutputs.insert({i, getAllOutputsForGraphicAdapter(adapter)});
        ++i;
    }
}

std::vector<ComPtr<IDXGIOutput> > CoreDirect3D::getAllOutputsForGraphicAdapter(ComPtr<IDXGIAdapter1> adapter)
{
    UINT i = 0;
    ComPtr<IDXGIOutput> output(nullptr);
    std::vector<ComPtr<IDXGIOutput>> listOutputs;
    while(adapter->EnumOutputs(i, output.GetAddressOf()) != DXGI_ERROR_NOT_FOUND)
    {
//        DXGI_OUTPUT_DESC desc;
//        output->GetDesc(&desc);

//        std::wstring text = L"***Output: "; text += desc.DeviceName;
//        text += L"\n"; OutputDebugString(text.c_str());

//        LogOutputDisplayModes(output, DXGI_FORMAT_B8G8R8A8_UNORM);

//        ReleaseCom(output);
        listOutputs.push_back(output);
        ++i;
    }
    return listOutputs;
}

void CoreDirect3D::tryCreateRealDevice()
{
    for(auto&& a: dx_graphicAdapters)
    {
        for(auto&& level: dx_featureLevels)
        {
            if(SUCCEEDED(D3D12CreateDevice(a.second.Get(), level, IID_PPV_ARGS(dx_Device.GetAddressOf()))))
            {
                dx_supportLevel = level;
                return;
            }
        }
    }
    tryCreateWARPDevice();
}

void CoreDirect3D::tryCreateWARPDevice()
{
    ComPtr<IDXGIAdapter> pWarpAdapter;
    DebugHR::debugging(dx_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)));
    for(auto&& level: dx_featureLevels)
    {
        HRESULT hardwareResult = D3D12CreateDevice(
                pWarpAdapter.Get(),
                level,
                IID_PPV_ARGS(&dx_Device));
        if(SUCCEEDED(hardwareResult))
        {
            dx_supportLevel = level;
            return;
        }
    }
    qFatal("Your GPU (and WARP) no support Direct 3D!");
}

void CoreDirect3D::createFenceCPUAndGPU()
{
    DebugHR::debugging(dx_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&dx_Fence)));
}

void CoreDirect3D::createDescriptorsBuffersGPU()
{
    dx_RtvDescriptorSize = dx_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    dx_DsvDescriptorSize = dx_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    dx_CbvSrvUavDescriptorSize = dx_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void CoreDirect3D::checkSupportMSAAQuality()
{
    for(uint i(2);;i*=2)
    {
        D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
        msQualityLevels.Format = dx_BackBufferFormat;
        msQualityLevels.SampleCount = i;
        msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
        msQualityLevels.NumQualityLevels = 0;
        DebugHR::debugging(dx_Device->CheckFeatureSupport(
                          D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
                          &msQualityLevels,
                          sizeof(msQualityLevels)));
        if(msQualityLevels.NumQualityLevels < 1)
            break;
        dx_msaaQuality.push_back(i);
    }
}

void CoreDirect3D::createCommandObjects()
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    DebugHR::debugging(dx_Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&dx_CommandQueue)));
    DebugHR::debugging(dx_Device->CreateCommandAllocator(
                      D3D12_COMMAND_LIST_TYPE_DIRECT,
                      IID_PPV_ARGS(dx_DirectCmdListAlloc.GetAddressOf())));
    DebugHR::debugging(dx_Device->CreateCommandList(
                      0,
                      D3D12_COMMAND_LIST_TYPE_DIRECT,
                      dx_DirectCmdListAlloc.Get(),
                      nullptr,
                      IID_PPV_ARGS(dx_CommandList.GetAddressOf())));
    dx_CommandList->Close();
}

void CoreDirect3D::createSwapChain()
{
    // Release the previous swapchain we will be recreating.
    dx_SwapChain.Reset();
    DXGI_SWAP_CHAIN_DESC sd;
    sd.BufferDesc.Width = dx_width;
    sd.BufferDesc.Height = dx_height;
    auto frequencyOutput(getFrequencyOutput(dx_adaptersOutputs[0]));
    sd.BufferDesc.RefreshRate.Numerator = frequencyOutput.first; // 60
    sd.BufferDesc.RefreshRate.Denominator = frequencyOutput.second; // 1
    sd.BufferDesc.Format = dx_BackBufferFormat;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.SampleDesc.Count = dx_msaaState ? dx_msaaQuality[0] : 1;
    sd.SampleDesc.Quality = dx_msaaState ? 1 : 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = dx_swapChainBufferCount;
    sd.OutputWindow = reinterpret_cast<HWND>(winId());
    sd.Windowed = true;
    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    // Note: Swap chain uses queue to perform flush.
    DebugHR::debugging(dx_dxgiFactory->CreateSwapChain(
                      dx_CommandQueue.Get(),
                      &sd, dx_SwapChain.GetAddressOf()));
}

std::pair<uint, uint> CoreDirect3D::getFrequencyOutput(std::vector<ComPtr<IDXGIOutput>> outputs)
{
    std::pair<uint, uint> result = {0, 0};
    uint numModes(0);
    DebugHR::debugging(outputs[0]->GetDisplayModeList(dx_BackBufferFormat, DXGI_ENUM_MODES_INTERLACED, &numModes, 0));
    DXGI_MODE_DESC * displayModeList = new DXGI_MODE_DESC[numModes];
    DebugHR::debugging(outputs[0]->GetDisplayModeList(dx_BackBufferFormat, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList));
    bool isStop(false);
    for(size_t i(0); i < numModes; i++)
    {
        if(displayModeList[i].Width == (uint)dx_width)
        {
            if(displayModeList[i].Height == (uint)dx_height)
            {
                uint numerator = displayModeList[i].RefreshRate.Numerator;
                uint denominator = displayModeList[i].RefreshRate.Denominator;
                result = {numerator, denominator};
                isStop = true;
                break;
            }
            if(!isStop)
                break;
        }
    }
    delete [] displayModeList;
    return result;
}

void CoreDirect3D::createRtvAndDsvDescriptorHeaps()
{
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
    rtvHeapDesc.NumDescriptors = dx_swapChainBufferCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    rtvHeapDesc.NodeMask = 0;
    DebugHR::debugging(dx_Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(dx_RtvHeap.GetAddressOf())));

    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
    dsvHeapDesc.NumDescriptors = 1;
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    dsvHeapDesc.NodeMask = 0;
    DebugHR::debugging(dx_Device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(dx_DsvHeap.GetAddressOf())));
}

void CoreDirect3D::createRenderTargetView()
{
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(dx_RtvHeap->GetCPUDescriptorHandleForHeapStart());
    for (uint i(0); i < dx_swapChainBufferCount; i++)
    {
        DebugHR::debugging(dx_SwapChain->GetBuffer(i, IID_PPV_ARGS(&dx_SwapChainBuffer[i])));
        dx_Device->CreateRenderTargetView(dx_SwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
        rtvHeapHandle.Offset(1, dx_RtvDescriptorSize);
    }
}

void CoreDirect3D::createDepthAndStencilBufferAndView()
{
    // Create the depth/stencil buffer and view.
    D3D12_RESOURCE_DESC depthStencilDesc;
    depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depthStencilDesc.Alignment = 0;
    depthStencilDesc.Width = dx_width;
    depthStencilDesc.Height = dx_height;
    depthStencilDesc.DepthOrArraySize = 1;
    depthStencilDesc.MipLevels = 1;

    // Correction 11/12/2016: SSAO chapter requires an SRV to the depth buffer to read from
    // the depth buffer.  Therefore, because we need to create two views to the same resource:
    //   1. SRV format: DXGI_FORMAT_R24_UNORM_X8_TYPELESS
    //   2. DSV Format: DXGI_FORMAT_D24_UNORM_S8_UINT
    // we need to create the depth buffer resource with a typeless format.
    depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    depthStencilDesc.SampleDesc.Count = dx_msaaState ? dx_msaaQuality[0] : 1;
    depthStencilDesc.SampleDesc.Quality = dx_msaaState ? 1 : 0;
    depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE optClear;
    optClear.Format = dx_DepthStencilFormat;
    optClear.DepthStencil.Depth = 1.0f;
    optClear.DepthStencil.Stencil = 0;
    CD3DX12_HEAP_PROPERTIES headp_properties(D3D12_HEAP_TYPE_DEFAULT);
    DebugHR::debugging(dx_Device->CreateCommittedResource(
                      &headp_properties,
                      D3D12_HEAP_FLAG_NONE,
                      &depthStencilDesc,
                      D3D12_RESOURCE_STATE_COMMON,
                      &optClear,
                      IID_PPV_ARGS(dx_DepthStencilBuffer.GetAddressOf())));

    // Create descriptor to mip level 0 of entire resource using the format of the resource.
    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Format = dx_DepthStencilFormat;
    dsvDesc.Texture2D.MipSlice = 0;
    dx_Device->CreateDepthStencilView(dx_DepthStencilBuffer.Get(), &dsvDesc, dx_DsvHeap->GetCPUDescriptorHandleForHeapStart());

    // Transition the resource from its initial state to be used as a depth buffer.
    auto resourceBarrierTransition = CD3DX12_RESOURCE_BARRIER::Transition(dx_DepthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
    dx_CommandList->ResourceBarrier(1, &resourceBarrierTransition);
}

void CoreDirect3D::createViewportAndSrissorRect()
{
    DebugHR::debugging(dx_CommandList->Close());

    ID3D12CommandList* cmdsLists[] = { dx_CommandList.Get() };
    dx_CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    // Wait until resize is complete.
    flushCommandQueue();

    // Update the viewport transform to cover the client area.
    dx_ScreenViewport.TopLeftX = 0;
    dx_ScreenViewport.TopLeftY = 0;
    dx_ScreenViewport.Width    = static_cast<float>(dx_width);
    dx_ScreenViewport.Height   = static_cast<float>(dx_height);
    dx_ScreenViewport.MinDepth = 0.0f;
    dx_ScreenViewport.MaxDepth = 1.0f;

    dx_ScissorRect = { 0, 0, dx_width, dx_height };
}

void CoreDirect3D::flushCommandQueue()
{
    // Advance the fence value to mark commands up to this fence point.
    dx_CurrentFence++;

    // Add an instruction to the command queue to set a new fence point.  Because we
    // are on the GPU timeline, the new fence point won't be set until the GPU finishes
    // processing all the commands prior to this Signal().
    DebugHR::debugging(dx_CommandQueue->Signal(dx_Fence.Get(), dx_CurrentFence));

    // Wait until the GPU has completed commands up to this fence point.
    if(dx_Fence->GetCompletedValue() < dx_CurrentFence)
    {
        HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
        // Fire event when GPU hits current fence.
//        DebugHR::generateThrow(dx_Fence->SetEventOnCompletion(dx_CurrentFence, eventHandle));
        dx_Fence->SetEventOnCompletion(dx_CurrentFence, eventHandle);
        // Wait until the GPU hits current fence event is fired.
        WaitForSingleObject(eventHandle, INFINITE);
        CloseHandle(eventHandle);
    }
}

void CoreDirect3D::startRender()
{
    updater.reset(new QTimer(this));
    updater->setInterval(0);
    connect(updater.get(), &QTimer::timeout, this, &CoreDirect3D::updaterRender);
    updater->start();
}

void CoreDirect3D::startFpsCounting()
{
    fpsCounter->start();
}

void CoreDirect3D::printUsingDirect3D(const D3D_FEATURE_LEVEL& l) const
{
    std::map<D3D_FEATURE_LEVEL, QString> shaders =
    {
        {D3D_FEATURE_LEVEL_12_1, "5.1"},
        {D3D_FEATURE_LEVEL_12_0, "5.1"},
        {D3D_FEATURE_LEVEL_11_1, "5.0"},
        {D3D_FEATURE_LEVEL_11_0, "5.0"},
        {D3D_FEATURE_LEVEL_10_1, "4.0"},
        {D3D_FEATURE_LEVEL_10_0, "4.0"}
    };
    std::map<D3D_FEATURE_LEVEL, QString> levels =
    {
        {D3D_FEATURE_LEVEL_12_1, "12.1"},
        {D3D_FEATURE_LEVEL_12_0, "12.0"},
        {D3D_FEATURE_LEVEL_11_1, "11.1"},
        {D3D_FEATURE_LEVEL_11_0, "11.0"},
        {D3D_FEATURE_LEVEL_10_1, "10.1"},
        {D3D_FEATURE_LEVEL_10_0, "10.0"}
    };
    qInfo() << QString("Using Direct3D version: " + levels[l] + ", shader model: " + shaders[l]).toStdString().c_str();
}

void CoreDirect3D::resizeEvent(QResizeEvent *event)
{
    resizeRender(width(), height());
}

void CoreDirect3D::closeEvent(QCloseEvent *event)
{
    emit closedChildrenWindows();
}

void CoreDirect3D::updaterRender()
{
    drawRender();
}

void CoreDirect3D::updateFps()
{
    setWindowTitle("Current FPS: " + QString::number(fpsCounter->getFps()));
}

void CoreDirect3D::initializeRender(int w, int h)
{
    resize(w, h);
    dx_width = width();
    dx_height = height();
    //enableDebugLayer();
    DebugHR::debugging(CreateDXGIFactory1(IID_PPV_ARGS(&dx_dxgiFactory)));
    getAllGraphicAdapters();
    tryCreateRealDevice();
    createFenceCPUAndGPU();
    createDescriptorsBuffersGPU();
    checkSupportMSAAQuality();
    createCommandObjects();
    createSwapChain();
    createRtvAndDsvDescriptorHeaps();
    resizeRender(dx_width, dx_height);
    startRender();
    startFpsCounting();
    printUsingDirect3D(dx_supportLevel);
}

void CoreDirect3D::resizeRender(int w, int h)
{
    dx_width = w;
    dx_height = h;

    // Flush before changing any resources.
    flushCommandQueue();

    DebugHR::debugging(dx_CommandList->Reset(dx_DirectCmdListAlloc.Get(), nullptr));

    // Release the previous resources we will be recreating.
    for (int i = 0; i < dx_swapChainBufferCount; ++i)
        dx_SwapChainBuffer[i].Reset();
    dx_DepthStencilBuffer.Reset();

    // Resize the swap chain.
    DebugHR::debugging(dx_SwapChain->ResizeBuffers(
                      dx_swapChainBufferCount,
                      dx_width, dx_height,
                      dx_BackBufferFormat,
                      DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

    dx_CurrBackBuffer = 0;

    createRenderTargetView();
    createDepthAndStencilBufferAndView();
    createViewportAndSrissorRect();
}

void CoreDirect3D::updateRender()
{

}

void CoreDirect3D::drawRender()
{

}

void CoreDirect3D::showRender()
{
    show();
}
