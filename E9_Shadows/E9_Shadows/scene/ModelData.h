#pragma once

#include <geometry/BaseMesh.h>
#include <Transform.h>
#include <shaders/Shader.h>
#include <memory>

class ModelData
{
public:
	ModelData(std::unique_ptr<BaseMesh>&&, Shader&, ID3D11ShaderResourceView* texture = nullptr, ID3D11ShaderResourceView* normalMap = nullptr, Transform = Transform());

	BaseMesh& Mesh() const;

	::Shader& Shader() const;
	void SetShader(::Shader*);
	Shader::IModelData& ShaderData(); const Shader::IModelData& ShaderData() const;
	template<typename DataType> typename DataType& ShaderDataAs() { return dynamic_cast<typename DataType&>(ShaderData()); }

	ID3D11ShaderResourceView* Texture() const; void SetTexture(ID3D11ShaderResourceView*);
	ID3D11ShaderResourceView* NormalMap() const;

	Transform& Trans(); const Transform& Trans() const;

	bool ShouldRender() const; void SetShouldRender(bool);

private:
	::Shader* p_shader;
	std::unique_ptr<Shader::IModelData> pm_shaderAdditionalData;
	ID3D11ShaderResourceView* p_texture;
	ID3D11ShaderResourceView* p_normalMap;
	std::unique_ptr<BaseMesh> pm_mesh;
	Transform m_transform;
	bool m_shouldRender = true;
};

template<typename T, typename ...Args> ModelData make_model(Shader& shader, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normalMap, Args&& ...args) {
	return ModelData(std::make_unique<T>(args...), shader, texture, normalMap);
}