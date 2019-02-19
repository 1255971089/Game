#include "CGeometryGenerator.h"

_XE_BEGIN


void CGeometryGenerator::CreateGrid(f32 width, f32 height, u32 m, u32 n, MeshData &mesh_data)
{
	mesh_data.vertices.clear();
	mesh_data.indices.clear();
	//每行的顶点数
	u32 nVertsRow = m + 1;
	u32 nVertsCol = n + 1;
	//起始的x,z坐标
	f32 oX = -width * 0.5f;
	f32 oZ = height * 0.5f;
	//每一格坐标的变化
	f32 dx = width / m;
	f32 dz = height / n;
	//顶点总数量：nVertsRow * nVertsCol
	mesh_data.vertices.resize(nVertsRow * nVertsCol);
	//逐个添加顶点
	for (UINT i = 0; i < nVertsCol; ++i)
	{
		float tmpZ = oZ - dz * i;
		for (UINT j = 0; j < nVertsRow; ++j)
		{
			UINT index = nVertsRow * i + j;
			mesh_data.vertices[index].position.x = oX + dx * j;
			mesh_data.vertices[index].position.y = 0.f;
			mesh_data.vertices[index].position.z = tmpZ;
			mesh_data.vertices[index].normal = XMFLOAT3(0.f, 1.f, 0.f);
			mesh_data.vertices[index].tangentu = XMFLOAT3(1.f, 0.f, 0.f);
			mesh_data.vertices[index].texc = XMFLOAT2(dx*i, dx*j);
		}

	}
	//总格子数m*n，索引数量为6*m*n(一个格子两个三角形，六个顶点)
	UINT nIndices = m * n * 6;
	mesh_data.indices.resize(nIndices);
	UINT tmp = 0;
	for (UINT i = 0; i < n; ++i)
	{
		for (UINT j = 0; j < m; ++j)
		{
			mesh_data.indices[tmp] = i * nVertsRow + j;
			mesh_data.indices[tmp + 1] = i * nVertsRow + j + 1;
			mesh_data.indices[tmp + 2] = (i + 1) * nVertsRow + j;
			mesh_data.indices[tmp + 3] = i * nVertsRow + j + 1;
			mesh_data.indices[tmp + 4] = (i + 1) * nVertsRow + j + 1;
			mesh_data.indices[tmp + 5] = (i + 1) * nVertsRow + j;
			tmp += 6;
		}
	}

}


