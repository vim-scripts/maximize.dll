/*
 * maximize.cpp
 * maximizing tool for Win32 gVim - source
 *
 * made by kAtremer <katremer@yandex.ru>
 * Last changed: 2007 Oct 16
 *
 *
 * COMPILATION
 * use MS Visual C++ (tested with 6.0 and 2005 aka 8.0)
 * build with cl /LD maximize.cpp user32.lib kernel32.lib
 *
 *
 * USAGE
 * call with libcallnr('maximize', 'Maximize', 1)
 * or libcallnr('maximize', 'Maximize', 0)
 */

#include <windows.h>

// so, we begin with no pragmas and a 28K DLL

// then this comes in:
#pragma comment(linker, "/ENTRY:DllMain")
// 20 KB
// the option just points to a function to use as an entry point
// instead of a regular startup that initializes
// memory manager and a whole bunch of other stuff

// it's strange it works, /NODEFAULTLIB is supposed to accompany it:
//#pragma comment(linker, "/NODEFAULTLIB")
// but it gives the same 20 KB
// it seems that, for a DLL, /ENTRY is enough

// then, we introduce the wonderful /FILEALIGN:
#pragma comment(linker, "/FILEALIGN:512")
// 4.5 KB
// it tells the compiler to pack code and data tighter:
// align on 512 Bytes instead of the default 4KB

// there're also these babies, but dunno what they're for
//#pragma comment(linker, "/ALIGN:512")
//#pragma comment(linker, "/OPT:NOWIN98")
// OPT:NOWIN98 seems to be undocumented,
// "seems" because the info is from the TfrEs article:
// I don't want to mess with big, rather big and very huge indeed MSDN

// /IGNORE, like, disables a warning
// dunno which 4078 is
//#pragma comment(linker, "/IGNORE:4078")

// these lines must merge several executable sections into one
// but they give 6.0 KB DLL, 4.5 KB without them
// don't know why, I'm not deep into PE file structure, especially DLLs
// not even sure the resulting DLL will work everywhere
//#pragma comment(linker, "/MERGE:.rdata=.text")
//#pragma comment(linker, "/MERGE:.data=.text")
//#pragma comment(linker, "/MERGE:.reloc=.text")
//#pragma comment(linker, "/SECTION:.text,EWRX")

// this tells the compiler to produce a Win32 executable
// just changes several bits
//#pragma comment(linker, "/SUBSYSTEM:console")

// this may be needed for EXEs, DLL seems to work well without it
//#pragma comment(linker, "/STACK:65536,65536")

BOOL APIENTRY DllMain(HANDLE hModule, DWORD reason, LPVOID lpReserved) {
	return TRUE;
}

#define VIMCLASSNAMEBUFSIZE 4
const char vimclassname[]="Vim";
char vimclassnamebuf[VIMCLASSNAMEBUFSIZE];

BOOL CALLBACK EnumThreadWndProc(HWND hwnd, LPARAM lParam) {
	HWND* vimwin=(HWND*)lParam;
	if (GetParent(hwnd)) {
		*vimwin=NULL;
		return TRUE;
	} else {
		if (GetClassName(hwnd, vimclassnamebuf, VIMCLASSNAMEBUFSIZE)==0) {
			*vimwin=NULL;
			return TRUE;
		} else {
			for (int i=0; i<VIMCLASSNAMEBUFSIZE; i++) {
				if (vimclassnamebuf[i]!=vimclassname[i]) {
					*vimwin=NULL;
					return TRUE;
				}
			}
			*vimwin=hwnd;
			return FALSE;
		}
	}
}

extern "C" __declspec(dllexport) LONG Maximize(LONG param) {
	HWND vimwin;
	DWORD threadid=GetCurrentThreadId();
	EnumThreadWindows(threadid, EnumThreadWndProc, (LPARAM) &vimwin);
	if (vimwin!=NULL) {
		if (param==1) ShowWindow(vimwin, SW_MAXIMIZE);
		else if (param==0) ShowWindow(vimwin, SW_RESTORE);
		return 1;
	} else {
		return 0;
	}

}

