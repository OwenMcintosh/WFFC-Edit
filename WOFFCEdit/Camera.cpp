#include "Camera.h"

Camera::Camera()
{
    m_Window = GetActiveWindow();
    
    //functional
    m_movespeed = 0.30f;
    m_camRotRate = 3.0f;

    //camera
    m_camPosition = DirectX::SimpleMath::Vector3(0.f,3.7f,-3.5f);
    
    m_camOrientation = DirectX::SimpleMath::Vector3(0.f,0.f,0.f);

    m_camLookAt = DirectX::SimpleMath::Vector3(0.f,0.f,0.f);

    m_camLookDirection = DirectX::SimpleMath::Vector3(0.f,0.f,0.f);

    m_camRight = DirectX::SimpleMath::Vector3(0.f,0.f,0.f);

    m_camUp = DirectX::SimpleMath::Vector3(0.f,1.f,0.f);
    
    m_mousePosition = DirectX::SimpleMath::Vector2(0.f,0.f);

    m_WarpCursorFrame = false;

    //m_ZoomInRate = 0.5;
    //
    //m_FOV = 0.f;
}

Camera::~Camera()
{
    
}


void Camera::Update()
{
    //update lookat point
    m_camLookAt = m_camPosition + m_camLookDirection;
    
    //apply camera vectors
    m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_camPosition, m_camLookAt, DirectX::SimpleMath::Vector3::UnitY);

    //RECT clientRect;
    //GetClientRect(GetActiveWindow(), &clientRect); // Get the client (drawable) area
    //POINT rectSizes;
    //rectSizes.x = (clientRect.right - clientRect.left);
    //rectSizes.y = (clientRect.bottom - clientRect.top);
    //
    //float aspectRatio = rectSizes.x / rectSizes.y;
    //float nearPlane = 0.01f;
    //float farPlane = 1000.0f;
    //float fov = 70.0f * 3.14159f / 180.0f;
    //
    //m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
    //    fov,
    //    aspectRatio,
    //    0.01f,
    //    1000.0f
    //);
    //
    //m_view *= m_projection;
}



void Camera::HandleInput(InputCommands* p_InputCommands)
{

    MouseInput(p_InputCommands);
    KeyboardInput(p_InputCommands);
    
}

void Camera::KeyboardInput(InputCommands* p_InputCommands)
{
    if (p_InputCommands->control) {
        return;
    }

    if (p_InputCommands->rotRight)
    {
        m_camOrientation.y -= m_camRotRate;
    }
    if (p_InputCommands->rotLeft)
    {
        m_camOrientation.y += m_camRotRate;
    }

    if (p_InputCommands->rotUp)
    {
        m_camOrientation.x += m_camRotRate;
        if (m_camOrientation.x > 89.f) m_camOrientation.x = 89.f;
    }
    if (p_InputCommands->rotDown)
    {
        m_camOrientation.x -= m_camRotRate;
        if (m_camOrientation.x < -89.f) m_camOrientation.x = -89.f;
    }

    //if (p_InputCommands->zoomIn) {

    //}

    
    float lv_PitchRads = (m_camOrientation.x * 3.1415f) / 180.0f;
    float lv_YawRads = (m_camOrientation.y * 3.1415f) / 180.0f;

    //create look direction from Euler angles in m_camOrientation
    m_camLookDirection.x = sinf(lv_YawRads) *cosf(lv_PitchRads);
    m_camLookDirection.y = sinf(lv_PitchRads);
    m_camLookDirection.z = cosf(lv_YawRads) *cosf(lv_PitchRads);
    m_camLookDirection.Normalize();

    // create movement direction from Euler angles (NOT CAM LOOK TO AVOID WEIRD MOVEMENT WHEN LOOKING UP/DOWN)
    m_camMovementDirection.x = sinf(lv_YawRads);
    m_camMovementDirection.z = cosf(lv_YawRads);
    m_camMovementDirection.Normalize();
    
    //create right vector from look Direction
    m_camMovementDirection.Cross(DirectX::SimpleMath::Vector3::UnitY, m_camRight);

    //process input and update camera position
    if (p_InputCommands->forward)
    {	
        m_camPosition += m_camMovementDirection * m_movespeed;
    }
    if (p_InputCommands->back)
    {
        m_camPosition -= m_camMovementDirection *m_movespeed;
    }
    if (p_InputCommands->right)
    {
        m_camPosition += m_camRight*m_movespeed;
    }
    if (p_InputCommands->left)
    {
        m_camPosition -= m_camRight*m_movespeed;
    }
    if (p_InputCommands->up)
    {
        m_camPosition += m_camUp * m_movespeed;
    }
    if (p_InputCommands->down)
    {
        m_camPosition -= m_camUp * m_movespeed;
    }
}

