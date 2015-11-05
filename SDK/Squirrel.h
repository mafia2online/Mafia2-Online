/*
Copyright (c) 2003-2009 Alberto Demichelis

This software is provided 'as-is', without any 
express or implied warranty. In no event will the 
authors be held liable for any damages arising from 
the use of this software.

Permission is granted to anyone to use this software 
for any purpose, including commercial applications, 
and to alter it and redistribute it freely, subject 
to the following restrictions:

		1. The origin of this software must not be 
		misrepresented; you must not claim that 
		you wrote the original software. If you 
		use this software in a product, an 
		acknowledgment in the product 
		documentation would be appreciated but is 
		not required.

		2. Altered source versions must be plainly 
		marked as such, and must not be 
		misrepresented as being the original 
		software.

		3. This notice may not be removed or 
		altered from any source distribution.

*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SQUIRREL_API
#define SQUIRREL_API extern
#endif

#if (defined(_WIN64) || defined(_LP64))
#define _SQ64
#endif

#ifdef _SQ64
#ifdef _MSC_VER
typedef __int64 int;
typedef unsigned __int64 SQUnsignedInteger;
typedef unsigned __int64 SQHash; /*should be the same size of a pointer*/
#else
typedef long int;
typedef unsigned long SQUnsignedInteger;
typedef unsigned long SQHash; /*should be the same size of a pointer*/
#endif
typedef int SQInt32; 
#else
typedef int SQInt32; /*must be 32 bits(also on 64bits processors)*/
typedef unsigned int SQUnsignedInteger;
typedef unsigned int SQHash; /*should be the same size of a pointer*/
#endif

#if defined(SQUSEDOUBLE) && !defined(_SQ64)
#ifdef _MSC_VER
typedef __int64 SQRawObjectVal; //must be 64bits
#else
typedef long SQRawObjectVal; //must be 64bits
#endif
#define SQ_OBJECT_RAWINIT() { _unVal.raw = 0; }
#else
typedef SQUnsignedInteger SQRawObjectVal; //is 32 bits on 32 bits builds and 64 bits otherwise
#define SQ_OBJECT_RAWINIT()
#endif

typedef void* SQUserPointer;
typedef SQUnsignedInteger SQBool;
typedef int SQRESULT;

#define SQTrue	(1)
#define SQFalse	(0)

struct SQVM;
struct SQTable;
struct SQArray;
struct SQString;
struct SQClosure;
struct SQGenerator;
struct SQNativeClosure;
struct SQUserData;
struct SQFunctionProto;
struct SQRefCounted;
struct SQClass;
struct SQInstance;
struct SQDelegable;
struct SQOuter;

#define MAX_CHAR 0xFF

#define SQUIRREL_VERSION	"Squirrel 3.0 alpha 2"
#define SQUIRREL_COPYRIGHT	"Copyright (C) 2003-2009 Alberto Demichelis"
#define SQUIRREL_AUTHOR		"Alberto Demichelis"

#define SQ_VMSTATE_IDLE			0
#define SQ_VMSTATE_RUNNING		1
#define SQ_VMSTATE_SUSPENDED	2

#define SQUIRREL_EOB 0
#define SQ_BYTECODE_STREAM_TAG	0xFAFA

#define SQOBJECT_REF_COUNTED	0x08000000
#define SQOBJECT_NUMERIC		0x04000000
#define SQOBJECT_DELEGABLE		0x02000000
#define SQOBJECT_CANBEFALSE		0x01000000

#define SQ_MATCHTYPEMASKSTRING (-99999)

#define _RT_MASK 0x00FFFFFF
#define _RAW_TYPE(type) (type&_RT_MASK)

#define _RT_NULL			0x00000001
#define _RT_INTEGER			0x00000002
#define _RT_FLOAT			0x00000004
#define _RT_BOOL			0x00000008
#define _RT_STRING			0x00000010
#define _RT_TABLE			0x00000020
#define _RT_ARRAY			0x00000040
#define _RT_USERDATA		0x00000080
#define _RT_CLOSURE			0x00000100
#define _RT_NATIVECLOSURE	0x00000200
#define _RT_GENERATOR		0x00000400
#define _RT_USERPOINTER		0x00000800
#define _RT_THREAD			0x00001000
#define _RT_FUNCPROTO		0x00002000
#define _RT_CLASS			0x00004000
#define _RT_INSTANCE		0x00008000
#define _RT_WEAKREF			0x00010000
#define _RT_OUTER			0x00020000

