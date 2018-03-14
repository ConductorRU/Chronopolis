#include "DC.h"
#include "Player.h"
#include "../Game/Game.h"
Player::Player()
{
	_isTargetPos = false;
}
Player::~Player()
{

}
void Player::Initialize(const Color &color)
{
	_basicStats.speed = 1.0f;
	_curStats = _maxStats = _basicStats;

	Manager *man = Engine::Get()->GetManager();
	Pass *pass = man->CreatePass();
	VertexShader *vs = man->CreateVS();
	vs->CompileFile(L"vs.txt", "mainVS");
	pass->SetVS(vs);
	PixelShader *ps = man->CreatePS();
	ps->CompileFile(L"vs.txt", "difPS");
	pass->SetPS(ps);

	Box *box = new Box(Game::Get()->GetInputLayout());
	box->SetVertexColor(color);
	box->SetSize(Vector(50.0_sm, 180.0_sm, 30.0_sm));
	box->SetMaterial(pass);
	box->GetNode()->SetParent(this);
	Game::Get()->GetEngine()->GetScene()->AddMesh(box);
	InputListener *input = new InputListener;
	box->GetNode()->SetPosition(Vector(0.0f, 90.0_sm, 0.0f));
	Engine::Get()->GetInput()->AddListener(input);
	input->onMouseHit = [this](MouseEventClick m, InputListener *lis)
	{
		if (m.btn != 0)
			return false;
		Vector pos;
		Vector nor = Vector::ONE_Y;
		Vector inter = Vector();
		float dist = 0.0f;
		Camera *cam = Game::Get()->GetEngine()->GetScene()->GetCamera();
		if(Engine::Get()->GetScene()->Pick(cam->GetWorldPosition(), cam->GetCursorPos(), inter))
		{
			_targetPos = inter;
			_isTargetPos = true;
		}
		return false;
	};
}
void Player::OnMove()
{
	Vector start = GetPosition(true);
	if(_isTargetPos)
	{
		_targetPos.y = GetPosition().y;
		float spf = Engine::Get()->GetTime().spf;
		Vector v = (_targetPos - start);
		Vector n = v;
		n.Normalize();
		Vector d = n*spf*2.0f;
		if (d.Length() > v.Length())
		{
			SetPosition(_targetPos);
			Node::Update();
			_isTargetPos = false;
		}
		else
		{
			Vector res = start + d;
			SetPosition(res);
			Node::Update();
		}
	}
}
void Player::Update()
{
	float spf = Engine::Get()->GetTime().spf;
	OnMove();
	Vector off = Vector(0, 1.0f, 0);
	Vector pos = GetPosition(true) + off;
	Vector p;
	if(Engine::Get()->GetScene()->Pick(pos, Vector::MINUS_Y, p))
	{
		pos.y = p.y;
		SetPosition(pos, true);
		Node::Update();
	}
}