void Camera::MouseInput(InputCommands* p_InputCommands)
{
     // not locking mouse, or app is not in focus
    if (!AllowMouseMovementControls( (p_InputCommands->WindowLock && GetForegroundWindow() == m_Window) ) )
    {
        return;
    }

    // hide cursor
    //SetCursor(NULL);
    
    // get current window dimensions (Window Space)
    RECT rect;
    GetClientRect(m_Window, &rect);
    
    // Upper left corner
    POINT ul;
    ul.x = rect.left;
    ul.y = rect.top;
    
    // lower right corner
    POINT lr;
    lr.x = rect.right;
    lr.y = rect.bottom;
    
    // map window based on UL/LR (Screen Space)
    MapWindowPoints(m_Window, nullptr, &ul, 1);
    MapWindowPoints(m_Window, nullptr, &lr, 1);
    
    // Store new window dimensions
    rect.left = ul.x;
    rect.top = ul.y;
    
    rect.right = lr.x;
    rect.bottom = lr.y;
    
    // lock cursor within window boundaries
    ClipCursor(&rect);
    
    // get center of screen (Screen Space)
    POINT center;
    center.x = ((rect.right + rect.left) / 2);
    center.y = ((rect.bottom + rect.top) / 2);
    
    SetCursorPos(center.x, center.y);

    POINT mousePos = { p_InputCommands->mouseXCoord, p_InputCommands->mouseYCoord };
    ClientToScreen(m_Window, &mousePos); // Convert window-space mouse coordinates to screen-space coordinates
    
    // differnce between previous and newest mouse positions
    DirectX::SimpleMath::Vector2 lv_MousePositionalDifference = DirectX::SimpleMath::Vector2(mousePos.x - center.x, mousePos.y - center.y);
    
    // check non-zero length
    if (lv_MousePositionalDifference.LengthSquared() == 0) {
        return;
    }
    
    // normalise
    lv_MousePositionalDifference.Normalize();
    
    // update roations
    m_camOrientation.y -= m_camRotRate * lv_MousePositionalDifference.x;
    m_camOrientation.x -= m_camRotRate * lv_MousePositionalDifference.y;
    
    // clamp rotations
    if (m_camOrientation.x > 89.f) m_camOrientation.x = 89.f;
    if (m_camOrientation.x < -89.f) m_camOrientation.x = -89.f;
    
    // set cursor to center
    //SetCursorPos(center.x, center.y);
    

    // new mouse position
    //m_mousePosition = DirectX::SimpleMath::Vector2(center.x, center.y);
}

bool Camera::AllowMouseMovementControls(bool p_LockState) {

    switch (p_LockState) {


    case true:
    {
        //SetCursor(NULL);
        //RECT rect;
        //GetClientRect(m_Window, &rect);
        //
        //POINT ul;
        //ul.x = rect.left;
        //ul.y = rect.top;
        //
        //POINT lr;
        //lr.x = rect.right;
        //lr.y = rect.bottom;
        //
        //MapWindowPoints(m_Window, nullptr, &ul, 1);
        //MapWindowPoints(m_Window, nullptr, &lr, 1);
        //
        //rect.left = ul.x;
        //rect.top = ul.y;
        //
        //rect.right = lr.x;
        //rect.bottom = lr.y;
        //
        //ClipCursor(&rect);
        //
        //break;
    }
    case false:
        ClipCursor(NULL);
        break;

    default:
        break;
    }

    return p_LockState;

}