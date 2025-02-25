#pragma once
#include "DisplayChunk.h"
#include "InputCommands.h"

class Camera
{
public:
Camera();
~Camera();
    
void Update();
void HandleInput(InputCommands* p_InputCommands);

DirectX::SimpleMath::Vector3 GetCameraPosition(){return m_camPosition; };
DirectX::SimpleMath::Matrix GetViewMatrix(){ return m_view; }
DirectX::SimpleMath::Matrix GetProjectionMatrix(){ return m_view; }
    
private:

    void MouseInput(InputCommands* p_InputCommands);
    void KeyboardInput(InputCommands* p_InputCommands);
    bool AllowMouseMovementControls(bool p_LockState);
    
    //functionality
    float								m_movespeed;

    //camera
    DirectX::SimpleMath::Vector3		m_camPosition;
    DirectX::SimpleMath::Vector3		m_camOrientation;
    DirectX::SimpleMath::Vector3		m_camLookAt;
    DirectX::SimpleMath::Vector3		m_camLookDirection;
    DirectX::SimpleMath::Vector3		m_camMovementDirection;
    DirectX::SimpleMath::Vector3		m_camRight;
    DirectX::SimpleMath::Vector3		m_camUp;

    float                               m_camRotRate;
    //float                               m_ZoomInRate;
    //float                               m_FOV;
    DirectX::SimpleMath::Matrix         m_view;
    //DirectX::SimpleMath::Matrix         m_projection;

    DirectX::SimpleMath::Vector2        m_mousePosition;

    HWND                                m_Window;
    bool                                m_WarpCursorFrame;
};
