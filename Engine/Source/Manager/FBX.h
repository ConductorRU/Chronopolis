#pragma once
#include <fbxsdk.h>
namespace DEN
{
	class FBX
	{
	private:
		static class Mesh *ParseMesh(FbxMesh *mesh);
	public:
		static class Mesh *Import(const string &filename);
	};
};