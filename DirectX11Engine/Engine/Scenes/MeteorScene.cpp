#include "MeteorScene.h"
#include "../../RenderWindow.h"
#include "../Graphics/Graphics.h"
#include "../Input/Input.h"
#include "../Graphics/Objects/Components/ComponentTypes.h"
#include "../Graphics/Objects/GeometryHelper.h"
#include <time.h>

bool MeteorScene::SphereIntersect(XMVECTOR orgDir, XMVECTOR rayDir)
{
    XMFLOAT3 temp;
    XMStoreFloat3(&temp, orgDir);
    float distsq = temp.x * temp.x + temp.y * temp.y + temp.z * temp.z;
    XMVECTOR p = XMVector3Dot(orgDir, rayDir); // XMVECTOR = 병렬 프로그래밍
    XMStoreFloat3(&temp, p);
    float dotsq = temp.x * temp.x;
    float dist = sqrt(distsq - dotsq);
    if (dist <= 1.0f)
    {
        //피킹
        return true;
    }
    return false;
}

void MeteorScene::Picking()
{
    if (model == nullptr) return;
    MousePoint mousePos = Input::mouse->GetPos();

    D3D11_VIEWPORT vp;
    UINT n = 1;
    dc->RSGetViewports(&n, &vp);

    Camera* cam = mainCam.GetComponent<Camera>();

    XMMATRIX mat = cam->GetprojectionMatrix();
    XMFLOAT4X4 proj;
    XMStoreFloat4x4(&proj, mat);

    XMFLOAT3 pos;
    pos.x = ((mousePos.x * 2.0f) / vp.Width - 1.0f) / proj(0, 0);
    pos.y = ((mousePos.y * -2.0f) / vp.Height + 1.0f) / proj(1, 1);
    pos.z = vp.MaxDepth;

    XMVECTOR rayDir = XMVectorSet(pos.x, pos.y, pos.z, 0.0f);

    XMMATRIX inverseView = XMMatrixInverse(nullptr, cam->GetViewMatrix());
    XMMATRIX inverseWorld = XMMatrixInverse(nullptr, model->GetTransform()->GetWorld());
    XMMATRIX Local = XMMatrixMultiply(inverseView, inverseWorld);

    XMVECTOR orgDir = XMVector2TransformCoord(XMVectorSet(0, 0, 0, 1), Local);
    rayDir = XMVector3Normalize(XMVector3TransformNormal(rayDir, Local));

    if (SphereIntersect(orgDir, rayDir))
    {
        model.release();
    }
}

bool MeteorScene::IsPickingObs(unique_ptr<GameObject> &obj)
{
    if (obj == nullptr) return false;
    MousePoint mousePos = Input::mouse->GetPos();

    D3D11_VIEWPORT vp;
    UINT n = 1;
    dc->RSGetViewports(&n, &vp);

    Camera* cam = mainCam.GetComponent<Camera>();

    XMMATRIX mat = cam->GetprojectionMatrix();
    XMFLOAT4X4 proj;
    XMStoreFloat4x4(&proj, mat);

    XMFLOAT3 pos;
    pos.x = ((mousePos.x * 2.0f) / vp.Width - 1.0f) / proj(0, 0);
    pos.y = ((mousePos.y * -2.0f) / vp.Height + 1.0f) / proj(1, 1);
    pos.z = vp.MaxDepth;

    XMVECTOR rayDir = XMVectorSet(pos.x, pos.y, pos.z, 0.0f);

    XMMATRIX inverseView = XMMatrixInverse(nullptr, cam->GetViewMatrix());
    XMMATRIX inverseWorld = XMMatrixInverse(nullptr, obj->GetTransform()->GetWorld());
    XMMATRIX Local = XMMatrixMultiply(inverseView, inverseWorld);

    XMVECTOR orgDir = XMVector2TransformCoord(XMVectorSet(0, 0, 0, 1), Local);
    rayDir = XMVector3Normalize(XMVector3TransformNormal(rayDir, Local));

    if (SphereIntersect(orgDir, rayDir))
    {
        return true;
    }
    return false;
}