typedef enum tagSQObjectType{
	OT_NULL =			(_RT_NULL|SQOBJECT_CANBEFALSE),
	OT_INTEGER =		(_RT_INTEGER|SQOBJECT_NUMERIC|SQOBJECT_CANBEFALSE),
	OT_FLOAT =			(_RT_FLOAT|SQOBJECT_NUMERIC|SQOBJECT_CANBEFALSE),
	OT_BOOL =			(_RT_BOOL|SQOBJECT_CANBEFALSE),
	OT_STRING =			(_RT_STRING|SQOBJECT_REF_COUNTED),
	OT_TABLE =			(_RT_TABLE|SQOBJECT_REF_COUNTED|SQOBJECT_DELEGABLE),
	OT_ARRAY =			(_RT_ARRAY|SQOBJECT_REF_COUNTED),
	OT_USERDATA =		(_RT_USERDATA|SQOBJECT_REF_COUNTED|SQOBJECT_DELEGABLE),
	OT_CLOSURE =		(_RT_CLOSURE|SQOBJECT_REF_COUNTED),
	OT_NATIVECLOSURE =	(_RT_NATIVECLOSURE|SQOBJECT_REF_COUNTED),
	OT_GENERATOR =		(_RT_GENERATOR|SQOBJECT_REF_COUNTED),
	OT_USERPOINTER =	_RT_USERPOINTER,
	OT_THREAD =			(_RT_THREAD|SQOBJECT_REF_COUNTED) ,
	OT_FUNCPROTO =		(_RT_FUNCPROTO|SQOBJECT_REF_COUNTED), //internal usage only
	OT_CLASS =			(_RT_CLASS|SQOBJECT_REF_COUNTED),
	OT_INSTANCE =		(_RT_INSTANCE|SQOBJECT_REF_COUNTED|SQOBJECT_DELEGABLE),
	OT_WEAKREF =		(_RT_WEAKREF|SQOBJECT_REF_COUNTED),
	OT_OUTER =			(_RT_OUTER|SQOBJECT_REF_COUNTED) //internal usage only
}SQObjectType;

#define ISREFCOUNTED(t) (t&SQOBJECT_REF_COUNTED)

typedef union tagSQObjectValue
{
	struct SQTable *pTable;
	struct SQArray *pArray;
	struct SQClosure *pClosure;
	struct SQOuter *pOuter;
	struct SQGenerator *pGenerator;
	struct SQNativeClosure *pNativeClosure;
	struct SQString *pString;
	struct SQUserData *pUserData;
	int nInteger;
	float fFloat;
	SQUserPointer pUserPointer;
	struct SQFunctionProto *pFunctionProto;
	struct SQRefCounted *pRefCounted;
	struct SQDelegable *pDelegable;
	struct SQVM *pThread;
	struct SQClass *pClass;
	struct SQInstance *pInstance;
	struct SQWeakRef *pWeakRef;
	SQRawObjectVal raw;
}SQObjectValue;

typedef struct tagSQObject
{
	SQObjectType _type;
	SQObjectValue _unVal;
}SQObject;

typedef struct tagSQStackInfos{
	const char* funcname;
	const char* source;
	int line;
}SQStackInfos;

typedef struct SQVM* HSQUIRRELVM;
typedef SQObject HSQOBJECT;
typedef int (*SQFUNCTION)(HSQUIRRELVM);
typedef int (*SQRELEASEHOOK)(SQUserPointer,int size);
typedef void (*SQCOMPILERERROR)(HSQUIRRELVM,const char *,const char *,int,int);
typedef void (*SQPRINTFUNCTION)(HSQUIRRELVM,const char * ,...);
typedef void (*SQDEBUGHOOK)(HSQUIRRELVM, int, const char *,int,const char *);
typedef int (*SQWRITEFUNC)(SQUserPointer,SQUserPointer,int);
typedef int (*SQREADFUNC)(SQUserPointer,SQUserPointer,int);

