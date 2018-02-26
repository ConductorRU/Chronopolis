#pragma once
#include <fbxsdk.h>
namespace DEN
{
	class FBX
	{
	private:

	public:
		static class Mesh *Import(const string &filename);
	};
};