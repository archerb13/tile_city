#include "stdafx.h"
#include "CppUnitTest.h"
#include "PointBin.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Gdiplus;

namespace Testing
{
	TEST_CLASS(CPointBinTest)
	{
	public:

		TEST_METHOD_INITIALIZE(methodName)
		{
			extern wchar_t g_dir[];
			::SetCurrentDirectory(g_dir);
		}
		
		TEST_METHOD(TestCPointBinNext) 
		{
			CPointBin bin;

			Assert::IsTrue(bin.GetNext() == nullptr);

			std::shared_ptr<CPointBin> next = std::make_shared<CPointBin>();
			bin.SetNext(next);

			Assert::IsTrue(bin.GetNext() == next);
		}
		
		TEST_METHOD(TestCPointBin)
		{
			// Bin to test
			CPointBin bin;

			// Initially empty
			Assert::AreEqual(0, bin.GetCnt());

			// Add a point
			Point a(4, 7);

			Assert::IsTrue(bin.Add(a));
			Assert::AreEqual(1, bin.GetCnt());

			Point p = bin.Get(0);
			Assert::AreEqual(a.X, p.X);
			Assert::AreEqual(a.Y, p.Y);

			// Add four more
			Point b(9, 3);
			Point c(7, 11);
			Point d(-18, 271);
			Point e(99, 0);

			Assert::IsTrue(bin.Add(b));
			Assert::IsTrue(bin.Add(c));
			Assert::IsTrue(bin.Add(d));
			Assert::IsTrue(bin.Add(e));

			Assert::AreEqual(5, bin.GetCnt());
			Assert::AreEqual(a.X, bin.Get(0).X);
			Assert::AreEqual(a.Y, bin.Get(0).Y);
			Assert::AreEqual(b.X, bin.Get(1).X);
			Assert::AreEqual(b.Y, bin.Get(1).Y);
			Assert::AreEqual(c.X, bin.Get(2).X);
			Assert::AreEqual(c.Y, bin.Get(2).Y);
			Assert::AreEqual(d.X, bin.Get(3).X);
			Assert::AreEqual(d.Y, bin.Get(3).Y);
			Assert::AreEqual(e.X, bin.Get(4).X);
			Assert::AreEqual(e.Y, bin.Get(4).Y);

			Point f(12, 31);
			Assert::IsFalse(bin.Add(f));
		}

	};
}