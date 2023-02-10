#include "StudySpecular.h"
#include "../../RenderWindow.h"
#include "../Graphics/Graphics.h"
#include "../Input/Input.h"
#include "../Graphics/Objects/Components/ComponentTypes.h"
#include "../Graphics/Objects/GeometryHelper.h"
bool StudySpecular::InitializeScene()
{
    shared_ptr<Camera> cam = make_shared<Camera>();
    cam->SetPerspective(90.0f, (float)RenderWindow::ScreenWidth / (float)RenderWindow::ScreenHeight,
        0.01f, 1000.0f);
    mainCam.AddComponent(cam);
    mainCam.GetTransform()->SetPos(0.0f, 0.0f, -3.0f);

    shared_ptr<MeshRenderer<Vertex>> render = make_shared<MeshRenderer<Vertex>>();
    vector<Vertex> vertices;
    vector<DWORD> indices;
    GeometryHelper::GeneratorSphere(1.0f, 100, 100, &vertices, &indices);
    render->Initialize(device, dc, "Assets/Textures/Ryan2.jpg", gfx->cb, vertices, indices);
    model.AddComponent(render);

    vs = ShaderHelper::GetShader(device, VS_SHADER::SPECULAR);
    ps = ShaderHelper::GetShader(device, PS_SHADER::SPECULAR);
    return true;
}

void StudySpecular::Update(float delta)
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

void StudySpecular::RenderFrame()
{
    gfx->cb_Campos.data.cameraPos = mainCam.GetTransform()->GetPos();
    gfx->cb_Campos.Update();
    dc->VSSetConstantBuffers(1, 1, gfx->cb_Campos.GetAddressOf());

    dc->IASetInputLayout(vs->GetInputLayout());
    dc->VSSetShader(vs->GetShader(), nullptr, 0);
    dc->PSSetShader(ps->GetShader(), nullptr, 0);
    model.GetComponent<Renderer>()->Draw(mainCam.GetComponent<Camera>()->GetViewprojection());
}

void StudySpecular::OnGUI()
{
    ImGui::Begin("Light");
    ImGui::DragFloat3("Light Dir", &gfx->cb_light.data.lightDir.x, 0.01f, -3.0f, 3.0f);
    ImGui::DragFloat("Intensity", &gfx->cb_light.data.lightStrength, 0.01f, 0.0f, 10.0f);
    ImGui::End();
}
