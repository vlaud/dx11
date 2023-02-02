#include "SkinnedMeshRenderer.h"
#include "../../../EngineFileSystem.h"
#include "../GameObject.h"
#include "../../../Time.h"
void SkinnedMeshRenderer::LoadAnimation(const aiScene* scene, vector<AnimationClipData>& animDatas)
{
    for (UINT i = 0; i < scene->mNumAnimations; ++i)
    {
        aiAnimation* srcAnim = scene->mAnimations[i];
        AnimationClipData info;
        info.name = srcAnim->mName.C_Str();
        info.duration = (float)srcAnim->mDuration;
        info.tickPerSec = (float)srcAnim->mTicksPerSecond;
        for (UINT n = 0; n < srcAnim->mNumChannels; ++n)
        {
            aiNodeAnim* srcNodeAnim = srcAnim->mChannels[n];
            AnimationNodeData keyFrames;
            keyFrames.name = srcNodeAnim->mNodeName.C_Str();
            for (UINT m = 0; m < srcNodeAnim->mNumPositionKeys; ++m)
            {
                KeyPosition pos;
                pos.pos.x = srcNodeAnim->mPositionKeys[m].mValue.x;
                pos.pos.y = srcNodeAnim->mPositionKeys[m].mValue.y;
                pos.pos.z = srcNodeAnim->mPositionKeys[m].mValue.z;
                pos.time = (float)srcNodeAnim->mPositionKeys[m].mTime;
                keyFrames.position.push_back(pos);
            }
            for (UINT m = 0; m < srcNodeAnim->mNumRotationKeys; ++m)
            {
                KeyRotation rot;
                rot.rot.x = srcNodeAnim->mRotationKeys[m].mValue.x;
                rot.rot.y = srcNodeAnim->mRotationKeys[m].mValue.y;
                rot.rot.z = srcNodeAnim->mRotationKeys[m].mValue.z;
                rot.rot.w = srcNodeAnim->mRotationKeys[m].mValue.w;
                rot.time = (float)srcNodeAnim->mRotationKeys[m].mTime;
                keyFrames.rotation.push_back(rot);
            }
            for (UINT m = 0; m < srcNodeAnim->mNumScalingKeys; ++m)
            {
                KeyScale scl;
                scl.scale.x = srcNodeAnim->mScalingKeys[m].mValue.x;
                scl.scale.y = srcNodeAnim->mScalingKeys[m].mValue.y;
                scl.scale.z = srcNodeAnim->mScalingKeys[m].mValue.z;
                scl.time = (float)srcNodeAnim->mScalingKeys[m].mTime;
                keyFrames.scales.push_back(scl);
            }
            info.keyFrames.emplace(keyFrames.name, keyFrames);
        }
        animDatas.push_back(info);
    }
}

