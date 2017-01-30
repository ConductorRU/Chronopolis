#pragma once
namespace DEN
{
	class Mesh;
	class Draw;
	class Color;
	class Camera;
	class Light;
	class ConstantBuffer;
	class Node;
	class InputLayout;
	class Pass;
	class RenderMesh;
	class GUI;
	class Vector;
	class Scene
	{
	private:
		set<Mesh*> _meshes;
		set<Draw*> _draws;
		set<Light*> _lights;
		Color _background;
		Camera *_camera;
		GUI *_gui;
	public:
		struct Stats
		{
			uint vertexCount;
			uint indexCount;
			uint primitiveCount;
		} stats;
		GUI *GetGUI() { return _gui; }
		Camera *GetCamera() { return _camera; }
		Scene();
		~Scene();
		void AddDraw(Draw *draw);
		void RemoveDraw(Draw *draw);
		void SetBackground(const Color &color);
		Color GetBackground();
		void AddMesh(Mesh *mesh);
		Light *CreateLight();
		void PrepareBuffer(RenderMesh *buf, Node *world, Light *light, Pass *pass);
		void Render();
		Mesh *Pick(const Vector &pos, const Vector &dir, Vector &point);
	};
}