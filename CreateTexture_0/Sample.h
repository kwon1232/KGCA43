#pragma once
#include "TDevice.h"
//#include "../../include/DirectXTex.h"
//������Ʈ �Ӽ����� C++-> �߰� ���� ���丮�� ../../include �Ѵ�.
#include "DirectXTex.h"

//������Ʈ �Ӽ��� ��Ŀ->�Է�->�߰����Ӽ� d3d11.lib; d3dcompiler.lib; DirectXTex.lib;
//#pragma commnet�� �̿��ϸ� ������Ʈ���� ������ �� �ִ�.
#pragma comment (lib, "DirectXTex.lib")

struct TVector2
{
	float x;
	float y;
	TVector2() { x = 0; y = 0;  }
	TVector2(float _x, float _y) { x = _x; y = _y;  }
};
struct TVector3
{
	float x;
	float y;
	float z;
	TVector3() { x = 0; y = 0; z = 0; }
	TVector3(float _x, float _y, float _z = 0.0f) { x = _x; y = _y; z = _z; }
};
struct TVector4
{
	float x;
	float y;
	float z;
	float w;
	TVector4() { x = 0; y = 0; z = 0; w = 0;}
	TVector4(float _x, float _y, float _z, float _w) { x = _x; y = _y; z = _z; w = _w; }
};

struct TVertex
{
	TVector3 pos;
	TVector4 color;
	TVector2 tex; //uv
	TVertex() {}
	TVertex(TVector3 p, TVector4 c, TVector2 t) { pos = p; color = c; tex = t; }
};

class Sample : public TDevice
{
	std::unique_ptr<DirectX::ScratchImage> m_tex;
	ID3D11ShaderResourceView* m_pTextureSRV = nullptr;//bk.png

	std::vector<TVertex>	m_VertexList;  // �ý��� �޸�
	std::vector<DWORD>		m_IndexList;
	ID3D11Buffer*			m_pVertexBuffer;//  ����ī�� �޸�
	ID3D11Buffer*			m_pIndexBuffer;//  ����ī�� �޸�
	ID3DBlob*				m_pVertexShaderByteCode = nullptr;
	ID3DBlob*				m_pPixelShaderByteCode = nullptr;
	ID3D11VertexShader*		m_pVertexShader = nullptr;
	ID3D11PixelShader*		m_pPixelShader = nullptr;
	ID3D11InputLayout*		m_pVertexlayout = nullptr;
public:
	bool	CreateVertexBuffer();
	bool	CreateIndexBuffer();	
	ID3D11Buffer* CreateBuffer(UINT ByteWidth, UINT BindFlags, void** pAddress);

	bool	CreateVertexShader();
	bool	CreatePixelShader();
	ID3DBlob* CreateShader(LPCWSTR pFileName, LPCSTR pEntrypoint, LPCSTR pTarget);

	bool	CreateInputLayout();

	bool	LoadTexture(std::wstring texFileName);
public:
	bool    Init()		override;
	bool    Render()	override;
	bool    Release()	override;
};