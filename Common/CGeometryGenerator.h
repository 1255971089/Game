#pragma once

#include<vector>

#include"global.h"
#include"d3dUtil.h"

_XE_BEGIN

class CGeometryGenerator
{
public:
	static CGeometryGenerator* GetInstance()
	{
		static CGeometryGenerator instance;
		return &instance;
	}
	struct Vertex
	{
		Vertex(){}
		Vertex(const XMFLOAT3& p, const XMFLOAT3& n,
			const XMFLOAT3& t, const XMFLOAT2& uv):
			position(p),normal(n),tangentu(t),texc(uv)
		{}
		Vertex(float px,float py,float pz,
			float nx,float ny,float nz,
			float tx,float ty,float tz,
			float u,float v):
			position(px,py,pz),normal(nx,ny,nz),
			tangentu(tx,ty,tz),texc(u,v)
		{}
		XMFLOAT3 position;	//位置
		XMFLOAT3 normal;  //顶点法线
		XMFLOAT3 tangentu; //顶点切线
		XMFLOAT2 texc;
	};

	struct MeshData
	{
		std::vector<Vertex> vertices; //顶点缓冲
		std::vector<u32> indices;	//索引缓冲
	};

	void CreateBox(f32 width, f32 height, f32 depth, MeshData &mesh);

	void CreateSphere(f32 radius, u32 slice, u32 stack, MeshData &mesh);

	void CreateGeosphere(f32 radius, u32 sum_subdivisions, MeshData& mesh_data);

	void CreateCylinder(f32 topRadius, f32 bottomRadius, f32 height, u32 slice, u32 stack, MeshData &mesh);

	void AddCylinderTopCap(f32 topRadius, f32 bottomRadius, f32 height, u32 slice, u32 stack, MeshData &mesh);

	void AddCylinderBottomCap(f32 topRadius, f32 bottomRadius, f32 height, u32 slice, u32 stack, MeshData &mesh);

	void CreateGrid(f32 width, f32 depth, u32 m, u32 n, MeshData& mesh_data);
};

_XE_END