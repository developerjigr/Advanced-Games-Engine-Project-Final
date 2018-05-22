
#include <assert.h>
#include "OpenAssetImporterMesh.h"

#pragma comment(lib, "lib/assimp.lib")

COpenAssetImporterMesh::MeshEntry::MeshEntry()
{
    NumIndices  = 0;
    MaterialIndex = INVALID_MATERIAL;
};

COpenAssetImporterMesh::MeshEntry::~MeshEntry()
{
}


COpenAssetImporterMesh::COpenAssetImporterMesh()
{
}


COpenAssetImporterMesh::~COpenAssetImporterMesh()
{
    Clear();
}


void COpenAssetImporterMesh::Clear()
{
}


bool COpenAssetImporterMesh::Load(const std::string& Filename)
{
    // Release the previously loaded mesh (if it exists)
    Clear();
    
    bool Ret = false;
    Assimp::Importer Importer;

    const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
    
    if (pScene) {
        Ret = InitFromScene(pScene, Filename);
    }
    else {
        MessageBox(NULL, Importer.GetErrorString(), "Error loading mesh model", MB_ICONHAND);
    }

    return Ret;
}

void COpenAssetImporterMesh::MeshEntry::Init(const std::vector<CVertex>& Vertices,
                          const std::vector<unsigned int>& Indices)
{
    NumIndices = Indices.size();

}

bool COpenAssetImporterMesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
{  
    m_Entries.resize(pScene->mNumMeshes);
    m_Textures.resize(pScene->mNumMaterials);

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        InitMesh(i, paiMesh);
    }

    return InitMaterials(pScene, Filename);
}

void COpenAssetImporterMesh::InitMesh(unsigned int Index, const aiMesh* paiMesh)
{
    m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;
    
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
        const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

        CVertex v(pPos->x, pPos->y, pPos->z,pNormal->x, pNormal->y, pNormal->z, pTexCoord->x, 1.0f-pTexCoord->y);

        m_Entries[Index].Vertices.push_back(v);
    }

    for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) {
        const aiFace& Face = paiMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        m_Entries[Index].Indices.push_back(Face.mIndices[0]);
        m_Entries[Index].Indices.push_back(Face.mIndices[1]);
        m_Entries[Index].Indices.push_back(Face.mIndices[2]);
    }

	m_Entries[Index].Init(m_Entries[Index].Vertices, m_Entries[Index].Indices);



}



bool COpenAssetImporterMesh::InitMaterials(const aiScene* pScene, const std::string& Filename)
{
    // Extract the directory part from the file name
    std::string::size_type SlashIndex = Filename.find_last_of("\\");
    std::string Dir;

    if (SlashIndex == std::string::npos) {
        Dir = ".";
    }
    else if (SlashIndex == 0) {
        Dir = "\\";
    }
    else {
        Dir = Filename.substr(0, SlashIndex);
    }

    bool Ret = true;

    // Initialize the materials
    for (unsigned int i = 0 ; i < pScene->mNumMaterials; i++) {
        const aiMaterial* pMaterial = pScene->mMaterials[i];

        m_Textures[i] = NULL;

		
        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString Path;
			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                std::string FullPath = Dir + "\\" + Path.data;

				m_Textures[i] = new CTexture();
				if (!m_Textures[i]->Load(FullPath, true)) {
 					MessageBox(NULL, FullPath.c_str(), "Error loading mesh texture", MB_ICONHAND);
                    delete m_Textures[i];
                    m_Textures[i] = NULL;
                    Ret = false;
                }
                else {
                    printf("Loaded texture '%s'\n", FullPath.c_str());
                }
            }
        }
		

        // Load a single colour texture matching the diffuse colour if no texture added
        if (!m_Textures[i]) {
		
			aiColor3D color (0.0f,0.0f,0.0f);
			pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE,color);

			m_Textures[i] = new CTexture();
			BYTE data[3];
			data[0] = (BYTE) (color[2]*255);
			data[1] = (BYTE) (color[1]*255);
			data[2] = (BYTE) (color[0]*255);
			m_Textures[i]->CreateFromRGB(data[2], data[1], data[0]);

        }
    }

    return Ret;
}

void COpenAssetImporterMesh::Render()
{
	
    for (unsigned int i = 0; i < m_Entries.size(); i++) {
		
		MeshEntry *pMesh = &m_Entries[i];

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, sizeof(CVertex), &(pMesh->Vertices[0].s));

		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, sizeof(CVertex), &(pMesh->Vertices[0].nx));

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(CVertex), &(pMesh->Vertices[0].x));


        const unsigned int MaterialIndex = pMesh->MaterialIndex;
        if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]) 
            m_Textures[MaterialIndex]->Bind();
        
		glDrawElements(GL_TRIANGLES, pMesh->NumIndices, GL_UNSIGNED_INT, &(pMesh->Indices[0]));

    }



}