void SkinnedMeshRenderer::MakeAnimationClip(vector<AnimationClipData>& animDatas)
{
    UINT size = animClips.size();
    KeyFramesIter iter;
    unordered_map<string, UINT>::iterator iter2;

    for (UINT i = 0; i < animDatas.size(); ++i)
    {
        UINT idx = size + i;
        animClips.emplace_back(AnimationClip());
        animClips[idx].duration = animDatas[i].duration + 1.0f;
        animClips[idx].name = animDatas[i].name;
        animClips[idx].tickPerSec = animDatas[i].tickPerSec;
        UINT durationCount = (UINT)animClips[idx].duration;
        animClips[idx].keyFrames = make_unique<unique_ptr<AnimationTransform[]>[]>(durationCount);
        for (UINT n = 0; n < durationCount; ++n)
        {
            animClips[idx].keyFrames[n] = make_unique<AnimationTransform[]>(nodes.size());
            for (UINT m = 0; m < nodes.size(); ++m)
            {
                XMFLOAT3 pos, scale;
                XMFLOAT4 rot;

                iter = animDatas[i].keyFrames.find(nodes[m].name);
                if (iter != animDatas[i].keyFrames.end())
                {
                    pos = CalcInterpolatedPosition(iter, (float)n, animClips[idx].duration);
                    rot = CalcInterpolatedRotation(iter, (float)n, animClips[idx].duration);
                    scale = CalcInterpolatedScale(iter, (float)n, animClips[idx].duration);
                }
                else
                {
                    // 11 12 13 14 대각선(scale): 11 22 33 44
                    // 21 22 23 24
                    // 31 32 33 34
                    // 41 42 43 44 : x y z w
                    // 
                    // 11 12 13 
                    // 21 22 23  = rotation 값
                    // 31 32 33 
                    XMFLOAT4X4 mat;
                    XMStoreFloat4x4(&mat, nodes[m].offset);
                    pos = { mat._41, mat._42, mat._43 };

                    XMStoreFloat4(&rot, XMQuaternionRotationMatrix(nodes[m].offset));
                    XMMATRIX R = XMMatrixRotationQuaternion(XMLoadFloat4(&rot));
                    XMMATRIX ST = nodes[m].offset * XMMatrixInverse(nullptr, R);
                    XMStoreFloat4x4(&mat, ST);
                    scale = { mat._11, mat._22, mat._33 };
                }
                animClips[idx].keyFrames[n][m].pos = pos;
                animClips[idx].keyFrames[n][m].rot = rot;
                animClips[idx].keyFrames[n][m].scale = scale;

                iter2 = boneMap.find(nodes[m].name);
                if (iter2 != boneMap.end())
                {
                    int boneIdx = iter2->second;
                    bones[boneIdx].idx = m;
                    bones[boneIdx].parentIdx = nodes[m].parentIdx;
                }
            }
        }
    }
}

void SkinnedMeshRenderer::ProcessBone(aiMesh* mesh, vector<SkinnedVertexWeights>& weights)
{
    for (UINT i = 0; i < mesh->mNumBones; ++i)
    {
        UINT boneIdx = 0;
        string name = mesh->mBones[i]->mName.C_Str();
        if (boneMap.find(name) == boneMap.end())
        {
            boneIdx = boneMap[name] = boneMap.size();

            BoneNode node;
            node.name = name;
            node.offset = XMMatrixTranspose(XMMATRIX(&mesh->mBones[i]->mOffsetMatrix.a1));
            bones.push_back(node);
        }
        else
        {
            boneIdx = boneMap[name];
        }
        for (UINT n = 0; n < mesh->mBones[i]->mNumWeights; ++n)
        {
            UINT vertexID = mesh->mBones[i]->mWeights[n].mVertexId;
            float w = mesh->mBones[i]->mWeights[n].mWeight;
            weights[vertexID].Add(boneIdx, w);
        }
    }
}