//创建立方体
void CGeometryGenerator::CreateBox(f32 width, f32 height, f32 depth, MeshData &mesh)
{
	mesh.vertices.clear();
	mesh.indices.clear();

	//一共24个顶点(每面4个)
	mesh.vertices.resize(24);
	//一共36个索引(每面6个)
	mesh.indices.resize(36);

	f32 halfW = width * 0.5f;
	f32 halfH = height * 0.5f;
	f32 halfD = depth * 0.5f;

	//眼睛面向z轴正方向
	//构建顶点
	//前面
	mesh.vertices[0].position = XMFLOAT3(-halfW, -halfH, -halfD);
	mesh.vertices[0].normal = XMFLOAT3(0.f, 0.f, -1.f);
	mesh.vertices[0].tangentu = XMFLOAT3(1.f, 0.f, 0.f);
	mesh.vertices[0].texc = XMFLOAT2(0.f, 1.f);
	mesh.vertices[1].position = XMFLOAT3(-halfW, halfH, -halfD);
	mesh.vertices[1].normal = XMFLOAT3(0.f, 0.f, -1.f);
	mesh.vertices[1].tangentu = XMFLOAT3(1.f, 0.f, 0.f);
	mesh.vertices[1].texc = XMFLOAT2(0.f, 0.f);
	mesh.vertices[2].position = XMFLOAT3(halfW, halfH, -halfD);
	mesh.vertices[2].normal = XMFLOAT3(0.f, 0.f, -1.f);
	mesh.vertices[2].tangentu = XMFLOAT3(1.f, 0.f, 0.f);
	mesh.vertices[2].texc = XMFLOAT2(1.f, 0.f);
	mesh.vertices[3].position = XMFLOAT3(halfW, -halfH, -halfD);
	mesh.vertices[3].normal = XMFLOAT3(0.f, 0.f, -1.f);
	mesh.vertices[3].tangentu = XMFLOAT3(1.f, 0.f, 0.f);
	mesh.vertices[3].texc = XMFLOAT2(1.f, 1.f);
	//左侧面
	mesh.vertices[4].position = XMFLOAT3(-halfW, -halfH, halfD);
	mesh.vertices[4].normal = XMFLOAT3(-1.f, 0.f, 0.f);
	mesh.vertices[4].tangentu = XMFLOAT3(0.f, 0.f, -1.f);
	mesh.vertices[4].texc = XMFLOAT2(0.f, 1.f);
	mesh.vertices[5].position = XMFLOAT3(-halfW, halfH, halfD);
	mesh.vertices[5].normal = XMFLOAT3(-1.f, 0.f, 0.f);
	mesh.vertices[5].tangentu = XMFLOAT3(0.f, 0.f, -1.f);
	mesh.vertices[5].texc = XMFLOAT2(0.f, 0.f);
	mesh.vertices[6].position = XMFLOAT3(-halfW, halfH, -halfD);
	mesh.vertices[6].normal = XMFLOAT3(-1.f, 0.f, 0.f);
	mesh.vertices[6].tangentu = XMFLOAT3(0.f, 0.f, -1.f);
	mesh.vertices[6].texc = XMFLOAT2(1.f, 0.f);
	mesh.vertices[7].position = XMFLOAT3(-halfW, -halfH, -halfD);
	mesh.vertices[7].normal = XMFLOAT3(-1.f, 0.f, 0.f);
	mesh.vertices[7].tangentu = XMFLOAT3(0.f, 0.f, -1.f);
	mesh.vertices[7].texc = XMFLOAT2(1.f, 1.f);
	//背面
	mesh.vertices[8].position = XMFLOAT3(halfW, -halfH, halfD);
	mesh.vertices[8].normal = XMFLOAT3(0.f, 0.f, 1.f);
	mesh.vertices[8].tangentu = XMFLOAT3(-1.f, 0.f, 0.f);
	mesh.vertices[8].texc = XMFLOAT2(0.f, 1.f);
	mesh.vertices[9].position = XMFLOAT3(halfW, halfH, halfD);
	mesh.vertices[9].normal = XMFLOAT3(0.f, 0.f, 1.f);
	mesh.vertices[9].tangentu = XMFLOAT3(-1.f, 0.f, 0.f);
	mesh.vertices[9].texc = XMFLOAT2(0.f, 0.f);
	mesh.vertices[10].position = XMFLOAT3(-halfW, halfH, halfD);
	mesh.vertices[10].normal = XMFLOAT3(0.f, 0.f, 1.f);
	mesh.vertices[10].tangentu = XMFLOAT3(-1.f, 0.f, 0.f);
	mesh.vertices[10].texc = XMFLOAT2(1.f, 0.f);
	mesh.vertices[11].position = XMFLOAT3(-halfW, -halfH, halfD);
	mesh.vertices[11].normal = XMFLOAT3(0.f, 0.f, 1.f);
	mesh.vertices[11].tangentu = XMFLOAT3(-1.f, 0.f, 0.f);
	mesh.vertices[11].texc = XMFLOAT2(1.f, 1.f);
	//右侧面
	mesh.vertices[12].position = XMFLOAT3(halfW, -halfH, -halfD);
	mesh.vertices[12].normal = XMFLOAT3(1.f, 0.f, 0.f);
	mesh.vertices[12].tangentu = XMFLOAT3(0.f, 0.f, 1.f);
	mesh.vertices[12].texc = XMFLOAT2(0.f, 1.f);
	mesh.vertices[13].position = XMFLOAT3(halfW, halfH, -halfD);
	mesh.vertices[13].normal = XMFLOAT3(1.f, 0.f, 0.f);
	mesh.vertices[13].tangentu = XMFLOAT3(0.f, 0.f, 1.f);
	mesh.vertices[13].texc = XMFLOAT2(0.f, 0.f);
	mesh.vertices[14].position = XMFLOAT3(halfW, halfH, halfD);
	mesh.vertices[14].normal = XMFLOAT3(1.f, 0.f, 0.f);
	mesh.vertices[14].tangentu = XMFLOAT3(0.f, 0.f, 1.f);
	mesh.vertices[14].texc = XMFLOAT2(1.f, 0.f);
	mesh.vertices[15].position = XMFLOAT3(halfW, -halfH, halfD);
	mesh.vertices[15].normal = XMFLOAT3(1.f, 0.f, 0.f);
	mesh.vertices[15].tangentu = XMFLOAT3(0.f, 0.f, 1.f);
	mesh.vertices[15].texc = XMFLOAT2(1.f, 1.f);
	//上面
	mesh.vertices[16].position = XMFLOAT3(-halfW, halfH, -halfD);
	mesh.vertices[16].normal = XMFLOAT3(0.f, 1.f, 0.f);
	mesh.vertices[16].tangentu = XMFLOAT3(1.f, 0.f, 0.f);
	mesh.vertices[16].texc = XMFLOAT2(0.f, 1.f);
	mesh.vertices[17].position = XMFLOAT3(-halfW, halfH, halfD);
	mesh.vertices[17].normal = XMFLOAT3(0.f, 1.f, 0.f);
	mesh.vertices[17].tangentu = XMFLOAT3(1.f, 0.f, 0.f);
	mesh.vertices[17].texc = XMFLOAT2(0.f, 0.f);
	mesh.vertices[18].position = XMFLOAT3(halfW, halfH, halfD);
	mesh.vertices[18].normal = XMFLOAT3(0.f, 1.f, 0.f);
	mesh.vertices[18].tangentu = XMFLOAT3(1.f, 0.f, 0.f);
	mesh.vertices[18].texc = XMFLOAT2(1.f, 0.f);
	mesh.vertices[19].position = XMFLOAT3(halfW, halfH, -halfD);
	mesh.vertices[19].normal = XMFLOAT3(0.f, 1.f, 0.f);
	mesh.vertices[19].tangentu = XMFLOAT3(1.f, 0.f, 0.f);
	mesh.vertices[19].texc = XMFLOAT2(1.f, 1.f);
	//底面
	mesh.vertices[20].position = XMFLOAT3(-halfW, -halfH, halfD);
	mesh.vertices[20].normal = XMFLOAT3(0.f, -1.f, 0.f);
	mesh.vertices[20].tangentu = XMFLOAT3(1.f, 0.f, 0.f);
	mesh.vertices[20].texc = XMFLOAT2(0.f, 1.f);
	mesh.vertices[21].position = XMFLOAT3(-halfW, -halfH, -halfD);
	mesh.vertices[21].normal = XMFLOAT3(0.f, -1.f, 0.f);
	mesh.vertices[21].tangentu = XMFLOAT3(1.f, 0.f, 0.f);
	mesh.vertices[21].texc = XMFLOAT2(0.f, 0.f);
	mesh.vertices[22].position = XMFLOAT3(halfW, -halfH, -halfD);
	mesh.vertices[22].normal = XMFLOAT3(0.f, -1.f, 0.f);
	mesh.vertices[22].tangentu = XMFLOAT3(1.f, 0.f, 0.f);
	mesh.vertices[22].texc = XMFLOAT2(1.f, 0.f);
	mesh.vertices[23].position = XMFLOAT3(halfW, -halfH, halfD);
	mesh.vertices[23].normal = XMFLOAT3(0.f, -1.f, 0.f);
	mesh.vertices[23].tangentu = XMFLOAT3(1.f, 0.f, 0.f);
	mesh.vertices[23].texc = XMFLOAT2(1.f, 1.f);

	//构建索引
	mesh.indices[0] = 0;
	mesh.indices[1] = 1;
	mesh.indices[2] = 2;
	mesh.indices[3] = 0;
	mesh.indices[4] = 2;
	mesh.indices[5] = 3;

	mesh.indices[6] = 4;
	mesh.indices[7] = 5;
	mesh.indices[8] = 6;
	mesh.indices[9] = 4;
	mesh.indices[10] = 6;
	mesh.indices[11] = 7;

	mesh.indices[12] = 8;
	mesh.indices[13] = 9;
	mesh.indices[14] = 10;
	mesh.indices[15] = 8;
	mesh.indices[16] = 10;
	mesh.indices[17] = 11;

	mesh.indices[18] = 12;
	mesh.indices[19] = 13;
	mesh.indices[20] = 14;
	mesh.indices[21] = 12;
	mesh.indices[22] = 14;
	mesh.indices[23] = 15;

	mesh.indices[24] = 16;
	mesh.indices[25] = 17;
	mesh.indices[26] = 18;
	mesh.indices[27] = 16;
	mesh.indices[28] = 18;
	mesh.indices[29] = 19;

	mesh.indices[30] = 20;
	mesh.indices[31] = 21;
	mesh.indices[32] = 22;
	mesh.indices[33] = 20;
	mesh.indices[34] = 22;
	mesh.indices[35] = 23;
}

