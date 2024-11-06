#include "DebugDrawables.h"

#include <DirectXMath.h>
#include <gui/ConversionFunctions.h>

using namespace Drawable;

Point::Point(XMFLOAT3 pos, float size, XMFLOAT4 colour) : pos(pos), size(size), colour(colour) {}
void Point::Draw(ImDrawList* drawList, Camera* camera) {
	auto pair = Project(pos, camera);
	ImVec2 ssPos = pair.first;
	float depth = pair.second;

	if (depth > 1.f) { return; }

	drawList->AddCircleFilled(ssPos, size, ImColor(colour.x, colour.y, colour.z, colour.w));
}


Arrow::Arrow(XMFLOAT3 start, XMFLOAT3 dir, float thickness, XMFLOAT4 colour) : start(start), dir(dir), thickness(thickness), colour(colour) {}
void Arrow::Draw(ImDrawList* drawList, Camera* camera) {
	auto spair = Project(start, camera);
	ImVec2 ssStart = spair.first;
	float depthStart = spair.second;

	XMVECTOR startVec = XMLoadFloat3(&start);
	XMVECTOR worldDirVec = XMLoadFloat3(&dir);
	XMVECTOR endPointVec = startVec + worldDirVec;
	XMVECTOR endPointShortVec = startVec + (worldDirVec * 0.9f);
	XMFLOAT3 endPoint; XMStoreFloat3(&endPoint, endPointVec);
	XMFLOAT3 endPointShort; XMStoreFloat3(&endPointShort, endPointShortVec);
	auto epair = Project(endPoint, camera);
	ImVec2 ssEnd = epair.first;
	float depthEnd = epair.second;

	auto epairShort = Project(endPointShort, camera);
	ImVec2 ssEndShort = epairShort.first;

	if (depthStart > 1.f) { return; }

	ImU32 col = ImColor(colour.x, colour.y, colour.z, colour.w);

	drawList->AddLine(ssStart, ssEndShort, col, thickness);
	
	/*
	float arrowLength; XMStoreFloat(&arrowLength, XMVector3Length(worldDirVec));
	float pointerSize = 1.f;//arrowLength / 2.f;
	constexpr float pointerAngle = XMConvertToRadians(30.f);

	XMFLOAT3 unitX = XMFLOAT3(1.f, 0.f, 0.f); XMVECTOR unitXVec = XMLoadFloat3(&unitX);
	XMFLOAT3 unitY = XMFLOAT3(0.f, 1.f, 0.f); XMVECTOR unitYVec = XMLoadFloat3(&unitY);
	XMFLOAT3 unitZ = XMFLOAT3(0.f, 0.f, 1.f); XMVECTOR unitZVec = XMLoadFloat3(&unitZ);

	XMVECTOR rotAxis = XMVector3Cross(worldDirVec, unitZVec);
	rotAxis = XMVector3Normalize(rotAxis);
	XMFLOAT3 rotAxisF; XMStoreFloat3(&rotAxisF, rotAxis);
	auto rotAxisPair = Project(rotAxisF, camera);
	auto originPair = Project(XMFLOAT3(), camera);
	drawList->AddLine(originPair.first, rotAxisPair.first, IM_COL32_BLACK, 10.f);

	XMVECTOR armVec = XMVector2Normalize(worldDirVec);
	armVec = XMVectorScale(armVec, pointerSize);
	XMVECTOR leftArmVec = XMVector3Transform(armVec, XMMatrixRotationAxis(rotAxis, XM_PI + pointerAngle));
	XMVECTOR rightArmVec = XMVector3Transform(armVec, XMMatrixRotationAxis(rotAxis, XM_PI - pointerAngle));
	leftArmVec = endPointVec + leftArmVec;
	rightArmVec = endPointVec + rightArmVec;

	auto laPair = Project(FLOAT3(leftArmVec), camera);
	auto raPair = Project(FLOAT3(rightArmVec), camera);

	drawList->AddTriangleFilled(ssEnd, laPair.first, raPair.first, col);
	*/
}