void SkinnedMeshRenderer::ProcessKeyFrameAnimation()
{
    vector<XMMATRIX> nodeMatrixes(nodes.size());
    for (UINT i = 0; i < nodes.size(); ++i)
    {
        XMFLOAT3 temp = animClips[animStateInfo.idx].keyFrames[animStateInfo.currentFrame][i].pos;

        XMVECTOR p1 = XMLoadFloat3(&animClips[animStateInfo.idx].keyFrames[animStateInfo.currentFrame][i].pos);
        XMVECTOR p2 = XMLoadFloat3(&animClips[animStateInfo.idx].keyFrames[animStateInfo.nextFrame][i].pos);
        XMVECTOR p = XMVectorLerp(p1, p2, animStateInfo.weight);
        if (isChangeAnimation)
        {
            p1 = XMLoadFloat3(&animClips[changeAnimStateInfo.idx].keyFrames[changeAnimStateInfo.currentFrame][i].pos);
            p2 = XMLoadFloat3(&animClips[changeAnimStateInfo.idx].keyFrames[changeAnimStateInfo.nextFrame][i].pos);
            p1 = XMVectorLerp(p1, p2, changeAnimStateInfo.weight);
            p = XMVectorLerp(p, p1, curBlendTime / blendTime);
        }

        XMVECTOR r1 = XMLoadFloat4(&animClips[animStateInfo.idx].keyFrames[animStateInfo.currentFrame][i].rot);
        XMVECTOR r2 = XMLoadFloat4(&animClips[animStateInfo.idx].keyFrames[animStateInfo.nextFrame][i].rot);
        XMVECTOR r = XMQuaternionSlerp(r1, r2, animStateInfo.weight);
        if (isChangeAnimation)
        {
            r1 = XMLoadFloat4(&animClips[changeAnimStateInfo.idx].keyFrames[changeAnimStateInfo.currentFrame][i].rot);
            r2 = XMLoadFloat4(&animClips[changeAnimStateInfo.idx].keyFrames[changeAnimStateInfo.nextFrame][i].rot);
            r1 = XMQuaternionSlerp(r1, r2, changeAnimStateInfo.weight);
            r = XMQuaternionSlerp(r, r1, curBlendTime / blendTime);
        }

        XMVECTOR s1 = XMLoadFloat3(&animClips[animStateInfo.idx].keyFrames[animStateInfo.currentFrame][i].scale);
        XMVECTOR s2 = XMLoadFloat3(&animClips[animStateInfo.idx].keyFrames[animStateInfo.nextFrame][i].scale);
        XMVECTOR s = XMVectorLerp(s1, s2, animStateInfo.weight);
        if (isChangeAnimation)
        {
            s1 = XMLoadFloat3(&animClips[changeAnimStateInfo.idx].keyFrames[changeAnimStateInfo.currentFrame][i].scale);
            s2 = XMLoadFloat3(&animClips[changeAnimStateInfo.idx].keyFrames[changeAnimStateInfo.nextFrame][i].scale);
            s1 = XMVectorLerp(s1, s2, changeAnimStateInfo.weight);
            s = XMVectorLerp(s, s1, curBlendTime / blendTime);
        }
        XMFLOAT3 pos, scale;
        XMStoreFloat3(&pos, p);
        XMStoreFloat3(&scale, s);
        XMMATRIX T = XMMatrixTranslation(pos.x, pos.y, pos.z);
        XMMATRIX R = XMMatrixRotationQuaternion(r);
        XMMATRIX S = XMMatrixScaling(scale.x, scale.y, scale.z);
        nodeMatrixes[i] = S * R * T;
    }

    for (UINT i = 0; i < nodes.size(); ++i)
    {
        if (nodes[i].parentIdx >= 0)
        {
            nodeMatrixes[i] *= nodeMatrixes[nodes[i].parentIdx];
        }
    }
    for (UINT i = 0; i < bones.size(); ++i)
    {
        boneData.data.bones[i] = bones[i].offset * nodeMatrixes[bones[i].idx];
    }
    if (boneData.Update())
    {
        dc->VSSetConstantBuffers(1, 1, boneData.GetAddressOf());
    }
}

bool SkinnedMeshRenderer::LoadFile(const string& filePath)
{
    directory = GetDirectoryFromPath(filePath);
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filePath, aiProcessPreset_TargetRealtime_Fast | aiProcess_ConvertToLeftHanded);
    if (scene == nullptr) return false;
    ProcessNode(scene->mRootNode, scene, XMMatrixIdentity(), 0, 0);

    vector<AnimationClipData> anims;
    LoadAnimation(scene, anims);
    MakeAnimationClip(anims);

    try
    {
        HRESULT hr = boneData.Initialize(device, dc);
        COM_ERROR(hr, L"컨스턴트 버퍼 생성에 실패 하였습니다.");
    }
    catch (ComException& ex)
    {
        Debug::Log(ex.what());
        return false;
    }
    
    if (animClips.size() > 0)
    {
        boneData.data.hasAnimation = 1;
    }
    return true;
}

void SkinnedMeshRenderer::ProcessNode(aiNode* node, const aiScene* scene, const XMMATRIX& parent,
    UINT idx, UINT parentIdx)
{
    BoneNode _node;
    _node.name = node->mName.C_Str();
    _node.idx = idx;
    _node.parentIdx = parentIdx;
    _node.offset = XMMatrixIdentity();
    nodes.push_back(_node);

    XMMATRIX local = XMMATRIX(&node->mTransformation.a1) * parent;
    for (UINT i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene, local));
    }
    for (UINT i = 0; i < node->mNumChildren; ++i)
    {
        ProcessNode(node->mChildren[i], scene, local, nodes.size(), idx);
    }
}

