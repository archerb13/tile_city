#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// This is a bit of a hack due to a stupid design decision
// in Visual Studio's testing framework. The default directory
// is getting reset prior to every test to the binary directory,
// which is not where our test data lives. Sadly, the behavior is
// not even consistent. So, I get a path to the correct directory
// and save it into a local variable. Then I set the directory prior
// to teach test, so it is where the data lives. It takes a global
// variable to do so, which is really sad, but that avoids adding 
// a header every test will have to include.
wchar_t g_dir[1000];

namespace Testing
{		
    ULONG_PTR           gdiplusToken;

    TEST_MODULE_INITIALIZE(Initialize)
    {
        Gdiplus::GdiplusStartupInput gdiplusStartupInput;
        // Initialize GDI+.
        Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

        ::SetCurrentDirectory(L"..");
        ::GetCurrentDirectory(sizeof(g_dir), g_dir);
    }

    TEST_MODULE_CLEANUP(Cleanup)
    {
        Gdiplus::GdiplusShutdown(gdiplusToken);
    }

	TEST_CLASS(UnitTest1)
	{
	public:
		


	};
}