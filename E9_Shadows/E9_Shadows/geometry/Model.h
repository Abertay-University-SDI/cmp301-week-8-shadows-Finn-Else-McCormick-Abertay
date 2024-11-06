/**
* \class Model mesh
*
* \brief Very basic OBJ loading mesh object
*
* Is treated like a standard mesh object, but loads a basic OBJ file based on provided filename.
* Future version will update/replace this model loader with something more complete.
*
* \author Paul Robertson
*/

#pragma once

#include <geometry/BaseMesh.h>
//#include "TokenStream.h"
#include <vector>
#include <fstream>

using namespace DirectX;

class Model : public BaseMesh
{
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	/** \brief Initialises the mesh and vertex list, but loading in from a file
	* Provide filename to OBJ object, will be loaded and store like other mesh objects.
	* @param device is the renderer device
	* @param device context is the renderer device context
	* @param filename is a char* for filename.
	*/
	Model(const DeviceInfo&, const char* filename);
	~Model();

protected:
	void InitBuffers();
	void loadModel(const char* filename);
	
	ModelType* model;
};