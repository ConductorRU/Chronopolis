#include "stdafx.h"
#include "Camera.h"
#include "../Math/Math.h"
#include "../Mesh/Mesh.h"
#include "../Material/Class.h"
#include "../GUI/GUI.h"
#include "../Render/Render.h"
#include "../Render/InputLayout.h"
#include "../Core/Engine.h"
#include "Light.h"
#include "Scene.h"
namespace DEN
{
	Scene::Scene()
	{
		_camera = new Camera();
		_gui = new GUI();
		_target = nullptr;
	}
	Scene::~Scene()
	{
		delete _gui;
		for(Light *li : _lights)
			delete li;
		delete _camera;
	}
	void Scene::SetRenderTarget(Target *target)
	{
		_target = target;
	}
	void Scene::AddDraw(Draw *draw)
	{
		_draws.insert(draw);
	}
	void Scene::RemoveDraw(Draw *draw)
	{
		_draws.erase(draw);
	}
	void Scene::SetBackground(const Color &color)
	{
		_background = color;
	}
	Color Scene::GetBackground()
	{
		return _background;
	}
	void Scene::AddMesh(Mesh *mesh)
	{
		_meshes.insert(mesh);
	}
	Light *Scene::CreateLight()
	{
		Light *li = new Light();
		_lights.insert(li);
		return li;
	}
	void Scene::PrepareBuffer(RenderMesh *buf, Node *world, Light *light, Pass *pass)
	{
		buf->GetInputLayout()->Bake(pass->GetVS());
		buf->UpdateConst();
		Render::Get()->RenderPass(pass);
		Matrix4 view = _camera->GetViewMatrix().ToMatrix();
		Matrix4 proj = _camera->GetProjMatrix();
		Matrix4 wrld = world->GetMatrix()->ToMatrix();
		Matrix4 wvp = MatrixMultiply(MatrixMultiply(wrld, view), proj);
		Vector cam = _camera->GetWorldPosition();
		Vector pos = light->GetNode()->GetPosition(true);
		//view = view.Transpose();
		//proj = proj.Transpose();
		//wrld = wrld.Transpose();
		//wvp = wvp.Transpose();
		Color col = Color(1.0f, 1.0f, 1.0f);
		buf->Copy(pass->GetVS(), "MatrixBuffer", "world", &wrld);
		buf->Copy(pass->GetVS(), "MatrixBuffer", "view", &view);
		buf->Copy(pass->GetVS(), "MatrixBuffer", "proj", &proj);
		buf->Copy(pass->GetVS(), "lightBuffer", "lightPos", &pos);
		buf->Copy(pass->GetPS(), "matBuffer", "matCol", &col);

		buf->Unmap();
		/*Vector liPos;
		if(light)
			liPos = light->GetNode()->GetPosition();
		buf->Copy(&liPos, 12, 272);
		float lEnable = 0.0f;
		if(light)
			lEnable = 1.0f;
		buf->Copy(&lEnable, 4, 284);
		buf->Copy(&cam, 12, 288);
		Color col;
		buf->Copy(&col, 16, 304);*/
	}
	void Scene::Render()
	{
		int i1 = clock();
		DEN::Render *render = Render::Get();
		_camera->SetAspectRatio(render->GetAspect());
		_camera->Update();
		memset(&stats, 0, sizeof(Scene::Stats));
		for(Mesh *m : _meshes)
		{
			m->BeforeRender();
			if(!m->IsBaked())
				m->Bake();
			if(m->GetPass())
				for(Light *li : _lights)
				{
					PrepareBuffer(m->GetVertexBuffer(), m->GetNode(), li, m->GetPass());
					render->ExecuteMesh(m->GetVertexBuffer());
					stats.vertexCount += m->GetVertexCount();
					stats.indexCount += m->GetIndexCount();
					stats.primitiveCount += stats.indexCount / 3;
				}
		}
		/*for(Draw *d : z_draws)
			for(Light *li : z_lights)
				d->Render(this, li);*/
		/*for(Line *m : z_lines)
		{
			ConstantBuffer *buf = m->GetVertexBuffer();
			Node *world = m->GetNode();
			Matrix4 view = z_camera->GetViewMatrix().ToMatrix();
			Matrix4 proj = z_camera->GetProjMatrix();
			Matrix4 wrld = world->GetMatrix()->ToMatrix();
			Matrix4 wvp = MatrixMultiply(MatrixMultiply(wrld, view), proj);
			wvp = wvp.Transpose();
			buf->Copy(&wvp, 64, 0);

			render->ExecuteMesh(m->GetVertexBuffer(), m->GetMaterial(), 0, D3D_PRIMITIVE_TOPOLOGY_LINELIST);
		}*/
		int t2 = clock() - i1;
		_gui->Render();
		int t3 = clock() - i1 - t2;
		static int fi = 0;
		fi += t3;
		if(Engine::Get()->GetTime().fNum == 0)
		{
			//z_gui->GetRoot()->GetChild(0)->GetProperty().SetInnerText(to_string(t2) + "/" + to_string(fi) + "/" + to_string(z_gui->GetBakeCounter()));
			fi = 0;
		}
	}
	Mesh *Scene::Pick(const Vector &pos, const Vector &dir, Vector &point)
	{
		for (Mesh *m : _meshes)
			if (m->Pick(pos, dir, point))
				return m;
		return nullptr;
	}
}