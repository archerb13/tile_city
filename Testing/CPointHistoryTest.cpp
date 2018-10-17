#include "stdafx.h"
#include <vector>
#include <memory>
#include "CppUnitTest.h"
#include "PointHistory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace Gdiplus;

namespace Testing
{
	/** Testing stub class, creates a vector from
	* the list. This is done so we can test this
	* before we create the iterator. DO NOT USE
	* THIS CODE to solve the iterator task */
	class CPointHistoryStub : public CPointHistory
	{
	public:
		std::vector<Point> GetPoints()
		{
			std::vector<Point> list;
			auto ptr = mHead;
			while (ptr != nullptr)
			{
				auto left = ptr->GetLeft();
				if (left != nullptr)
				{
					list.push_back(left->Get());
				}

				list.push_back(ptr->Get());
				auto right = ptr->GetRight();
				if (right != nullptr)
				{
					list.push_back(right->Get());
				}

				ptr = ptr->GetNext();
			}

			return list;
		}

		std::shared_ptr<MiniTreeNode> GetHead()
		{
			return mHead;
		}
	};

	TEST_CLASS(CPointHistoryTest)
	{
	public:

		TEST_METHOD_INITIALIZE(methodName)
		{
			extern wchar_t g_dir[];
			::SetCurrentDirectory(g_dir);
		}

		TEST_METHOD(TestCPointHistoryMiniTreePointers)
		{
			CPointHistoryStub history;

			Point p(194, -241);
			history.Add(p);
			history.Add(p);
			history.Add(p);

			auto root = history.GetHead();
			auto left = root->GetLeft();
			auto right = root->GetRight();

			Assert::IsTrue(root == left->GetParent());
			Assert::IsTrue(root == right->GetParent());
		}

		TEST_METHOD(TestCPointHistoryAdd)
		{
			CPointHistoryStub history;

			// Vector for test data
			vector<Point> testData;

			// Ensure initially empty
			TestEqual(testData, history.GetPoints());

			// Create some simple test data
			for (int i = 0; i < 3; i++)
			{
				Point p(187 - i, 23 + i);
				testData.push_back(p);
				history.Add(p);
			}

			// And test
			TestEqual(testData, history.GetPoints());

			// Create some simple test data
			for (int i = 0; i < 117; i++)
			{
				Point p(i, 87 - i);
				testData.push_back(p);
				history.Add(p);
			}

			// And test
			TestEqual(testData, history.GetPoints());

			// Test over a range of sizes
			for (int size = 0; size < 100; size++)
			{
				CPointHistoryStub history1;
				vector<Point> testData1;

				for (int i = 0; i < size; i++)
				{
					Point p(21 + i, 8723 - i);
					testData1.push_back(p);
					history1.Add(p);
				}

				// And test
				TestEqual(testData1, history1.GetPoints());
			}
		}

		void TestEqual(vector<Point> a, vector<Point> b)
		{
			Assert::AreEqual(a.size(), b.size());
			for (size_t i = 0; i < a.size(); i++) 
			{
				Assert::AreEqual(a[i].X, b[i].X);
				Assert::AreEqual(a[i].Y, b[i].Y);
			}
		}

		//
		// The following two functions are used by the test.
		// They must be declared before the test.
		//

		/**
		 * Helper function to assert that two points are equal
		 * \param a First point
		 * \param c Second point
		 */
		void AssertEqual(Point a, Point b)
		{
			Assert::AreEqual(a.X, b.X);
			Assert::AreEqual(a.Y, b.Y);
		}

		/**
		 * Helper function to test that the iterator works
		 * for an arbitrary number of points.
		 * \param num Number of points to test (>= 0)
		 */
		void TestAtSize(int num)
		{
			CPointHistory history;
			vector<Point> points;

			// Create num random test values
			for (int i = 0; i < num; i++)
			{
				Point p(rand() % 100, rand() % 100);
				points.push_back(p);
				history.Add(p);
			}

			auto iter = history.begin();
			auto end = history.end();

			// Test that they are all correct
			for (int i = 0; i < num; i++)
			{
				Assert::IsTrue(iter != end);

				Point a = points[i];
				Point b = *iter;

				Assert::AreEqual(a.X, b.X);
				Assert::AreEqual(a.Y, b.Y);

				++iter;		// Next location
			}

			Assert::IsFalse(iter != end);
		}

		TEST_METHOD(TestCPointHistoryIterator)
		{
			CPointHistory history;

			//
			// Ensure the iterator indicates initially empty
			//
			auto iter = history.begin();
			auto end = history.end();
			Assert::IsFalse(iter != end);

			// 
			// Test with one item in the list
			//
			Point a(78, 22);
			history.Add(a);

			iter = history.begin();
			end = history.end();
			Assert::IsTrue(iter != end);
			AssertEqual(*iter, a);

			++iter;
			Assert::IsFalse(iter != end);

			// 
			// Test with two items in the list
			//
			Point b(66, -687);
			history.Add(b);

			iter = history.begin();
			end = history.end();
			Assert::IsTrue(iter != end);
			//AssertEqual(*iter, a);

			++iter;
			//Assert::IsTrue(iter != end);
			//AssertEqual(*iter, b);

			//++iter;
			//Assert::IsFalse(iter != end);

			//
			// Test with varying numbers of items in the list
			//
			//for (int num = 0; num < 21; num++)
			//{
			//	wstringstream str;
			//	str << L"Testing " << num << " items in list." << endl;
			//	Logger::WriteMessage(str.str().c_str());
			//	TestAtSize(num);
			//}
		};

	};
}