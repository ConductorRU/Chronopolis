#pragma once
#include "../Math/Rect.h"
#include "../Math/Square.h"
namespace DEN
{
	class Texture;
	class AtlasImage
	{
	private:
		map<int, Rect> z_pieces;
		UINT z_width;
		UINT z_height;
		Texture *z_texture;
	public:
		Texture *GetTexture() { return z_texture; }
		AtlasImage();
		~AtlasImage();
		void SetSize(UINT width, UINT height);
		void AddPiece(int id, const Rect &rect);
		void Slice(UINT sizeX, UINT sizeY, UINT countX, UINT countY);
		Square GetUV(int id);
	};
	class Atlas
	{
	private:
		map<int, AtlasImage*> z_atlases;
	public:
		Atlas();
		~Atlas();
		AtlasImage *CreateAtlas(int id);
		AtlasImage *GetAtlas(int id);
		Square GetUV(int atlasId, int pieceId);
	};
};