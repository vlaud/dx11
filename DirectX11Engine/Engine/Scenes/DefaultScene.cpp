#include "DefaultScene.h"
#include "../../RenderWindow.h"
#include "../Graphics/Graphics.h"
#include "../Input/Input.h"
#include "../Graphics/Objects/Components/ComponentTypes.h"

bool DefaultScene::InitializeScene()
{
    shared_ptr<Camera> cam = make_shared<Camera>();
    cam->SetPerspective(90.0f, (float)RenderWindow::ScreenWidth / (float)RenderWindow::ScreenHeight,
        0.01f, 1000.0f);
    mainCam.AddComponent(cam);
    mainCam.GetTransform()->SetPos(0.0f, 0.0f, -3.0f);

    shared_ptr<SkinnedMeshRenderer> render = make_shared<SkinnedMeshRenderer>();
    render->Initialize("Assets/Animations/Capoeira.fbx", device, dc, gfx->cb);
    model.AddComponent(render);
    model.GetTransform()->SetScale(0.01f, 0.01f, 0.01f);

    vs = ShaderHelper::GetShader(device, VS_SHADER::SKINNED);
    ps = ShaderHelper::GetShader(device, PS_SHADER::DEFAULT);
    return true;
}

void DefaultScene::Update(float delta)
{
    if (Input::mouse->IsRightDown())
    {
        mainCam.GetTransform()->Rotate((float)Input::mouse->GetRawY() * 0.03f, 
            (float)Input::mouse->GetRawX() * 0.03f, 0.0f);
    }
    if (Input::keyboard->KeyIsPressed('W'))
    {
        mainCam.GetTransform()->Translate(mainCam.GetTransform()->GetForward() * delta);
    }
    if (Input::keyboard->KeyIsPressed('S'))
    {
        mainCam.GetTransform()->Translate(mainCam.GetTransform()->GetBackward() * delta);
    }
    if (Input::keyboard->KeyIsPressed('A'))
    {
        mainCam.GetTransform()->Translate(mainCam.GetTransform()->GetLeft() * delta);
    }
    if (Input::keyboard->KeyIsPressed('D'))
    {
        mainCam.GetTransform()->Translate(mainCam.GetTransform()->GetRight() * delta);
    }
    if (Input::keyboard->KeyIsPressed('Q'))
    {
        mainCam.GetTransform()->Translate(mainCam.GetTransform()->GetUp() * delta);
    }
    if (Input::keyboard->KeyIsPressed('E'))
    {
        mainCam.GetTransform()->Translate(mainCam.GetTransform()->GetDown() * delta);
    }
}

void DefaultScene::RenderFrame()
{
    dc->IASetInputLayout(vs->GetInputLayout());
    dc->VSSetShader(vs->GetShader(), nullptr, 0);
    dc->PSSetShader(ps->GetShader(), nullptr, 0);
    model.GetComponent<Renderer>()->Draw(mainCam.GetComponent<Camera>()->GetViewprojection());
}

void DefaultScene::OnGUI()
{
}
