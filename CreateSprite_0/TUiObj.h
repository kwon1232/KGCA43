#pragma once
#include "TPlaneShape.h"

enum T_CONTROL_STATE
{
	T_STATE_NORMAL = 0,
	T_STATE_MOUSEOVER,	// HOVER
	T_STATE_PRESSED,	// PUSH				
	T_STATE_DISABLED,
	
	//T_STATE_HOLD,       // HOLD
	//T_STATE_SELECT,		// PUSH+UP
	//T_STATE_HIDDEN,
	T_STATE_MAXSTATE,
};

class TUiObj 
{
public:
	W_STR                   m_csName;
	RECT					m_rtRect;
	T_CONTROL_STATE			m_uiState;
	BOOL					m_isSelected = FALSE;
	std::vector<TTexture*>  m_pTexArray;
	std::vector<TVertex>	m_VertexList;  // �ý��� �޸�
	TTexture*	m_ptTex		= nullptr;
public:
	virtual bool    Init();
	virtual bool    Create(W_STR name) {
		return true;
	};
	virtual bool    Create(W_STR name, T_STR_VECTOR texFileName);
	virtual bool    Create(W_STR name, W_STR texFileName);
	virtual bool    Load(std::wstring LoadFileName);
	virtual bool	LoadTexture(T_STR_VECTOR texArray);
	virtual bool    LoadTexture(std::wstring texFileName);
	virtual bool    PreRender();
	virtual bool    Frame(float fElapsTime);
	virtual bool    Render(ID3D11DeviceContext* pd3dContext);
	virtual bool    PostRender();
	virtual bool    Release();
};

class TUiNumber : public TUiObj
{
public:	
	UINT    m_iApplyIndex = 0;
	float   m_fTimer = 0.0f;
	float   m_fAnimationTime = 1.0f;
	float   m_fStepTime = 0.0f;
public:	
	bool    Create(W_STR name);
	bool	LoadTexture();
public:
	bool    Create(W_STR name, T_STR_VECTOR texFileName) override;
	bool	LoadTexture(T_STR_VECTOR texArray) override;	
	bool    Frame(float fElapsTime) override;
	bool    Render(ID3D11DeviceContext* pd3dContext) override;
	bool    Release() override;
};