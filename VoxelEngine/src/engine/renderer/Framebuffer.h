#pragma once

#include "engine/core/Base.h"

#include "glm/glm.hpp"
#include <vector>

namespace VoxelEngine {

	enum class FramebufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,
		RED_INTEGER,

		// Depth/stencil
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureFormat> attachments)
			: Attachments(attachments) {}

		std::vector<FramebufferTextureFormat> Attachments;
	};

	struct FramebufferSpecification
	{
		uint32_t Width = 0, Height = 0;
		FramebufferAttachmentSpecification Attachments;
	};

	class Framebuffer
	{
	public:
		Framebuffer(const FramebufferSpecification& specification);
		~Framebuffer();

		void Bind();
		void Unbind();

		void Resize(int width, int height);

		void Invalidate();

		void BindAttachmentTexture(uint32_t index, uint32_t slot);
		void BindDepthTexture(uint32_t slot);

		const glm::vec2& GetSize() const { return { m_Specification.Width, m_Specification.Height }; }
	private:
		uint32_t m_RendererID = 0;
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureFormat> m_ColorAttachmentSpecifications;
		FramebufferTextureFormat m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment;
	};
}

