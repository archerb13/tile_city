#include "stdafx.h"
#include "CppUnitTest.h"

#include "City.h"
#include "TileRoad.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace Testing
{
	TEST_CLASS(CTileRoadTest)
	{
	public:
        TEST_METHOD_INITIALIZE(methodName)
        {
            extern wchar_t g_dir[];
            ::SetCurrentDirectory(g_dir);
        }

		TEST_METHOD(TestCTileRoadConstruct)
		{
            CCity city;
            CTileRoad road(&city);
		}

        TEST_METHOD(TestCTileRoadAdjacencies)
        {
            CCity city;
            CTileRoad road(&city);

            Assert::AreEqual(wstring(L"roadlr.png"), road.GetFile());

            // Nothing
            road.SetAdjacencies(false, false, false, false);
            Assert::AreEqual(wstring(L"roadlr.png"), road.GetFile());

            // One adjacency
            road.SetAdjacencies(true, false, false, false);
            Assert::AreEqual(wstring(L"roadlr.png"), road.GetFile());

            road.SetAdjacencies(false, true, false, false);
            Assert::AreEqual(wstring(L"roadud.png"), road.GetFile());

            road.SetAdjacencies(false, false, true, false);
            Assert::AreEqual(wstring(L"roadud.png"), road.GetFile());

            road.SetAdjacencies(false, false, false, true);
            Assert::AreEqual(wstring(L"roadlr.png"), road.GetFile());

            // Two adjacencies
            road.SetAdjacencies(true, true, false, false);
            Assert::AreEqual(wstring(L"roadlu.png"), road.GetFile());

            road.SetAdjacencies(true, false, true, false);
            Assert::AreEqual(wstring(L"roadld.png"), road.GetFile());

            road.SetAdjacencies(true, false, false, true);
            Assert::AreEqual(wstring(L"roadlr.png"), road.GetFile());

            road.SetAdjacencies(false, true, true, false);
            Assert::AreEqual(wstring(L"roadud.png"), road.GetFile());

            road.SetAdjacencies(false, true, false, true);
            Assert::AreEqual(wstring(L"roadur.png"), road.GetFile());

            road.SetAdjacencies(false, false, true, true);
            Assert::AreEqual(wstring(L"roaddr.png"), road.GetFile());

            // Three adjacencies
            road.SetAdjacencies(false, true, true, true);
            Assert::AreEqual(wstring(L"roadudr.png"), road.GetFile());

            road.SetAdjacencies(true, false, true, true);
            Assert::AreEqual(wstring(L"roadldr.png"), road.GetFile());

            road.SetAdjacencies(true, true, false, true);
            Assert::AreEqual(wstring(L"roadlur.png"), road.GetFile());

            road.SetAdjacencies(true, true, true, false);
            Assert::AreEqual(wstring(L"roadlud.png"), road.GetFile());

            // All four
            road.SetAdjacencies(true, true, true, true);
            Assert::AreEqual(wstring(L"roadludr.png"), road.GetFile());

        }

	};
}