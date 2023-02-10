#include "StudyPicking.h"
#include "../../RenderWindow.h"
#include "../Graphics/Graphics.h"
#include "../Input/Input.h"
#include "../Graphics/Objects/Components/ComponentTypes.h"
#include "../Graphics/Objects/GeometryHelper.h"

bool StudyPicking::SphereIntersect(XMVECTOR orgDir, XMVECTOR rayDir)
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

bool StudyPicking::TriangleIntersect(XMVECTOR rayPos, XMVECTOR rayDir, XMVECTOR v0, XMVECTOR v1, XMVECTOR v2, float* t, float* u, float* v)
{
    XMVECTOR edge1 = v1 - v0;
    XMVECTOR edge2 = v2 - v0;
    XMVECTOR pvec = XMVector3Cross(rayDir, edge2); // 레이 방향과 edge2 외적 값
    XMVECTOR detV = XMVector3Dot(edge1, pvec); // pvec(레이 방향과 edge2 외적 값)과 edge1 내적 값
    XMFLOAT3 detF;
    XMStoreFloat3(&detF, detV);
    float det = detF.x;

    XMVECTOR tvec;
    if (det > 0.0f)
    {
        tvec = rayPos - v0;
    }
    else
    {
        tvec = v0 - rayPos;
        det = -det;
    }

    if (det < 0.0001f) return false;

    detV = XMVector3Dot(tvec, pvec);
    XMStoreFloat3(&detF, detV);
    *u = detF.x;
    if (*u < 0.0f || *u > det) return false;

    XMVECTOR qvec = XMVector3Cross(tvec, edge1);
    detV = XMVector3Dot(rayDir, qvec);
    XMStoreFloat3(&detF, detV);
    *v = detF.x;
    if (*v < 0.0f || *u + *v > det) return false;

    detV = XMVector3Dot(edge2, qvec);
    XMStoreFloat3(&detF, detV);
    *t = detF.x;
    float fInvDet = 1.0f / det;
    *t *= fInvDet;
    *u *= fInvDet;
    *v *= fInvDet;

    return true;
}

void StudyPicking::Picking()
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

    /*
    if (SphereIntersect(orgDir, rayDir))
    {
        model.release();
    }
    */

    pickFace = -1;
    pickDist = D3D11_FLOAT32_MAX;
    UINT faceCount = 0;
    for (UINT i = 0; i < indices.size(); i += 3)
    {
        XMVECTOR v1 = XMLoadFloat3(&vertices[indices[i]].pos);
        XMVECTOR v2 = XMLoadFloat3(&vertices[indices[i+1]].pos);
        XMVECTOR v3 = XMLoadFloat3(&vertices[indices[i+2]].pos);
        float t, u, v;
        if (TriangleIntersect(orgDir, rayDir, v1, v2, v3, &t, &u, &v))
        {
            //model.release();
            if (pickFace >= 0)
            {
                if (pickDist > t)
                {
                    pickFace = faceCount;
                    pickDist = t;
                }
            }
            else
            {
                pickFace = faceCount;
                pickDist = t;
            }
        }
        faceCount++;
    }
}

bool StudyPicking::InitializeScene()
{
    shared_ptr<Camera> cam = make_shared<Camera>();
    cam->SetPerspective(90.0f, (float)RenderWindow::ScreenWidth / (float)RenderWindow::ScreenHeight,
        0.01f, 1000.0f);
    mainCam.AddComponent(cam);
    mainCam.GetTransform()->SetPos(0.0f, 0.0f, -3.0f);

    shared_ptr<MeshRenderer<Vertex>> render = make_shared<MeshRenderer<Vertex>>();
   
    //GeometryHelper::GeneratorSphere(1.0f, 100, 100, &vertices, &indices);
    GeometryHelper::GeneratorBox(1, 1, 1, &vertices, &indices);
    render->Initialize(device, dc, "Assets/Textures/Ryan2.jpg", gfx->cb, vertices, indices);
    model = make_unique<GameObject>();
    model->AddComponent(render);

    vs = ShaderHelper::GetShader(device, VS_SHADER::DEFAULT);
    ps = ShaderHelper::GetShader(device, PS_SHADER::DEFAULT);
    return true;
}

void StudyPicking::Update(float delta)
{
    if (Input::mouse->IsLeftDown())
    {
        Picking();
    }
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

void StudyPicking::RenderFrame()
{
    dc->IASetInputLayout(vs->GetInputLayout());
    dc->VSSetShader(vs->GetShader(), nullptr, 0);
    dc->PSSetShader(ps->GetShader(), nullptr, 0);
    if (model != nullptr)
    {
        if (pickFace >= 0)
        {
            model->GetComponent<MeshRenderer<Vertex>>()->Draw(mainCam.GetComponent<Camera>()->GetViewprojection(),
                pickFace, vertices, indices, gfx->rsCullBack.Get(), gfx->rsWire.Get());
        }
        else
        {
            model->GetComponent<Renderer>()->Draw(mainCam.GetComponent<Camera>()->GetViewprojection());
        }
    }
}

void StudyPicking::OnGUI()
{
}