typedef int (*SQLEXREADFUNC)(SQUserPointer);

typedef struct tagSQRegFunction{
	const char *name;
	SQFUNCTION f;
	int nparamscheck;
	const char *typemask;
}SQRegFunction;

typedef struct tagSQFunctionInfo {
	SQUserPointer funcid;
	const char *name;
	const char *source;
}SQFunctionInfo;

typedef HSQUIRRELVM (*sq_open_t)(int initialstacksize);
typedef HSQUIRRELVM ( *sq_newthread_t)(HSQUIRRELVM friendvm, int initialstacksize);
typedef void ( *sq_seterrorhandler_t)(HSQUIRRELVM v);
typedef void ( *sq_close_t)(HSQUIRRELVM v);
typedef void ( *sq_setforeignptr_t)(HSQUIRRELVM v,SQUserPointer p);
typedef SQUserPointer ( *sq_getforeignptr_t)(HSQUIRRELVM v);
typedef void ( *sq_setprintfunc_t)(HSQUIRRELVM v, SQPRINTFUNCTION printfunc,SQPRINTFUNCTION errfunc);
typedef SQPRINTFUNCTION ( *sq_getprintfunc_t)(HSQUIRRELVM v);
typedef SQPRINTFUNCTION ( *sq_geterrorfunc_t)(HSQUIRRELVM v);
typedef SQRESULT ( *sq_suspendvm_t)(HSQUIRRELVM v);
typedef SQRESULT ( *sq_wakeupvm_t)(HSQUIRRELVM v,SQBool resumedret,SQBool retval,SQBool raiseerror,SQBool throwerror);
typedef int ( *sq_getvmstate_t)(HSQUIRRELVM v);
typedef SQRESULT ( *sq_compile_t)(HSQUIRRELVM v,SQLEXREADFUNC read,SQUserPointer p,const char *sourcename,SQBool raiseerror);
typedef SQRESULT ( *sq_compilebuffer_t)(HSQUIRRELVM v,const char *s,int size,const char *sourcename,SQBool raiseerror);
typedef void ( *sq_enabledebuginfo_t)(HSQUIRRELVM v, SQBool enable);
typedef void ( *sq_notifyallexceptions_t)(HSQUIRRELVM v, SQBool enable);
typedef void ( *sq_setcompilererrorhandler_t)(HSQUIRRELVM v,SQCOMPILERERROR f);
typedef void ( *sq_push_t)(HSQUIRRELVM v,int idx);
typedef void ( *sq_pop_t)(HSQUIRRELVM v,int nelemstopop);
typedef void ( *sq_poptop_t)(HSQUIRRELVM v);
typedef void ( *sq_remove_t)(HSQUIRRELVM v,int idx);
typedef int ( *sq_gettop_t)(HSQUIRRELVM v);
typedef void ( *sq_settop_t)(HSQUIRRELVM v,int newtop);
typedef void ( *sq_reservestack_t)(HSQUIRRELVM v,int nsize);
typedef int ( *sq_cmp_t)(HSQUIRRELVM v);
typedef void ( *sq_move_t)(HSQUIRRELVM dest,HSQUIRRELVM src,int idx);
typedef SQUserPointer ( *sq_newuserdata_t)(HSQUIRRELVM v,SQUnsignedInteger size);
typedef void ( *sq_newtable_t)(HSQUIRRELVM v);
typedef void ( *sq_newarray_t)(HSQUIRRELVM v,int size);
typedef void ( *sq_newclosure_t)(HSQUIRRELVM v,SQFUNCTION func,SQUnsignedInteger nfreevars);
typedef SQRESULT ( *sq_setparamscheck_t)(HSQUIRRELVM v,int nparamscheck,const char *typemask);
typedef SQRESULT ( *sq_bindenv_t)(HSQUIRRELVM v,int idx);
typedef void ( *sq_pushstring_t)(HSQUIRRELVM v,const char *s,int len);
typedef void ( *sq_pushfloat_t)(HSQUIRRELVM v,float f);
typedef void ( *sq_pushinteger_t)(HSQUIRRELVM v,int n);
typedef void ( *sq_pushbool_t)(HSQUIRRELVM v,SQBool b);
typedef void ( *sq_pushuserpointer_t)(HSQUIRRELVM v,SQUserPointer p);
typedef void ( *sq_pushnull_t)(HSQUIRRELVM v);
typedef SQObjectType ( *sq_gettype_t)(HSQUIRRELVM v,int idx);
typedef int ( *sq_getsize_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_getbase_t)(HSQUIRRELVM v,int idx);
typedef SQBool ( *sq_instanceof_t)(HSQUIRRELVM v);
typedef void ( *sq_tostring_t)(HSQUIRRELVM v,int idx);
typedef void ( *sq_tobool_t)(HSQUIRRELVM v, int idx, SQBool *b);
typedef SQRESULT ( *sq_getstring_t)(HSQUIRRELVM v,int idx,const char **c);
typedef SQRESULT ( *sq_getinteger_t)(HSQUIRRELVM v,int idx,int *i);
typedef SQRESULT ( *sq_getfloat_t)(HSQUIRRELVM v,int idx,float *f);
typedef SQRESULT ( *sq_getbool_t)(HSQUIRRELVM v,int idx,SQBool *b);
typedef SQRESULT ( *sq_getthread_t)(HSQUIRRELVM v,int idx,HSQUIRRELVM *thread);
typedef SQRESULT ( *sq_getuserpointer_t)(HSQUIRRELVM v,int idx,SQUserPointer *p);
typedef SQRESULT ( *sq_getuserdata_t)(HSQUIRRELVM v,int idx,SQUserPointer *p,SQUserPointer *typetag);
typedef SQRESULT ( *sq_settypetag_t)(HSQUIRRELVM v,int idx,SQUserPointer typetag);
typedef SQRESULT ( *sq_gettypetag_t)(HSQUIRRELVM v,int idx,SQUserPointer *typetag);
typedef void ( *sq_setreleasehook_t)(HSQUIRRELVM v,int idx,SQRELEASEHOOK hook);
typedef char ( *sq_getscratchpad_t)(HSQUIRRELVM v,int minsize);
typedef SQRESULT ( *sq_getfunctioninfo_t)(HSQUIRRELVM v,int idx,SQFunctionInfo *fi);
typedef SQRESULT ( *sq_getclosureinfo_t)(HSQUIRRELVM v,int idx,SQUnsignedInteger *nparams,SQUnsignedInteger *nfreevars);
typedef SQRESULT ( *sq_setnativeclosurename_t)(HSQUIRRELVM v,int idx,const char *name);
typedef SQRESULT ( *sq_setinstanceup_t)(HSQUIRRELVM v, int idx, SQUserPointer p);
typedef SQRESULT ( *sq_getinstanceup_t)(HSQUIRRELVM v, int idx, SQUserPointer *p,SQUserPointer typetag);
typedef SQRESULT ( *sq_setclassudsize_t)(HSQUIRRELVM v, int idx, int udsize);
typedef SQRESULT ( *sq_newclass_t)(HSQUIRRELVM v,SQBool hasbase);
typedef SQRESULT ( *sq_createinstance_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_setattributes_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_getattributes_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_getclass_t)(HSQUIRRELVM v,int idx);
typedef void ( *sq_weakref_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_getdefaultdelegate_t)(HSQUIRRELVM v,SQObjectType t);
typedef void ( *sq_pushroottable_t)(HSQUIRRELVM v);
typedef void ( *sq_pushregistrytable_t)(HSQUIRRELVM v);
typedef void ( *sq_pushconsttable_t)(HSQUIRRELVM v);
typedef SQRESULT ( *sq_setroottable_t)(HSQUIRRELVM v);
typedef SQRESULT ( *sq_setconsttable_t)(HSQUIRRELVM v);
typedef SQRESULT ( *sq_newslot_t)(HSQUIRRELVM v, int idx, SQBool bstatic);
typedef SQRESULT ( *sq_deleteslot_t)(HSQUIRRELVM v,int idx,SQBool pushval);
typedef SQRESULT ( *sq_set_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_get_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_rawget_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_rawset_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_rawdeleteslot_t)(HSQUIRRELVM v,int idx,SQBool pushval);
typedef SQRESULT ( *sq_arrayappend_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_arraypop_t)(HSQUIRRELVM v,int idx,SQBool pushval);
typedef SQRESULT ( *sq_arrayresize_t)(HSQUIRRELVM v,int idx,int newsize);
typedef SQRESULT ( *sq_arrayreverse_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_arrayremove_t)(HSQUIRRELVM v,int idx,int itemidx);
typedef SQRESULT ( *sq_arrayinsert_t)(HSQUIRRELVM v,int idx,int destpos);
typedef SQRESULT ( *sq_setdelegate_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_getdelegate_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_clone_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_setfreevariable_t)(HSQUIRRELVM v,int idx,SQUnsignedInteger nval);
typedef SQRESULT ( *sq_next_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_getweakrefval_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_clear_t)(HSQUIRRELVM v,int idx);
typedef SQRESULT ( *sq_call_t)(HSQUIRRELVM v,int params,SQBool retval,SQBool raiseerror);
typedef SQRESULT ( *sq_resume_t)(HSQUIRRELVM v,SQBool retval,SQBool raiseerror);
typedef const char *( *sq_getlocal_t)(HSQUIRRELVM v,SQUnsignedInteger level,SQUnsignedInteger idx);
typedef const char *( *sq_getfreevariable_t)(HSQUIRRELVM v,int idx,SQUnsignedInteger nval);
typedef SQRESULT ( *sq_throwerror_t)(HSQUIRRELVM v,const char *err);
typedef void ( *sq_reseterror_t)(HSQUIRRELVM v);
typedef void ( *sq_getlasterror_t)(HSQUIRRELVM v);
typedef SQRESULT ( *sq_getstackobj_t)(HSQUIRRELVM v,int idx,HSQOBJECT *po);
typedef void ( *sq_pushobject_t)(HSQUIRRELVM v,HSQOBJECT obj);
typedef void ( *sq_addref_t)(HSQUIRRELVM v,HSQOBJECT *po);
typedef SQBool ( *sq_release_t)(HSQUIRRELVM v,HSQOBJECT *po);
typedef void ( *sq_resetobject_t)(HSQOBJECT *po);
typedef const char *( *sq_objtostring_t)(HSQOBJECT *o);
typedef SQBool ( *sq_objtobool_t)(HSQOBJECT *o);
typedef int ( *sq_objtointeger_t)(HSQOBJECT *o);
typedef float ( *sq_objtofloat_t)(HSQOBJECT *o);
typedef SQRESULT ( *sq_getobjtypetag_t)(HSQOBJECT *o,SQUserPointer * typetag);
typedef int ( *sq_collectgarbage_t)(HSQUIRRELVM v);
typedef SQRESULT ( *sq_writeclosure_t)(HSQUIRRELVM vm,SQWRITEFUNC writef,SQUserPointer up);
typedef SQRESULT ( *sq_readclosure_t)(HSQUIRRELVM vm,SQREADFUNC readf,SQUserPointer up);
typedef void ( *sq_malloc_t)(SQUnsignedInteger size);
typedef void ( *sq_realloc_t)(void* p,SQUnsignedInteger oldsize,SQUnsignedInteger newsize);
typedef void ( *sq_free_t)(void *p,SQUnsignedInteger size);
typedef SQRESULT ( *sq_stackinfos_t)(HSQUIRRELVM v,int level,SQStackInfos *si);
typedef void ( *sq_setdebughook_t)(HSQUIRRELVM v);