//创建圆柱
void CGeometryGenerator::CreateCylinder(f32 topRadius, f32 bottomRadius, f32 height, u32 slice, u32 stack, MeshData &mesh)
{
	mesh.vertices.clear();
	mesh.indices.clear();

	//从上到下每个stack半径变化量:dRadius
	f32 dRadius = (bottomRadius - topRadius) / stack;
	//每个stack高度:dHeight
	f32 dHeight = height / stack;

	//每个圆周上顶点数量:slice+1
	u32 vertsPerRow = slice + 1;
	//顶点行数:stack+1
	u32 nRows = stack + 1;

	//总顶点数
	u32 nVerts = vertsPerRow * nRows;
	//总索引数
	u32 nIndices = slice * stack * 6;

	mesh.vertices.resize(nVerts);
	mesh.indices.resize(nIndices);

	//顶部Y坐标
	f32 topY = height * 0.5f;

	for (u32 i = 0; i < nRows; ++i)
	{
		f32 tmpY = topY - dHeight * i;
		f32 tmpRadius = topRadius + i * dRadius;

		for (u32 j = 0; j < vertsPerRow; ++j)
		{
			f32 theta = XM_2PI * j / slice;
			u32 index = i * vertsPerRow + j;
			mesh.vertices[index].position = XMFLOAT3(tmpRadius*cos(theta), tmpY, tmpRadius*sin(theta));
		}
	}

	UINT tmp(0);
	for (u32 i = 0; i < stack; ++i)
	{
		for (u32 j = 0; j < slice; ++j)
		{
			mesh.indices[tmp] = i * vertsPerRow + j;
			mesh.indices[tmp + 1] = (i + 1) * vertsPerRow + j + 1;
			mesh.indices[tmp + 2] = (i + 1) * vertsPerRow + j;
			mesh.indices[tmp + 3] = i * vertsPerRow + j;
			mesh.indices[tmp + 4] = i * vertsPerRow + j + 1;
			mesh.indices[tmp + 5] = (i + 1) * vertsPerRow + j + 1;

			tmp += 6;
		}
	}
}
//圆柱上底面
void CGeometryGenerator::AddCylinderTopCap(f32 topRadius, f32 bottomRadius, f32 height, u32 slice, u32 stack, MeshData &mesh)
{
	UINT start = mesh.vertices.size();

	for (u32 i = 0; i < slice + 1; ++i)
	{
		f32 theta = XM_2PI * i / slice;

		f32 x = topRadius * cosf(theta);
		f32 y = height * 0.5f;
		f32 z = topRadius * sinf(theta);

		f32 u = x / height + 0.5f;
		f32 v = z / height + 0.5f;

		mesh.vertices.push_back(Vertex(XMFLOAT3(x, y, z), XMFLOAT3(0.f, 1.f, 0.f), XMFLOAT3(1.f, 0.f, 0.f), XMFLOAT2(u, v)));
	}

	mesh.vertices.push_back(Vertex(XMFLOAT3(0.f, height*0.5f, 0.f), XMFLOAT3(0.f, 1.f, 0.f), XMFLOAT3(1.f, 0.f, 0.f), XMFLOAT2(0.5f, 0.5f)));

	UINT center = mesh.vertices.size() - 1;
	for (u32 i = 0; i < slice; ++i)
	{
		mesh.indices.push_back(center);
		mesh.indices.push_back(start + i + 1);
		mesh.indices.push_back(start + i);
	}
}
//圆柱下底面
void CGeometryGenerator::AddCylinderBottomCap(f32 topRadius, f32 bottomRadius, f32 height, u32 slice, u32 stack, MeshData &mesh)
{
	UINT start = mesh.vertices.size();

	for (u32 i = 0; i < slice + 1; ++i)
	{
		f32 theta = XM_2PI * i / slice;

		f32 x = bottomRadius * cosf(theta);
		f32 y = -height * 0.5f;
		f32 z = bottomRadius * sinf(theta);

		f32 u = x / height + 0.5f;
		f32 v = z / height + 0.5f;

		mesh.vertices.push_back(Vertex(XMFLOAT3(x, y, z), XMFLOAT3(0.f, -1.f, 0.f), XMFLOAT3(1.f, 0.f, 0.f), XMFLOAT2(u, v)));
	}

	mesh.vertices.push_back(Vertex(XMFLOAT3(0.f, -height * 0.5f, 0.f), XMFLOAT3(0.f, -1.f, 0.f), XMFLOAT3(1.f, 0.f, 0.f), XMFLOAT2(0.5f, 0.5f)));

	UINT center = mesh.vertices.size() - 1;
	for (u32 i = 0; i < slice; ++i)
	{
		mesh.indices.push_back(center);
		mesh.indices.push_back(start + i);
		mesh.indices.push_back(start + i + 1);
	}
}
//创建球
void CGeometryGenerator::CreateSphere(f32 radius, u32 slice, u32 stack, MeshData &mesh)
{

	mesh.vertices.clear();
	mesh.indices.clear();

	u32 vertsPerRow = slice + 1;
	u32 nRows = stack - 1;

	u32 nVerts = vertsPerRow * nRows + 2;
	u32 nIndices = (nRows - 1)*slice * 6 + slice * 6;

	mesh.vertices.resize(nVerts);
	mesh.indices.resize(nIndices);

	for (u32 i = 1; i <= nRows; ++i)
	{
		f32 phy = XM_PI * i / stack;
		f32 tmpRadius = radius * sin(phy);
		for (u32 j = 0; j < vertsPerRow; ++j)
		{
			f32 theta = XM_2PI * j / slice;
			UINT index = (i - 1)*vertsPerRow + j;

			f32 x = tmpRadius * cos(theta);
			f32 y = radius * cos(phy);
			f32 z = tmpRadius * sin(theta);

			//位置坐标
			mesh.vertices[index].position = XMFLOAT3(x, y, z);
			//法线
			XMVECTOR N = XMVectorSet(x, y, z, 0.f);
			XMStoreFloat3(&mesh.vertices[index].normal, XMVector3Normalize(N));
			//切线
			XMVECTOR T = XMVectorSet(-sin(theta), 0.f, cos(theta), 0.f);
			XMStoreFloat3(&mesh.vertices[index].tangentu, XMVector3Normalize(T));
			//纹理坐标
			mesh.vertices[index].texc = XMFLOAT2(j*1.f / slice, i*1.f / stack);
		}
	}

	u32 size = vertsPerRow * nRows;
	//添加顶部和底部两个顶点信息
	mesh.vertices[size].position = XMFLOAT3(0.f, radius, 0.f);
	mesh.vertices[size].normal = XMFLOAT3(0.f, 1.f, 0.f);
	mesh.vertices[size].tangentu = XMFLOAT3(1.f, 0.f, 0.f);
	mesh.vertices[size].texc = XMFLOAT2(0.f, 0.f);

	mesh.vertices[size + 1].position = XMFLOAT3(0.f, -radius, 0.f);
	mesh.vertices[size + 1].normal = XMFLOAT3(0.f, -1.f, 0.f);
	mesh.vertices[size + 1].tangentu = XMFLOAT3(1.f, 0.f, 0.f);
	mesh.vertices[size + 1].texc = XMFLOAT2(0.f, 1.f);

	UINT tmp(0);
	u32 start1 = 0;
	u32 start2 = mesh.vertices.size() - vertsPerRow - 2;
	u32 top = size;
	u32 bottom = size + 1;
	for (u32 i = 0; i < slice; ++i)
	{
		mesh.indices[tmp] = top;
		mesh.indices[tmp + 1] = start1 + i + 1;
		mesh.indices[tmp + 2] = start1 + i;

		tmp += 3;
	}

	for (u32 i = 0; i < slice; ++i)
	{
		mesh.indices[tmp] = bottom;
		mesh.indices[tmp + 1] = start2 + i;
		mesh.indices[tmp + 2] = start2 + i + 1;

		tmp += 3;
	}

	for (u32 i = 0; i < nRows - 1; ++i)
	{
		for (u32 j = 0; j < slice; ++j)
		{
			mesh.indices[tmp] = i * vertsPerRow + j;
			mesh.indices[tmp + 1] = (i + 1) * vertsPerRow + j + 1;
			mesh.indices[tmp + 2] = (i + 1) * vertsPerRow + j;
			mesh.indices[tmp + 3] = i * vertsPerRow + j;
			mesh.indices[tmp + 4] = i * vertsPerRow + j + 1;
			mesh.indices[tmp + 5] = (i + 1) * vertsPerRow + j + 1;

			tmp += 6;
		}
	}
}
_XE_END
