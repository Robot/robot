////////////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------------- //
//                                                                            //
//                       (C) 2010-2016 Robot Developers                       //
//                       See LICENSE for licensing info                       //
//                                                                            //
// -------------------------------------------------------------------------- //
////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------//
// Prefaces                                                                   //
//----------------------------------------------------------------------------//

#include "Test.h"



//----------------------------------------------------------------------------//
// Functions                                                                  //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

static bool TestGlobal (void)
{
	VERIFY (sizeof ( int8 ) == 1);
	VERIFY (sizeof ( int16) == 2);
	VERIFY (sizeof ( int32) == 4);
	VERIFY (sizeof ( int64) == 8);

	VERIFY (sizeof (uint8 ) == 1);
	VERIFY (sizeof (uint16) == 2);
	VERIFY (sizeof (uint32) == 4);
	VERIFY (sizeof (uint64) == 8);

	VERIFY (sizeof (real32) == 4);
	VERIFY (sizeof (real64) == 8);

#ifdef ROBOT_ARCH_32

	VERIFY (sizeof ( intptr) == 4);
	VERIFY (sizeof (uintptr) == 4);

#endif
#ifdef ROBOT_ARCH_64

	VERIFY (sizeof ( intptr) == 8);
	VERIFY (sizeof (uintptr) == 8);

#endif

	uint32 endian = 0x01;
	// Verify endianness just in case
	VERIFY (*(uint8*) &endian == 0x01);

	return true;
}

////////////////////////////////////////////////////////////////////////////////

enum TestEnum1
{
	Enum1Value1,
	Enum1Value2,
	Enum1Value3,
	Enum1Value4,
};

enum TestEnum2
{
	Enum2Value1,
	Enum2Value2,
};

// CAUTION: Template specializations
// cannot be in different namespaces
// when compiling with GCC.

ROBOT_NS_BEGIN

	ROBOT_ENUM (TestEnum1)
	{
		ROBOT_ENUM_MAP (Enum1Value1, "ENUM1Value1");
		ROBOT_ENUM_MAP (Enum1Value2, "ENUM1Value2");
		ROBOT_ENUM_MAP (Enum1Value3, "ENUM1Value3");
		ROBOT_ENUM_MAP (Enum1Value4, "ENUM1Value4");
	}

	ROBOT_ENUM (TestEnum2)
	{
		ROBOT_ENUM_MAP (Enum2Value1);
		ROBOT_ENUM_MAP (Enum2Value2);
	}

ROBOT_NS_END