/*UTILITY MACRO*/
#define sq_isnumeric(o) ((o)._type&SQOBJECT_NUMERIC)
#define sq_istable(o) ((o)._type==OT_TABLE)
#define sq_isarray(o) ((o)._type==OT_ARRAY)
#define sq_isfunction(o) ((o)._type==OT_FUNCPROTO)
#define sq_isclosure(o) ((o)._type==OT_CLOSURE)
#define sq_isgenerator(o) ((o)._type==OT_GENERATOR)
#define sq_isnativeclosure(o) ((o)._type==OT_NATIVECLOSURE)
#define sq_isstring(o) ((o)._type==OT_STRING)
#define sq_isinteger(o) ((o)._type==OT_INTEGER)
#define sq_isfloat(o) ((o)._type==OT_FLOAT)
#define sq_isuserpointer(o) ((o)._type==OT_USERPOINTER)
#define sq_isuserdata(o) ((o)._type==OT_USERDATA)
#define sq_isthread(o) ((o)._type==OT_THREAD)
#define sq_isnull(o) ((o)._type==OT_NULL)
#define sq_isclass(o) ((o)._type==OT_CLASS)
#define sq_isinstance(o) ((o)._type==OT_INSTANCE)
#define sq_isbool(o) ((o)._type==OT_BOOL)
#define sq_isweakref(o) ((o)._type==OT_WEAKREF)
#define sq_isouter(o) ((o)._type==OT_OUTER)
#define sq_type(o) ((o)._type)