const char hhgttg[]=
"\x19\x3e\x66\x66\x5b\x17\x58\x5d\x6b\x5c\x69\x65\x66\x66\x65\x17\x59\x66"
"\x70\x6a\x25\x19\x01\x4b\x5f\x5c\x17\x6d\x66\x60\x5a\x5c\x17\x6e\x58\x6a"
"\x17\x66\x5b\x5b\x63\x70\x17\x5d\x58\x64\x60\x63\x60\x58\x69\x23\x17\x59"
"\x6c\x6b\x17\x66\x5b\x5b\x63\x70\x17\x5b\x60\x5d\x5d\x5c\x69\x5c\x65\x6b"
"\x25\x17\x40\x6b\x17\x5f\x58\x5b\x17\x58\x17\x64\x58\x6b\x69\x60\x58\x69"
"\x5a\x5f\x58\x63\x17\x6b\x6e\x58\x65\x5e\x25\x17\x40\x6b\x17\x58\x65\x65"
"\x66\x6c\x65\x5a\x5c\x5b\x17\x60\x6b\x6a\x5c\x63\x5d\x17\x6b\x66\x17\x6b"
"\x5f\x5c\x17\x5a\x69\x5c\x6e\x17\x58\x6a\x17\x6b\x5f\x5c\x70\x17\x58\x69"
"\x69\x60\x6d\x5c\x5b\x17\x58\x6b\x17\x6b\x5f\x5c\x17\x58\x60\x69\x63\x66"
"\x5a\x62\x17\x5f\x58\x6b\x5a\x5f\x6e\x58\x70\x17\x6b\x5f\x58\x6b\x17\x6e"
"\x66\x6c\x63\x5b\x17\x63\x5c\x6b\x17\x6b\x5f\x5c\x64\x17\x66\x6c\x6b\x17"
"\x66\x65\x17\x6b\x5f\x5c\x17\x67\x63\x58\x65\x5c\x6b\x17\x6a\x6c\x69\x5d"
"\x58\x5a\x5c\x25\x01\x4b\x5f\x5c\x70\x17\x63\x66\x66\x62\x5c\x5b\x17\x58"
"\x6b\x17\x5c\x58\x5a\x5f\x17\x66\x6b\x5f\x5c\x69\x17\x60\x65\x17\x67\x6c"
"\x71\x71\x63\x5c\x64\x5c\x65\x6b\x25\x01\x19\x40\x6b\x1e\x6a\x17\x6b\x5f"
"\x5c\x17\x5a\x66\x64\x67\x6c\x6b\x5c\x69\x23\x19\x17\x5c\x6f\x67\x63\x58"
"\x60\x65\x5c\x5b\x17\x51\x58\x67\x5f\x66\x5b\x25\x17\x19\x40\x17\x5b\x60"
"\x6a\x5a\x66\x6d\x5c\x69\x5c\x5b\x17\x60\x6b\x17\x5f\x58\x5b\x17\x58\x65"
"\x17\x5c\x64\x5c\x69\x5e\x5c\x65\x5a\x70\x17\x59\x58\x5a\x62\x24\x6c\x67"
"\x17\x67\x5c\x69\x6a\x66\x65\x58\x63\x60\x6b\x70\x17\x6b\x5f\x58\x6b\x17"
"\x40\x17\x6b\x5f\x66\x6c\x5e\x5f\x6b\x17\x64\x60\x5e\x5f\x6b\x17\x6e\x66"
"\x69\x62\x17\x66\x6c\x6b\x17\x59\x5c\x6b\x6b\x5c\x69\x25\x19\x01\x19\x45"
"\x66\x6e\x17\x6b\x5f\x60\x6a\x17\x60\x6a\x17\x5e\x66\x60\x65\x5e\x17\x6b"
"\x66\x17\x59\x5c\x17\x70\x66\x6c\x69\x17\x5d\x60\x69\x6a\x6b\x17\x5b\x58"
"\x70\x17\x66\x6c\x6b\x17\x66\x65\x17\x58\x17\x6a\x6b\x69\x58\x65\x5e\x5c"
"\x17\x65\x5c\x6e\x17\x67\x63\x58\x65\x5c\x6b\x23\x19\x17\x5a\x66\x65\x6b"
"\x60\x65\x6c\x5c\x5b\x17\x3c\x5b\x5b\x60\x5c\x1e\x6a\x17\x65\x5c\x6e\x17"
"\x6d\x66\x60\x5a\x5c\x23\x17\x19\x6a\x66\x17\x40\x17\x6e\x58\x65\x6b\x17"
"\x70\x66\x6c\x17\x58\x63\x63\x17\x6e\x69\x58\x67\x67\x5c\x5b\x17\x6c\x67"
"\x17\x6a\x65\x6c\x5e\x17\x58\x65\x5b\x17\x6e\x58\x69\x64\x23\x17\x58\x65"
"\x5b\x17\x65\x66\x17\x67\x63\x58\x70\x60\x65\x5e\x17\x6e\x60\x6b\x5f\x17"
"\x58\x65\x70\x17\x65\x58\x6c\x5e\x5f\x6b\x70\x17\x59\x6c\x5e\x24\x5c\x70"
"\x5c\x5b\x17\x64\x66\x65\x6a\x6b\x5c\x69\x6a\x25\x19\x01\x51\x58\x67\x5f"
"\x66\x5b\x17\x6b\x58\x67\x67\x5c\x5b\x17\x60\x64\x67\x58\x6b\x60\x5c\x65"
"\x6b\x63\x70\x17\x66\x65\x17\x6b\x5f\x5c\x17\x5f\x58\x6b\x5a\x5f\x25\x01"
"\x19\x40\x1e\x64\x17\x6a\x66\x69\x69\x70\x23\x19\x17\x5f\x5c\x17\x6a\x58"
"\x60\x5b\x23\x17\x19\x40\x17\x6b\x5f\x60\x65\x62\x17\x6e\x5c\x17\x64\x60"
"\x5e\x5f\x6b\x17\x59\x5c\x17\x59\x5c\x6b\x6b\x5c\x69\x17\x66\x5d\x5d\x17"
"\x6e\x60\x6b\x5f\x17\x58\x17\x6a\x63\x60\x5b\x5c\x17\x69\x6c\x63\x5c\x25"
"\x19\x01\x19\x49\x60\x5e\x5f\x6b\x18\x19\x17\x6a\x65\x58\x67\x67\x5c\x5b"
"\x17\x6b\x5f\x5c\x17\x5a\x66\x64\x67\x6c\x6b\x5c\x69\x25\x17\x19\x4e\x5f"
"\x66\x17\x6a\x58\x60\x5b\x17\x6b\x5f\x58\x6b\x36\x19\x01\x19\x4e\x60\x63"
"\x63\x17\x70\x66\x6c\x17\x66\x67\x5c\x65\x17\x6b\x5f\x5c\x17\x5c\x6f\x60"
"\x6b\x17\x5f\x58\x6b\x5a\x5f\x17\x67\x63\x5c\x58\x6a\x5c\x23\x17\x5a\x66"
"\x64\x67\x6c\x6b\x5c\x69\x36\x19\x17\x6a\x58\x60\x5b\x17\x51\x58\x67\x5f"
"\x66\x5b\x17\x6b\x69\x70\x60\x65\x5e\x17\x65\x66\x6b\x17\x6b\x66\x17\x5e"
"\x5c\x6b\x17\x58\x65\x5e\x69\x70\x25\x01\x19\x45\x66\x6b\x17\x6c\x65\x6b"
"\x60\x63\x17\x6e\x5f\x66\x5c\x6d\x5c\x69\x17\x6a\x58\x60\x5b\x17\x6b\x5f"
"\x58\x6b\x17\x66\x6e\x65\x6a\x17\x6c\x67\x23\x19\x17\x6c\x69\x5e\x5c\x5b"
"\x17\x6b\x5f\x5c\x17\x5a\x66\x64\x67\x6c\x6b\x5c\x69\x23\x17\x6a\x6b\x58"
"\x64\x67\x60\x65\x5e\x17\x58\x17\x5d\x5c\x6e\x17\x6a\x70\x65\x58\x67\x6a"
"\x5c\x6a\x17\x5a\x63\x66\x6a\x5c\x5b\x25\x01\x19\x46\x5f\x17\x3e\x66\x5b"
"\x23\x19\x17\x64\x6c\x6b\x6b\x5c\x69\x5c\x5b\x17\x3d\x66\x69\x5b\x23\x17"
"\x6a\x63\x6c\x64\x67\x5c\x5b\x17\x58\x5e\x58\x60\x65\x6a\x6b\x17\x58\x17"
"\x59\x6c\x63\x62\x5f\x5c\x58\x5b\x17\x58\x65\x5b\x17\x6a\x6b\x58\x69\x6b"
"\x5c\x5b\x17\x6b\x66\x17\x5a\x66\x6c\x65\x6b\x17\x6b\x66\x17\x6b\x5c\x65"
"\x25\x17\x3f\x5c\x17\x6e\x58\x6a\x17\x5b\x5c\x6a\x67\x5c\x69\x58\x6b\x5c"
"\x63\x70\x17\x6e\x66\x69\x69\x60\x5c\x5b\x17\x6b\x5f\x58\x6b\x17\x66\x65"
"\x5c\x17\x5b\x58\x70\x17\x6a\x5c\x65\x6b\x60\x5c\x65\x6b\x17\x63\x60\x5d"
"\x5c\x17\x5d\x66\x69\x64\x6a\x17\x6e\x66\x6c\x63\x5b\x17\x5d\x66\x69\x5e"
"\x5c\x6b\x17\x5f\x66\x6e\x17\x6b\x66\x17\x5b\x66\x17\x6b\x5f\x60\x6a\x25"
"\x17\x46\x65\x63\x70\x17\x59\x70\x17\x5a\x66\x6c\x65\x6b\x60\x65\x5e\x17"
"\x5a\x66\x6c\x63\x5b\x17\x5f\x6c\x64\x58\x65\x6a\x17\x5b\x5c\x64\x66\x65"
"\x6a\x6b\x69\x58\x6b\x5c\x17\x6b\x5f\x5c\x60\x69\x17\x60\x65\x5b\x5c\x67"
"\x5c\x65\x5b\x5c\x65\x5a\x5c\x17\x66\x5d\x17\x5a\x66\x64\x67\x6c\x6b\x5c"
"\x69\x6a\x25\x01\x19\x3a\x66\x64\x5c\x17\x66\x65\x23\x19\x17\x6a\x58\x60"
"\x5b\x17\x3c\x5b\x5b\x60\x5c\x17\x6a\x6b\x5c\x69\x65\x63\x70\x25\x01\x19"
"\x3a\x66\x64\x67\x6c\x6b\x5c\x69\x25\x25\x25\x19\x17\x59\x5c\x5e\x58\x65"
"\x17\x51\x58\x67\x5f\x66\x5b\x25\x25\x25\x01\x19\x40\x1e\x64\x17\x6e\x58"
"\x60\x6b\x60\x65\x5e\x23\x19\x17\x60\x65\x6b\x5c\x69\x69\x6c\x67\x6b\x5c"
"\x5b\x17\x3c\x5b\x5b\x60\x5c\x25\x17\x19\x40\x17\x5a\x58\x65\x17\x6e\x58"
"\x60\x6b\x17\x58\x63\x63\x17\x5b\x58\x70\x17\x60\x5d\x17\x65\x5c\x5a\x5c"
"\x6a\x6a\x58\x69\x70\x25\x25\x25\x19\x01\x19\x3a\x66\x64\x67\x6c\x6b\x5c"
"\x69\x25\x25\x25\x19\x17\x6a\x58\x60\x5b\x17\x51\x58\x67\x5f\x66\x5b\x17"
"\x58\x5e\x58\x60\x65\x23\x17\x6e\x5f\x66\x17\x5f\x58\x5b\x17\x59\x5c\x5c"
"\x65\x17\x6b\x69\x70\x60\x65\x5e\x17\x6b\x66\x17\x6b\x5f\x60\x65\x62\x17"
"\x66\x5d\x17\x6a\x66\x64\x5c\x17\x6a\x6c\x59\x6b\x63\x5c\x17\x67\x60\x5c"
"\x5a\x5c\x17\x66\x5d\x17\x69\x5c\x58\x6a\x66\x65\x60\x65\x5e\x17\x6b\x66"
"\x17\x67\x6c\x6b\x17\x6b\x5f\x5c\x17\x5a\x66\x64\x67\x6c\x6b\x5c\x69\x17"
"\x5b\x66\x6e\x65\x17\x6e\x60\x6b\x5f\x23\x17\x58\x65\x5b\x17\x5f\x58\x5b"
"\x17\x5b\x5c\x5a\x60\x5b\x5c\x5b\x17\x65\x66\x6b\x17\x6b\x66\x17\x59\x66"
"\x6b\x5f\x5c\x69\x17\x5a\x66\x64\x67\x5c\x6b\x60\x65\x5e\x17\x6e\x60\x6b"
"\x5f\x17\x60\x6b\x17\x66\x65\x17\x60\x6b\x6a\x17\x66\x6e\x65\x17\x5e\x69"
"\x66\x6c\x65\x5b\x23\x17\x19\x60\x5d\x17\x70\x66\x6c\x17\x5b\x66\x65\x1e"
"\x6b\x17\x66\x67\x5c\x65\x17\x6b\x5f\x58\x6b\x17\x5c\x6f\x60\x6b\x17\x5f"
"\x58\x6b\x5a\x5f\x17\x6b\x5f\x60\x6a\x17\x64\x66\x64\x5c\x65\x6b\x17\x40"
"\x17\x6a\x5f\x58\x63\x63\x17\x71\x58\x67\x17\x6a\x6b\x69\x58\x60\x5e\x5f"
"\x6b\x17\x66\x5d\x5d\x17\x6b\x66\x17\x70\x66\x6c\x69\x17\x64\x58\x61\x66"
"\x69\x17\x5b\x58\x6b\x58\x17\x59\x58\x65\x62\x6a\x17\x58\x65\x5b\x17\x69"
"\x5c\x67\x69\x66\x5e\x69\x58\x64\x17\x70\x66\x6c\x17\x6e\x60\x6b\x5f\x17"
"\x58\x17\x6d\x5c\x69\x70\x17\x63\x58\x69\x5e\x5c\x17\x58\x6f\x5c\x23\x17"
"\x5e\x66\x6b\x17\x6b\x5f\x58\x6b\x36\x19\x01\x3c\x5b\x5b\x60\x5c\x23\x17"
"\x6a\x5f\x66\x5a\x62\x5c\x5b\x23\x17\x67\x58\x6c\x6a\x5c\x5b\x17\x58\x65"
"\x5b\x17\x5a\x66\x65\x6a\x60\x5b\x5c\x69\x5c\x5b\x17\x6b\x5f\x60\x6a\x25"
"\x17\x3d\x66\x69\x5b\x17\x5a\x58\x69\x69\x60\x5c\x5b\x17\x66\x65\x17\x5a"
"\x66\x6c\x65\x6b\x60\x65\x5e\x17\x68\x6c\x60\x5c\x6b\x63\x70\x25\x17\x4b"
"\x5f\x60\x6a\x17\x60\x6a\x17\x58\x59\x66\x6c\x6b\x17\x6b\x5f\x5c\x17\x64"
"\x66\x6a\x6b\x17\x58\x5e\x5e\x69\x5c\x6a\x6a\x60\x6d\x5c\x17\x6b\x5f\x60"
"\x65\x5e\x17\x70\x66\x6c\x17\x5a\x58\x65\x17\x5b\x66\x17\x6b\x66\x17\x58"
"\x17\x5a\x66\x64\x67\x6c\x6b\x5c\x69\x23\x17\x6b\x5f\x5c\x17\x5c\x68\x6c"
"\x60\x6d\x58\x63\x5c\x65\x6b\x17\x66\x5d\x17\x5e\x66\x60\x65\x5e\x17\x6c"
"\x67\x17\x6b\x66\x17\x58\x17\x5f\x6c\x64\x58\x65\x17\x59\x5c\x60\x65\x5e"
"\x17\x58\x65\x5b\x17\x6a\x58\x70\x60\x65\x5e\x17\x39\x63\x66\x66\x5b\x25"
"\x25\x25\x17\x59\x63\x66\x66\x5b\x25\x25\x25\x17\x59\x63\x66\x66\x5b\x25"
"\x25\x25\x17\x59\x63\x66\x66\x5b\x25\x25\x25\x01\x3d\x60\x65\x58\x63\x63"
"\x70\x17\x3c\x5b\x5b\x60\x5c\x17\x6a\x58\x60\x5b\x17\x68\x6c\x60\x5c\x6b"
"\x63\x70\x23\x17\x19\x40\x17\x5a\x58\x65\x17\x6a\x5c\x5c\x17\x6b\x5f\x60"
"\x6a\x17\x69\x5c\x63\x58\x6b\x60\x66\x65\x6a\x5f\x60\x67\x17\x60\x6a\x17"
"\x6a\x66\x64\x5c\x6b\x5f\x60\x65\x5e\x17\x6e\x5c\x1e\x69\x5c\x17\x58\x63"
"\x63\x17\x5e\x66\x60\x65\x5e\x17\x6b\x66\x17\x5f\x58\x6d\x5c\x17\x6b\x66"
"\x17\x6e\x66\x69\x62\x17\x58\x6b\x23\x19\x17\x58\x65\x5b\x17\x6b\x5f\x5c"
"\x17\x5f\x58\x6b\x5a\x5f\x6e\x58\x70\x17\x66\x67\x5c\x65\x5c\x5b\x25"
;

char buf[2017];

extern "C" __declspec(dllexport) char* HHGttG(LONG param) {
	for (int i=0; i<sizeof(hhgttg); i++) {
		buf[i]=hhgttg[i]+9;
	}
	buf[sizeof(hhgttg)]=0;
	return buf;
}
