#ifndef JL_API_DEFINES_HPP
#define JL_API_DEFINES_HPP

	// Define attribute specifiers when compiling on Windows
	#ifdef JL_OS_WINDOWS
		#ifdef JL_API_EXPORT
			
			#define JL_API_ATTRIBUTE __declspec(dllexport)

		#else

			#define JL_API_ATTRIBUTE __declspec(dllimport)

		#endif
	#else

		#define JL_API_ATTRIBUTE

	#endif

#endif