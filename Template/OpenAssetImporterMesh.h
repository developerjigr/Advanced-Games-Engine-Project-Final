/*

	Copyright 2011 Etay Meiri

	// Modified by Dr Greg Slabaugh to work with OpenGL template

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <map>
#include <vector>
#include "include/glew.h"
#include "include/assimp/Importer.hpp"      // C++ importer interface
#include "include/assimp/scene.h"       // Output data structure
#include "include/assimp/PostProcess.h" // Post processing flags

#include "Texture.h"
#include "Vertex.h"

#define INVALID_OGL_VALUE 0xFFFFFFFF
#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }


class COpenAssetImporterMesh
{
public:
    COpenAssetImporterMesh();
    ~COpenAssetImporterMesh();
    bool Load(const std::string& Filename);
    void Render();

private:
    bool InitFromScene(const aiScene* pScene, const std::string& Filename);
    void InitMesh(unsigned int Index, const aiMesh* paiMesh);
    bool InitMaterials(const aiScene* pScene, const std::string& Filename);
    void Clear();
	

#define INVALID_MATERIAL 0xFFFFFFFF

    struct MeshEntry {
        MeshEntry();

        ~MeshEntry();

		void Init(const std::vector<CVertex>& Vertices,
                  const std::vector<unsigned int>& Indices);
		std::vector<CVertex> Vertices;
		std::vector<unsigned int> Indices;
	  
		unsigned int NumIndices;
        unsigned int MaterialIndex;

    };

    std::vector<MeshEntry> m_Entries;
    std::vector<CTexture*> m_Textures;
	
};



