/**
* \class AModel
*
* \brief Improved model loader, using the assimp library
*
* Inherits from Base Mesh, read a provided file and builds a mesh from the file data.
*
* \author Paul Robertson
*/

#pragma once

#include <geometry/BaseMesh.h>
#include "assimp\Importer.hpp"      // C++ importer interface
#include "assimp\scene.h"           // Output data structure
#include "assimp\postprocess.h"     // Post processing flags
#include <vector>

using namespace DirectX;

class AModel : public BaseMesh
{
public:
	/** \brief Imports model and builds mesh representation.
	*
	* Loads a sub-set of model. Tested with single mesh FBX and OBJ. Currently does not auto load textures. 
	* @param device is the renderer device
	* @param file path to model file
	*/
	AModel(const DeviceInfo&, const std::string& file);
	~AModel();

protected:
	void InitBuffers();
	void importModel(const std::string& pFile);
	void modelProcessing(const aiScene* scene);

	void processScene(const aiScene* scene);
	void processNode(const aiNode* node, const aiScene* scene);
	void processMesh(const aiMesh* mesh, const aiScene* scene);

	std::vector<VertexType> vertices;
	std::vector<unsigned long> indices;
};