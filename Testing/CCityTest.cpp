#include "stdafx.h"
#include "CppUnitTest.h"

#include "City.h"
#include "TileRoad.h"
#include "TileLandscape.h"
#include "TileCoalmine.h"
#include "TileBuilding.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Testing
{
	class CTestVisitor : public CTileVisitor
	{
	public:
		virtual void VisitRoad(CTileRoad *road) override { mNumRoads++; }
		int mNumRoads = 0;

		virtual void VisitBuilding(CTileBuilding *building) override { mNumBuildings++; }
		int mNumBuildings = 0;

		virtual void VisitLandscape(CTileLandscape *landscape) override { mNumLandscapes++; }
		int mNumLandscapes = 0;

		virtual void VisitCoalmine(CTileCoalmine *coalmine) override { mNumCoalmines++; }
		int mNumCoalmines = 0;
	};
	TEST_CLASS(CCityTest)
	{
	public:
        TEST_METHOD_INITIALIZE(methodName)
        {
            extern wchar_t g_dir[];
            ::SetCurrentDirectory(g_dir);
        }

		TEST_METHOD(TestCCityConstructor)
		{
            CCity city;
		}

        /**  Tests for the GetAdjacent function
         */
        TEST_METHOD(TestCCityAdjacent)
        {
            CCity city;
            int grid = CCity::GridSpacing;

            // Add a center tile to test
            auto center = make_shared<CTileRoad>(&city);
            center->SetLocation(grid * 10, grid * 17);
            city.Add(center);

            // Upper left
            auto ul = make_shared<CTileRoad>(&city);
            ul->SetLocation(grid * 8, grid * 16);
            city.Add(ul);
            city.SortTiles();

            Assert::IsTrue(city.GetAdjacent(center, -1, -1) == ul, L"Upper left test");
            Assert::IsTrue(city.GetAdjacent(center, 1, -1) == nullptr, L"Upper right null test");

            // Upper right
            auto ur = make_shared<CTileRoad>(&city);
            ur->SetLocation(grid * 12, grid * 16);
            city.Add(ur);

            // Lower left
            auto ll = make_shared<CTileRoad>(&city);
            ll->SetLocation(grid * 8, grid * 18);
            city.Add(ll);

            // Lower right
            auto lr = make_shared<CTileRoad>(&city);
            lr->SetLocation(grid * 12, grid * 18);
            city.Add(lr);

            city.SortTiles();

            Assert::IsTrue(city.GetAdjacent(center, 1, -1) == ur, L"Upper right test");
            Assert::IsTrue(city.GetAdjacent(center, -1, 1) == ll, L"Lower left test");
            Assert::IsTrue(city.GetAdjacent(center, 1, 1) == lr, L"Lower right test");
        }

		TEST_METHOD(TestCCityIterator)
		{
			// Construct a city object
			CCity city;

			// Add some tiles
			auto tile1 = make_shared<CTileRoad>(&city);
			auto tile2 = make_shared<CTileRoad>(&city);
			auto tile3 = make_shared<CTileRoad>(&city);

			city.Add(tile1);
			city.Add(tile2);
			city.Add(tile3);

			// Begin point to the first tile
			auto iter1 = city.begin();
			// End points to the last item
			auto iter2 = city.end();

			Assert::IsTrue(tile1 == *iter1, L"First item correct");

			++iter1;
			Assert::IsTrue(tile2 == *iter1, L"Second item correct");

			++iter1;
			Assert::IsTrue(tile3 == *iter1, L"Third item correct");

			++iter1;
			Assert::IsFalse(iter1 != iter2);
		}

		TEST_METHOD(TestCCityVisitor)
		{
			// Construct a city object
			CCity city;

			// Add some tiles of each time
			auto tile1 = make_shared<CTileRoad>(&city);
			auto tile2 = make_shared<CTileBuilding>(&city);
			auto tile3 = make_shared<CTileLandscape>(&city);
			auto tile4 = make_shared<CTileCoalmine>(&city);

			Assert::IsTrue(city.EmptyCity(), L"City is Empty");

			city.Add(tile1);
			city.Add(tile2);
			city.Add(tile3);
			city.Add(tile4);

			Assert::IsFalse(city.EmptyCity(), L"City is not empty");

			CTestVisitor visitor;
			city.Accept(&visitor);
			Assert::AreEqual(1, visitor.mNumRoads,
				L"Visitor number of roads");

			Assert::AreEqual(1, visitor.mNumBuildings,
				L"Visitor number of buildings");

			Assert::AreEqual(1, visitor.mNumLandscapes,
				L"Visitor number of roads");

			Assert::AreEqual(1, visitor.mNumCoalmines,
				L"Visitor number of roads");

		}
        
	};
}