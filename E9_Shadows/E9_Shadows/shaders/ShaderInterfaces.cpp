#include "ShaderInterfaces.h"

void ISceneDataShader::UploadSceneData() {
	__Internal_UploadSceneData(SceneData());
}

std::unique_ptr<ISceneDataShader::ISceneData> ISceneDataShader::DefaultSceneData() const {
	return std::make_unique<ISceneDataShader::ISceneData>();
}

ISceneDataShader::ISceneData& ISceneDataShader::SceneData() {
	// Can't do this in constructor cause virtual functions
	if (!pm_sceneData) { pm_sceneData = DefaultSceneData(); }
	return *pm_sceneData;
}