bool TestEnum (void)
{
	VERIFY (Enum<TestEnum1>::Size() == 4);
	VERIFY (Enum<TestEnum2>::Size() == 2);

	VERIFY (Enum<TestEnum1>::Parse ("ENUM1Value1") == Enum1Value1);
	VERIFY (Enum<TestEnum1>::Parse ("ENUM1Value2") != Enum1Value3);
	VERIFY (Enum<TestEnum1>::Parse ("ENUM1Value3") != Enum1Value2);
	VERIFY (Enum<TestEnum1>::Parse ("ENUM1Value4") == Enum1Value4);
	VERIFY (Enum<TestEnum2>::Parse ("Enum2Value1") == Enum2Value1);
	VERIFY (Enum<TestEnum2>::Parse ("Enum2Value2") == Enum2Value2);

	VERIFY (Enum<TestEnum1>::Parse (Enum1Value1) == "ENUM1Value1");
	VERIFY (Enum<TestEnum1>::Parse (Enum1Value2) != "ENUM1Value3");
	VERIFY (Enum<TestEnum1>::Parse (Enum1Value3) != "ENUM1Value2");
	VERIFY (Enum<TestEnum1>::Parse (Enum1Value4) == "ENUM1Value4");
	VERIFY (Enum<TestEnum2>::Parse (Enum2Value1) == "Enum2Value1");
	VERIFY (Enum<TestEnum2>::Parse (Enum2Value2) == "Enum2Value2");

	VERIFY (Enum<TestEnum1>::Parse ("BadValue") == (TestEnum1) -1);
	VERIFY (Enum<TestEnum2>::Parse ("BadValue") == (TestEnum2) -1);
	VERIFY (Enum<TestEnum1>::Parse ("BadValue", Enum1Value2) == Enum1Value2);
	VERIFY (Enum<TestEnum2>::Parse ("BadValue", Enum2Value2) == Enum2Value2);

	VERIFY (Enum<TestEnum1>::Parse ((TestEnum1) 9).empty());
	VERIFY (Enum<TestEnum2>::Parse ((TestEnum2) 9).empty());
	VERIFY (Enum<TestEnum1>::Parse ((TestEnum1) 9, "empty1") == "empty1");
	VERIFY (Enum<TestEnum2>::Parse ((TestEnum2) 9, "empty2") == "empty2");

	VERIFY (Enum<Button>::Parse ("LEFT"  ) == ButtonLeft  );
	VERIFY (Enum<Button>::Parse ("MID"   ) == ButtonMid   );
	VERIFY (Enum<Button>::Parse ("MIDDLE") == ButtonMiddle);
	VERIFY (Enum<Button>::Parse ("RIGHT" ) == ButtonRight );

	VERIFY (Enum<Button>::Parse (ButtonX1) == "X1");
	VERIFY (Enum<Button>::Parse (ButtonX2) == "X2");

	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestHash (void)
{
	Hash h1 ((uint8*) "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 26);
	Hash h2 ((uint8*) "PZNXECYBMOVHUWSGIKRFLAQDTJ", 26);
	Hash h3 ((uint8*) "PZNXECYBMOVHUWSGIKRFLAQDTJ", 26);
	Hash h4 ((uint8*) "MKQYRNCZFBGXLHEIWAOJPSVTUD", 26);
	Hash h5 ((uint8*) "KMQYRNCZFBGXLHEIWAOJPSVTUD", 26);
	Hash h6 ((uint8*) "LSIWNEURFMZAXYJKQBVGHCPTOD", 26);
	Hash h7 ((uint8*) "LSIWNEURFMZAXYJKQBVGHCPTDO", 26);
	Hash h8 ((uint8*) "QHWBMKZRELNYDJAFUPTXICGOSV", 26);
	Hash h9 ((uint8*) "QHWBMKZRELYNDJAFUPTXICGOSV", 26);

	VERIFY (h1 == 0xABF77822);
	VERIFY (h2 == 0xB402F824);
	VERIFY (h3 == 0xB402F824);
	VERIFY (h4 == 0x211B870C);
	VERIFY (h5 == 0x751E361B);
	VERIFY (h6 == 0x86A920A7);
	VERIFY (h7 == 0xF28F20E4);
	VERIFY (h8 == 0x94942DD5);
	VERIFY (h9 == 0x50297B6B);

	VERIFY ( (h1 == h1));
	VERIFY (!(h1 == h2));
	VERIFY (!(h1 != h1));
	VERIFY ( (h1 != h2));

	VERIFY (h2 == h3);
	VERIFY (h3 == h2);
	VERIFY (h4 != h5);
	VERIFY (h5 != h4);
	VERIFY (h6 != h7);
	VERIFY (h7 != h6);
	VERIFY (h8 != h9);
	VERIFY (h9 != h8);

	h1 = Hash ((uint8*) ".M:.Ak=-n.~.%^njfb|*-!9+dO3o",     28);
	h2 = Hash ((uint8*) "7J4P67_09;!5=-;55.2_x!533aH7_X",   30);
	h3 = Hash ((uint8*) "ai!.tc_Z-*%x6%3TCb0Yjxh^84D8A",    29);
	h4 = Hash ((uint8*) ";e.o;V7F.qut;thwdO^:|^=!3;.!+~H",  31);
	h5 = Hash ((uint8*) "q3_!GG:%8z54=%c!!o7~!.^Wwj%=_R",   30);
	h6 = Hash ((uint8*) "b;B7+t=.^^.L*%i9;|*%S++56q.|%~^u", 32);
	h7 = Hash ((uint8*) "Us*Fki-XgYTp.OVWL|t~GxrQ5G;7xC",   30);
	h8 = Hash ((uint8*) "%e*5aY681;Z0TX_-n~:4~^0W-6_D",     26);
	h9 = Hash ((uint8*) "r73%Q-9dz!YB:j2!9*64B+M%j+O4e",    24);

	VERIFY (h1.Result == 0x73FECF56);
	VERIFY (h2.Result == 0xA99190FB);
	VERIFY (h3.Result == 0xBC85CDC1);
	VERIFY (h4.Result == 0xC30A3672);
	VERIFY (h5.Result == 0xAF8EEBEA);
	VERIFY (h6.Result == 0x92FDEC2E);
	VERIFY (h7.Result == 0x129F85F3);
	VERIFY (h8.Result == 0x3611A160);
	VERIFY (h9.Result == 0x0726C24A);

	h1 = Hash(); VERIFY (h1.Result == 0);

	h1.Append ((uint8*) "z=6x_p2-8F--=IYi%;jZ3*+i", 24); VERIFY (h1.Result == 0x7192FCF6);
	h1.Append ((uint8*) "32Yl1+*%%I_S:5;4.=8805|v", 22); VERIFY (h1.Result == 0xD54939BF);
	h1.Append ((uint8*) "_3hdn-!nAA!+B~+:l;7Oz..K", 22); VERIFY (h1.Result == 0x913291BB);
	h1.Append ((uint8*) "0+g|1|T324G;=~=.~^.i;aZn", 23); VERIFY (h1.Result == 0xC68E47A2);

	h1.Result = 0; h2.Result = 0; h3.Result = 0;
	h4.Result = 0; h5.Result = 0; h6.Result = 0;
	h7.Result = 0; h8.Result = 0; h9.Result = 0;

	const char* file = "ROBOT-HASH";
	ofstream data (file); if (data)
	{
		data << "";
		data.close();
		VERIFY ( h1.Append (file   ));
		VERIFY (!h1.Append (nullptr));
		VERIFY (h1 == 0);

		data.open (file);
		data << ":.=;q*xt95=6.ZlkisU=vrdr";
		data.close();
		VERIFY (h1.Append (file));
		VERIFY (h2.Append (file));

		data.open (file);
		data << "!~:6|!k+*5^BLs6w-;!73T";
		data.close();
		VERIFY (h1.Append (file));
		VERIFY (h3.Append (file));

		data.open (file);
		data << "=7g1.=1V=s|8m5tD5._;T^+87v";
		data.close();
		VERIFY (h1.Append (file));
		VERIFY (h4.Append (file));

		VERIFY (h1 == 0x32C0860E);
		VERIFY (h2 == 0x086DA034);
		VERIFY (h3 == 0x353C2BE4);
		VERIFY (h4 == 0x0474D424);
	}

	else
	{
		cout << "Warning: Hash test file cannot be written\n"
			 << "         Skipping this part of the test!!\n\n";
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestColor (void)
{
	Color c1;
	Color c2 (0xDC195A);
	Color c3 (0x5A6E2882);
	Color c4 (100, 215, 180);
	Color c5 (110, 40, 130, 90);

	VERIFY (c1.A ==   0 && c1.R ==    0 && c1.G ==   0 && c1.B ==   0);
	VERIFY (c2.A ==   0 && c2.R ==  220 && c2.G ==  25 && c2.B ==  90);
	VERIFY (c3.A ==  90 && c3.R ==  110 && c3.G ==  40 && c3.B == 130);
	VERIFY (c4.A == 255 && c4.R ==  100 && c4.G == 215 && c4.B == 180);
	VERIFY (c5.A ==  90 && c5.R ==  110 && c5.G ==  40 && c5.B == 130);

	VERIFY (c1.GetARGB() == 0         );
	VERIFY (c2.GetARGB() == 0xDC195A  );
	VERIFY (c3.GetARGB() == 0x5A6E2882);
	VERIFY (c4.GetARGB() == 0xFF64D7B4);
	VERIFY (c5.GetARGB() == 0x5A6E2882);

	c2.SetARGB (0xFF64D7B4);

	VERIFY ( (c1 == c1));
	VERIFY (!(c1 == c2));
	VERIFY (!(c1 != c1));
	VERIFY ( (c1 != c2));

	VERIFY (c2 != c3);
	VERIFY (c3 != c2);
	VERIFY (c3 != c4);
	VERIFY (c4 != c3);
	VERIFY (c4 != c5);
	VERIFY (c5 != c4);
	VERIFY (c2 == c4);
	VERIFY (c4 == c2);
	VERIFY (c3 == c5);
	VERIFY (c5 == c3);

	return true;
}

////////////////////////////////////////////////////////////////////////////////

static Image TestGetImage (void)
{
	Image i (2, 2);
	i.GetData()[0] = 0x00808080;
	i.GetData()[1] = 0x08808080;
	i.GetData()[2] = 0x80808080;
	i.GetData()[3] = 0xFF808080;
	return i;
}

static bool TestImage1 (void)
{
	Image i1;
	Image i2 (0);
	Image i3 (6);
	Image i4 (2, 4);
	Image i5 (0, 4);
	Image i6 (2, 0);

	uint32* d1 = nullptr;
	uint32* d2 = nullptr;

	VERIFY (!i1.IsValid());
	VERIFY (!i2.IsValid());
	VERIFY ( i3.IsValid());
	VERIFY ( i4.IsValid());
	VERIFY (!i5.IsValid());
	VERIFY (!i6.IsValid());

	VERIFY (i1.GetWidth() == 0); VERIFY (i1.GetHeight() == 0);
	VERIFY (i2.GetWidth() == 0); VERIFY (i2.GetHeight() == 0);
	VERIFY (i3.GetWidth() == 6); VERIFY (i3.GetHeight() == 6);
	VERIFY (i4.GetWidth() == 2); VERIFY (i4.GetHeight() == 4);
	VERIFY (i5.GetWidth() == 0); VERIFY (i5.GetHeight() == 0);
	VERIFY (i6.GetWidth() == 0); VERIFY (i6.GetHeight() == 0);

	VERIFY (i1.GetLength() ==  0); VERIFY (!i1.GetData());
	VERIFY (i2.GetLength() ==  0); VERIFY (!i2.GetData());
	VERIFY (i3.GetLength() == 36); VERIFY ( i3.GetData());
	VERIFY (i4.GetLength() ==  8); VERIFY ( i4.GetData());
	VERIFY (i5.GetLength() ==  0); VERIFY (!i5.GetData());
	VERIFY (i6.GetLength() ==  0); VERIFY (!i6.GetData());

	VERIFY ( i3.Create (6, 8));
	VERIFY (!i3.Create (6, 0));
	VERIFY (!i3.Create (0, 8));
	VERIFY (!i3.Create (0, 0));
	VERIFY (!i3.Create (0   ));
	d1 = i3.GetData();

	VERIFY (i3.IsValid() == true);
	VERIFY (i3.GetWidth () ==  6);
	VERIFY (i3.GetHeight() ==  8);
	VERIFY (i3.GetLength() == 48);
	VERIFY (i3.GetLimit () == 48);

	VERIFY (i3.Create (4));
	VERIFY (i4.Create (4));
	d2 = i3.GetData();
	VERIFY (d1 == d2);

	VERIFY (i3.IsValid() == true);
	VERIFY (i3.GetWidth () ==  4);
	VERIFY (i3.GetHeight() ==  4);
	VERIFY (i3.GetLength() == 16);
	VERIFY (i3.GetLimit () == 48);

	VERIFY (i4.IsValid() == true);
	VERIFY (i4.GetWidth () ==  4);
	VERIFY (i4.GetHeight() ==  4);
	VERIFY (i4.GetLength() == 16);
	VERIFY (i4.GetLimit () == 16);

	i3.GetData()[ 0] = 0x00808080;
	i3.GetData()[ 1] = 0x08808080;
	i3.GetData()[ 2] = 0x80808080;
	i3.GetData()[ 3] = 0xFF808080;

	i3.GetData()[ 4] = 0x00FF0000;
	i3.GetData()[ 5] = 0x08FF0000;
	i3.GetData()[ 6] = 0x80FF0000;
	i3.GetData()[ 7] = 0xFFFF0000;

	i3.GetData()[ 8] = 0x0000FF00;
	i3.GetData()[ 9] = 0x0800FF00;
	i3.GetData()[10] = 0x8000FF00;
	i3.GetData()[11] = 0xFF00FF00;

	i3.GetData()[12] = 0x000000FF;
	i3.GetData()[13] = 0x080000FF;
	i3.GetData()[14] = 0x800000FF;
	i3.GetData()[15] = 0xFF0000FF;

	i4.SetPixel (0, 0, 0x00808080);
	i4.SetPixel (1, 0, 0x08808080);
	i4.SetPixel (2, 0, 0x80808080);
	i4.SetPixel (3, 0, 0xFF808080);

	i4.SetPixel (0, 1, 0x00FF0000);
	i4.SetPixel (1, 1, 0x08FF0000);
	i4.SetPixel (2, 1, 0x80FF0000);
	i4.SetPixel (3, 1, 0xFFFF0000);

	i4.SetPixel (0, 2, 0x0000FF00);
	i4.SetPixel (1, 2, 0x0800FF00);
	i4.SetPixel (2, 2, 0x8000FF00);
	i4.SetPixel (3, 2, 0xFF00FF00);

	i4.SetPixel (0, 3, 0x000000FF);
	i4.SetPixel (1, 3, 0x080000FF);
	i4.SetPixel (2, 3, 0x800000FF);
	i4.SetPixel (3, 3, 0xFF0000FF);

	VERIFY (i3.GetPixel (0) == 0x00808080);
	VERIFY (i3.GetPixel (1) == 0x08FF0000);
	VERIFY (i3.GetPixel (2) == 0x8000FF00);
	VERIFY (i3.GetPixel (3) == 0xFF0000FF);

	VERIFY (i3.GetPixel (3, 0) == 0xFF808080);
	VERIFY (i3.GetPixel (2, 1) == 0x80FF0000);
	VERIFY (i3.GetPixel (1, 2) == 0x0800FF00);
	VERIFY (i3.GetPixel (0, 3) == 0x000000FF);

	VERIFY (i4.GetPixel (0) == 0x00808080);
	VERIFY (i4.GetPixel (1) == 0x08FF0000);
	VERIFY (i4.GetPixel (2) == 0x8000FF00);
	VERIFY (i4.GetPixel (3) == 0xFF0000FF);

	VERIFY (i4.GetPixel (3, 0) == 0xFF808080);
	VERIFY (i4.GetPixel (2, 1) == 0x80FF0000);
	VERIFY (i4.GetPixel (1, 2) == 0x0800FF00);
	VERIFY (i4.GetPixel (0, 3) == 0x000000FF);

	VERIFY ( (i3 == i4));
	VERIFY (!(i3 != i4));

	i2 = Image (i3);
	i4 = i3;
	i5 = TestGetImage();
	i6 = std::move (i3);

	VERIFY (i4.GetPixel (0) == 0x00808080);
	VERIFY (i4.GetPixel (1) == 0x08FF0000);
	VERIFY (i4.GetPixel (2) == 0x8000FF00);
	VERIFY (i4.GetPixel (3) == 0xFF0000FF);

	VERIFY (i4.GetPixel (3, 0) == 0xFF808080);
	VERIFY (i4.GetPixel (2, 1) == 0x80FF0000);
	VERIFY (i4.GetPixel (1, 2) == 0x0800FF00);
	VERIFY (i4.GetPixel (0, 3) == 0x000000FF);

	VERIFY ( i2.IsValid());
	VERIFY (!i3.IsValid());
	VERIFY ( i4.IsValid());
	VERIFY ( i5.IsValid());
	VERIFY ( i6.IsValid());

	VERIFY (i2.GetWidth() == 4); VERIFY (i2.GetHeight() == 4);
	VERIFY (i3.GetWidth() == 0); VERIFY (i3.GetHeight() == 0);
	VERIFY (i4.GetWidth() == 4); VERIFY (i4.GetHeight() == 4);
	VERIFY (i5.GetWidth() == 2); VERIFY (i5.GetHeight() == 2);
	VERIFY (i6.GetWidth() == 4); VERIFY (i6.GetHeight() == 4);

	VERIFY (i2.GetLength() == 16); VERIFY ( i2.GetData());
	VERIFY (i3.GetLength() ==  0); VERIFY (!i3.GetData());
	VERIFY (i4.GetLength() == 16); VERIFY ( i4.GetData());
	VERIFY (i5.GetLength() ==  4); VERIFY ( i5.GetData());
	VERIFY (i6.GetLength() == 16); VERIFY ( i6.GetData());

	VERIFY (i6.GetData() == d1);
	VERIFY (i6.GetData() == d2);
	VERIFY (i2.GetData() != i4.GetData());
	VERIFY (i2.GetData() != i5.GetData());
	VERIFY (i2.GetData() != i6.GetData());
	VERIFY (i4.GetData() != i5.GetData());
	VERIFY (i4.GetData() != i6.GetData());
	VERIFY (i5.GetData() != i6.GetData());

	VERIFY ( (i1 == i1));
	VERIFY (!(i1 == i2));
	VERIFY (!(i1 != i1));
	VERIFY ( (i1 != i2));

	VERIFY (i2 != i3);
	VERIFY (i3 != i2);
	VERIFY (i2 == i4);
	VERIFY (i4 == i2);
	VERIFY (i4 != i5);
	VERIFY (i5 != i4);
	VERIFY (i4 == i6);
	VERIFY (i6 == i4);

	d1 = i4.GetData();
	VERIFY (i4.Create (2, 2));
	d2 = i4.GetData();
	VERIFY (d1 == d2);

	VERIFY (i4 == i5);
	VERIFY (i5 == i4);
	i5.Destroy();
	VERIFY (i4 != i5);
	VERIFY (i5 != i4);

	Image i7;
	Image i8;
	Image i9 (i1);
	i7 = i1;
	i8 = std::move (i1);

	i2 = i7;
	i4 = Image (i8);

	i2 = i2;
	i4 = std::move (i4);

	VERIFY (!i2.IsValid());
	VERIFY (!i4.IsValid());
	VERIFY (!i7.IsValid());
	VERIFY (!i8.IsValid());
	VERIFY (!i9.IsValid());

	VERIFY (i2.GetLength() == 0);
	VERIFY (i4.GetLength() == 0);
	VERIFY (i7.GetLength() == 0);
	VERIFY (i8.GetLength() == 0);
	VERIFY (i9.GetLength() == 0);

	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestImage2 (void)
{
	Image i1, i2 (2, 3);
	uint32* data = i2.GetData();
	data[0] = 0x00FF0000;
	data[1] = 0x0000FF00;
	data[2] = 0x000000FF;
	data[3] = 0xFFFF0000;
	data[4] = 0xFF00FF00;
	data[5] = 0xFF0000FF;

	VERIFY (!i1.Fill (0x00, 0x00, 0x00, 0x00));
	VERIFY (!i2.Swap (nullptr));
	VERIFY (!i1.Swap (nullptr));
	VERIFY (!i1.Swap ( "argb"));
	VERIFY (!i1.Flip (false, false));

	VERIFY (!i2.Swap (""     ));
	VERIFY (!i2.Swap ("gb"   ));
	VERIFY (!i2.Swap ("r"    ));
	VERIFY (!i2.Swap ("agb"  ));
	VERIFY (!i2.Swap ("x"    ));
	VERIFY (!i2.Swap ("airgb"));
	VERIFY (!i2.Swap ("aargb"));
	VERIFY (!i2.Swap ("argbr"));

	i1 = i2; data = i1.GetData();
	VERIFY (i1.Swap ("argb"));
	VERIFY (data[0] == 0x00FF0000);
	VERIFY (data[1] == 0x0000FF00);
	VERIFY (data[2] == 0x000000FF);
	VERIFY (data[3] == 0xFFFF0000);
	VERIFY (data[4] == 0xFF00FF00);
	VERIFY (data[5] == 0xFF0000FF);

	i1 = i2; data = i1.GetData();
	VERIFY (i1.Swap ("rgba"));
	VERIFY (data[0] == 0xFF000000);
	VERIFY (data[1] == 0x00FF0000);
	VERIFY (data[2] == 0x0000FF00);
	VERIFY (data[3] == 0xFF0000FF);
	VERIFY (data[4] == 0x00FF00FF);
	VERIFY (data[5] == 0x0000FFFF);

	i1 = i2; data = i1.GetData();
	VERIFY (i1.Swap ("abgr"));
	VERIFY (data[0] == 0x000000FF);
	VERIFY (data[1] == 0x0000FF00);
	VERIFY (data[2] == 0x00FF0000);
	VERIFY (data[3] == 0xFF0000FF);
	VERIFY (data[4] == 0xFF00FF00);
	VERIFY (data[5] == 0xFFFF0000);

	i1 = i2; data = i1.GetData();
	VERIFY (i1.Swap ("bgra"));
	VERIFY (data[0] == 0x0000FF00);
	VERIFY (data[1] == 0x00FF0000);
	VERIFY (data[2] == 0xFF000000);
	VERIFY (data[3] == 0x0000FFFF);
	VERIFY (data[4] == 0x00FF00FF);
	VERIFY (data[5] == 0xFF0000FF);

	VERIFY (i1.Create (2, 2));
	VERIFY (data == i1.GetData());
	VERIFY (i1.GetLimit () == 6);

	VERIFY (i1.Fill (0x00, 0x00, 0x00, 0x00));
	VERIFY (data[0] == 0x00000000);
	VERIFY (data[1] == 0x00000000);
	VERIFY (data[2] == 0x00000000);
	VERIFY (data[3] == 0x00000000);

	VERIFY (i1.Fill (0xFF, 0x00, 0xFF, 0x00));
	VERIFY (data[0] == 0x00FF00FF);
	VERIFY (data[1] == 0x00FF00FF);
	VERIFY (data[2] == 0x00FF00FF);
	VERIFY (data[3] == 0x00FF00FF);

	VERIFY (i1.Fill (0x00, 0xFF, 0x00));
	VERIFY (data[0] == 0xFF00FF00);
	VERIFY (data[1] == 0xFF00FF00);
	VERIFY (data[2] == 0xFF00FF00);
	VERIFY (data[3] == 0xFF00FF00);

	VERIFY (i1.Fill (0x80, 0x08, 0x80, 0x08));
	VERIFY (data[0] == 0x08800880);
	VERIFY (data[1] == 0x08800880);
	VERIFY (data[2] == 0x08800880);
	VERIFY (data[3] == 0x08800880);

	i1 = i2; data = i1.GetData();
	VERIFY (i1.Flip (false, false));
	VERIFY (data[0] == 0x00FF0000); VERIFY (data[1] == 0x0000FF00);
	VERIFY (data[2] == 0x000000FF); VERIFY (data[3] == 0xFFFF0000);
	VERIFY (data[4] == 0xFF00FF00); VERIFY (data[5] == 0xFF0000FF);

	i1 = i2; data = i1.GetData();
	VERIFY (i1.Flip (true, false));
	VERIFY (data[0] == 0x0000FF00); VERIFY (data[1] == 0x00FF0000);
	VERIFY (data[2] == 0xFFFF0000); VERIFY (data[3] == 0x000000FF);
	VERIFY (data[4] == 0xFF0000FF); VERIFY (data[5] == 0xFF00FF00);

	i1 = i2; data = i1.GetData();
	VERIFY (i1.Flip (false, true));
	VERIFY (data[0] == 0xFF00FF00); VERIFY (data[1] == 0xFF0000FF);
	VERIFY (data[2] == 0x000000FF); VERIFY (data[3] == 0xFFFF0000);
	VERIFY (data[4] == 0x00FF0000); VERIFY (data[5] == 0x0000FF00);

	i1 = i2; data = i1.GetData();
	VERIFY (i1.Flip (true, true));
	VERIFY (data[0] == 0xFF0000FF); VERIFY (data[1] == 0xFF00FF00);
	VERIFY (data[2] == 0xFFFF0000); VERIFY (data[3] == 0x000000FF);
	VERIFY (data[4] == 0x0000FF00); VERIFY (data[5] == 0x00FF0000);

	VERIFY (i1.Create (1, 5));
	data = i1.GetData();
	data[0] = 0xFF000000;
	data[1] = 0x00FF0000;
	data[2] = 0xFFFFFFFF;
	data[3] = 0x0000FF00;
	data[4] = 0x000000FF;
	VERIFY (i1.Flip (true, true));
	VERIFY (data[0] == 0x000000FF);
	VERIFY (data[1] == 0x0000FF00);
	VERIFY (data[2] == 0xFFFFFFFF);
	VERIFY (data[3] == 0x00FF0000);
	VERIFY (data[4] == 0xFF000000);

	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestRange (void)
{
	Range r1;
	Range r2 ( 5    );
	Range r3 ( 2,  8);
	Range r4 ( 7,  3);
	Range r5 (-4,  5);
	Range r6 ( 3, -6);
	Range r7 (-8, -4);
	Range r8 (-3, -7);
	Range r9 (-6, -6);

	VERIFY (r1.Min ==  0 && r1.Max ==  0);
	VERIFY (r2.Min ==  5 && r2.Max ==  5);
	VERIFY (r3.Min ==  2 && r3.Max ==  8);
	VERIFY (r4.Min ==  7 && r4.Max ==  3);
	VERIFY (r5.Min == -4 && r5.Max ==  5);
	VERIFY (r6.Min ==  3 && r6.Max == -6);
	VERIFY (r7.Min == -8 && r7.Max == -4);
	VERIFY (r8.Min == -3 && r8.Max == -7);
	VERIFY (r9.Min == -6 && r9.Max == -6);

	VERIFY (r1.GetRange() ==  0);
	VERIFY (r2.GetRange() ==  0);
	VERIFY (r3.GetRange() ==  6);
	VERIFY (r4.GetRange() == -4);
	VERIFY (r5.GetRange() ==  9);
	VERIFY (r6.GetRange() == -9);
	VERIFY (r7.GetRange() ==  4);
	VERIFY (r8.GetRange() == -4);
	VERIFY (r9.GetRange() ==  0);

	VERIFY ( r1.Contains (0));
	VERIFY (!r2.Contains (4));
	VERIFY ( r2.Contains (5));
	VERIFY (!r2.Contains (6));
	VERIFY (!r3.Contains (1));
	VERIFY ( r3.Contains (2));
	VERIFY ( r3.Contains (3));
	VERIFY ( r3.Contains (7));
	VERIFY ( r3.Contains (8));
	VERIFY (!r3.Contains (9));
	VERIFY (!r4.Contains (6));
	VERIFY (!r4.Contains (7));
	VERIFY (!r4.Contains (8));
	VERIFY (!r4.Contains (2));
	VERIFY (!r4.Contains (3));
	VERIFY (!r4.Contains (4));

	VERIFY (!r1.Contains (0, false));
	VERIFY (!r2.Contains (4, false));
	VERIFY (!r2.Contains (5, false));
	VERIFY (!r2.Contains (6, false));
	VERIFY (!r3.Contains (1, false));
	VERIFY (!r3.Contains (2, false));
	VERIFY ( r3.Contains (3, false));
	VERIFY ( r3.Contains (7, false));
	VERIFY (!r3.Contains (8, false));
	VERIFY (!r3.Contains (9, false));
	VERIFY (!r4.Contains (6, false));
	VERIFY (!r4.Contains (7, false));
	VERIFY (!r4.Contains (8, false));
	VERIFY (!r4.Contains (2, false));
	VERIFY (!r4.Contains (3, false));
	VERIFY (!r4.Contains (4, false));

	int32 min1 = 99; int32 max1 = -99;
	int32 min2 = 99; int32 max2 = -99;
	int32 min3 = 99; int32 max3 = -99;
	int32 min4 = 99; int32 max4 = -99;
	int32 min5 = 99; int32 max5 = -99;
	int32 min6 = 99; int32 max6 = -99;
	int32 min7 = 99; int32 max7 = -99;
	int32 min8 = 99; int32 max8 = -99;
	int32 min9 = 99; int32 max9 = -99;

	for (int32 i = 0; i < 99999; ++i)
	{
		int32 rand1 = r1.GetRandom();
		int32 rand2 = r2.GetRandom();
		int32 rand3 = r3.GetRandom();
		int32 rand4 = r4.GetRandom();
		int32 rand5 = r5.GetRandom();
		int32 rand6 = r6.GetRandom();
		int32 rand7 = r7.GetRandom();
		int32 rand8 = r8.GetRandom();
		int32 rand9 = r9.GetRandom();

		if (min1 > rand1) min1 = rand1; if (max1 < rand1) max1 = rand1;
		if (min2 > rand2) min2 = rand2; if (max2 < rand2) max2 = rand2;
		if (min3 > rand3) min3 = rand3; if (max3 < rand3) max3 = rand3;
		if (min4 > rand4) min4 = rand4; if (max4 < rand4) max4 = rand4;
		if (min5 > rand5) min5 = rand5; if (max5 < rand5) max5 = rand5;
		if (min6 > rand6) min6 = rand6; if (max6 < rand6) max6 = rand6;
		if (min7 > rand7) min7 = rand7; if (max7 < rand7) max7 = rand7;
		if (min8 > rand8) min8 = rand8; if (max8 < rand8) max8 = rand8;
		if (min9 > rand9) min9 = rand9; if (max9 < rand9) max9 = rand9;
	}

	VERIFY (min1 ==  0 && max1 ==  0);
	VERIFY (min2 ==  5 && max2 ==  5);
	VERIFY (min3 ==  2 && max3 ==  7);
	VERIFY (min4 ==  7 && max4 ==  7);
	VERIFY (min5 == -4 && max5 ==  4);
	VERIFY (min6 ==  3 && max6 ==  3);
	VERIFY (min7 == -8 && max7 == -5);
	VERIFY (min8 == -3 && max8 == -3);
	VERIFY (min9 == -6 && max9 == -6);

	r6 = r2;
	r7 = r3;
	r2.SetRange ( 6   );
	r3.SetRange ( 3, 9);
	r4.SetRange ( 6, 2);
	r5.SetRange (-3, 8);
	r8 = r4;
	r9.SetRange (-3, 8);

	VERIFY (r1.Min ==  0 && r1.Max == 0);
	VERIFY (r2.Min ==  6 && r2.Max == 6);
	VERIFY (r3.Min ==  3 && r3.Max == 9);
	VERIFY (r4.Min ==  6 && r4.Max == 2);
	VERIFY (r5.Min == -3 && r5.Max == 8);
	VERIFY (r6.Min ==  5 && r6.Max == 5);
	VERIFY (r7.Min ==  2 && r7.Max == 8);
	VERIFY (r8.Min ==  6 && r8.Max == 2);
	VERIFY (r9.Min == -3 && r9.Max == 8);

	VERIFY ( (r1 == r1));
	VERIFY (!(r1 == r2));
	VERIFY (!(r1 != r1));
	VERIFY ( (r1 != r2));

	VERIFY (r6 != r2);
	VERIFY (r2 != r6);
	VERIFY (r8 == r4);
	VERIFY (r4 == r8);
	VERIFY (r3 != r5);
	VERIFY (r5 != r3);
	VERIFY (r9 == r5);
	VERIFY (r5 == r9);

	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestBounds (void)
{
	Point p1;
	Point p2 ( 5    );
	Point p3 ( 0,  8);
	Point p4 (-4,  0);
	Point p5 ( 3, -6);
	Point p6 (-3, -7);

	Size s1;
	Size s2 ( 5    );
	Size s3 ( 0,  8);
	Size s4 (-4,  0);
	Size s5 ( 3, -6);
	Size s6 (-3, -7);

	Point p7 = s2.ToPoint();
	Point p8 = s6.ToPoint();
	Point p9 = -6;

	Size s7 = p2.ToSize();
	Size s8 = p6.ToSize();
	Size s9 = -6;

	VERIFY (p1.X ==  0 && p1.Y ==  0);
	VERIFY (p2.X ==  5 && p2.Y ==  5);
	VERIFY (p3.X ==  0 && p3.Y ==  8);
	VERIFY (p4.X == -4 && p4.Y ==  0);
	VERIFY (p5.X ==  3 && p5.Y == -6);
	VERIFY (p6.X == -3 && p6.Y == -7);
	VERIFY (p7.X ==  5 && p7.Y ==  5);
	VERIFY (p8.X == -3 && p8.Y == -7);
	VERIFY (p9.X == -6 && p9.Y == -6);

	VERIFY (s1.W ==  0 && s1.H ==  0);
	VERIFY (s2.W ==  5 && s2.H ==  5);
	VERIFY (s3.W ==  0 && s3.H ==  8);
	VERIFY (s4.W == -4 && s4.H ==  0);
	VERIFY (s5.W ==  3 && s5.H == -6);
	VERIFY (s6.W == -3 && s6.H == -7);
	VERIFY (s7.W ==  5 && s7.H ==  5);
	VERIFY (s8.W == -3 && s8.H == -7);
	VERIFY (s9.W == -6 && s9.H == -6);

	VERIFY ( p1.IsZero());
	VERIFY (!p2.IsZero());
	VERIFY (!p3.IsZero());
	VERIFY (!p4.IsZero());
	VERIFY (!p5.IsZero());
	VERIFY (!p6.IsZero());
	VERIFY (!p7.IsZero());
	VERIFY (!p8.IsZero());
	VERIFY (!p9.IsZero());

	VERIFY ( s1.IsZero()); VERIFY ( s1.IsEmpty());
	VERIFY (!s2.IsZero()); VERIFY (!s2.IsEmpty());
	VERIFY (!s3.IsZero()); VERIFY ( s3.IsEmpty());
	VERIFY (!s4.IsZero()); VERIFY ( s4.IsEmpty());
	VERIFY (!s5.IsZero()); VERIFY (!s5.IsEmpty());
	VERIFY (!s6.IsZero()); VERIFY (!s6.IsEmpty());
	VERIFY (!s7.IsZero()); VERIFY (!s7.IsEmpty());
	VERIFY (!s8.IsZero()); VERIFY (!s8.IsEmpty());
	VERIFY (!s9.IsZero()); VERIFY (!s9.IsEmpty());

	VERIFY (p1.ToSize ().W ==  0 && p1.ToSize ().H == 0);
	VERIFY (p2.ToSize ().W ==  5 && p2.ToSize ().H == 5);
	VERIFY (p3.ToSize ().W ==  0 && p3.ToSize ().H == 8);
	VERIFY (p4.ToSize ().W == -4 && p4.ToSize ().H == 0);

	VERIFY (s1.ToPoint().X ==  0 && s1.ToPoint().Y == 0);
	VERIFY (s2.ToPoint().X ==  5 && s2.ToPoint().Y == 5);
	VERIFY (s3.ToPoint().X ==  0 && s3.ToPoint().Y == 8);
	VERIFY (s4.ToPoint().X == -4 && s4.ToPoint().Y == 0);

	p2 += p1;
	p3 += p2;
	p4 -= p3;
	p5 -= p4;
	p6 = p2 + p3;
	p7 = p4 + p5;
	p8 = p2 - p3;
	p9 = p4 - p5;

	s2 += s1;
	s3 += s2;
	s4 -= s3;
	s5 -= s4;
	s6 = s2 + s3;
	s7 = s4 + s5;
	s8 = s2 - s3;
	s9 = s4 - s5;

	VERIFY (p1.X ==   0 && p1.Y ==   0);
	VERIFY (p2.X ==   5 && p2.Y ==   5);
	VERIFY (p3.X ==   5 && p3.Y ==  13);
	VERIFY (p4.X ==  -9 && p4.Y == -13);
	VERIFY (p5.X ==  12 && p5.Y ==   7);
	VERIFY (p6.X ==  10 && p6.Y ==  18);
	VERIFY (p7.X ==   3 && p7.Y ==  -6);
	VERIFY (p8.X ==   0 && p8.Y ==  -8);
	VERIFY (p9.X == -21 && p9.Y == -20);

	VERIFY (s1.W ==   0 && s1.H ==   0);
	VERIFY (s2.W ==   5 && s2.H ==   5);
	VERIFY (s3.W ==   5 && s3.H ==  13);
	VERIFY (s4.W ==  -9 && s4.H == -13);
	VERIFY (s5.W ==  12 && s5.H ==   7);
	VERIFY (s6.W ==  10 && s6.H ==  18);
	VERIFY (s7.W ==   3 && s7.H ==  -6);
	VERIFY (s8.W ==   0 && s8.H ==  -8);
	VERIFY (s9.W == -21 && s9.H == -20);

	p6 = p2;
	p7 = p3;
	p2 = Point ( 6   );
	p3 = Point ( 3, 9);
	p4 = Point ( 6, 2);
	p5 = Point (-3, 8);
	p8 = p4;
	p9 = Point (-3, 8);

	s6 = s2;
	s7 = s3;
	s2 = Size ( 6   );
	s3 = Size ( 3, 9);
	s4 = Size ( 6, 2);
	s5 = Size (-3, 8);
	s8 = s4;
	s9 = Size (-3, 8);

	VERIFY (p1.X ==  0 && p1.Y ==  0);
	VERIFY (p2.X ==  6 && p2.Y ==  6);
	VERIFY (p3.X ==  3 && p3.Y ==  9);
	VERIFY (p4.X ==  6 && p4.Y ==  2);
	VERIFY (p5.X == -3 && p5.Y ==  8);
	VERIFY (p6.X ==  5 && p6.Y ==  5);
	VERIFY (p7.X ==  5 && p7.Y == 13);
	VERIFY (p8.X ==  6 && p8.Y ==  2);
	VERIFY (p9.X == -3 && p9.Y ==  8);

	VERIFY (s1.W ==  0 && s1.H ==  0);
	VERIFY (s2.W ==  6 && s2.H ==  6);
	VERIFY (s3.W ==  3 && s3.H ==  9);
	VERIFY (s4.W ==  6 && s4.H ==  2);
	VERIFY (s5.W == -3 && s5.H ==  8);
	VERIFY (s6.W ==  5 && s6.H ==  5);
	VERIFY (s7.W ==  5 && s7.H == 13);
	VERIFY (s8.W ==  6 && s8.H ==  2);
	VERIFY (s9.W == -3 && s9.H ==  8);

	VERIFY ( (p1 == p1));
	VERIFY (!(p1 == p2));
	VERIFY (!(p1 != p1));
	VERIFY ( (p1 != p2));

	VERIFY (p6 != p2);
	VERIFY (p2 != p6);
	VERIFY (p8 == p4);
	VERIFY (p4 == p8);
	VERIFY (p3 != p5);
	VERIFY (p5 != p3);
	VERIFY (p9 == p5);
	VERIFY (p5 == p9);

	VERIFY ( (s1 == s1));
	VERIFY (!(s1 == s2));
	VERIFY (!(s1 != s1));
	VERIFY ( (s1 != s2));

	VERIFY (s6 != s2);
	VERIFY (s2 != s6);
	VERIFY (s8 == s4);
	VERIFY (s4 == s8);
	VERIFY (s3 != s5);
	VERIFY (s5 != s3);
	VERIFY (s9 == s5);
	VERIFY (s5 == s9);

	p2 = -p2;
	p3 = -p3;
	p4 = -p5;
	p5 = -p4;
	p6 = -Point (4, 8);
	p7 = -s7.ToPoint();

	VERIFY (p2.X == -6 && p2.Y ==  -6);
	VERIFY (p3.X == -3 && p3.Y ==  -9);
	VERIFY (p4.X ==  3 && p4.Y ==  -8);
	VERIFY (p5.X == -3 && p5.Y ==   8);
	VERIFY (p6.X == -4 && p6.Y ==  -8);
	VERIFY (p7.X == -5 && p7.Y == -13);

	p3 = Point ( 0, 8);
	p4 = Point (-4, 0);
	s3 = Size  (-4, 0);
	s4 = Size  ( 0, 8);

	Bounds b1;
	Bounds b2 ( 2,  8,  7,  3);
	Bounds b3 (-4,  5,  3, -6);
	Bounds b4 (-8, -4, -3, -7);
	Bounds b5 (p3, s3);
	Bounds b6 (p4, s4);
	Bounds b7 (20    );
	Bounds b8 (20, 40);

	VERIFY (b1.X ==  0 && b1.Y ==  0 && b1.W ==  0 && b1.H ==  0);
	VERIFY (b2.X ==  2 && b2.Y ==  8 && b2.W ==  7 && b2.H ==  3);
	VERIFY (b3.X == -4 && b3.Y ==  5 && b3.W ==  3 && b3.H == -6);
	VERIFY (b4.X == -8 && b4.Y == -4 && b4.W == -3 && b4.H == -7);
	VERIFY (b5.X ==  0 && b5.Y ==  8 && b5.W == -4 && b5.H ==  0);
	VERIFY (b6.X == -4 && b6.Y ==  0 && b6.W ==  0 && b6.H ==  8);
	VERIFY (b7.X == 20 && b7.Y == 20 && b7.W == 20 && b7.H == 20);
	VERIFY (b8.X == 20 && b8.Y == 20 && b8.W == 40 && b8.H == 40);

	VERIFY ( b1.IsZero()); VERIFY ( b1.IsEmpty()); VERIFY (!b1.IsValid());
	VERIFY (!b2.IsZero()); VERIFY (!b2.IsEmpty()); VERIFY ( b2.IsValid());
	VERIFY (!b3.IsZero()); VERIFY (!b3.IsEmpty()); VERIFY (!b3.IsValid());
	VERIFY (!b4.IsZero()); VERIFY (!b4.IsEmpty()); VERIFY (!b4.IsValid());
	VERIFY (!b5.IsZero()); VERIFY ( b5.IsEmpty()); VERIFY (!b5.IsValid());
	VERIFY (!b6.IsZero()); VERIFY ( b6.IsEmpty()); VERIFY (!b6.IsValid());
	VERIFY (!b7.IsZero()); VERIFY (!b7.IsEmpty()); VERIFY ( b7.IsValid());
	VERIFY (!b8.IsZero()); VERIFY (!b8.IsEmpty()); VERIFY ( b8.IsValid());

	VERIFY (b1.GetLeft () ==   0); VERIFY (b1.GetTop   () ==   0);
	VERIFY (b1.GetRight() ==   0); VERIFY (b1.GetBottom() ==   0);
	VERIFY (b2.GetLeft () ==   2); VERIFY (b2.GetTop   () ==   8);
	VERIFY (b2.GetRight() ==   9); VERIFY (b2.GetBottom() ==  11);
	VERIFY (b3.GetLeft () ==  -4); VERIFY (b3.GetTop   () ==   5);
	VERIFY (b3.GetRight() ==  -1); VERIFY (b3.GetBottom() ==  -1);
	VERIFY (b4.GetLeft () ==  -8); VERIFY (b4.GetTop   () ==  -4);
	VERIFY (b4.GetRight() == -11); VERIFY (b4.GetBottom() == -11);
	VERIFY (b5.GetLeft () ==   0); VERIFY (b5.GetTop   () ==   8);
	VERIFY (b5.GetRight() ==  -4); VERIFY (b5.GetBottom() ==   8);
	VERIFY (b6.GetLeft () ==  -4); VERIFY (b6.GetTop   () ==   0);
	VERIFY (b6.GetRight() ==  -4); VERIFY (b6.GetBottom() ==   8);
	VERIFY (b7.GetLeft () ==  20); VERIFY (b7.GetTop   () ==  20);
	VERIFY (b7.GetRight() ==  40); VERIFY (b7.GetBottom() ==  40);
	VERIFY (b8.GetLeft () ==  20); VERIFY (b8.GetTop   () ==  20);
	VERIFY (b8.GetRight() ==  60); VERIFY (b8.GetBottom() ==  60);

	VERIFY (b1.GetCenter() == Point ( 0,  0));
	VERIFY (b2.GetCenter() == Point ( 5,  9));
	VERIFY (b3.GetCenter() == Point (-3,  2));
	VERIFY (b4.GetCenter() == Point (-9, -7));
	VERIFY (b5.GetCenter() == Point (-2,  8));
	VERIFY (b6.GetCenter() == Point (-4,  4));
	VERIFY (b7.GetCenter() == Point (30, 30));
	VERIFY (b8.GetCenter() == Point (40, 40));

	b7.SetLTRB (3,  9, 8, 10);
	b8.SetLTRB (5, 13, 7,  6);
	VERIFY (!b1.Contains (b7)); VERIFY (!b1.Intersects (b7));
	VERIFY ( b2.Contains (b7)); VERIFY ( b2.Intersects (b7));
	VERIFY (!b4.Contains (b7)); VERIFY (!b4.Intersects (b7));
	VERIFY (!b6.Contains (b7)); VERIFY (!b6.Intersects (b7));
	VERIFY (!b1.Contains (b8)); VERIFY (!b1.Intersects (b8));
	VERIFY (!b2.Contains (b8)); VERIFY ( b2.Intersects (b8));
	VERIFY (!b4.Contains (b8)); VERIFY (!b4.Intersects (b8));
	VERIFY (!b6.Contains (b8)); VERIFY (!b6.Intersects (b8));

	VERIFY ((b7 | b1) == Bounds (  3,   9,  5,  1)); VERIFY ((b7 & b1) == Bounds (0, 0, 0, 0));
	VERIFY ((b8 | b1) == Bounds (  5,   6,  2,  7)); VERIFY ((b8 & b1) == Bounds (0, 0, 0, 0));

	VERIFY ((b1 | b7) == Bounds (  3,   9,  5,  1)); VERIFY ((b1 & b7) == Bounds (0, 0, 0, 0));
	VERIFY ((b2 | b7) == Bounds (  2,   8,  7,  3)); VERIFY ((b2 & b7) == Bounds (3, 9, 5, 1));
	VERIFY ((b4 | b7) == Bounds (-11, -11, 19, 21)); VERIFY ((b4 & b7) == Bounds (0, 0, 0, 0));
	VERIFY ((b6 | b7) == Bounds ( -4,   0, 12, 10)); VERIFY ((b6 & b7) == Bounds (0, 0, 0, 0));
	VERIFY ((b1 | b8) == Bounds (  5,   6,  2,  7)); VERIFY ((b1 & b8) == Bounds (0, 0, 0, 0));
	VERIFY ((b2 | b8) == Bounds (  2,   6,  7,  7)); VERIFY ((b2 & b8) == Bounds (5, 8, 2, 3));
	VERIFY ((b4 | b8) == Bounds (-11, -11, 18, 24)); VERIFY ((b4 & b8) == Bounds (0, 0, 0, 0));
	VERIFY ((b6 | b8) == Bounds ( -4,   0, 11, 13)); VERIFY ((b6 & b8) == Bounds (0, 0, 0, 0));
	b7.Normalize(); VERIFY (b7 == Bounds (3, 9, 5, 1));
	b8.Normalize(); VERIFY (b8 == Bounds (5, 6, 2, 7));
	b8 |= b2; VERIFY (b8 == Bounds (2, 6, 7, 7));
	b7 &= b2; VERIFY (b7 == Bounds (3, 9, 5, 1));

	b7.SetLTRB (-2, 10, -2, 6);
	b8.SetLTRB (-6,  2, -2, 2);
	VERIFY (!b3.Contains (b7)); VERIFY (!b3.Intersects (b7));
	VERIFY (!b4.Contains (b7)); VERIFY (!b4.Intersects (b7));
	VERIFY (!b5.Contains (b7)); VERIFY ( b5.Intersects (b7));
	VERIFY (!b6.Contains (b7)); VERIFY (!b6.Intersects (b7));
	VERIFY (!b3.Contains (b8)); VERIFY ( b3.Intersects (b8));
	VERIFY (!b4.Contains (b8)); VERIFY (!b4.Intersects (b8));
	VERIFY (!b5.Contains (b8)); VERIFY (!b5.Intersects (b8));
	VERIFY (!b6.Contains (b8)); VERIFY ( b6.Intersects (b8));

	VERIFY ((b3 | b7) == Bounds ( -4,  -1, 3, 11)); VERIFY ((b3 & b7) == Bounds ( 0, 0, 0, 0));
	VERIFY ((b4 | b7) == Bounds (-11, -11, 9, 21)); VERIFY ((b4 & b7) == Bounds ( 0, 0, 0, 0));
	VERIFY ((b5 | b7) == Bounds ( -4,   6, 4,  4)); VERIFY ((b5 & b7) == Bounds (-2, 8, 0, 0));
	VERIFY ((b6 | b7) == Bounds ( -4,   0, 2, 10)); VERIFY ((b6 & b7) == Bounds ( 0, 0, 0, 0));
	VERIFY ((b3 | b8) == Bounds ( -6,  -1, 5,  6)); VERIFY ((b3 & b8) == Bounds (-4, 2, 2, 0));
	VERIFY ((b4 | b8) == Bounds (-11, -11, 9, 13)); VERIFY ((b4 & b8) == Bounds ( 0, 0, 0, 0));
	VERIFY ((b5 | b8) == Bounds ( -6,   2, 6,  6)); VERIFY ((b5 & b8) == Bounds ( 0, 0, 0, 0));
	VERIFY ((b6 | b8) == Bounds ( -6,   0, 4,  8)); VERIFY ((b6 & b8) == Bounds (-4, 2, 0, 0));
	b7.Normalize(); VERIFY (b7 == Bounds (-2, 6, 0, 4));
	b8.Normalize(); VERIFY (b8 == Bounds (-6, 2, 4, 0));
	b7 |= b3; VERIFY (b7 == Bounds (-4, -1, 3, 11));
	b8 &= b3; VERIFY (b8 == Bounds (-4,  2, 2,  0));

	b7 = Bounds (-2, 1, -1, -1);
	b8 = Bounds (-3, 3, -2,  1);
	VERIFY (!b1.Contains (b7)); VERIFY (!b1.Intersects (b7));
	VERIFY (!b2.Contains (b7)); VERIFY (!b2.Intersects (b7));
	VERIFY ( b3.Contains (b7)); VERIFY ( b3.Intersects (b7));
	VERIFY (!b6.Contains (b7)); VERIFY (!b6.Intersects (b7));
	VERIFY (!b1.Contains (b8)); VERIFY (!b1.Intersects (b8));
	VERIFY (!b2.Contains (b8)); VERIFY (!b2.Intersects (b8));
	VERIFY (!b3.Contains (b8)); VERIFY ( b3.Intersects (b8));
	VERIFY (!b6.Contains (b8)); VERIFY ( b6.Intersects (b8));

	VERIFY ((b1 | b7) == Bounds (-3,  0,  1,  1)); VERIFY ((b1 & b7) == Bounds ( 0, 0, 0, 0));
	VERIFY ((b2 | b7) == Bounds (-3,  0, 12, 11)); VERIFY ((b2 & b7) == Bounds ( 0, 0, 0, 0));
	VERIFY ((b3 | b7) == Bounds (-4, -1,  3,  6)); VERIFY ((b3 & b7) == Bounds (-3, 0, 1, 1));
	VERIFY ((b6 | b7) == Bounds (-4,  0,  2,  8)); VERIFY ((b6 & b7) == Bounds ( 0, 0, 0, 0));
	VERIFY ((b1 | b8) == Bounds (-5,  3,  2,  1)); VERIFY ((b1 & b8) == Bounds ( 0, 0, 0, 0));
	VERIFY ((b2 | b8) == Bounds (-5,  3, 14,  8)); VERIFY ((b2 & b8) == Bounds ( 0, 0, 0, 0));
	VERIFY ((b3 | b8) == Bounds (-5, -1,  4,  6)); VERIFY ((b3 & b8) == Bounds (-4, 3, 1, 1));
	VERIFY ((b6 | b8) == Bounds (-5,  0,  2,  8)); VERIFY ((b6 & b8) == Bounds (-4, 3, 0, 1));
	b7.Normalize(); VERIFY (b7 == Bounds (-3, 0, 1, 1));
	b8.Normalize(); VERIFY (b8 == Bounds (-5, 3, 2, 1));
	b8 |= b3; VERIFY (b8 == Bounds (-5, -1, 4, 6));
	b7 &= b3; VERIFY (b7 == Bounds (-3,  0, 1, 1));

	b7 = Bounds (-11, -9, 2,  2);
	b8 = Bounds ( -8, -5, 2, -2);
	VERIFY ( b4.Contains (b7,  true)); VERIFY ( b4.Intersects (b7,  true));
	VERIFY (!b4.Contains (b8,  true)); VERIFY ( b4.Intersects (b8,  true));
	VERIFY (!b4.Contains (b7, false)); VERIFY ( b4.Intersects (b7, false));
	VERIFY (!b4.Contains (b8, false)); VERIFY (!b4.Intersects (b8, false));
	VERIFY ((b4 | b7) == Bounds (-11, -11, 3, 7)); VERIFY ((b4 & b7) == Bounds (-11, -9, 2, 2));
	VERIFY ((b4 | b8) == Bounds (-11, -11, 5, 7)); VERIFY ((b4 & b8) == Bounds ( -8, -7, 0, 2));
	b7.Normalize(); VERIFY (b7 == Bounds (-11, -9, 2, 2));
	b8.Normalize(); VERIFY (b8 == Bounds ( -8, -7, 2, 2));
	b7 |= b4; VERIFY (b7 == Bounds (-11, -11, 3, 7));
	b8 &= b4; VERIFY (b8 == Bounds ( -8,  -7, 0, 2));

	b7 = Bounds (-8, -10,  0, 1);
	b8 = Bounds (-9,  -4, -1, 0);
	VERIFY ( b4.Contains (b7,  true)); VERIFY ( b4.Intersects (b7,  true));
	VERIFY ( b4.Contains (b8,  true)); VERIFY ( b4.Intersects (b8,  true));
	VERIFY (!b4.Contains (b7, false)); VERIFY (!b4.Intersects (b7, false));
	VERIFY (!b4.Contains (b8, false)); VERIFY (!b4.Intersects (b8, false));
	VERIFY ((b4 | b7) == Bounds (-11, -11, 3, 7)); VERIFY ((b4 & b7) == Bounds ( -8, -10, 0, 1));
	VERIFY ((b4 | b8) == Bounds (-11, -11, 3, 7)); VERIFY ((b4 & b8) == Bounds (-10,  -4, 1, 0));
	b7.Normalize(); VERIFY (b7 == Bounds ( -8, -10, 0, 1));
	b8.Normalize(); VERIFY (b8 == Bounds (-10,  -4, 1, 0));
	b8 |= b4; VERIFY (b8 == Bounds (-11, -11, 3, 7));
	b7 &= b4; VERIFY (b7 == Bounds ( -8, -10, 0, 1));

	b7 = Bounds (-1, 8, -1,  0);
	b8 = Bounds (-4, 8,  0, -2);
	VERIFY ( b5.Contains (b7,  true)); VERIFY ( b5.Intersects (b7,  true));
	VERIFY (!b6.Contains (b7,  true)); VERIFY (!b6.Intersects (b7,  true));
	VERIFY (!b5.Contains (b8,  true)); VERIFY ( b5.Intersects (b8,  true));
	VERIFY ( b6.Contains (b8,  true)); VERIFY ( b6.Intersects (b8,  true));
	VERIFY (!b5.Contains (b7, false)); VERIFY (!b5.Intersects (b7, false));
	VERIFY (!b6.Contains (b7, false)); VERIFY (!b6.Intersects (b7, false));
	VERIFY (!b5.Contains (b8, false)); VERIFY (!b5.Intersects (b8, false));
	VERIFY (!b6.Contains (b8, false)); VERIFY (!b6.Intersects (b8, false));
	VERIFY ((b5 | b7) == Bounds (-4, 8, 4, 0)); VERIFY ((b5 & b7) == Bounds (-2, 8, 1, 0));
	VERIFY ((b6 | b7) == Bounds (-4, 0, 3, 8)); VERIFY ((b6 & b7) == Bounds ( 0, 0, 0, 0));
	VERIFY ((b5 | b8) == Bounds (-4, 6, 4, 2)); VERIFY ((b5 & b8) == Bounds (-4, 8, 0, 0));
	VERIFY ((b6 | b8) == Bounds (-4, 0, 0, 8)); VERIFY ((b6 & b8) == Bounds (-4, 6, 0, 2));
	b7.Normalize(); VERIFY (b7 == Bounds (-2, 8, 1, 0));
	b8.Normalize(); VERIFY (b8 == Bounds (-4, 6, 0, 2));
	b7 |= b6; VERIFY (b7 == Bounds (-4, 0, 3, 8));
	b8 &= b6; VERIFY (b8 == Bounds (-4, 6, 0, 2));

	b1.SetLeft (4); b2.SetTop (-6); b3.SetRight (7); b4.SetBottom (-2);
	b5.SetLeft (8); b5.SetTop (-2); b6.SetRight (6); b6.SetBottom (-3);

	VERIFY (b1.X ==  4 && b1.Y ==  0 && b1.W ==  0 && b1.H ==  0);
	VERIFY (b2.X ==  2 && b2.Y == -6 && b2.W ==  7 && b2.H ==  3);
	VERIFY (b3.X == -4 && b3.Y ==  5 && b3.W == 11 && b3.H == -6);
	VERIFY (b4.X == -8 && b4.Y == -4 && b4.W == -3 && b4.H ==  2);
	VERIFY (b5.X ==  8 && b5.Y == -2 && b5.W == -4 && b5.H ==  0);
	VERIFY (b6.X == -4 && b6.Y ==  0 && b6.W == 10 && b6.H == -3);

	b1.SetLTRB (-6, 8, 4, -2);
	b2.SetLTRB (-5, 9, 3, -3);
	int32 bl1, bt1, br1, bb1;
	int32 bl2, bt2, br2, bb2;
	b1.GetLTRB (bl1, bt1, br1, bb1);
	b2.GetLTRB (bl2, bt2, br2, bb2);

	VERIFY (b1.X == -6 && b1.Y ==  8 && b1.W == 10 && b1.H == -10);
	VERIFY (b2.X == -5 && b2.Y ==  9 && b2.W ==  8 && b2.H == -12);
	VERIFY (bl1  == -6 && bt1  ==  8 && br1  ==  4 && bb1  ==  -2);
	VERIFY (bl2  == -5 && bt2  ==  9 && br2  ==  3 && bb2  == - 3);

	p1 = Point (-6, 12);
	p2 = Point ( 3,  4);
	p3 = Point ( 8,  3);
	p4 = Point (-5,  3);
	p5 = Point (-2,  8);
	p6 = Point (-3, -3);

	VERIFY (!b1.Contains (-7,  3));
	VERIFY ( b1.Contains (-6,  3));
	VERIFY ( b1.Contains (-5,  3));
	VERIFY ( b1.Contains ( 3,  3));
	VERIFY ( b1.Contains ( 4,  3));
	VERIFY (!b1.Contains ( 5,  3));
	VERIFY ( b1.Contains ( 0,  7));
	VERIFY ( b1.Contains ( 0,  8));
	VERIFY (!b1.Contains ( 0,  9));
	VERIFY (!b1.Contains ( 0, -3));
	VERIFY ( b1.Contains ( 0, -2));
	VERIFY ( b1.Contains ( 0, -1));

	VERIFY (!b2.Contains (p1));
	VERIFY ( b2.Contains (p2));
	VERIFY (!b2.Contains (p3));
	VERIFY ( b2.Contains (p4));
	VERIFY ( b2.Contains (p5));
	VERIFY ( b2.Contains (p6));

	VERIFY (!b1.Contains (-7,  3, false));
	VERIFY (!b1.Contains (-6,  3, false));
	VERIFY ( b1.Contains (-5,  3, false));
	VERIFY ( b1.Contains ( 3,  3, false));
	VERIFY (!b1.Contains ( 4,  3, false));
	VERIFY (!b1.Contains ( 5,  3, false));
	VERIFY ( b1.Contains ( 0,  7, false));
	VERIFY (!b1.Contains ( 0,  8, false));
	VERIFY (!b1.Contains ( 0,  9, false));
	VERIFY (!b1.Contains ( 0, -3, false));
	VERIFY (!b1.Contains ( 0, -2, false));
	VERIFY ( b1.Contains ( 0, -1, false));

	VERIFY (!b2.Contains (p1, false));
	VERIFY (!b2.Contains (p2, false));
	VERIFY (!b2.Contains (p3, false));
	VERIFY (!b2.Contains (p4, false));
	VERIFY ( b2.Contains (p5, false));
	VERIFY (!b2.Contains (p6, false));

	b1.SetPoint ( 2,  8);
	b2.SetPoint ( 7,  3);
	b3.SetPoint (-8, -4);
	b4.SetPoint (Point ( 7,  3));
	b5.SetPoint (Point ( 7,  3));
	b6.SetPoint (Point (-3, -7));

	b1.SetSize ( 2,  8);
	b2.SetSize ( 3, -6);
	b3.SetSize (-3, -7);
	b4.SetSize (Size ( 2,  8));
	b5.SetSize (Size ( 2,  8));
	b6.SetSize (Size (-8, -4));

	VERIFY (b1.GetPoint().X ==  2 && b1.GetPoint().Y ==  8);
	VERIFY (b2.GetPoint().X ==  7 && b2.GetPoint().Y ==  3);
	VERIFY (b3.GetPoint().X == -8 && b3.GetPoint().Y == -4);
	VERIFY (b4.GetPoint().X ==  7 && b4.GetPoint().Y ==  3);
	VERIFY (b5.GetPoint().X ==  7 && b5.GetPoint().Y ==  3);
	VERIFY (b6.GetPoint().X == -3 && b6.GetPoint().Y == -7);

	VERIFY (b1.GetSize().W ==  2 && b1.GetSize().H ==  8);
	VERIFY (b2.GetSize().W ==  3 && b2.GetSize().H == -6);
	VERIFY (b3.GetSize().W == -3 && b3.GetSize().H == -7);
	VERIFY (b4.GetSize().W ==  2 && b4.GetSize().H ==  8);
	VERIFY (b5.GetSize().W ==  2 && b5.GetSize().H ==  8);
	VERIFY (b6.GetSize().W == -8 && b6.GetSize().H == -4);

	VERIFY ( (b1 == b1));
	VERIFY (!(b1 == b2));
	VERIFY (!(b1 != b1));
	VERIFY ( (b1 != b2));

	VERIFY (b6 != b2);
	VERIFY (b2 != b6);
	VERIFY (b4 == b5);
	VERIFY (b5 == b4);
	VERIFY (b4 != b2);
	VERIFY (b2 != b4);
	VERIFY (b5 != b1);
	VERIFY (b1 != b5);

	return true;
}

////////////////////////////////////////////////////////////////////////////////

bool TestTypes (void)
{
	cout << "BEGIN TYPES TESTING\n------------------------------\n";
	if (!TestGlobal()) { cout << ">> Global Failed\n\n"; return false; }
	if (!TestEnum  ()) { cout << ">> Enum Failed  \n\n"; return false; }
	if (!TestHash  ()) { cout << ">> Hash Failed  \n\n"; return false; }
	if (!TestColor ()) { cout << ">> Color Failed \n\n"; return false; }
	if (!TestImage1()) { cout << ">> Image1 Failed\n\n"; return false; }
	if (!TestImage2()) { cout << ">> Image2 Failed\n\n"; return false; }
	if (!TestRange ()) { cout << ">> Range Failed \n\n"; return false; }
	if (!TestBounds()) { cout << ">> Bounds Failed\n\n"; return false; }
	cout << ">> Success\n\n"; return true;
}
