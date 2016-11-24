#pragma once
#include "../Material/Shader.h"
namespace DEN
{
	class Pass;
	class ConstantBuffer
	{
	private:
		Pass *_pass;
		Shader *_shaders[5];
		bool _upd[5];
		bool _updPass;
		struct Buffer
		{
			ID3D11Buffer *buf;
			UCHAR *data;
			UINT size;
			bool isChange;
			uint index;
		};
		vector<Buffer> _buffers[5];
		void ReleaseConst(SHADER_TYPE type);
		bool UpdateConst(Shader *sh);
	public:
		ConstantBuffer();
		~ConstantBuffer();
		void Init(Pass *pass);
		void Init(VertexShader *sh);
		void Init(PixelShader *sh);
		void Init(GeometryShader *sh);
		void Init(HullShader *sh);
		void Init(DomainShader *sh);
		bool UpdateConst();
		void Map();
		void Unmap();
		//void *Get();
		void Copy(Shader *sh, const string &buffer, const string &field, void *data);
		void Copy(SHADER_TYPE sType, uint bufferId, void *data, UINT size, UINT offset = 0);
		void Clear();
	};
}