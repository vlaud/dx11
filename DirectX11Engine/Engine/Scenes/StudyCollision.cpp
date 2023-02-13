#include "StudyCollision.h"
#include "../../RenderWindow.h"
#include "../Graphics/Graphics.h"
#include "../Input/Input.h"
#include "../Graphics/Objects/Components/ComponentTypes.h"
#include "../Graphics/Objects/GeometryHelper.h"
#include <time.h>

bool StudyCollision::InitializeScene()
{
    shared_ptr<Camera> cam = make_shared<Camera>();
    cam->SetPerspective(90.0f, (float)RenderWindow::ScreenWidth / (float)RenderWindow::ScreenHeight,
        0.01f, 1000.0f);
    mainCam.AddComponent(cam);
    mainCam.GetTransform()->SetPos(0.0f, 0.0f, -3.0f);

    vector<Vertex> vertices;
    vector<DWORD> indices;
    /*
    shared_ptr<MeshRenderer<Vertex>> render = make_shared<MeshRenderer<Vertex>>();
    GeometryHelper::GeneratorSphere(1.0f, 100, 100, &vertices, &indices);
    render->Initialize(device, dc, "Assets/Textures/Ryan2.jpg", gfx->cb, vertices, indices);
    model.AddComponent(render);
   
    shared_ptr<SphereCollider> collider = make_shared<SphereCollider>();
    collider->Initialize(1.0f);
    model.AddComponent(collider);

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

        obj->GetTransform()->SetPos((float)(rand()%100) - 45.0f, (float)(rand() % 100) - 45.0f, (float)(rand() % 100) - 45.0f);

        obstacles.push_back(move(obj));
    }
    */
    shared_ptr<MeshRenderer<Vertex>> renderBox1 = make_shared<MeshRenderer<Vertex>>();
    GeometryHelper::GeneratorBox(1, 1, 1, &vertices, &indices);
    renderBox1->Initialize(device, dc, "Assets/Textures/Ryan2.jpg", gfx->cb, vertices, indices);
    box1.AddComponent(renderBox1);

    shared_ptr<MeshRenderer<Vertex>> renderBox2 = make_shared<MeshRenderer<Vertex>>();
    GeometryHelper::GeneratorBox(1, 1, 2, &vertices, &indices);
    renderBox2->Initialize(device, dc, "Assets/Textures/Ryan2.jpg", gfx->cb, vertices, indices);
    box2.AddComponent(renderBox2);
    box2.GetTransform()->Translate(box2.GetTransform()->GetForward() * 2.0f);

    shared_ptr<BoxCollider> boxCollider1 = make_shared<BoxCollider>();
    boxCollider1->Initialize(1,1,1);
    box1.AddComponent(boxCollider1);

    shared_ptr<BoxCollider> boxCollider2 = make_shared<BoxCollider>();
    boxCollider2->Initialize(1, 1, 2);
    box2.AddComponent(boxCollider2);

    vs = ShaderHelper::GetShader(device, VS_SHADER::DEFAULT);
    ps = ShaderHelper::GetShader(device, PS_SHADER::DEFAULT);
    return true;
}

void StudyCollision::Update(float delta)
{
    if (Input::mouse->IsRightDown())
    {
        mainCam.GetTransform()->Rotate((float)Input::mouse->GetRawY() * 0.03f,
            (float)Input::mouse->GetRawX() * 0.03f, 0.0f);
    }
    if (Input::keyboard->KeyIsPressed('W'))
    {
        mainCam.GetTransform()->Translate(mainCam.GetTransform()->GetForward() * delta * 3.0f);
    }
    if (Input::keyboard->KeyIsPressed('S'))
    {
        mainCam.GetTransform()->Translate(mainCam.GetTransform()->GetBackward() * delta * 3.0f);
    }
    if (Input::keyboard->KeyIsPressed('A'))
    {
        mainCam.GetTransform()->Translate(mainCam.GetTransform()->GetLeft() * delta * 3.0f);
    }
    if (Input::keyboard->KeyIsPressed('D'))
    {
        mainCam.GetTransform()->Translate(mainCam.GetTransform()->GetRight() * delta * 3.0f);
    }
    if (Input::keyboard->KeyIsPressed('Q'))
    {
        mainCam.GetTransform()->Translate(mainCam.GetTransform()->GetUp() * delta * 3.0f);
    }
    if (Input::keyboard->KeyIsPressed('E'))
    {
        mainCam.GetTransform()->Translate(mainCam.GetTransform()->GetDown() * delta * 3.0f);
    }
    /*
    if (Input::keyboard->KeyIsPressed(VK_UP))
    {
        model.GetTransform()->Translate(mainCam.GetTransform()->GetForward() * delta * 3.0f);
    }
    if (Input::keyboard->KeyIsPressed(VK_DOWN))
    {
        model.GetTransform()->Translate(mainCam.GetTransform()->GetBackward() * delta * 3.0f);
    }
    if (Input::keyboard->KeyIsPressed(VK_LEFT))
    {
        model.GetTransform()->Translate(mainCam.GetTransform()->GetLeft() * delta * 3.0f);
    }
    if (Input::keyboard->KeyIsPressed(VK_RIGHT))
    {
        model.GetTransform()->Translate(mainCam.GetTransform()->GetRight() * delta * 3.0f);
    }
   
    for (auto iter = obstacles.begin(); iter != obstacles.end();)
    {
        if (model.GetComponent<Collider>()->CheckCrash(*model.GetComponent<Collider>(), *(*iter)->GetComponent<Collider>()))
        {
            iter = obstacles.erase(iter);
            continue;
        }
        iter++;
    }*/
    if (Input::keyboard->KeyIsPressed(VK_UP))
    {
        box1.GetTransform()->Translate(box1.GetTransform()->GetForward() * delta * 3.0f);
    }
    if (Input::keyboard->KeyIsPressed(VK_DOWN))
    {
        box1.GetTransform()->Translate(box1.GetTransform()->GetBackward() * delta * 3.0f);
    }
    if (Input::keyboard->KeyIsPressed(VK_LEFT))
    {
        box1.GetTransform()->Rotate(0.0f, -delta * XM_PI, 0.0f);
    }
    if (Input::keyboard->KeyIsPressed(VK_RIGHT))
    {
        box1.GetTransform()->Rotate(0.0f, delta * XM_PI, 0.0f);
    }
    boxCrash = box1.GetComponent<Collider>()->CheckCrash(*box1.GetComponent<Collider>(), *box2.GetComponent<Collider>());
}

void StudyCollision::RenderFrame()
{
    dc->IASetInputLayout(vs->GetInputLayout());
    dc->VSSetShader(vs->GetShader(), nullptr, 0);
    dc->PSSetShader(ps->GetShader(), nullptr, 0);

    /*
    model.GetComponent<Renderer>()->Draw(mainCam.GetComponent<Camera>()->GetViewprojection());
   
    for (UINT i = 0; i < obstacles.size(); ++i)
    {
        obstacles[i]->GetComponent<Renderer>()->Draw(mainCam.GetComponent<Camera>()->GetViewprojection());
    }*/

    if (boxCrash)
    {
        dc->RSSetState(gfx->rsWire.Get());
    }
    box1.GetComponent<Renderer>()->Draw(mainCam.GetComponent<Camera>()->GetViewprojection());
    box2.GetComponent<Renderer>()->Draw(mainCam.GetComponent<Camera>()->GetViewprojection());
}

void StudyCollision::OnGUI()
{
}

void StudyCollision::OnText(unique_ptr<SpriteBatch>& spriteBatch)
{
}

int StudyCollision::GetScore()
{
    return 0;
}
