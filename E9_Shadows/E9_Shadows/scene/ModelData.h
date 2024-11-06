#pragma once

#include <geometry/BaseMesh.h>
#include <Transform.h>
#include <memory>

#include <shaders/BaseShader.h>
#include <shaders/ShaderInterfaces.h>

class ModelData
{
public:
	ModelData(std::unique_ptr<BaseMesh>&&, BaseShader&, ID3D11ShaderResourceView* texture = nullptr, ID3D11ShaderResourceView* normalMap = nullptr, Transform = Transform());

	BaseMesh& Mesh() const;

	::BaseShader& Shader() const;
	void SetShader(::BaseShader*);
	IModelDataShader::IModelData& ShaderData(); const IModelDataShader::IModelData& ShaderData() const;
	template<typename DataType> typename DataType& ShaderDataAs() { return dynamic_cast<DataType&>(ShaderData()); }

	ID3D11ShaderResourceView* Texture() const; void SetTexture(ID3D11ShaderResourceView*);
	ID3D11ShaderResourceView* NormalMap() const;

	Transform& Trans(); const Transform& Trans() const;

	bool ShouldRender() const; void SetShouldRender(bool);

private:
	::BaseShader* p_shader;
	std::unique_ptr<IModelDataShader::IModelData> pm_shaderAdditionalData;
	ID3D11ShaderResourceView* p_texture;
	ID3D11ShaderResourceView* p_normalMap;
	std::unique_ptr<BaseMesh> pm_mesh;
	Transform m_transform;
	bool m_shouldRender = true;
};

template<typename T, typename ...Args> ModelData make_model(BaseShader& shader, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normalMap, Args&& ...args) {
	return ModelData(std::make_unique<T>(args...), shader, texture, normalMap);
}