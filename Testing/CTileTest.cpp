#include "stdafx.h"
#include "CppUnitTest.h"

#include "City.h"
#include "Tile.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace Testing
{
    /**
    *  CTile mock derived object
    */
    class CTileMock : public CTile
    {
    public:
        /**  Constructor
         * \param city City this tile is a member of */
        CTileMock(CCity *city) : CTile(city)
        {
        }

        ///  Default constructor (disabled)
        CTileMock() = delete;

        ///  Copy constructor (disabled)
        CTileMock(const CTileMock &) = delete;

		/** Accept a visitor
		* \param visitor The visitor we accept */
		virtual void Accept(CTileVisitor *visitor) override { }

        ~CTileMock() {}
    };


	TEST_CLASS(CTileTest)
	{
	public:
        TEST_METHOD_INITIALIZE(methodName)
        {
            extern wchar_t g_dir[];
            ::SetCurrentDirectory(g_dir);
        }

		TEST_METHOD(TestConstructor)
		{
            CCity city;
            CTileMock tile(&city);
		}

        /**  Tests for the GetAdjacent function
        */
        TEST_METHOD(TestCItemAdjacenct)
        {
            CCity city;
            int grid = CCity::GridSpacing;

            // Add a center tile to test
            auto center = make_shared<CTileMock>(&city);
            center->SetLocation(grid * 10, grid * 17);
            city.Add(center);

            // Upper left
            auto ul = make_shared<CTileMock>(&city);
            ul->SetLocation(grid * 8, grid * 16);
            city.Add(ul);
            city.SortTiles();

            Assert::IsTrue(center->GetAdjacent(-1, -1) == ul, L"Upper left test");
            Assert::IsTrue(center->GetAdjacent(1, -1) == nullptr, L"Upper right null test");

            // Upper right
            auto ur = make_shared<CTileMock>(&city);
            ur->SetLocation(grid * 12, grid * 16);
            city.Add(ur);

            // Lower left
            auto ll = make_shared<CTileMock>(&city);
            ll->SetLocation(grid * 8, grid * 18);
            city.Add(ll);

            // Lower right
            auto lr = make_shared<CTileMock>(&city);
            lr->SetLocation(grid * 12, grid * 18);
            city.Add(lr);

            city.SortTiles();

            Assert::IsTrue(center->GetAdjacent(1, -1) == ur, L"Upper right test");
            Assert::IsTrue(center->GetAdjacent(-1, 1) == ll, L"Lower left test");
            Assert::IsTrue(center->GetAdjacent(1, 1) == lr, L"Lower right test");
        }


        TEST_METHOD(TestCItemSetLocation)
        {
            CCity city;
            CTileMock tile(&city);

            Assert::AreEqual(0, tile.GetX());
            Assert::AreEqual(0, tile.GetY());

            tile.SetLocation(17, 23);
            Assert::AreEqual(17, tile.GetX());
            Assert::AreEqual(23, tile.GetY());

        }

    };


}