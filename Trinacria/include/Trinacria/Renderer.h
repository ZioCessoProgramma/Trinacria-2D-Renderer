#pragma once

#include <glm/glm.hpp>
#include <string>
#include "Texture.h"
#include <vector>
#include "Shader.h"

namespace TRCN_CORE_NAMESPACE
{
	struct QuadTexCoords
	{
		glm::vec2 Coord0;
		glm::vec2 Coord1;
		glm::vec2 Coord2;
		glm::vec2 Coord3;

		QuadTexCoords(float f) : Coord0(f), Coord1(f), Coord2(f), Coord3(f) { }

		QuadTexCoords(const glm::vec2& coord0, const glm::vec2& coord1, const glm::vec2& coord2, const glm::vec2& coord3) :
			Coord0(coord0), Coord1(coord1), Coord2(coord2), Coord3(coord3) { }

		void Normalize(uint32_t texWidth, uint32_t texHeight);
	};

	struct TriangleTexCoords
	{
		glm::vec2 Coord0;
		glm::vec2 Coord1;
		glm::vec2 Coord2;

		TriangleTexCoords(float f) : Coord0(f), Coord1(f), Coord2(f) { }

		TriangleTexCoords(const glm::vec2& coord0, const glm::vec2& coord1, const glm::vec2& coord2) :
			Coord0(coord0), Coord1(coord1), Coord2(coord2) { }

		void Normalize(uint32_t texWidth, uint32_t texHeight);
	};

	struct Vertex
	{
		glm::vec2 Position;
		int TextureIndex; // Using float 'cause GPUs like floats
		glm::vec2 TexCoords;
		glm::vec3 Color;

		Vertex(const glm::vec2& pos, float textureIndex, const glm::vec2& texCoords, const glm::vec3& color) : Position(pos),
			TextureIndex(textureIndex), TexCoords(texCoords), Color(color) { }
	};

	enum class TriangleOrientation
	{
		Orientation_RIGHT, Orientation_LEFT
	};
	
	class Renderer
	{
	public:
		static void Init();

		static void ClearColorBuffer(const glm::vec3& color = { 0.1f, 0.1f, 0.1f });

		static void CreateQuad(
			const glm::vec2& position,
			Texture* texture,
			const glm::vec2& size = glm::vec2(0.5f),
			const glm::vec3& color = glm::vec3(1.f),
			const glm::mat4& transform = glm::mat4(1.0f),
			const QuadTexCoords& texCoords = QuadTexCoords(glm::vec2(0.f), glm::vec2(1.f, 0.f), glm::vec2(1.f), glm::vec2(0.f, 1.f))
		);

		static void CreateQuad(
			const glm::vec2& position,
			class Sprite* sprite,
			const glm::vec2& size = glm::vec2(0.5f),
			const glm::vec3& color = glm::vec3(1.f),
			const glm::mat4& transform = glm::mat4(1.0f)
		);

		static void CreateTriangle(
			const glm::vec2& position,
			Texture* texture,
			TriangleOrientation orientation = TriangleOrientation::Orientation_RIGHT,
			const glm::vec2& size = glm::vec2(0.5f),
			const glm::vec3& color = glm::vec3(1.f),
			const glm::mat4& transform = glm::mat4(1.f),
			const TriangleTexCoords& texCoords = TriangleTexCoords(glm::vec2(0.f), glm::vec2(1.f, 0.f), glm::vec2(0.f, 1.f))
		);

		static void CreateTriangle(
			const glm::vec2& position,
			Sprite* sprite,
			TriangleOrientation orientation = TriangleOrientation::Orientation_RIGHT,
			const glm::vec2& size = glm::vec2(0.5f),
			const glm::vec3& color = glm::vec3(1.f),
			const glm::mat4& transform = glm::mat4(1.f)
		);

		static void EndScene();
		static void Draw();

		static void CleanUp();

		static void FlushBuffers();

		static Shader ShaderProgram;

		static const size_t MaxQuads = 1000;
		static const size_t MaxQuadVertices = MaxQuads * 4;
		static const size_t MaxQuadIndices = MaxQuads * 6;

		static const size_t MaxTriangles = 1000;
		static const size_t MaxTrianglesVertices = MaxTriangles * 3;

	private:
		static std::vector<Vertex> _quadBuffer;
		static std::vector<uint32_t> _quadIndexBuffer;

		static std::vector<Vertex> _triangleBuffer;

		static uint32_t _vao, _vbo, _ebo;
		static uint32_t _triangleVao, _triangleVbo;

		static std::vector<std::pair<Texture*, uint32_t>> _textures;

		static void attribPointer(uint32_t location, uint32_t nParameters, uint32_t parameterType,
			size_t stride, void* offsetFromOrigin);

		static bool findTextureIndex(uint32_t& out, const Texture* textureToFind);
	};
}