Mesh<VertexSkinned> SkinnedMeshRenderer::ProcessMesh(aiMesh* mesh, const aiScene* scene, const XMMATRIX& local)
{
    vector<SkinnedVertexWeights> weights(mesh->mNumVertices);
    ProcessBone(mesh, weights);

    vector<VertexSkinned> vertices;
    vector<DWORD> indices;
    for (UINT i = 0; i < mesh->mNumVertices; ++i)
    {
        VertexSkinned v;
        v.pos = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
        v.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
        if (mesh->mTextureCoords[0])
        {
            v.uv.x = (float)mesh->mTextureCoords[0][i].x;
            v.uv.y = (float)mesh->mTextureCoords[0][i].y;
        }
        weights[i].Normalize();
        v.boneIDs.x = (float)weights[i].boneIDs[0];
        v.boneIDs.y = (float)weights[i].boneIDs[1];
        v.boneIDs.z = (float)weights[i].boneIDs[2];
        v.boneIDs.w = (float)weights[i].boneIDs[3];

        v.weights.x = weights[i].weights[0];
        v.weights.y = weights[i].weights[1];
        v.weights.z = weights[i].weights[2];
        v.weights.w = weights[i].weights[3];

        vertices.push_back(v);
    }
    for (UINT i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (UINT n = 0; n < face.mNumIndices; ++n)
        {
            indices.push_back(face.mIndices[n]);
        }
    }
    aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
    vector<Texture> textures = LoadMaterialTextures(mat, aiTextureType::aiTextureType_DIFFUSE, scene);

    return Mesh<VertexSkinned>(device, dc, vertices, indices, textures, local);
}

TextureStorageType SkinnedMeshRenderer::DetermineTextureStorageType(const aiScene* scene, aiMaterial* material, UINT idx, aiTextureType type)
{
    if (material->GetTextureCount(type) == 0) return TextureStorageType::None;

    aiString path;
    material->GetTexture(type, idx, &path);
    string texPath = path.C_Str();
    if (texPath[0] == '*') // 배열
    {
        if (scene->mTextures[0]->mHeight == 0) // 압축되어있음
        {
            return TextureStorageType::EmbeddedIndexCompressed;
        }
        else
        {
            assert("Support does not exist yet for indexed no compressed textures!" && 0);
            return TextureStorageType::EmbeddedIndexNonCompressed;
        }
    }
    if (auto tex = scene->GetEmbeddedTexture(texPath.c_str()))
    {
        if (tex->mHeight == 0)
        {
            return TextureStorageType::EmbeddedCompressed;
        }
        else
        {
            assert("Support does not exist yet for no compressed textures!" && 0);
            return TextureStorageType::EmbeddedNonCompressed;
        }
    }
    if (texPath.find('.') != string::npos)
    {
        return TextureStorageType::Disk;
    }
    return TextureStorageType::Invaild;
}

vector<Texture> SkinnedMeshRenderer::LoadMaterialTextures(aiMaterial* material, aiTextureType type, const aiScene* scene)
{
    vector<Texture> textures;

    TextureStorageType texType = TextureStorageType::Invaild;
    UINT texCount = material->GetTextureCount(type);
    if (texCount == 0)
    {
        texType = TextureStorageType::None;
        aiColor3D aiColor = { 0,0,0 };
        switch (type)
        {
        case aiTextureType_DIFFUSE:
            material->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
            if (aiColor.IsBlack())
            {
                textures.push_back(Texture(device, COLORS::UnLoadedTextureColor, type));
                break;
            }
            textures.push_back(Texture(device, Color(aiColor.r * 255, aiColor.g * 255, aiColor.b * 255), type));
            break;
        }
    }
    else
    {
        for (UINT i = 0; i < texCount; ++i)
        {
            aiString path;
            material->GetTexture(type, i, &path);
            texType = DetermineTextureStorageType(scene, material, i, type);
            switch (texType)
            {
            case TextureStorageType::Disk:
            {
                string filename = directory + '/' + path.C_Str();
                textures.push_back(Texture(device, filename, type));
                break;
            }
            case TextureStorageType::EmbeddedCompressed:
            {
                const aiTexture* tex = scene->GetEmbeddedTexture(path.C_Str());
                Texture embeddedTex(device, (uint8_t*)tex->pcData, tex->mWidth, type);
                textures.push_back(embeddedTex);
                break;
            }
            case TextureStorageType::EmbeddedIndexCompressed:
            {
                int idx = GetTextureIndex(&path);
                Texture embeddedTex(device, (uint8_t*)scene->mTextures[idx]->pcData,
                    scene->mTextures[idx]->mWidth, type);
                textures.push_back(embeddedTex);
                break;
            }
            }
        }
    }

    if (textures.size() == 0)
    {
        textures.push_back(Texture(device, COLORS::UnHandledTextureColor, aiTextureType::aiTextureType_DIFFUSE));
    }
    return textures;
}

