#include "direct3dwidget.h"

Direct3DWidget::Direct3DWidget(QWidget* obj):CoreDirect3D (obj){}

void Direct3DWidget::initializeRender(int w, int h)
{
    CoreDirect3D::initializeRender(w, h);
    qInfo() << "DirectX render load succesfull"; // add dx version output + shaders version
}

void Direct3DWidget::updateRender()
{
    // todo
}

void Direct3DWidget::drawRender()
{
    auto currBackBuf = currentBackBufferView();
    auto depthStancilView = depthStencilView();
    // Reuse the memory associated with command recording.
    // We can only reset when the associated command lists have finished execution on the GPU.
    DebugHR::debugging(dx_DirectCmdListAlloc->Reset());

    // A command list can be reset after it has been added to the command queue via ExecuteCommandList.
    // Reusing the command list reuses memory.
    DebugHR::debugging(dx_CommandList->Reset(dx_DirectCmdListAlloc.Get(), nullptr));

    // Indicate a state transition on the resource usage.
    auto resourceBarrierTransition = CD3DX12_RESOURCE_BARRIER::Transition(currentBackBuffer().Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    dx_CommandList->ResourceBarrier(1, &resourceBarrierTransition);

    // Set the viewport and scissor rect.  This needs to be reset whenever the command list is reset.
    dx_CommandList->RSSetViewports(1, &dx_ScreenViewport);
    dx_CommandList->RSSetScissorRects(1, &dx_ScissorRect);

    // Clear the back buffer and depth buffer.
    dx_CommandList->ClearRenderTargetView(currBackBuf, Colors::LightSteelBlue, 0, nullptr);
    dx_CommandList->ClearDepthStencilView(depthStancilView, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

    // Specify the buffers we are going to render to.
    dx_CommandList->OMSetRenderTargets(1, &currBackBuf, true, &depthStancilView);

    // Indicate a state transition on the resource usage.
    auto resBarrier = CD3DX12_RESOURCE_BARRIER::Transition(currentBackBuffer().Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    dx_CommandList->ResourceBarrier(1, &resBarrier);

    // Done recording commands.
    DebugHR::debugging(dx_CommandList->Close());

    // Add the command list to the queue for execution.
    ID3D12CommandList* cmdsLists[] = { dx_CommandList.Get() };
    dx_CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    // swap the back and front buffers
    //DebugHR::generateThrow(dx_SwapChain->Present(0, 0));
    dx_SwapChain->Present(0, 0);

    dx_CurrBackBuffer = (dx_CurrBackBuffer + 1) % dx_swapChainBufferCount;

    // Wait until frame commands are complete.  This waiting is inefficient and is
    // done for simplicity.  Later we will show how to organize our rendering code
    // so we do not have to wait per frame.
    flushCommandQueue();

    ++(*fpsCounter);
    fpsCounter->setAfterTime();
    if(fpsCounter->isReady())
    {
        updateFps();
        fpsCounter->setBeforeTime();
    }
}

Direct3DWidget::~Direct3DWidget()
{
    // todo
}
