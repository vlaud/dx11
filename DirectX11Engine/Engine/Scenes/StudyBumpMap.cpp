#include "StudyBumpMap.h"
#include "../../RenderWindow.h"
#include "../Graphics/Graphics.h"
#include "../Input/Input.h"
#include "../Graphics/Objects/Components/ComponentTypes.h"
#include "../Graphics/Objects/GeometryHelper.h"

bool StudyBumpMap::InitializeScene()
{
    shared_ptr<Camera> cam = make_shared<Camera>();
    cam->SetPerspective(90.0f, (float)RenderWindow::ScreenWidth / (float)RenderWindow::ScreenHeight,
        0.01f, 1000.0f);
    mainCam.AddComponent(cam);
    mainCam.GetTransform()->SetPos(0.0f, 0.0f, -3.0f);

    shared_ptr<MeshRenderer<VertexBump>> render = make_shared<MeshRenderer<VertexBump>>();
    vector<VertexBump> vertices;
    vector<DWORD> indices;
    //GeometryHelper::GeneratorBox<VertexBump>(1.0f, 1.0f, 1.0f, &vertices, &indices);
    GeometryHelper::GeneratorSphere(1.0f, 100, 100, &vertices, &indices);
    for (int i = 0; i < indices.size(); i += 3)
    {
        CalculatorTangentBinormal(vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]]);
    }

    render->Initialize(device, dc, "Assets/Textures/Stone01.dds", gfx->cb, vertices, indices);
    model.AddComponent(render);

    bumpTex = make_unique<Texture>(device, "Assets/Textures/bump01.dds", aiTextureType::aiTextureType_NORMALS);
    vs = ShaderHelper::GetShader(device, VS_SHADER::BUMP);
    ps = ShaderHelper::GetShader(device, PS_SHADER::BUMP);
    return true;
}

void StudyBumpMap::Update(float delta)
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

void StudyBumpMap::CalculatorTangentBinormal(VertexBump& v1, VertexBump& v2, VertexBump& v3)
{
    XMFLOAT3 tangent, binormal, vec1, vec2;
    XMFLOAT2 tuVec, tvVec;
    float den;

    vec1.x = v2.pos.x - v1.pos.x;
    vec1.y = v2.pos.y - v1.pos.y;
    vec1.z = v2.pos.z - v1.pos.z;

    vec2.x = v3.pos.x - v1.pos.x;
    vec2.y = v3.pos.y - v1.pos.y;
    vec2.z = v3.pos.z - v1.pos.z;

    tuVec.x = v2.uv.x - v1.uv.x;
    tvVec.x = v2.uv.y - v1.uv.y;

    tuVec.y = v3.uv.x - v1.uv.x;
    tvVec.y = v3.uv.y - v1.uv.y;

    den = 1.0f / (tuVec.x * tvVec.y - tuVec.y * tvVec.x);
    
    tangent.x = (tvVec.y * vec1.x - tvVec.x * vec2.x) * den;
    tangent.y = (tvVec.y * vec1.y - tvVec.x * vec2.y) * den;
    tangent.z = (tvVec.y * vec1.z - tvVec.x * vec2.z) * den;

    binormal.x = (tuVec.x * vec2.x - tuVec.y * vec1.x) * den;
    binormal.y = (tuVec.x * vec2.y - tuVec.y * vec1.y) * den;
    binormal.z = (tuVec.x * vec2.z - tuVec.y * vec1.z) * den;

    v1.tangent = tangent;
    v2.tangent = tangent;
    v3.tangent = tangent;

    v1.binormal = binormal;
    v2.binormal = binormal;
    v3.binormal = binormal;
}

void StudyBumpMap::RenderFrame()
{
    dc->IASetInputLayout(vs->GetInputLayout());
    dc->VSSetShader(vs->GetShader(), nullptr, 0);
    dc->PSSetShader(ps->GetShader(), nullptr, 0);

    dc->PSSetConstantBuffers(1, 1, gfx->cb_light.GetAddressOf());
    dc->PSSetShaderResources(1, 1, bumpTex->GetTextureResourceViewAddress());
    model.GetComponent<Renderer>()->Draw(mainCam.GetComponent<Camera>()->GetViewprojection());
}

void StudyBumpMap::OnGUI()
{
    ImGui::Begin("Light");
    ImGui::DragFloat3("Light Dir", &gfx->cb_light.data.lightDir.x, 0.01f, -3.0f, 3.0f);
    ImGui::DragFloat("Intensity", &gfx->cb_light.data.lightStrength, 0.01f, 0.0f, 10.0f);
    ImGui::End();
}
