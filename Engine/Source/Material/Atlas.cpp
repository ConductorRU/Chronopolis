#include "stdafx.h"
#include "Texture.h"
#include "Atlas.h"
namespace DEN
{
	AtlasImage::AtlasImage()
	{
		z_texture = new Texture;
		z_width = 0;
		z_height = 0;
	}
	AtlasImage::~AtlasImage()
	{
		delete z_texture;
	}
	void AtlasImage::SetSize(UINT width, UINT height)
	{
		z_width = width;
		z_height = height;
	}
	void AtlasImage::AddPiece(int id, const Rect &rect)
	{
		z_pieces[id] = rect;
	}
	void AtlasImage::Slice(UINT sizeX, UINT sizeY, UINT countX, UINT countY)
	{
		int pNum = 0;
		for(UINT i = 0; i < countX; ++i)
			for(UINT j = 0; j < countY; ++j)
				z_pieces[++pNum] = Rect(int(sizeX*i), int(sizeY*j), int(sizeX*(i + 1)), int(sizeY*(j + 1)));
	}
	Square AtlasImage::GetUV(int id)
	{
		Square s = Square(0.0f, 0.0f, 1.0f, 1.0f);
		if(z_width == 0 || z_height == 0)
			return s;
		auto &el = z_pieces.find(id);
		if(el == z_pieces.end())
			return s;
		s.minX = float(el->second.minX)/float(z_width);
		s.minY = float(el->second.minY) / float(z_height);
		s.maxX = float(el->second.maxX) / float(z_width);
		s.maxY = float(el->second.maxY) / float(z_height);
		return s;
	}
	Atlas::Atlas()
	{
		
	}
	Atlas::~Atlas()
	{
		for(auto &it : z_atlases)
			delete it.second;
	}
	AtlasImage *Atlas::CreateAtlas(int id)
	{
		AtlasImage *atlas = new AtlasImage;
		z_atlases[id] = atlas;
		return atlas;
	}
	AtlasImage *Atlas::GetAtlas(int id)
	{
		auto &it = z_atlases.find(id);
		if(it == z_atlases.end())
			return nullptr;
		return it->second;
	}
	Square Atlas::GetUV(int atlasId, int pieceId)
	{
		AtlasImage *at = GetAtlas(atlasId);
		if(!at)
			return Square(0.0f, 0.0f, 1.0f, 1.0f);
		return at->GetUV(pieceId);
	}
};