bool MeteorScene::InitializeScene()
{
    shared_ptr<Camera> cam = make_shared<Camera>();
    cam->SetPerspective(90.0f, (float)RenderWindow::ScreenWidth / (float)RenderWindow::ScreenHeight,
        0.01f, 1000.0f);
    mainCam.AddComponent(cam);
    mainCam.GetTransform()->SetPos(0.0f, 0.0f, -3.0f);

    vector<Vertex> vertices;
    vector<DWORD> indices;
    shared_ptr<MeshRenderer<Vertex>> render = make_shared<MeshRenderer<Vertex>>();
    GeometryHelper::GeneratorSphere(1.0f, 100, 100, &vertices, &indices);
    render->Initialize(device, dc, "Assets/Textures/Ryan2.jpg", gfx->cb, vertices, indices);
    model = make_unique<GameObject>();
    model->AddComponent(render);
    XMFLOAT3 pos = model->GetTransform()->GetPos();
    campos.GetTransform()->SetPos(pos);

    shared_ptr<SphereCollider> collider = make_shared<SphereCollider>();
    collider->Initialize(1.0f);
    model->AddComponent(collider);

    srand(time(nullptr));
    for (UINT i = 0; i < 100; ++i)
    {
        float radius = (float)(rand() % 4 + 1) / 2.0f;
        unique_ptr<GameObject> obj = make_unique<GameObject>();
        shared_ptr<MeshRenderer<Vertex>> render = make_shared<MeshRenderer<Vertex>>();
        GeometryHelper::GeneratorSphere(radius, 100, 100, &vertices, &indices);
        render->Initialize(device, dc, "Assets/Textures/stone01.dds", gfx->cb, vertices, indices);
        obj->AddComponent(render);

        shared_ptr<SphereCollider> collider = make_shared<SphereCollider>();
        collider->Initialize(radius);
        obj->AddComponent(collider);

        obj->GetTransform()->SetPos((float)(rand() % 100) - 45.0f, (float)(rand() % 100) - 45.0f, (float)(rand() % 100) - 45.0f);

        obstacles.push_back(move(obj));
    }

    vs = ShaderHelper::GetShader(device, VS_SHADER::DEFAULT);
    ps = ShaderHelper::GetShader(device, PS_SHADER::DEFAULT);
    return true;
}

void MeteorScene::Update(float delta)
{
    if (Input::mouse->IsRightDown())
    {
        campos.GetTransform()->Rotate((float)Input::mouse->GetRawY() * 0.03f,
            (float)Input::mouse->GetRawX() * 0.03f, 0.0f);
    }
    model->GetTransform()->Rotate(0.0f, delta * 0.03f, 0.0f);
    XMFLOAT3 rot = campos.GetTransform()->GetRot();
    mainCam.GetTransform()->SetRot(rot);
    XMVECTOR back = campos.GetTransform()->GetBackward();
    mainCam.GetTransform()->SetPos(back * 3.0f);
   
    if (Input::mouse->IsLeftDown())
    {
        for (auto iter = obstacles.begin(); iter != obstacles.end();)
        {
            if (IsPickingObs(*iter))
            {
                iter = obstacles.erase(iter);
                continue;
            }
            iter++;
        }
    }
    if (model != nullptr)
    {
        for (auto iter = obstacles.begin(); iter != obstacles.end();)
        {
            if (model->GetComponent<Collider>()->CheckCrash(*model->GetComponent<Collider>(), *(*iter)->GetComponent<Collider>()))
            {
                iter = obstacles.erase(iter);
                continue;
            }
            iter++;
        }
    }
}

void MeteorScene::RenderFrame()
{
    dc->IASetInputLayout(vs->GetInputLayout());
    dc->VSSetShader(vs->GetShader(), nullptr, 0);
    dc->PSSetShader(ps->GetShader(), nullptr, 0);
    if (model != nullptr)
    {
        model->GetComponent<Renderer>()->Draw(mainCam.GetComponent<Camera>()->GetViewprojection());
    }
    for (UINT i = 0; i < obstacles.size(); ++i)
    {
        obstacles[i]->GetComponent<Renderer>()->Draw(mainCam.GetComponent<Camera>()->GetViewprojection());
    }
}

void MeteorScene::OnGUI()
{
}
