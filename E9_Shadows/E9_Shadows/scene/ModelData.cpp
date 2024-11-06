#include "ModelData.h"

ModelData::ModelData(std::unique_ptr<BaseMesh>&& mesh, ::BaseShader& shader, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normalMap, Transform trans)
	: p_shader(&shader), pm_shaderAdditionalData(nullptr), pm_mesh(std::move(mesh)), p_texture(texture), p_normalMap(normalMap), m_transform(trans) {

	auto modelDataShaderPtr = dynamic_cast<IModelDataShader*>(&shader);
	if (modelDataShaderPtr) { pm_shaderAdditionalData = modelDataShaderPtr->DefaultModelData(); }
}

BaseMesh& ModelData::Mesh() const { return *pm_mesh; }

BaseShader& ModelData::Shader() const { return *p_shader; }
void ModelData::SetShader(::BaseShader* shader) { p_shader = shader; }

IModelDataShader::IModelData& ModelData::ShaderData() { return *pm_shaderAdditionalData; }
const IModelDataShader::IModelData& ModelData::ShaderData() const { return *pm_shaderAdditionalData; }

ID3D11ShaderResourceView* ModelData::Texture() const { return p_texture; }
void ModelData::SetTexture(ID3D11ShaderResourceView* texture) { p_texture = texture; }
ID3D11ShaderResourceView* ModelData::NormalMap() const { return p_normalMap; }

Transform& ModelData::Trans() { return m_transform; }
const Transform& ModelData::Trans() const { return m_transform; }

bool ModelData::ShouldRender() const { return m_shouldRender; }
void ModelData::SetShouldRender(bool v) { m_shouldRender = v; }