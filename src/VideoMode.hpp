#ifndef JL_VIDEOMODE_HPP
#define JL_VIDEOMODE_HPP

namespace jl
{
	enum OpenGLProfiles
	{
		Core,
		Compatibility,
		ES
	};

	class VideoMode
	{

	public:

		explicit VideoMode(OpenGLProfiles glProfile);
	};
};

#endif