#pragma once


namespace Mist {

	class Texture2D {
	public:
		virtual ~Texture2D() {}

		virtual void Bind(uint32_t slot) = 0;
		virtual void Unbind() = 0;

		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
	};

}