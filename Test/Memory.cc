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
// Macros                                                                     //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

#define TEST_INVALID_RW( mem, address )						\
	{														\
		VERIFY (mem. ReadData (address, nullptr, 0) == 0);	\
		VERIFY (mem. ReadData (address, nullptr, 1) == 0);	\
		VERIFY (mem. ReadData (address,   &data, 0) == 0);	\
		VERIFY (mem. ReadData (address,   &data, 1) == 0);	\
															\
		VERIFY (mem.WriteData (address, nullptr, 0) == 0);	\
		VERIFY (mem.WriteData (address, nullptr, 1) == 0);	\
		VERIFY (mem.WriteData (address,   &data, 0) == 0);	\
		VERIFY (mem.WriteData (address,   &data, 1) == 0);	\
	}



//----------------------------------------------------------------------------//
// Locals                                                                     //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

#ifdef ROBOT_OS_LINUX

	#define  PINT16 (is64Bit ? 0x604202 : 0x804D112)
	#define  PINT32 (is64Bit ? 0x604204 : 0x804D114)
	#define  PINT64 (is64Bit ? 0x604208 : 0x804D118)

	#define PREAL32 (is64Bit ? 0x604210 : 0x804D120)
	#define PREAL64 (is64Bit ? 0x604218 : 0x804D128)

	#define  PPTR16 (is64Bit ? 0x604220 : 0x804D130)
	#define  PPTR32 (is64Bit ? 0x604228 : 0x804D134)
	#define  PPTR64 (is64Bit ? 0x604230 : 0x804D138)

	#define  PARRAY (is64Bit ? 0x604238 : 0x804D13C)

	#define PFINITE (is64Bit ? 0x604240 : 0x804D140)
	#define PVECTOR (is64Bit ? 0x604250 : 0x804D150)

	#define   PPAT1 (is64Bit ? 0x402E40 : 0x804B0A0)
	#define   PPAT2 (is64Bit ? 0x402E60 : 0x804B0C0)
	#define   PPAT3 (is64Bit ? 0x604260 : 0x804D160)
	#define   PPAT4 (is64Bit ? 0x604268 : 0x804D164)
	#define   PPAT5 (is64Bit ? 0x402E80 : 0x804B0E0)
	#define   PPAT6 (is64Bit ? 0x402EA0 : 0x804B100)

#endif
#ifdef ROBOT_OS_MAC

	#define  PINT16 (is64Bit ? 0xA400 : 0x0)
	#define  PINT32 (is64Bit ? 0xA404 : 0x0)
	#define  PINT64 (is64Bit ? 0xA408 : 0x0)

	#define PREAL32 (is64Bit ? 0xA410 : 0x0)
	#define PREAL64 (is64Bit ? 0xA418 : 0x0)

	#define  PPTR16 (is64Bit ? 0xA420 : 0x0)
	#define  PPTR32 (is64Bit ? 0xA428 : 0x0)
	#define  PPTR64 (is64Bit ? 0xA430 : 0x0)

	#define  PARRAY (is64Bit ? 0xA3F8 : 0x0)

	#define PFINITE (is64Bit ? 0xA438 : 0x0)
	#define PVECTOR (is64Bit ? 0xA448 : 0x0)

	#define   PPAT1 (is64Bit ? 0x9880 : 0x0)
	#define   PPAT2 (is64Bit ? 0x98A0 : 0x0)
	#define   PPAT3 (is64Bit ? 0xA3E8 : 0x0)
	#define   PPAT4 (is64Bit ? 0xA3F0 : 0x0)
	#define   PPAT5 (is64Bit ? 0x9840 : 0x0)
	#define   PPAT6 (is64Bit ? 0x9860 : 0x0)

#endif
#ifdef ROBOT_OS_WIN

	#define  PINT16 (is64Bit ? 0xAD820 : 0x84CC0)
	#define  PINT32 (is64Bit ? 0xAD824 : 0x84CC4)
	#define  PINT64 (is64Bit ? 0xAD828 : 0x84CC8)

	#define PREAL32 (is64Bit ? 0xAD830 : 0x84CD0)
	#define PREAL64 (is64Bit ? 0xAD838 : 0x84CD8)

	#define  PPTR16 (is64Bit ? 0xAD840 : 0x84CD4)
	#define  PPTR32 (is64Bit ? 0xAD848 : 0x84CE0)
	#define  PPTR64 (is64Bit ? 0xAD850 : 0x84CE4)

	#define  PARRAY (is64Bit ? 0xAD858 : 0x84CE8)

	#define PFINITE (is64Bit ? 0xAD860 : 0x84CF0)
	#define PVECTOR (is64Bit ? 0xAD870 : 0x84D00)

	#define   PPAT1 (is64Bit ? 0x89120 : 0x70380)
	#define   PPAT2 (is64Bit ? 0x89140 : 0x703A0)
	#define   PPAT3 (is64Bit ? 0xAD880 : 0x84CEC)
	#define   PPAT4 (is64Bit ? 0xAD888 : 0x84D10)
	#define   PPAT5 (is64Bit ? 0x8A308 : 0x70968)
	#define   PPAT6 (is64Bit ? 0x8A330 : 0x70990)

#endif



//----------------------------------------------------------------------------//
// Functions                                                                  //
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////