/* deprecated */
#define sq_createslot(v,n) sq_newslot(v,n,SQFalse)

#define SQ_OK (0)
#define SQ_ERROR (-1)

#define SQ_FAILED(res) (res<0)
#define SQ_SUCCEEDED(res) (res>=0)

#ifdef __cplusplus
} /*extern "C"*/
#endif

#define sq_newthread Squirrel.sqnewthread
#define sq_seterrorhandler Squirrel.sqseterrorhandler
#define sq_close Squirrel.sqclose
#define sq_setforeignptr Squirrel.sqsetforeignptr
#define sq_getforeignptr Squirrel.sqgetforeignptr
#define sq_setprintfunc Squirrel.sqsetprintfunc
#define sq_getprintfunc Squirrel.sqgetprintfunc
#define sq_geterrorfunc Squirrel.sqgeterrorfunc
#define sq_suspendvm Squirrel.sqsuspendvm
#define sq_wakeupvm Squirrel.sqwakeupvm
#define sq_getvmstate Squirrel.sqgetvmstate
#define sq_compile Squirrel.sqcompile
#define sq_compilebuffer Squirrel.sqcompilebuffer
#define sq_enabledebuginfo Squirrel.sqenabledebuginfo
#define sq_notifyallexceptions Squirrel.sqnotifyallexceptions
#define sq_setcompilererrorhandler Squirrel.sqsetcompilererrorhandler
#define sq_push Squirrel.sqpush
#define sq_pop Squirrel.sqpop
#define sq_poptop Squirrel.sqpoptop
#define sq_remove Squirrel.sqremove
#define sq_gettop Squirrel.sqgettop
#define sq_settop Squirrel.sqsettop
#define sq_reservestack Squirrel.sqreservestack
#define sq_cmp Squirrel.sqcmp
#define sq_move Squirrel.sqmove
#define sq_newuserdata Squirrel.sqnewuserdata
#define sq_newtable Squirrel.sqnewtable
#define sq_newarray Squirrel.sqnewarray
#define sq_newclosure Squirrel.sqnewclosure
#define sq_setparamscheck Squirrel.sqsetparamscheck
#define sq_bindenv Squirrel.sqbindenv
#define sq_pushstring Squirrel.sqpushstring
#define sq_pushfloat Squirrel.sqpushfloat
#define sq_pushinteger Squirrel.sqpushinteger
#define sq_pushbool Squirrel.sqpushbool
#define sq_pushuserpointer Squirrel.sqpushuserpointer
#define sq_pushnull Squirrel.sqpushnull
#define sq_gettype Squirrel.sqgettype
#define sq_getsize Squirrel.sqgetsize
#define sq_getbase Squirrel.sqgetbase
#define sq_instanceof Squirrel.sqinstanceof
#define sq_tostring Squirrel.sqtostring
#define sq_tobool Squirrel.sqtobool
#define sq_getstring Squirrel.sqgetstring
#define sq_getinteger Squirrel.sqgetinteger
#define sq_getfloat Squirrel.sqgetfloat
#define sq_getbool Squirrel.sqgetbool
#define sq_getthread Squirrel.sqgetthread
#define sq_getuserpointer Squirrel.sqgetuserpointer
#define sq_getuserdata Squirrel.sqgetuserdata
#define sq_settypetag Squirrel.sqsettypetag
#define sq_gettypetag Squirrel.sqgettypetag
#define sq_setreleasehook Squirrel.sqsetreleasehook
#define sq_getscratchpad Squirrel.sqgetscratchpad
#define sq_getfunctioninfo Squirrel.sqgetfunctioninfo
#define sq_getclosureinfo Squirrel.sqgetclosureinfo
#define sq_setnativeclosurename Squirrel.sqsetnativeclosurename
#define sq_setinstanceup Squirrel.sqsetinstanceup
#define sq_getinstanceup Squirrel.sqgetinstanceup
#define sq_setclassudsize Squirrel.sqsetclassudsize
#define sq_newclass Squirrel.sqnewclass
#define sq_createinstance Squirrel.sqcreateinstance
#define sq_setattributes Squirrel.sqsetattributes
#define sq_getattributes Squirrel.sqgetattributes
#define sq_getclass Squirrel.sqgetclass
#define sq_weakref Squirrel.sqweakref
#define sq_getdefaultdelegate Squirrel.sqgetdefaultdelegate
#define sq_pushroottable Squirrel.sqpushroottable
#define sq_pushregistrytable Squirrel.sqpushregistrytable
#define sq_pushconsttable Squirrel.sqpushconsttable
#define sq_setroottable Squirrel.sqsetroottable
#define sq_setconsttable Squirrel.sqsetconsttable
#define sq_newslot Squirrel.sqnewslot
#define sq_deleteslot Squirrel.sqdeleteslot
#define sq_set Squirrel.sqset
#define sq_get Squirrel.sqget
#define sq_rawget Squirrel.sqrawget
#define sq_rawset Squirrel.sqrawset
#define sq_rawdeleteslot Squirrel.sqrawdeleteslot
#define sq_arrayappend Squirrel.sqarrayappend
#define sq_arraypop Squirrel.sqarraypop
#define sq_arrayresize Squirrel.sqarrayresize
#define sq_arrayreverse Squirrel.sqarrayreverse
#define sq_arrayremove Squirrel.sqarrayremove
#define sq_arrayinsert Squirrel.sqarrayinsert
#define sq_setdelegate Squirrel.sqsetdelegate
#define sq_getdelegate Squirrel.sqgetdelegate
#define sq_clone Squirrel.sqclone
#define sq_setfreevariable Squirrel.sqsetfreevariable
#define sq_next Squirrel.sqnext
#define sq_getweakrefval Squirrel.sqgetweakrefval
#define sq_clear Squirrel.sqclear
#define sq_call Squirrel.sqcall
#define sq_resume Squirrel.sqresume
#define sq_getlocal Squirrel.sqgetlocal
#define sq_getfreevariable Squirrel.sqgetfreevariable
#define sq_throwerror Squirrel.sqthrowerror
#define sq_reseterror Squirrel.sqreseterror
#define sq_getlasterror Squirrel.sqgetlasterror
#define sq_getstackobj Squirrel.sqgetstackobj
#define sq_pushobject Squirrel.sqpushobject
#define sq_addref Squirrel.sqaddref
#define sq_release Squirrel.sqrelease
#define sq_resetobject Squirrel.sqresetobject
#define sq_objtostring Squirrel.sqobjtostring
#define sq_objtobool Squirrel.sqobjtobool
#define sq_objtointeger Squirrel.sqobjtointeger
#define sq_objtofloat Squirrel.sqobjtofloat
#define sq_getobjtypetag Squirrel.sqgetobjtypetag
#define sq_collectgarbage Squirrel.sqcollectgarbage
#define sq_writeclosure Squirrel.sqwriteclosure
#define sq_readclosure Squirrel.sqreadclosure
#define sq_malloc Squirrel.sqmalloc
#define sq_realloc Squirrel.sqrealloc
#define sq_free Squirrel.sqfree
#define sq_stackinfos Squirrel.sqstackinfos
#define sq_setdebughook Squirrel.sqsetdebughook
#define sq_open Squirrel.sqopen
#define sq_close Squirrel.sqclose