int SkinnedMeshRenderer::GetTextureIndex(aiString* aiStr)
{
    assert(aiStr->length >= 2);
    return atoi(&aiStr->C_Str()[1]);
}

void SkinnedMeshRenderer::Update()
{
    if (isChangeAnimation)
    {
        curBlendTime += Time::GetDeltaTime();
        if (curBlendTime >= blendTime)
        {
            isChangeAnimation = false;
            animStateInfo.idx = changeAnimStateInfo.idx;
            animStateInfo.currentFrame = changeAnimStateInfo.currentFrame;
            animStateInfo.nextFrame = changeAnimStateInfo.nextFrame;
        }
        else
        {
            
            changeAnimStateInfo.weight += Time::GetDeltaTime() * animClips[changeAnimStateInfo.idx].tickPerSec;
            if (changeAnimStateInfo.weight >= 1.0f)
            {
                changeAnimStateInfo.weight = 0.0f;
                changeAnimStateInfo.currentFrame++;
                changeAnimStateInfo.nextFrame = changeAnimStateInfo.currentFrame + 1;
                if (changeAnimStateInfo.nextFrame == animClips[changeAnimStateInfo.idx].duration)
                {
                    changeAnimStateInfo.currentFrame = 0;
                    changeAnimStateInfo.nextFrame = 1;
                }
            }
            
        }
    }
    //else
    {
        animStateInfo.weight += Time::GetDeltaTime() * animClips[animStateInfo.idx].tickPerSec;
        if (animStateInfo.weight >= 1.0f)
        {
            animStateInfo.weight = 0.0f;
            animStateInfo.currentFrame++;
            animStateInfo.nextFrame = animStateInfo.currentFrame + 1;
            if (animStateInfo.nextFrame == animClips[animStateInfo.idx].duration)
            {
                animStateInfo.currentFrame = 0;
                animStateInfo.nextFrame = 1;
            }
        }
    }
}

void SkinnedMeshRenderer::Draw(const XMMATRIX vpMat)
{
    Update();
    ProcessKeyFrameAnimation();
    UINT offset = 0;
    dc->VSSetConstantBuffers(0, 1, cb_mat->GetAddressOf());
    for (Mesh<VertexSkinned> mesh : meshes)
    {
        cb_mat->data.world = mesh.GetLocal() * gameObject->GetTransform()->GetWorld();
        cb_mat->data.vp = vpMat;
        cb_mat->Update();
        mesh.Draw();
    }
}

void SkinnedMeshRenderer::AddAnimation(const string& filePath)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filePath, aiProcessPreset_TargetRealtime_Fast | aiProcess_ConvertToLeftHanded);
    if (scene == nullptr) return;
   
    vector<AnimationClipData> anims;
    LoadAnimation(scene, anims);
    MakeAnimationClip(anims);
}

void SkinnedMeshRenderer::ChangeAnimation(int idx, float t)
{
    if (animStateInfo.idx == idx) return;

    isChangeAnimation = true;
    curBlendTime = 0.0f;
    blendTime = t;

    changeAnimStateInfo.idx = idx;
    changeAnimStateInfo.currentFrame = 0;
    changeAnimStateInfo.nextFrame = 1;
}