static bool TestInvalid (void)
{
	Module mod; VERIFY (!mod.IsValid());
	Memory mem; VERIFY (!mem.IsValid());
	VERIFY (mod.GetProcess() == Process());
	VERIFY (mem.GetProcess() == Process());

	VERIFY (mod.GetName    ().empty());
	VERIFY (mod.GetPath    ().empty());
	VERIFY (mod.GetBase    () == 0);
	VERIFY (mod.GetSize    () == 0);
	VERIFY (mod.GetSegments().empty());

	auto p = Process( ); VERIFY (!  p.IsValid());
	   mem = Memory (p); VERIFY (!mem.IsValid());
	VERIFY (mem.GetProcess() == p);

	VERIFY (p.IsDebugged() == false);
	VERIFY (p.GetModules().empty ());

	Memory::Stats stats = mem.GetStats();
	VERIFY (stats.SystemReads  == 0);
	VERIFY (stats.CachedReads  == 0);
	VERIFY (stats.SystemWrites == 0);
	VERIFY (stats.AccessWrites == 0);
	VERIFY (stats.ReadErrors   == 0);
	VERIFY (stats.WriteErrors  == 0);

	VERIFY (!mem.GetRegion (0).Valid);
	VERIFY (!mem.GetRegion (1).Valid);
	VERIFY (!mem.GetRegion (mem.GetMinAddress()-1).Valid);
	VERIFY (!mem.GetRegion (mem.GetMinAddress()  ).Valid);
	VERIFY (!mem.GetRegion (mem.GetMinAddress()+1).Valid);
	VERIFY (!mem.GetRegion (mem.GetMaxAddress()-1).Valid);
	VERIFY (!mem.GetRegion (mem.GetMaxAddress()  ).Valid);
	VERIFY (!mem.GetRegion (mem.GetMaxAddress()+1).Valid);

	VERIFY (mem.GetRegions().empty());

	Memory::Region region = { 0 };
	VERIFY (!mem.SetAccess (region, 0));
	VERIFY (!mem.SetAccess (region, 1));
	VERIFY (!mem.SetAccess (region,
			 false, false, false));

	region.Valid = true;
	VERIFY (!mem.SetAccess (region, 0));
	VERIFY (!mem.SetAccess (region, 1));
	VERIFY (!mem.SetAccess (region,
			 false, false, false));

	region.Bound = true;
	VERIFY (!mem.SetAccess (region, 0));
	VERIFY (!mem.SetAccess (region, 1));
	VERIFY (!mem.SetAccess (region,
			 false, false, false));

	VERIFY (mem.Find (nullptr).empty());
	VERIFY (mem.Find ("     ").empty());

	uint8 data = 0;
	TEST_INVALID_RW (mem, 0);
	TEST_INVALID_RW (mem, 1);

	TEST_INVALID_RW (mem, mem.GetMinAddress()-1);
	TEST_INVALID_RW (mem, mem.GetMinAddress()  );
	TEST_INVALID_RW (mem, mem.GetMinAddress()+1);

	TEST_INVALID_RW (mem, mem.GetMaxAddress()-1);
	TEST_INVALID_RW (mem, mem.GetMaxAddress()  );
	TEST_INVALID_RW (mem, mem.GetMaxAddress()+1);

	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestEquals (void)
{
	{
		Module module1, module2;

		VERIFY (module1 == module1); VERIFY (module1 == module2);
		VERIFY (module2 == module2); VERIFY (module2 == module1);

		module1 = Module (Process(), "", "", 0, 0);
		VERIFY (module1 == module1); VERIFY (module1 != module2);
		VERIFY (module2 == module2); VERIFY (module2 != module1);

		module2 = Module (Process(), "", "", 0, 0);
		VERIFY (module1 == module1); VERIFY (module1 == module2);
		VERIFY (module2 == module2); VERIFY (module2 == module1);

		module1 = Module (Process(), "", "", 3, 0); VERIFY (module1 != module2);
		module2 = Module (Process(), "", "", 3, 0); VERIFY (module1 == module2);
		module1 = Module (Process(), "", "", 0, 4); VERIFY (module1 != module2);
		module2 = Module (Process(), "", "", 0, 4); VERIFY (module1 == module2);

		module1 = Module (Process::GetCurrent(), "", "", 0, 0); VERIFY (module1 != module2);
		module2 = Module (Process::GetCurrent(), "", "", 0, 0); VERIFY (module1 == module2);

		VERIFY (module1.GetSegments().empty());
		VERIFY (module2.GetSegments().empty());

		module1 = Module (Process(), "", "", 5, 0);
		module2 = Module (Process(), "", "", 5, 0);
		VERIFY (!(module1 <  module2)); VERIFY (!(module1 <  module2.GetBase()));
		VERIFY (!(module1 >  module2)); VERIFY (!(module1 >  module2.GetBase()));
		VERIFY ( (module1 <= module2)); VERIFY ( (module1 <= module2.GetBase()));
		VERIFY ( (module1 >= module2)); VERIFY ( (module1 >= module2.GetBase()));

		module1 = Module (Process(), "", "", 4, 0);
		VERIFY ( (module1 <  module2)); VERIFY ( (module1 <  module2.GetBase()));
		VERIFY (!(module1 >  module2)); VERIFY (!(module1 >  module2.GetBase()));
		VERIFY ( (module1 <= module2)); VERIFY ( (module1 <= module2.GetBase()));
		VERIFY (!(module1 >= module2)); VERIFY (!(module1 >= module2.GetBase()));

		module1 = Module (Process(), "", "", 6, 0);
		VERIFY (!(module1 <  module2)); VERIFY (!(module1 <  module2.GetBase()));
		VERIFY ( (module1 >  module2)); VERIFY ( (module1 >  module2.GetBase()));
		VERIFY (!(module1 <= module2)); VERIFY (!(module1 <= module2.GetBase()));
		VERIFY ( (module1 >= module2)); VERIFY ( (module1 >= module2.GetBase()));

		module1 = Module (Process(), "", "", 1000, 1000);
		VERIFY (!module1.Contains ( 999));
		VERIFY ( module1.Contains (1000));
		VERIFY ( module1.Contains (1001));
		VERIFY ( module1.Contains (1999));
		VERIFY (!module1.Contains (2000));
		VERIFY (!module1.Contains (2001));
	}

	//----------------------------------------------------------------------------//

	{
		Module::Segment segment1, segment2;

		segment1.Valid = segment2.Valid = true;
		segment1.Base  = segment2.Base  = 1;
		segment1.Size  = segment2.Size  = 2;

		memset (segment1.Name, 'z', 15);
		memset (segment2.Name, 'z', 15);
		segment1.Name[ 0] = segment2.Name[ 0] = 'a';
		segment1.Name[ 7] = segment2.Name[ 7] = 'b';
		segment1.Name[14] = segment2.Name[14] = 'c';
		segment1.Name[15] = segment2.Name[15] = '\0';

		VERIFY (segment1 == segment1); VERIFY (segment1 == segment2);
		VERIFY (segment2 == segment2); VERIFY (segment2 == segment1);

		segment1.Valid    = false; VERIFY (segment1 != segment2);
		segment2.Valid    = false; VERIFY (segment1 == segment2);
		segment1.Base     = 0;     VERIFY (segment1 != segment2);
		segment2.Base     = 0;     VERIFY (segment1 == segment2);
		segment2.Size     = 0;     VERIFY (segment2 != segment1);
		segment1.Size     = 0;     VERIFY (segment2 == segment1);

		segment1.Name[ 0] = 'z';   VERIFY (segment1 != segment2);
		segment2.Name[ 0] = 'z';   VERIFY (segment1 == segment2);
		segment2.Name[ 7] = 'z';   VERIFY (segment2 != segment1);
		segment1.Name[ 7] = 'z';   VERIFY (segment2 == segment1);
		segment1.Name[14] = 'z';   VERIFY (segment1 != segment2);
		segment2.Name[14] = 'z';   VERIFY (segment1 == segment2);

		segment1.Base = 5;
		segment2.Base = 5;
		VERIFY (!(segment1 <  segment2)); VERIFY (!(segment1 <  segment2.Base));
		VERIFY (!(segment1 >  segment2)); VERIFY (!(segment1 >  segment2.Base));
		VERIFY ( (segment1 <= segment2)); VERIFY ( (segment1 <= segment2.Base));
		VERIFY ( (segment1 >= segment2)); VERIFY ( (segment1 >= segment2.Base));

		segment1.Base = 4;
		VERIFY ( (segment1 <  segment2)); VERIFY ( (segment1 <  segment2.Base));
		VERIFY (!(segment1 >  segment2)); VERIFY (!(segment1 >  segment2.Base));
		VERIFY ( (segment1 <= segment2)); VERIFY ( (segment1 <= segment2.Base));
		VERIFY (!(segment1 >= segment2)); VERIFY (!(segment1 >= segment2.Base));

		segment1.Base = 6;
		VERIFY (!(segment1 <  segment2)); VERIFY (!(segment1 <  segment2.Base));
		VERIFY ( (segment1 >  segment2)); VERIFY ( (segment1 >  segment2.Base));
		VERIFY (!(segment1 <= segment2)); VERIFY (!(segment1 <= segment2.Base));
		VERIFY ( (segment1 >= segment2)); VERIFY ( (segment1 >= segment2.Base));

		segment1.Base = 1000;
		segment1.Size = 1000;
		VERIFY (!segment1.Contains ( 999));
		VERIFY ( segment1.Contains (1000));
		VERIFY ( segment1.Contains (1001));
		VERIFY ( segment1.Contains (1999));
		VERIFY (!segment1.Contains (2000));
		VERIFY (!segment1.Contains (2001));
	}

	//----------------------------------------------------------------------------//

	{
		Memory::Stats stats1, stats2;

		stats1.SystemReads  = stats2.SystemReads  = 1;
		stats1.CachedReads  = stats2.CachedReads  = 2;
		stats1.SystemWrites = stats2.SystemWrites = 3;
		stats1.AccessWrites = stats2.AccessWrites = 4;

		stats1. ReadErrors  = stats2. ReadErrors  = 5;
		stats1.WriteErrors  = stats2.WriteErrors  = 6;

		VERIFY (stats1 == stats1); VERIFY (stats1 == stats2);
		VERIFY (stats2 == stats2); VERIFY (stats2 == stats1);

		stats1.SystemReads  = 0; VERIFY (stats1 != stats2);
		stats2.SystemReads  = 0; VERIFY (stats1 == stats2);
		stats2.CachedReads  = 0; VERIFY (stats2 != stats1);
		stats1.CachedReads  = 0; VERIFY (stats2 == stats1);
		stats1.SystemWrites = 0; VERIFY (stats1 != stats2);
		stats2.SystemWrites = 0; VERIFY (stats1 == stats2);
		stats2.AccessWrites = 0; VERIFY (stats2 != stats1);
		stats1.AccessWrites = 0; VERIFY (stats2 == stats1);

		stats1. ReadErrors  = 0; VERIFY (stats1 != stats2);
		stats2. ReadErrors  = 0; VERIFY (stats1 == stats2);
		stats2.WriteErrors  = 0; VERIFY (stats2 != stats1);
		stats1.WriteErrors  = 0; VERIFY (stats2 == stats1);
	}

	//----------------------------------------------------------------------------//

	{
		Memory::Region region1, region2;

		region1.Valid      = region2.Valid      = true;
		region1.Bound      = region2.Bound      = true;

		region1.Start      = region2.Start      = 1;
		region1.Stop       = region2.Stop       = 2;
		region1.Size       = region2.Size       = 3;

		region1.Readable   = region2.Readable   = true;
		region1.Writable   = region2.Writable   = true;
		region1.Executable = region2.Executable = true;
		region1.Access     = region2.Access     = 4;

		region1.Private    = region2.Private    = true;
		region1.Guarded    = region2.Guarded    = true;

		VERIFY (region1 == region1); VERIFY (region1 == region2);
		VERIFY (region2 == region2); VERIFY (region2 == region1);

		region1.Valid      = false; VERIFY (region1 != region2);
		region2.Valid      = false; VERIFY (region1 == region2);
		region1.Bound      = false; VERIFY (region1 != region2);
		region2.Bound      = false; VERIFY (region1 == region2);

		region1.Start      = 0;     VERIFY (region1 != region2);
		region2.Start      = 0;     VERIFY (region1 == region2);
		region1.Stop       = 0;     VERIFY (region1 != region2);
		region2.Stop       = 0;     VERIFY (region1 == region2);
		region2.Size       = 0;     VERIFY (region2 != region1);
		region1.Size       = 0;     VERIFY (region2 == region1);

		region1.Readable   = false; VERIFY (region1 != region2);
		region2.Readable   = false; VERIFY (region1 == region2);
		region2.Writable   = false; VERIFY (region2 != region1);
		region1.Writable   = false; VERIFY (region2 == region1);
		region1.Executable = false; VERIFY (region1 != region2);
		region2.Executable = false; VERIFY (region1 == region2);
		region2.Access     = 0;     VERIFY (region2 != region1);
		region1.Access     = 0;     VERIFY (region2 == region1);

		region1.Private    = false; VERIFY (region1 != region2);
		region2.Private    = false; VERIFY (region1 == region2);
		region2.Guarded    = false; VERIFY (region2 != region1);
		region1.Guarded    = false; VERIFY (region2 == region1);

		region1.Start = 5;
		region2.Start = 5;
		VERIFY (!(region1 <  region2)); VERIFY (!(region1 <  region2.Start));
		VERIFY (!(region1 >  region2)); VERIFY (!(region1 >  region2.Start));
		VERIFY ( (region1 <= region2)); VERIFY ( (region1 <= region2.Start));
		VERIFY ( (region1 >= region2)); VERIFY ( (region1 >= region2.Start));

		region1.Start = 4;
		VERIFY ( (region1 <  region2)); VERIFY ( (region1 <  region2.Start));
		VERIFY (!(region1 >  region2)); VERIFY (!(region1 >  region2.Start));
		VERIFY ( (region1 <= region2)); VERIFY ( (region1 <= region2.Start));
		VERIFY (!(region1 >= region2)); VERIFY (!(region1 >= region2.Start));

		region1.Start = 6;
		VERIFY (!(region1 <  region2)); VERIFY (!(region1 <  region2.Start));
		VERIFY ( (region1 >  region2)); VERIFY ( (region1 >  region2.Start));
		VERIFY (!(region1 <= region2)); VERIFY (!(region1 <= region2.Start));
		VERIFY ( (region1 >= region2)); VERIFY ( (region1 >= region2.Start));

		region1.Start = 1000;
		region1.Stop  = 2000;
		VERIFY (!region1.Contains ( 999));
		VERIFY ( region1.Contains (1000));
		VERIFY ( region1.Contains (1001));
		VERIFY ( region1.Contains (1999));
		VERIFY (!region1.Contains (2000));
		VERIFY (!region1.Contains (2001));
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestParams (void)
{
	// Perform parameter tests on self
	Process p = Process::GetCurrent();

#ifdef ROBOT_ARCH_64

	VERIFY ( p.IsValid());
	VERIFY ( p.Is64Bit());

#else

	VERIFY ( p.IsValid());
	VERIFY (!p.Is64Bit());

#endif

#ifdef ROBOT_OS_LINUX

	VERIFY (p.GetHandle() == 0);

#endif
#if defined (ROBOT_OS_MAC) || \
	defined (ROBOT_OS_WIN)

	VERIFY (p.GetHandle() != 0);

#endif

	Memory m = Memory (p);
	VERIFY (m.IsValid ());
	VERIFY (m.GetProcess() == p);

	cout << "Please verify the following\n" << hex
		 << "Min VM: 0x" << m.GetMinAddress() << "\n"
		 << "Max VM: 0x" << m.GetMaxAddress() << "\n"
		 << "PgSize: 0x" << m.GetPageSize  () << "\n\n" << dec;

	//----------------------------------------------------------------------------//

	VERIFY (!m.GetRegion (0).Valid);
	VERIFY (!m.GetRegion (1).Valid);
	VERIFY (!m.GetRegion (m.GetMinAddress()-1).Valid);
	VERIFY (!m.GetRegion (m.GetMaxAddress()  ).Valid);
	VERIFY (!m.GetRegion (m.GetMaxAddress()+1).Valid);

	Memory::Region region = { 0 };
	VERIFY (!m.SetAccess (region, 0));
	VERIFY (!m.SetAccess (region, 1));
	VERIFY (!m.SetAccess (region,
			false, false, false));

	region.Valid = true;
	VERIFY (!m.SetAccess (region, 0));
	VERIFY (!m.SetAccess (region, 1));
	VERIFY (!m.SetAccess (region,
			false, false, false));

	//----------------------------------------------------------------------------//

	VERIFY (m.Find (nullptr).empty());
	VERIFY (m.Find ("     ").empty());
	VERIFY (m.Find (""     ).empty());

	VERIFY (m.Find ("0" ).empty());
	VERIFY (m.Find ("X" ).empty());
	VERIFY (m.Find ("X0").empty());
	VERIFY (m.Find ("0X").empty());
	VERIFY (m.Find ("?0").empty());
	VERIFY (m.Find ("0?").empty());

	VERIFY (m.Find ("?? 0" ).empty());
	VERIFY (m.Find ("00 X" ).empty());
	VERIFY (m.Find ("0 00" ).empty());
	VERIFY (m.Find ("X ??" ).empty());
	VERIFY (m.Find ("00 X0").empty());
	VERIFY (m.Find ("?? 0X").empty());
	VERIFY (m.Find ("?? ?0").empty());
	VERIFY (m.Find ("00 0?").empty());
	VERIFY (m.Find ("X0 ??").empty());
	VERIFY (m.Find ("0X 00").empty());
	VERIFY (m.Find ("?0 00").empty());
	VERIFY (m.Find ("0? ??").empty());

	VERIFY (m.GetRegions (0x100000, 0xFFFFF).empty());
	VERIFY (m.Find ("00", 0x100000, 0xFFFFF).empty());

	VERIFY (m.Find ("00", 0x100000, 0xFFFFF, 0, ""    ).empty());
	VERIFY (m.Find ("00", 0x100000, 0xFFFFF, 0, " "   ).empty());
	VERIFY (m.Find ("00", 0x100000, 0xFFFFF, 0, "-"   ).empty());
	VERIFY (m.Find ("00", 0x100000, 0xFFFFF, 0, "?"   ).empty());

	VERIFY (m.Find ("00", 0x100000, 0xFFFFF, 0, "r"   ).empty());
	VERIFY (m.Find ("00", 0x100000, 0xFFFFF, 0, "w"   ).empty());
	VERIFY (m.Find ("00", 0x100000, 0xFFFFF, 0, "x"   ).empty());

	VERIFY (m.Find ("00", 0x100000, 0xFFFFF, 0, "Ww"  ).empty());
	VERIFY (m.Find ("00", 0x100000, 0xFFFFF, 0, "-x"  ).empty());
	VERIFY (m.Find ("00", 0x100000, 0xFFFFF, 0, " p"  ).empty());

	VERIFY (m.Find ("00", 0x100000, 0xFFFFF, 0, "W x" ).empty());
	VERIFY (m.Find ("00", 0x100000, 0xFFFFF, 0, "-Xp" ).empty());
	VERIFY (m.Find ("00", 0x100000, 0xFFFFF, 0, " -g" ).empty());

	VERIFY (m.Find ("00", 0x100000, 0xFFFFF, 0, "W -p").empty());
	VERIFY (m.Find ("00", 0x100000, 0xFFFFF, 0, "-X g").empty());
	VERIFY (m.Find ("00", 0x100000, 0xFFFFF, 0, " -Py").empty());

	VERIFY (m.Find ("00", 0x100000, 0xFFFFF, 0, "    abc").empty());
	VERIFY (m.Find ("00", 0x100000, 0xFFFFF, 0, "----xyz").empty());
	VERIFY (m.Find ("00", 0x100000, 0xFFFFF, 0, "wxpg123").empty());
	VERIFY (m.Find ("00", 0x100000, 0xFFFFF, 0, "WXPG123").empty());

	//----------------------------------------------------------------------------//

	VERIFY (!m.IsCaching()); VERIFY (m.GetCacheSize() == 0);

	VERIFY (!m.CreateCache (    0,    0,     0));
	VERIFY (!m.CreateCache (12288,    0,     0));
	VERIFY (!m.CreateCache (12287,    0,     0));
	VERIFY (!m.CreateCache (    0, 4096,     0));
	VERIFY (!m.CreateCache (12288, 4096,     0));
	VERIFY (!m.CreateCache (12287, 4096,     0));
	VERIFY (!m.CreateCache (    0, 4095,     0));
	VERIFY (!m.CreateCache (12288, 4095,     0));
	VERIFY (!m.CreateCache (12287, 4095,     0));

	VERIFY (!m.CreateCache (    0,    0, 65536));
	VERIFY (!m.CreateCache (12288,    0, 65536));
	VERIFY (!m.CreateCache (12287,    0, 65536));
	VERIFY (!m.CreateCache (    0, 4096, 65536));
	VERIFY ( m.CreateCache (16384, 4096, 65536));
	VERIFY (!m.CreateCache (12288, 4096, 65536));
	VERIFY (!m.CreateCache (12287, 4096, 65536));
	VERIFY (!m.CreateCache (    0, 4095, 65536));
	VERIFY (!m.CreateCache (12288, 4095, 65536));
	VERIFY (!m.CreateCache (12287, 4095, 65536));

	VERIFY (!m.CreateCache (    0,    0, 65535));
	VERIFY (!m.CreateCache (12288,    0, 65535));
	VERIFY (!m.CreateCache (12287,    0, 65535));
	VERIFY (!m.CreateCache (    0, 4096, 65535));
	VERIFY (!m.CreateCache (12288, 4096, 65535));
	VERIFY (!m.CreateCache (12287, 4096, 65535));
	VERIFY (!m.CreateCache (    0, 4095, 65535));
	VERIFY (!m.CreateCache (12288, 4095, 65535));
	VERIFY (!m.CreateCache (12287, 4095, 65535));

	VERIFY (!m.CreateCache (    0,    0, 12288));
	VERIFY (!m.CreateCache (12288,    0, 12288));
	VERIFY (!m.CreateCache (12287,    0, 12288));
	VERIFY (!m.CreateCache (    0, 4096, 12288));
	VERIFY (!m.CreateCache (12288, 4096, 12288));
	VERIFY (!m.CreateCache (12287, 4096, 12288));
	VERIFY (!m.CreateCache (    0, 4095, 12288));
	VERIFY (!m.CreateCache (12288, 4095, 12288));
	VERIFY (!m.CreateCache (12287, 4095, 12288));

	VERIFY ( m.CreateCache (16384, 4096, 65536, 65536, 524288));
	VERIFY (!m.CreateCache (12288, 4096, 65536, 65536, 524288));
	VERIFY (!m.CreateCache (12288, 4096, 65536, 65536, 524287));
	VERIFY (!m.CreateCache (12288, 4096, 65536, 65535, 524288));
	VERIFY (!m.CreateCache (12288, 4096, 65536, 65535, 524287));

	VERIFY (!m.CreateCache (4096, 12288, 65536, 0, 0));

	VERIFY ( m.CreateCache (16384, 4096, 65536, 65536));
	VERIFY ( m.CreateCache (16384, 4096, 65536, 20480));
	VERIFY (!m.CreateCache (12288, 4096, 65536, 65536));
	VERIFY (!m.CreateCache (12288, 4096, 65536, 16384));
	VERIFY (!m.CreateCache (12288, 4096, 65536, 12288));

	VERIFY ( m.CreateCache (16384, 4096, 65536, 65536, 524288));
	VERIFY ( m.CreateCache (16384, 4096, 65536, 65536,  65536));
	VERIFY (!m.CreateCache (12288, 4096, 65536, 65536, 524288));
	VERIFY (!m.CreateCache (12288, 4096, 65536, 65536,  65536));
	VERIFY (!m.CreateCache (12288, 4096, 65536, 65536,  12288));

	VERIFY ( m.IsCaching()); VERIFY (m.GetCacheSize() == 65536);

	//----------------------------------------------------------------------------//

	uint8 data = 0;
	TEST_INVALID_RW (m, 0);
	TEST_INVALID_RW (m, 1);

	TEST_INVALID_RW (m, m.GetMinAddress()-1);
	TEST_INVALID_RW (m, m.GetMaxAddress()  );
	TEST_INVALID_RW (m, m.GetMaxAddress()+1);

	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestDebug (const Process& p)
{
	// Check if the parameter is this process
	bool local = Process::GetCurrent() == p;
	VERIFY ( p.IsValid   ());
	VERIFY (!p.IsDebugged());

	if (local)
		cout << "Attach a debugger to the current app";
	else
		cout << "Attach a debugger to the selected app";

	getchar(); VERIFY ( p.IsDebugged());

	if (local)
		cout << "Detach debugger from the current app\n";
	else
		cout << "Detach debugger from the selected app\n";

	getchar(); VERIFY (!p.IsDebugged());

	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestRegion (const Process& p)
{
	// Check if the parameter is this process
	bool local = Process::GetCurrent() == p;
	VERIFY (p.IsValid());

	if (local)
		cout << "Verify regions for the current app\n";
	else
		cout << "Verify regions for the selected app\n";

	Memory m (p); uintptr i, j;
	Memory::RegionList list1 = m.GetRegions();
	Memory::RegionList list2 = m.GetRegions
					(0x10000000, 0x80000000);

	cout << "Total regions: "
		 << list1.size() << " | Bounded: "
		 << list2.size() << "\n";

	VERIFY (is_sorted (ALL (list1)));
	VERIFY (is_sorted (ALL (list2)));

	VERIFY (unique (ALL (list1)) == list1.end());
	VERIFY (unique (ALL (list2)) == list2.end());

	// Print relevant info about the regions
	for (i = 0, j = 0; i < list1.size(); ++i)
	{
		VERIFY ( list1[i].Valid);
		VERIFY ( list1[i].Start >= m.GetMinAddress());
		VERIFY ( list1[i].Stop  <= m.GetMaxAddress());
		VERIFY ( list1[i].Start < list1[i].Stop);
		VERIFY ((list1[i].Start + list1[i].Size ) == list1[i].Stop );
		VERIFY ((list1[i].Stop  - list1[i].Start) == list1[i].Size );
		VERIFY ((list1[i].Stop  - list1[i].Size ) == list1[i].Start);

		if (list1[i].Bound)
		{
			cout << setfill ('0') << setw (12) << hex << list1[i].Start << " : "
				 << setfill ('0') << setw (12) << hex << list1[i].Stop  << " ("
				 << setfill ('0') << setw (12) << hex << list1[i].Size  << ") - "
				 << (list1[i].Readable   ? "R" : "-") << (list1[i].Writable ? "W" : "-")
				 << (list1[i].Executable ? "X" : "-") << (list1[i].Private  ? "P" : "S")
				 << " (0x" << setfill ('0') << setw (3) << hex << list1[i].Access << ") "
				 << (list1[i].Guarded ? "G " : "  ") << dec;
		}

		else
		{
			VERIFY (!list1[i].Readable  );
			VERIFY (!list1[i].Writable  ); VERIFY (!list1[i].Private);
			VERIFY (!list1[i].Executable); VERIFY (!list1[i].Guarded);

		#ifdef ROBOT_OS_WIN
			VERIFY (list1[i].Access == 1);
		#else
			VERIFY (list1[i].Access == 0);
		#endif

			cout << setfill ('0') << setw (12) << hex << list1[i].Start << " : "
				 << setfill ('0') << setw (12) << hex << list1[i].Stop  << " ("
				 << setfill ('0') << setw (12) << hex << list1[i].Size  << ") "
				 << "                 " << dec;
		}

	#ifdef ROBOT_OS_LINUX

		VERIFY (!m.SetAccess (list1[i], false, false, false));
		VERIFY (!m.SetAccess (list1[i], false, false, true ));
		VERIFY (!m.SetAccess (list1[i], false, true , false));
		VERIFY (!m.SetAccess (list1[i], false, true , true ));
		VERIFY (!m.SetAccess (list1[i], true , false, false));
		VERIFY (!m.SetAccess (list1[i], true , false, true ));
		VERIFY (!m.SetAccess (list1[i], true , true , false));
		VERIFY (!m.SetAccess (list1[i], true , true , true ));

		// This is unused on Linux
		VERIFY (!list1[i].Guarded);

	#endif
	#ifdef ROBOT_OS_MAC

		// This is unused on Mac
		VERIFY (!list1[i].Guarded);

	#endif
	#if defined (ROBOT_OS_MAC) || \
		defined (ROBOT_OS_WIN)

		// Attempt to change the regions protection value
		if ( list1[i].Readable   && !list1[i].Writable &&
			!list1[i].Executable && !list1[i].Guarded)
		{
			VERIFY (list1[i].Bound); cout << "M ";
			// Attempt to make the current region writable
			if (m.SetAccess (list1[i], true, true, false))
			{
				Memory::Region r = m.GetRegion (list1[i].Start);

				if (!r.Writable)
					cout << "Y ";
				else
					cout << "  ";

				VERIFY (m.SetAccess (list1[i], list1[i].Access));
				VERIFY (m.GetRegion (list1[i].Start) == list1[i]);
			}

			else cout << "X ";
		}

		else cout << "    ";

	#endif

		// Check if we hit the second list
		if (list1[i].Start < 0x80000000 &&
			list1[i].Stop  > 0x10000000)
		{
			VERIFY (j < list2.size());

			cout << "<< ";
			if (list1[i].Start != list2[j++].Start)
				cout << "X ";
		}

		cout << endl;
		if (i > 0) VERIFY (list1[i-1].Stop == list1[i].Start);
	}

	// If we matched all values
	VERIFY (j == list2.size());
	cout << "Press enter to continue\n";
	getchar();
	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestModules (const Process& p)
{
	// Check if the parameter is this process
	bool local = Process::GetCurrent() == p;
	VERIFY (p.IsValid());

	if (local)
		cout << "Verify module lists for the current app\n\n";
	else
		cout << "Verify module lists for the selected app\n\n";

#if defined (ROBOT_OS_LINUX) || \
	defined (ROBOT_OS_WIN  )

	// This result is unreliable on OSX
	VERIFY (p.GetModules ("*").empty());
	VERIFY (p.GetModules (")").empty());

#endif

	ModuleList list1 = p.GetModules (    );
	ModuleList list2 = p.GetModules (".*");
	cout << "List all - " << list1.size() << endl;

	VERIFY (list1.size() !=            0);
	VERIFY (list1.size() == list2.size());

	VERIFY (is_sorted (ALL (list1)));
	VERIFY (is_sorted (ALL (list2)));

	VERIFY (unique (ALL (list1)) == list1.end());
	VERIFY (unique (ALL (list2)) == list2.end());

	for (uintptr i = 0; i < list1.size(); ++i)
	{
		VERIFY (list1[i].IsValid());
		VERIFY (list2[i].IsValid());
		VERIFY (list1[i].GetBase());
		VERIFY (list1[i].GetProcess() == p);

		VERIFY ( (list1[i] == list2[i]));
		VERIFY (!(list1[i] != list2[i]));

		cout << "0x" << setfill ('0') << setw (12)
			 << hex << list1[i].GetBase() << " : "
			 << "0x" << setfill ('0') << setw (12)
			 << hex << list1[i].GetSize() << " - "
			 << list1[i].GetName() << dec << endl;

		Module::SegmentList segs1 =
			list1[i].GetSegments();
		Module::SegmentList segs2 =
			list2[i].GetSegments();

	#if defined (ROBOT_OS_LINUX) || \
		defined (ROBOT_OS_WIN  )

		VERIFY (list1[i].GetSize());
		VERIFY (segs1.empty     ());
		VERIFY (segs2.empty     ());

	#endif
	#ifdef ROBOT_OS_MAC

		VERIFY (segs1.size() == segs2.size());

		VERIFY (list1[i].GetSize() == 0);
		VERIFY (is_sorted (ALL (segs1)));
		VERIFY (is_sorted (ALL (segs2)));

		VERIFY (unique (ALL (segs1)) == segs1.end());
		VERIFY (unique (ALL (segs2)) == segs2.end());

		for (uintptr j = 0; j < segs1.size(); ++j)
		{
			VERIFY (segs1[j].Valid);
			VERIFY (segs2[j].Valid);
			VERIFY (segs1[j].Size );
			VERIFY (segs1[j].Name[0] != '\0');

			VERIFY ( (segs1[j] == segs2[j]));
			VERIFY (!(segs1[j] != segs2[j]));

			cout << "0x" << setfill ('0') << setw (12)
				 << hex << segs1[j].Base << " : "
				 << "0x" << setfill ('0') << setw (12)
				 << hex << segs1[j].Base + segs1[j].Size  << " ("
				 << "0x" << setfill ('0') << setw (12)
				 << hex << segs1[j].Size << ") - "
				 << segs1[j].Name << dec << endl;
		}

		cout << "------------------------------\n";

	#endif
	}

	cout << "Press enter to continue\n";
	getchar();

	list1 = p.GetModules (".*a.*");
	list2 = p.GetModules (".*A.*");
	cout << "List *a* - " << list1.size() << endl;
	VERIFY (list1.size() == list2.size());

	VERIFY (is_sorted (ALL (list1)));
	VERIFY (is_sorted (ALL (list2)));

	VERIFY (unique (ALL (list1)) == list1.end());
	VERIFY (unique (ALL (list2)) == list2.end());

	for (uintptr i = 0; i < list1.size(); ++i)
	{
		auto name = list1[i].GetName();
		VERIFY (name.find ('a') != string::npos ||
				name.find ('A') != string::npos);
	}

	cout << "Verified\n\n";
	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestStress (void)
{
	cout << "Stress test started\n";
	ProcessList pList = Process::GetList();
	for (uintptr p = 0; p < pList.size(); ++p)
	{
		// Process might have disappeared
		if (!pList[p].IsValid()) continue;

	#ifdef ROBOT_ARCH_32
		// Ensure architecture is correct
		if (pList[p].Is64Bit()) continue;
	#endif

		auto m = Memory (pList[p]);
		auto rList = m.GetRegions();

		VERIFY (is_sorted (ALL (rList)));
		VERIFY (unique (ALL (rList)) == rList.end());
		for (uintptr r = 0; r < rList.size(); ++r)
		{
			VERIFY ( rList[r].Valid);
			VERIFY ( rList[r].Start >= m.GetMinAddress());
			VERIFY ( rList[r].Stop  <= m.GetMaxAddress());
			VERIFY ( rList[r].Start < rList[r].Stop);
			VERIFY ((rList[r].Start + rList[r].Size ) == rList[r].Stop );
			VERIFY ((rList[r].Stop  - rList[r].Start) == rList[r].Size );
			VERIFY ((rList[r].Stop  - rList[r].Size ) == rList[r].Start);

			if (!rList[r].Bound)
			{
				VERIFY (!rList[r].Readable  );
				VERIFY (!rList[r].Writable  ); VERIFY (!rList[r].Private);
				VERIFY (!rList[r].Executable); VERIFY (!rList[r].Guarded);

			#ifdef ROBOT_OS_WIN
				VERIFY (rList[r].Access == 1);
			#else
				VERIFY (rList[r].Access == 0);
			#endif
			}

		#ifdef ROBOT_OS_LINUX
			VERIFY (rList[r] == m.GetRegion (rList[r].Start));
		#endif

			if (r > 0) VERIFY (rList[r-1].Stop == rList[r].Start);
		}

		ModuleList mList = pList[p].GetModules();

		VERIFY (is_sorted (ALL (mList)));
		VERIFY (unique (ALL (mList)) == mList.end());
		for (uintptr m = 0; m < mList.size(); ++m)
		{
			VERIFY (mList[m].IsValid());
			VERIFY (mList[m].GetBase());
			VERIFY (mList[m].GetProcess() == pList[p]);

			Module::SegmentList sList =
				mList[m].GetSegments();

			#if defined (ROBOT_OS_LINUX) || \
				defined (ROBOT_OS_WIN  )

				VERIFY (mList[m].GetSize());
				VERIFY (sList.empty     ());

			#endif
			#ifdef ROBOT_OS_MAC

				VERIFY (mList[m].GetSize() == 0);
				VERIFY (is_sorted (ALL (sList)));
				VERIFY (unique (ALL (sList)) == sList.end());
				for (uintptr s = 0; s < sList.size(); ++s)
				{
					VERIFY (sList[s].Valid);
					VERIFY (sList[s].Name[0] != '\0');

					if (sList[s].Size == 0)
					{
						// Probably just a __RESTRICT segment
						cout << "Found a size zero segment: "
							 << sList[s].Name << " p: " << p
							 << " pid: " << pList[p].GetPID ()
							 << " m: "   << mList[m].GetName()
							 << endl;
					}
				}

			#endif
		}
	}

	cout << "Stress test finished\n\n";
	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestRW (const Process& p, Memory* mem = nullptr)
{
	// Attempt to retrieve the main peon module
	ModuleList list = p.GetModules (".*peon.*");
	VERIFY (list.size() == 1);

#if defined (ROBOT_OS_MAC) ||\
	defined (ROBOT_OS_WIN)

	// Compute offset using module base
	uintptr module = list[0].GetBase();

#endif
#if defined (ROBOT_OS_LINUX)

	// Always absolute
	uintptr module = 0;

#endif

	bool is64Bit = p.Is64Bit ();
	uintptr l = is64Bit ? 8 : 4;

	Memory m (p);
	if (mem == nullptr) mem = &m;
	auto f = Memory::SkipErrors;
	VERIFY (l == mem->GetPtrSize());

	//----------------------------------------------------------------------------//

	 int16 vInt16  = 0;
	 int32 vInt32  = 0;
	 int64 vInt64  = 0;

	real32 vReal32 = 0;
	real64 vReal64 = 0;

	uintptr vPtr1  = 0;
	uintptr vPtr2  = 0;
	uintptr vPtr3  = 0;

	 int32 vArray [4] = { 0 };
	real32 vVector[4] = { 0 };

	//----------------------------------------------------------------------------//

	VERIFY (mem->ReadData (module +  PINT16, &vInt16,  2, f) == 2); VERIFY (vInt16 == 100);
	VERIFY (mem->ReadData (module +  PINT32, &vInt32,  4   ) == 4); VERIFY (vInt32 == 200);
	VERIFY (mem->ReadData (module +  PINT64, &vInt64,  8   ) == 8); VERIFY (vInt64 == 300);

	VERIFY (mem->ReadData (module + PREAL32, &vReal32, 4) == 4); VERIFY (vReal32 == 400);
	VERIFY (mem->ReadData (module + PREAL64, &vReal64, 8) == 8); VERIFY (vReal64 == 500);

	VERIFY (mem->ReadData (module +  PPTR16, &vPtr1,   l) == l); VERIFY (vPtr1 == module + PINT16);
	VERIFY (mem->ReadData (module +  PPTR32, &vPtr2,   l) == l); VERIFY (vPtr2 == module + PINT32);
	VERIFY (mem->ReadData (module +  PPTR64, &vPtr3,   l) == l); VERIFY (vPtr3 == module + PINT64);

	VERIFY (mem->ReadData (vPtr1, &vInt16, 2) == 2); VERIFY (vInt16 == 100);
	VERIFY (mem->ReadData (vPtr2, &vInt32, 4) == 4); VERIFY (vInt32 == 200);
	VERIFY (mem->ReadData (vPtr3, &vInt64, 8) == 8); VERIFY (vInt64 == 300);

	VERIFY (mem->ReadData (module + PARRAY, &vPtr1,  l) ==  l);
	VERIFY (mem->ReadData (          vPtr1, vArray, 16) == 16);
	VERIFY (vArray[0] == 0); VERIFY (vArray[1] == 1);
	VERIFY (vArray[2] == 2); VERIFY (vArray[3] == 3);

	VERIFY (mem->ReadData (module + PFINITE, vArray, 16) == 16);
	VERIFY (vArray[0] == 10); VERIFY (vArray[1] == 20);
	VERIFY (vArray[2] == 30); VERIFY (vArray[3] ==  0);

	VERIFY (mem->ReadData (module + PVECTOR, vVector, 16) == 16);
	VERIFY (vVector[0] == 1.75f); VERIFY (vVector[1] == 2.75f);
	VERIFY (vVector[2] == 3.75f); VERIFY (vVector[3] == 4.75f);

	//----------------------------------------------------------------------------//

	vReal32 = 1.5f;
	vReal64 = 2.5;

	vPtr2 = module + PFINITE;

	vArray[0] = 123;
	vArray[1] = 456;
	vArray[2] = 789;
	vArray[3] =   0;

	VERIFY (mem->WriteData (module + PREAL32, &vReal32, 4) ==  4);
	VERIFY (mem->WriteData (module + PREAL64, &vReal64, 8) ==  8);
	VERIFY (mem->WriteData (module +  PPTR32, &vPtr2,   l) ==  l);
	VERIFY (mem->WriteData (module + PFINITE, vArray,  16) == 16);

	cout << "Verify Peon values and press enter"; getchar();

	//----------------------------------------------------------------------------//

	vReal32 = 0;
	vReal64 = 0;
	vPtr2   = 0;

	vArray[0] = 0;
	vArray[1] = 0;
	vArray[2] = 0;
	vArray[3] = 0;

	mem->ClearCache();
	VERIFY (mem->ReadData (module + PREAL32, &vReal32, 4, f) == 4); VERIFY (vReal32 == 1.5f);
	VERIFY (mem->ReadData (module + PREAL64, &vReal64, 8   ) == 8); VERIFY (vReal64 == 2.5 );

	VERIFY (mem->ReadData (module +  PPTR32, &vPtr2,   l) == l);
	VERIFY (vPtr2 == module + PFINITE);

	VERIFY (mem->ReadData (vPtr2, vArray, 16) == 16);
	VERIFY (vArray[0] == 123); VERIFY (vArray[1] == 456);
	VERIFY (vArray[2] == 789); VERIFY (vArray[3] ==   0);

	cout << "Reset  Peon values and press enter"; getchar();

	//----------------------------------------------------------------------------//

	if (!mem->IsCaching())
	{
		auto stats1 = mem->GetStats (true);
		VERIFY (stats1.SystemReads  != 0);
		VERIFY (stats1.CachedReads  == 0);
		VERIFY (stats1.SystemWrites != 0);
		VERIFY (stats1.AccessWrites == 0);
		VERIFY (stats1.ReadErrors   == 0);
		VERIFY (stats1.WriteErrors  == 0);

		auto stats2 = mem->GetStats();
		VERIFY (stats2.SystemReads  == 0);
		VERIFY (stats2.CachedReads  == 0);
		VERIFY (stats2.SystemWrites == 0);
		VERIFY (stats2.AccessWrites == 0);
		VERIFY (stats2.ReadErrors   == 0);
		VERIFY (stats2.WriteErrors  == 0);
		VERIFY (!m.GetCacheSize());
	}

	cout << endl;
	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestCache (const Process& p)
{
	Memory m (p);
	// Create 8K cache with 8K reads and 16K Enlarge
	VERIFY (m.CreateCache (4096, 4096, 8192, 16384));
	VERIFY (m.IsCaching());

	// Call ReadWrite test from before
	if (!TestRW (p, &m)) return false;

	auto stats1 = m.GetStats (true);
	VERIFY (stats1.SystemReads  != 0);
	VERIFY (stats1.CachedReads  != 0);
	VERIFY (stats1.SystemWrites != 0);
	VERIFY (stats1.AccessWrites == 0);
	VERIFY (stats1.ReadErrors   == 0);
	VERIFY (stats1.WriteErrors  == 0);
	VERIFY (m.GetCacheSize() == 24576);

	// No more cache
	m.DeleteCache();
	VERIFY (!m.IsCaching   ());
	VERIFY (!m.GetCacheSize());

	// Call ReadWrite test from before
	if (!TestRW (p, &m)) return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestFlags (const Process& p)
{
	Memory m (p);

	bool test1 = false; // R Zero
	bool test2 = false; // W Zero
	bool test3 = false; // R Auto
	bool test4 = false; // W Auto
	bool test5 = false; // R Auto + Fail
	bool test6 = false; // W Auto + Fail
	bool test7 = false; // R Auto (Alt)
	bool test8 = false; // W Auto (Alt)

#ifdef ROBOT_OS_LINUX
	test3 = true;
	test4 = true;
	test7 = true;
	test8 = true;
#endif

	// Returns the type of data contained within the array
	// 0 = All zeroes | 1 = All ones | 2 = Just random data
	auto GetType = [] (uint8* data, uintptr size) -> uint8
	{
		uint8 average = data[0];
		// Iterate through the entire array
		for (uintptr i = 1; i < size; ++i)
		{
			if (data[i] == 0 && average == 0) continue;
			if (data[i] == 1 && average == 1) continue;
			return 2;
		}

		return average;
	};

	uintptr size = 1048576;
	// Initially allocate single MB
	uint8*  data = new uint8[size];

	// Try and find a good series of regions
	Memory::RegionList list = m.GetRegions();
	for (uintptr i = 0; i < list.size(); ++i)
	{
		// Reached end of list
		if (i+1 >= list.size())
		{
			if (!test1)
				cout << "Warning: Flags test 1 was not completed!\n";
			if (!test2)
				cout << "Warning: Flags test 2 was not completed!\n";
			if (!test3 && !test7)
				cout << "Warning: Flags test 3 was not completed!\n";
			if (!test4 && !test8)
				cout << "Warning: Flags test 4 was not completed!\n";
			if (!test5)
				cout << "Warning: Flags test 5 was not completed!\n";
			if (!test6)
				cout << "Warning: Flags test 6 was not completed!\n";
			break;
		}

		const auto& r1 = list[i + 0];
		const auto& r2 = list[i + 1];
		uintptr len = r1.Size + r2.Size;

		// Keep sizes under 64 MB
		if (r1.Size > 67108864 ||
			r2.Size > 67108864)
			continue;

		if (size < len)
		{
			// Multiply by two until
			// the length is reached
			while ((size *= 2) < len);

			delete[] data;
			// Reallocate the buffer
			data = new uint8[size];
		}

		// Do flagged memory read tests
		if ( r1.Readable &&  r1.Writable && !r1.Guarded &&
			!r2.Readable && !r2.Writable && !r2.Guarded)
		{
			// Check if tested
			if (test1 == false)
			{
				memset (data, 1, len);
				VERIFY (m.ReadData (r1.Start, data, len, Memory::SkipErrors) == len);
				if (GetType (data +       0, r1.Size) == 2 &&
					GetType (data + r1.Size, r2.Size) == 0)
					test1 = true;
			}

			// Skip if test already done
			if (test3 && test5) continue;

			// Attempt to set access of the region
			if (m.SetAccess (r2, true, false, false))
			{
				// Restore previous access
				m.SetAccess (r2, r2.Access);

				// Skip if already succeeded
				if (test3 == true) continue;

				memset (data, 1, len);
				VERIFY (m.ReadData (r1.Start, data, len, Memory::AutoAccess) == len);
				if (GetType (data +       0, r1.Size) == 2 &&
					GetType (data + r1.Size, r2.Size) == 2)
					test3 = true;
			}

			else
			{
				// Skip if already succeeded
				if (test5 == true) continue;

				memset (data, 1, len);
				VERIFY (m.ReadData (r1.Start, data, len, Memory::AutoAccess) == len);
				if (GetType (data +       0, r1.Size) == 2 &&
					GetType (data + r1.Size, r2.Size) == 0)
					test5 = true;
			}
		}

		// Do flagged memory write tests
		if (r1.Readable &&  r1.Writable && !r1.Guarded &&
			r2.Readable && !r2.Writable && !r2.Guarded)
		{
			// Check if tested
			if (test2 == false)
			{
				uint32 value = 0xAABBCCDD;
				VERIFY (m.WriteData (r2.Start-2, &value, 4, Memory::SkipErrors) == 4);
				VERIFY (m. ReadData (r2.Start-2, &value, 4, Memory::SkipErrors) == 4);
				VERIFY ((value & 0x0000FFFF) == 0x0000CCDD);
				VERIFY ((value & 0xFFFFFFFF) != 0xAABBCCDD);
				test2 = true;
			}

			// Skip if test already done
			if (test4 && test6) continue;

			// Attempt to set access of the region
			if (m.SetAccess (r2, true, true, false))
			{
				// Restore previous access
				m.SetAccess (r2, r2.Access);

				// Skip if already succeeded
				if (test4 == true) continue;

				uint32 value = 0xAABBCCDD;
				VERIFY (m.WriteData (r2.Start-2, &value, 4, Memory::AutoAccess) == 4);
				VERIFY (m. ReadData (r2.Start-2, &value, 4, Memory::AutoAccess) == 4);
				VERIFY (value == 0xAABBCCDD);
				test4 = true;
			}

			else
			{
				// Skip if already succeeded
				if (test6 == true) continue;

				uint32 value = 0xAABBCCDD;
				VERIFY (m.WriteData (r2.Start-2, &value, 4, Memory::AutoAccess) == 4);
				VERIFY (m. ReadData (r2.Start-2, &value, 4, Memory::AutoAccess) == 4);
				VERIFY ((value & 0x0000FFFF) == 0x0000CCDD);
				VERIFY ((value & 0xFFFFFFFF) != 0xAABBCCDD);
				test6 = true;
			}
		}

		// Do flagged memory access test
		if (r1.Readable && r1.Writable && !r1.Guarded &&
			r2.Readable && r2.Writable && !r2.Guarded)
		{
			// Attempt to set access of the region if not tested
			if (!test7 && m.SetAccess (r2, false, false, false))
			{
				memset (data, 1, len);
				VERIFY (m.ReadData (r1.Start, data, len, Memory::AutoAccess) == len);
				if (GetType (data +       0, r1.Size) == 2 &&
					GetType (data + r1.Size, r2.Size) == 2)
					test7 = true;

				// Restore previous access
				m.SetAccess (r2, r2.Access);
			}

			// Attempt to set access of the region if not tested
			if (!test8 && m.SetAccess (r2, true, false, false))
			{
				uint32 value = 0xAABBCCDD;
				VERIFY (m.WriteData (r2.Start-2, &value, 4, Memory::AutoAccess) == 4);
				VERIFY (m. ReadData (r2.Start-2, &value, 4, Memory::AutoAccess) == 4);
				VERIFY (value == 0xAABBCCDD);
				test8 = true;

				// Restore previous access
				m.SetAccess (r2, r2.Access);
			}
		}
	}

	delete[] data;
	return true;
}

////////////////////////////////////////////////////////////////////////////////

static bool TestFind (const Process& p)
{
	// Attempt to retrieve the main peon module
	ModuleList list = p.GetModules (".*peon.*");
	VERIFY (list.size() == 1);

#if defined (ROBOT_OS_MAC) ||\
	defined (ROBOT_OS_WIN)

	// Compute offset using module base
	uintptr module = list[0].GetBase();

#endif
#if defined (ROBOT_OS_LINUX)

	// Always absolute
	uintptr module = 0;

#endif

	bool is64Bit = p.Is64Bit ();
	uintptr l = is64Bit ? 8 : 4;
	 Memory m = Memory (p);
	VERIFY (l == m.GetPtrSize());

#ifdef ROBOT_OS_LINUX

	// To find a correct value within an address list
	auto finder = [] (AddressList list, uintptr value)
	{
		uintptr i;
		for (i = 0; i < list.size(); ++i)
			if (list[i] == value) break;
		return i < list.size();
	};

#endif

	//----------------------------------------------------------------------------//

	uintptr vPat3 = 0; VERIFY (m.ReadData (module + PPAT3, &vPat3, l) == l);
	uintptr vPat4 = 0; VERIFY (m.ReadData (module + PPAT4, &vPat4, l) == l);

	AddressList pat, list1, list2;
	pat.push_back (module + PPAT1);
	pat.push_back (module + PPAT2);
	pat.push_back (module + PPAT5);
	pat.push_back (module + PPAT6);
	pat.push_back (vPat3);
	pat.push_back (vPat4); sort (ALL (pat));

	//----------------------------------------------------------------------------//

	list1 = m.Find ("04 08 15 16 23 42");
	list2 = m.Find ("040815     162342");
	VERIFY (list1 == list2); sort (ALL (list1));

#ifdef ROBOT_OS_LINUX

	VERIFY (list1.size() >= 6);
	VERIFY (list2.size() >= 6);

	VERIFY (finder (list1, pat[0] + 0x00));
	VERIFY (finder (list1, pat[1] + 0x12));
	VERIFY (finder (list1, pat[2] + 0x00));
	VERIFY (finder (list1, pat[3] + 0x12));
	VERIFY (finder (list1, pat[4] + 0x00));
	VERIFY (finder (list1, pat[5] + 0x12));

#else

	VERIFY (list1.size() == 6);
	VERIFY (list2.size() == 6);

	VERIFY (list1[0] == pat[0] + 0x00);
	VERIFY (list1[1] == pat[1] + 0x12);
	VERIFY (list1[2] == pat[2] + 0x00);
	VERIFY (list1[3] == pat[3] + 0x12);
	VERIFY (list1[4] == pat[4] + 0x00);
	VERIFY (list1[5] == pat[5] + 0x12);

#endif

	list1 = m.Find ("0408  0F10  172A");
	list2 = m.Find ("04  080f1017  2a");
	VERIFY (list1 == list2); sort (ALL (list1));

#ifdef ROBOT_OS_LINUX

	VERIFY (list1.size() >= 6);
	VERIFY (list2.size() >= 6);

	VERIFY (finder (list1, pat[0] + 0x12));
	VERIFY (finder (list1, pat[1] + 0x00));
	VERIFY (finder (list1, pat[2] + 0x12));
	VERIFY (finder (list1, pat[3] + 0x00));
	VERIFY (finder (list1, pat[4] + 0x12));
	VERIFY (finder (list1, pat[5] + 0x00));

#else

	VERIFY (list1.size() == 6);
	VERIFY (list2.size() == 6);

	VERIFY (list1[0] == pat[0] + 0x12);
	VERIFY (list1[1] == pat[1] + 0x00);
	VERIFY (list1[2] == pat[2] + 0x12);
	VERIFY (list1[3] == pat[3] + 0x00);
	VERIFY (list1[4] == pat[4] + 0x12);
	VERIFY (list1[5] == pat[5] + 0x00);

#endif

	//----------------------------------------------------------------------------//

	list1 = m.Find ("04 08  ? 16 23 42");
	list2 = m.Find ("04 08 15  ? 23 42");
	VERIFY (list1 == list2); sort (ALL (list1));

#ifdef ROBOT_OS_LINUX

	VERIFY (list1.size() >= 6);
	VERIFY (list2.size() >= 6);

	VERIFY (finder (list1, pat[0] + 0x00));
	VERIFY (finder (list1, pat[1] + 0x12));
	VERIFY (finder (list1, pat[2] + 0x00));
	VERIFY (finder (list1, pat[3] + 0x12));
	VERIFY (finder (list1, pat[4] + 0x00));
	VERIFY (finder (list1, pat[5] + 0x12));

#else

	VERIFY (list1.size() == 6);
	VERIFY (list2.size() == 6);

	VERIFY (list1[0] == pat[0] + 0x00);
	VERIFY (list1[1] == pat[1] + 0x12);
	VERIFY (list1[2] == pat[2] + 0x00);
	VERIFY (list1[3] == pat[3] + 0x12);
	VERIFY (list1[4] == pat[4] + 0x00);
	VERIFY (list1[5] == pat[5] + 0x12);

#endif

	list1 = m.Find ("04  ? 0F 10 17 2A");
	list2 = m.Find ("04 08 0f 10  ? 2a");
	VERIFY (list1 == list2); sort (ALL (list1));

#ifdef ROBOT_OS_LINUX

	VERIFY (list1.size() >= 6);
	VERIFY (list2.size() >= 6);

	VERIFY (finder (list1, pat[0] + 0x12));
	VERIFY (finder (list1, pat[1] + 0x00));
	VERIFY (finder (list1, pat[2] + 0x12));
	VERIFY (finder (list1, pat[3] + 0x00));
	VERIFY (finder (list1, pat[4] + 0x12));
	VERIFY (finder (list1, pat[5] + 0x00));

#else

	VERIFY (list1.size() == 6);
	VERIFY (list2.size() == 6);

	VERIFY (list1[0] == pat[0] + 0x12);
	VERIFY (list1[1] == pat[1] + 0x00);
	VERIFY (list1[2] == pat[2] + 0x12);
	VERIFY (list1[3] == pat[3] + 0x00);
	VERIFY (list1[4] == pat[4] + 0x12);
	VERIFY (list1[5] == pat[5] + 0x00);

#endif

	//----------------------------------------------------------------------------//

	list1 = m.Find ("04 08 ? ? ? ? DE AD");
	list2 = m.Find ("04 08 ? ? ? ?  ?  ?");
	VERIFY (list1 != list2); sort (ALL (list1));

#ifdef ROBOT_OS_LINUX

	VERIFY (list1.size() >=  6);
	VERIFY (list2.size() >  12);

	VERIFY (finder (list1, pat[0] + 0x00));
	VERIFY (finder (list1, pat[1] + 0x00));
	VERIFY (finder (list1, pat[2] + 0x00));
	VERIFY (finder (list1, pat[3] + 0x00));
	VERIFY (finder (list1, pat[4] + 0x00));
	VERIFY (finder (list1, pat[5] + 0x00));

#else

	VERIFY (list1.size() ==  6);
	VERIFY (list2.size() >  12);

	VERIFY (list1[0] == pat[0] + 0x00);
	VERIFY (list1[1] == pat[1] + 0x00);
	VERIFY (list1[2] == pat[2] + 0x00);
	VERIFY (list1[3] == pat[3] + 0x00);
	VERIFY (list1[4] == pat[4] + 0x00);
	VERIFY (list1[5] == pat[5] + 0x00);

#endif

	//----------------------------------------------------------------------------//

	list1 = m.Find ("04 08 15 16 23 42", 0, -1, 8, "w"); VERIFY (list1.size() ==  2);
	list1 = m.Find ("04 08",             0, -1, 12    ); VERIFY (list1.size() == 12);
	list2 = m.Find ("04 08",             0, -1, 12, ""); VERIFY (list2.size() == 12);
	VERIFY (list1 == list2);

#if defined (ROBOT_OS_LINUX) ||\
	defined (ROBOT_OS_WIN  )

	uintptr mS = list[0].GetBase();
	uintptr mT = list[0].GetBase() +
				 list[0].GetSize();

#endif
#if defined (ROBOT_OS_MAC)

	auto segs = list[0].GetSegments();
	VERIFY (segs.size() >= 2);
	uintptr mS = segs[1].Base;
	uintptr mT = segs[1].Base +
				 segs[1].Size;

#endif

	list1 = m.Find ("00", mS, mT);
	VERIFY (list1.size() != 0); sort (ALL (list1));
	VERIFY (list1.front() >= mS);
	VERIFY (list1.back () <= mT);

	return true;
}

////////////////////////////////////////////////////////////////////////////////
// This section can be used to write miscellaneous tests on other applications.

static bool TestMisc (void)
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////

bool TestMemory (void)
{
	cout << "BEGIN MEMORY TESTING\n------------------------------\n";

	cout << "Warning: Some set of tests cannot be automated\n"
		 << "         Please execute the following commands\n\n";

	cout << uppercase;
	char input[32] = { 0 }; Process p1 = Process::GetCurrent(), p2;
	cout << "Open a program and input PID: ";  cin.getline (input, 32);
	int32 pid = atoi (input); VERIFY (pid > 0); VERIFY (p2.Open (pid));
	VERIFY (p1.IsValid()); VERIFY (!p1.HasExited());
	VERIFY (p2.IsValid()); VERIFY (!p2.HasExited()); cout << endl;

#ifdef ROBOT_ARCH_32
	VERIFY (!p2.Is64Bit()); // We don't support 64-Bit targets through 32-Bit
#endif

	if (!TestInvalid (  )) { cout << ">> Invalid Failed   \n\n"; return false; }
	if (!TestEquals  (  )) { cout << ">> Equals Failed    \n\n"; return false; }
	if (!TestParams  (  )) { cout << ">> Params Failed    \n\n"; return false; }
	if (!TestDebug   (p1)) { cout << ">> Debug p1 Failed  \n\n"; return false; }
	if (!TestDebug   (p2)) { cout << ">> Debug p2 Failed  \n\n"; return false; }
	if (!TestRegion  (p1)) { cout << ">> Region p1 Failed \n\n"; return false; }
	if (!TestRegion  (p2)) { cout << ">> Region p2 Failed \n\n"; return false; }
	if (!TestModules (p1)) { cout << ">> Modules p1 Failed\n\n"; return false; }
	if (!TestModules (p2)) { cout << ">> Modules p2 Failed\n\n"; return false; }
	if (!TestStress  (  )) { cout << ">> Modules p2 Failed\n\n"; return false; }

	if (p2.GetName().substr (0, 4) != "Peon")
	{
		cout << "Open peon app and input PID: "; cin.getline (input, 32);
		int32 pid = atoi (input); VERIFY (pid > 0); VERIFY (p2.Open (pid));
		VERIFY (p2.IsValid()); VERIFY (!p2.HasExited());
		VERIFY (p2.GetName().substr (0, 4) == "Peon"); cout << endl;
	}

#ifdef ROBOT_ARCH_32
	VERIFY (!p2.Is64Bit()); // We don't support 64-Bit targets through 32-Bit
#endif

	if (!TestRW    (p2)) { cout << ">> RW Failed   \n\n"; return false; }
	if (!TestCache (p2)) { cout << ">> Cache Failed\n\n"; return false; }
	if (!TestFlags (p2)) { cout << ">> Flags Failed\n\n"; return false; }
	if (!TestFind  (p2)) { cout << ">> Find Failed \n\n"; return false; }
	if (!TestMisc  (  )) { cout << ">> Misc Failed \n\n"; return false; }

	cout << nouppercase;
	cout << ">> Success\n\n"; return true;
}
