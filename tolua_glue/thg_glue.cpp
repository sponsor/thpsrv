/*
** Lua binding: thg
** Generated automatically by tolua++-1.0.92 on 12/02/18 19:14:13.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_thg_open (lua_State* tolua_S);

#include "..\ext.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_type_session (lua_State* tolua_S)
{
 type_session* self = (type_session*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_wchar_t (lua_State* tolua_S)
{
 wchar_t* self = (wchar_t*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_type_blt (lua_State* tolua_S)
{
 type_blt* self = (type_blt*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_type_ground (lua_State* tolua_S)
{
 type_ground* self = (type_ground*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"type_ground");
 tolua_usertype(tolua_S,"E_TYPE_USER_DIRECTION");
 tolua_usertype(tolua_S,"type_session");
 tolua_usertype(tolua_S,"wchar_t");
 tolua_usertype(tolua_S,"TCHARA_SCR_INFO");
 tolua_usertype(tolua_S,"type_blt");
}

/* get function: obj_state of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_obj_state
static int tolua_get_type_session_obj_state(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'obj_state'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->obj_state);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: obj_state of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_obj_state
static int tolua_set_type_session_obj_state(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'obj_state'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->obj_state = ((E_TYPE_OBJ_STATE) (int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: obj_type of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_unsigned_obj_type
static int tolua_get_type_session_unsigned_obj_type(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'obj_type'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->obj_type);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: obj_type of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_unsigned_obj_type
static int tolua_set_type_session_unsigned_obj_type(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'obj_type'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->obj_type = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: vx of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_vx
static int tolua_get_type_session_vx(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vx'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->vx);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: vx of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_vx
static int tolua_set_type_session_vx(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vx'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->vx = ((short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: vy of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_vy
static int tolua_get_type_session_vy(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vy'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->vy);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: vy of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_vy
static int tolua_set_type_session_vy(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vy'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->vy = ((short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: ax of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_ax
static int tolua_get_type_session_ax(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ax'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->ax);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: ax of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_ax
static int tolua_set_type_session_ax(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ax'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->ax = ((short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: ay of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_ay
static int tolua_get_type_session_ay(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ay'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->ay);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: ay of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_ay
static int tolua_set_type_session_ay(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ay'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->ay = ((short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: angle of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_angle
static int tolua_get_type_session_angle(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'angle'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->angle);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: angle of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_angle
static int tolua_set_type_session_angle(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'angle'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->angle = ((short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: obj_no of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_obj_no
static int tolua_get_type_session_obj_no(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'obj_no'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->obj_no);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: obj_no of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_obj_no
static int tolua_set_type_session_obj_no(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'obj_no'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->obj_no = ((short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: sess_index of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_unsigned_sess_index
static int tolua_get_type_session_unsigned_sess_index(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sess_index'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->sess_index);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: sess_index of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_unsigned_sess_index
static int tolua_set_type_session_unsigned_sess_index(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sess_index'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->sess_index = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: proc_flg of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_unsigned_proc_flg
static int tolua_get_type_session_unsigned_proc_flg(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'proc_flg'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->proc_flg);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: proc_flg of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_unsigned_proc_flg
static int tolua_set_type_session_unsigned_proc_flg(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'proc_flg'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->proc_flg = ((unsigned short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: frame_count of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_unsigned_frame_count
static int tolua_get_type_session_unsigned_frame_count(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'frame_count'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->frame_count);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: frame_count of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_unsigned_frame_count
static int tolua_set_type_session_unsigned_frame_count(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'frame_count'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->frame_count = ((unsigned short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: turn_count of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_unsigned_turn_count
static int tolua_get_type_session_unsigned_turn_count(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'turn_count'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->turn_count);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: turn_count of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_unsigned_turn_count
static int tolua_set_type_session_unsigned_turn_count(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'turn_count'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->turn_count = ((unsigned short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: scrinfo of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_scrinfo_ptr
static int tolua_get_type_session_scrinfo_ptr(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'scrinfo'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)self->scrinfo,"TCHARA_SCR_INFO");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: scrinfo of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_scrinfo_ptr
static int tolua_set_type_session_scrinfo_ptr(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'scrinfo'",NULL);
  if (!tolua_isusertype(tolua_S,2,"TCHARA_SCR_INFO",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->scrinfo = ((TCHARA_SCR_INFO*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: extdata1 of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_unsigned_extdata1
static int tolua_get_type_session_unsigned_extdata1(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'extdata1'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->extdata1);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: extdata1 of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_unsigned_extdata1
static int tolua_set_type_session_unsigned_extdata1(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'extdata1'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->extdata1 = ((unsigned long)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: extdata2 of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_unsigned_extdata2
static int tolua_get_type_session_unsigned_extdata2(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'extdata2'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->extdata2);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: extdata2 of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_unsigned_extdata2
static int tolua_set_type_session_unsigned_extdata2(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'extdata2'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->extdata2 = ((unsigned long)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: option of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_thg_type_session_option
static int tolua_get_thg_type_session_option(lua_State* tolua_S)
{
 int tolua_index;
  type_session* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (type_session*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=OBJ_OPTION_COUNT)
  tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->option[tolua_index]);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: option of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_thg_type_session_option
static int tolua_set_thg_type_session_option(lua_State* tolua_S)
{
 int tolua_index;
  type_session* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (type_session*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=OBJ_OPTION_COUNT)
  tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
  self->option[tolua_index] = ((unsigned long)  tolua_tonumber(tolua_S,3,0));
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: name of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_thg_type_session_name
static int tolua_get_thg_type_session_name(lua_State* tolua_S)
{
 int tolua_index;
  type_session* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (type_session*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=MAX_USER_NAME)
  tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->name[tolua_index],"wchar_t");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: name of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_thg_type_session_name
static int tolua_set_thg_type_session_name(lua_State* tolua_S)
{
 int tolua_index;
  type_session* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (type_session*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=MAX_USER_NAME)
  tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
  self->name[tolua_index] = *((wchar_t*)  tolua_tousertype(tolua_S,3,0));
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: name_len of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_unsigned_name_len
static int tolua_get_type_session_unsigned_name_len(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'name_len'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->name_len);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: name_len of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_unsigned_name_len
static int tolua_set_type_session_unsigned_name_len(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'name_len'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->name_len = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: lx of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_lx
static int tolua_get_type_session_lx(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'lx'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->lx);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: lx of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_lx
static int tolua_set_type_session_lx(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'lx'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->lx = ((short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: ly of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_ly
static int tolua_get_type_session_ly(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ly'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->ly);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: ly of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_ly
static int tolua_set_type_session_ly(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ly'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->ly = ((short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: sock of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_sock
static int tolua_get_type_session_sock(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sock'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->sock);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: sock of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_sock
static int tolua_set_type_session_sock(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sock'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->sock = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: addr of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_addr
static int tolua_get_type_session_addr(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'addr'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->addr);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: addr of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_addr
static int tolua_set_type_session_addr(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'addr'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->addr = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: sessionkey of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_sessionkey
static int tolua_get_type_session_sessionkey(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sessionkey'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->sessionkey);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: sessionkey of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_sessionkey
static int tolua_set_type_session_sessionkey(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sessionkey'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->sessionkey = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: clientver of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_clientver
static int tolua_get_type_session_clientver(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'clientver'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->clientver);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: clientver of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_clientver
static int tolua_set_type_session_clientver(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'clientver'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->clientver = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: port of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_port
static int tolua_get_type_session_port(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'port'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->port);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: port of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_port
static int tolua_set_type_session_port(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'port'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->port = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: cost of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_unsigned_cost
static int tolua_get_type_session_unsigned_cost(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'cost'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->cost);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: cost of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_unsigned_cost
static int tolua_set_type_session_unsigned_cost(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'cost'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->cost = ((unsigned short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: items of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_thg_type_session_items
static int tolua_get_thg_type_session_items(lua_State* tolua_S)
{
 int tolua_index;
  type_session* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (type_session*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=GAME_ITEM_STOCK_MAX_COUNT)
  tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->items[tolua_index]);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: items of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_thg_type_session_items
static int tolua_set_thg_type_session_items(lua_State* tolua_S)
{
 int tolua_index;
  type_session* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (type_session*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=GAME_ITEM_STOCK_MAX_COUNT)
  tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
  self->items[tolua_index] = ((unsigned long)  tolua_tonumber(tolua_S,3,0));
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: dir of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_dir
static int tolua_get_type_session_dir(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dir'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->dir,"E_TYPE_USER_DIRECTION");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: dir of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_dir
static int tolua_set_type_session_dir(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dir'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"E_TYPE_USER_DIRECTION",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->dir = *((E_TYPE_USER_DIRECTION*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: master of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_unsigned_master
static int tolua_get_type_session_unsigned_master(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'master'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->master);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: master of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_unsigned_master
static int tolua_set_type_session_unsigned_master(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'master'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->master = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: game_ready of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_unsigned_game_ready
static int tolua_get_type_session_unsigned_game_ready(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'game_ready'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->game_ready);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: game_ready of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_unsigned_game_ready
static int tolua_set_type_session_unsigned_game_ready(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'game_ready'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->game_ready = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: team_no of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_unsigned_team_no
static int tolua_get_type_session_unsigned_team_no(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'team_no'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->team_no);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: team_no of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_unsigned_team_no
static int tolua_set_type_session_unsigned_team_no(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'team_no'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->team_no = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: MV_c of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_MV_c
static int tolua_get_type_session_MV_c(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'MV_c'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->MV_c);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: MV_c of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_MV_c
static int tolua_set_type_session_MV_c(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'MV_c'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->MV_c = ((short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: MV_m of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_MV_m
static int tolua_get_type_session_MV_m(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'MV_m'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->MV_m);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: MV_m of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_MV_m
static int tolua_set_type_session_MV_m(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'MV_m'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->MV_m = ((short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: HP_c of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_HP_c
static int tolua_get_type_session_HP_c(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'HP_c'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->HP_c);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: HP_c of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_HP_c
static int tolua_set_type_session_HP_c(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'HP_c'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->HP_c = ((short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: HP_m of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_HP_m
static int tolua_get_type_session_HP_m(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'HP_m'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->HP_m);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: HP_m of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_HP_m
static int tolua_set_type_session_HP_m(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'HP_m'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->HP_m = ((short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: delay of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_delay
static int tolua_get_type_session_delay(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'delay'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->delay);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: delay of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_delay
static int tolua_set_type_session_delay(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'delay'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->delay = ((short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: EXP_c of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_EXP_c
static int tolua_get_type_session_EXP_c(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EXP_c'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->EXP_c);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: EXP_c of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_EXP_c
static int tolua_set_type_session_EXP_c(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'EXP_c'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->EXP_c = ((short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: live_count of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_unsigned_live_count
static int tolua_get_type_session_unsigned_live_count(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'live_count'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->live_count);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: live_count of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_unsigned_live_count
static int tolua_set_type_session_unsigned_live_count(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'live_count'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->live_count = ((unsigned short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: entity of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_unsigned_entity
static int tolua_get_type_session_unsigned_entity(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'entity'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->entity);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: entity of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_unsigned_entity
static int tolua_set_type_session_unsigned_entity(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'entity'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->entity = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: chara_type of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_unsigned_chara_type
static int tolua_get_type_session_unsigned_chara_type(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'chara_type'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->chara_type);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: chara_type of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_unsigned_chara_type
static int tolua_set_type_session_unsigned_chara_type(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'chara_type'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->chara_type = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: chara_state of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_chara_state
static int tolua_get_type_session_chara_state(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'chara_state'",NULL);
#endif
  tolua_pushstring(tolua_S,(const char*)self->chara_state);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: chara_state of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_chara_state
static int tolua_set_type_session_chara_state(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'chara_state'",NULL);
  if (!tolua_istable(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 strncpy((char*)
self->chara_state,(const char*)tolua_tostring(tolua_S,2,0),CHARA_STATE_COUNT-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: connect_state of class  type_session */
#ifndef TOLUA_DISABLE_tolua_get_type_session_unsigned_connect_state
static int tolua_get_type_session_unsigned_connect_state(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'connect_state'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->connect_state);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: connect_state of class  type_session */
#ifndef TOLUA_DISABLE_tolua_set_type_session_unsigned_connect_state
static int tolua_set_type_session_unsigned_connect_state(lua_State* tolua_S)
{
  type_session* self = (type_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'connect_state'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->connect_state = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: obj_state of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_get_type_blt_obj_state
static int tolua_get_type_blt_obj_state(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'obj_state'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->obj_state);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: obj_state of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_set_type_blt_obj_state
static int tolua_set_type_blt_obj_state(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'obj_state'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->obj_state = ((E_TYPE_OBJ_STATE) (int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: obj_type of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_get_type_blt_unsigned_obj_type
static int tolua_get_type_blt_unsigned_obj_type(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'obj_type'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->obj_type);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: obj_type of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_set_type_blt_unsigned_obj_type
static int tolua_set_type_blt_unsigned_obj_type(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'obj_type'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->obj_type = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: vx of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_get_type_blt_vx
static int tolua_get_type_blt_vx(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vx'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->vx);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: vx of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_set_type_blt_vx
static int tolua_set_type_blt_vx(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vx'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->vx = ((short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: vy of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_get_type_blt_vy
static int tolua_get_type_blt_vy(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vy'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->vy);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: vy of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_set_type_blt_vy
static int tolua_set_type_blt_vy(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vy'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->vy = ((short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: ax of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_get_type_blt_ax
static int tolua_get_type_blt_ax(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ax'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->ax);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: ax of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_set_type_blt_ax
static int tolua_set_type_blt_ax(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ax'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->ax = ((short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: ay of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_get_type_blt_ay
static int tolua_get_type_blt_ay(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ay'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->ay);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: ay of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_set_type_blt_ay
static int tolua_set_type_blt_ay(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ay'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->ay = ((short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: angle of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_get_type_blt_angle
static int tolua_get_type_blt_angle(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'angle'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->angle);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: angle of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_set_type_blt_angle
static int tolua_set_type_blt_angle(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'angle'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->angle = ((short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: obj_no of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_get_type_blt_obj_no
static int tolua_get_type_blt_obj_no(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'obj_no'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->obj_no);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: obj_no of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_set_type_blt_obj_no
static int tolua_set_type_blt_obj_no(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'obj_no'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->obj_no = ((short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: sess_index of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_get_type_blt_unsigned_sess_index
static int tolua_get_type_blt_unsigned_sess_index(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sess_index'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->sess_index);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: sess_index of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_set_type_blt_unsigned_sess_index
static int tolua_set_type_blt_unsigned_sess_index(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sess_index'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->sess_index = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: proc_flg of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_get_type_blt_unsigned_proc_flg
static int tolua_get_type_blt_unsigned_proc_flg(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'proc_flg'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->proc_flg);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: proc_flg of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_set_type_blt_unsigned_proc_flg
static int tolua_set_type_blt_unsigned_proc_flg(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'proc_flg'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->proc_flg = ((unsigned short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: frame_count of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_get_type_blt_unsigned_frame_count
static int tolua_get_type_blt_unsigned_frame_count(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'frame_count'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->frame_count);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: frame_count of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_set_type_blt_unsigned_frame_count
static int tolua_set_type_blt_unsigned_frame_count(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'frame_count'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->frame_count = ((unsigned short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: turn_count of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_get_type_blt_unsigned_turn_count
static int tolua_get_type_blt_unsigned_turn_count(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'turn_count'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->turn_count);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: turn_count of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_set_type_blt_unsigned_turn_count
static int tolua_set_type_blt_unsigned_turn_count(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'turn_count'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->turn_count = ((unsigned short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: scrinfo of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_get_type_blt_scrinfo_ptr
static int tolua_get_type_blt_scrinfo_ptr(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'scrinfo'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)self->scrinfo,"TCHARA_SCR_INFO");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: scrinfo of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_set_type_blt_scrinfo_ptr
static int tolua_set_type_blt_scrinfo_ptr(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'scrinfo'",NULL);
  if (!tolua_isusertype(tolua_S,2,"TCHARA_SCR_INFO",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->scrinfo = ((TCHARA_SCR_INFO*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: extdata1 of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_get_type_blt_unsigned_extdata1
static int tolua_get_type_blt_unsigned_extdata1(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'extdata1'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->extdata1);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: extdata1 of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_set_type_blt_unsigned_extdata1
static int tolua_set_type_blt_unsigned_extdata1(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'extdata1'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->extdata1 = ((unsigned long)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: extdata2 of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_get_type_blt_unsigned_extdata2
static int tolua_get_type_blt_unsigned_extdata2(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'extdata2'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->extdata2);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: extdata2 of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_set_type_blt_unsigned_extdata2
static int tolua_set_type_blt_unsigned_extdata2(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'extdata2'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->extdata2 = ((unsigned long)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: option of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_get_thg_type_blt_option
static int tolua_get_thg_type_blt_option(lua_State* tolua_S)
{
 int tolua_index;
  type_blt* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (type_blt*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=OBJ_OPTION_COUNT)
  tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->option[tolua_index]);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: option of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_set_thg_type_blt_option
static int tolua_set_thg_type_blt_option(lua_State* tolua_S)
{
 int tolua_index;
  type_blt* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (type_blt*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=OBJ_OPTION_COUNT)
  tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
  self->option[tolua_index] = ((unsigned long)  tolua_tonumber(tolua_S,3,0));
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: proc_type of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_get_type_blt_unsigned_proc_type
static int tolua_get_type_blt_unsigned_proc_type(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'proc_type'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->proc_type);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: proc_type of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_set_type_blt_unsigned_proc_type
static int tolua_set_type_blt_unsigned_proc_type(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'proc_type'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->proc_type = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: hit_range of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_get_type_blt_unsigned_hit_range
static int tolua_get_type_blt_unsigned_hit_range(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'hit_range'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->hit_range);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: hit_range of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_set_type_blt_unsigned_hit_range
static int tolua_set_type_blt_unsigned_hit_range(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'hit_range'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->hit_range = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: chr_obj_no of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_get_type_blt_chr_obj_no
static int tolua_get_type_blt_chr_obj_no(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'chr_obj_no'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->chr_obj_no);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: chr_obj_no of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_set_type_blt_chr_obj_no
static int tolua_set_type_blt_chr_obj_no(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'chr_obj_no'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->chr_obj_no = ((short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: chara_type of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_get_type_blt_unsigned_chara_type
static int tolua_get_type_blt_unsigned_chara_type(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'chara_type'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->chara_type);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: chara_type of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_set_type_blt_unsigned_chara_type
static int tolua_set_type_blt_unsigned_chara_type(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'chara_type'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->chara_type = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: bullet_type of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_get_type_blt_unsigned_bullet_type
static int tolua_get_type_blt_unsigned_bullet_type(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bullet_type'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->bullet_type);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: bullet_type of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_set_type_blt_unsigned_bullet_type
static int tolua_set_type_blt_unsigned_bullet_type(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bullet_type'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->bullet_type = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: tex_index of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_get_type_blt_unsigned_tex_index
static int tolua_get_type_blt_unsigned_tex_index(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'tex_index'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->tex_index);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: tex_index of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_set_type_blt_unsigned_tex_index
static int tolua_set_type_blt_unsigned_tex_index(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'tex_index'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->tex_index = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: bx of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_get_type_blt_bx
static int tolua_get_type_blt_bx(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bx'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->bx);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: bx of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_set_type_blt_bx
static int tolua_set_type_blt_bx(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bx'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->bx = ((short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: by of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_get_type_blt_by
static int tolua_get_type_blt_by(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'by'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->by);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: by of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_set_type_blt_by
static int tolua_set_type_blt_by(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'by'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->by = ((short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: adx of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_get_type_blt_adx
static int tolua_get_type_blt_adx(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'adx'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->adx);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: adx of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_set_type_blt_adx
static int tolua_set_type_blt_adx(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'adx'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->adx = ((char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: ady of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_get_type_blt_ady
static int tolua_get_type_blt_ady(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ady'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->ady);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: ady of class  type_blt */
#ifndef TOLUA_DISABLE_tolua_set_type_blt_ady
static int tolua_set_type_blt_ady(lua_State* tolua_S)
{
  type_blt* self = (type_blt*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ady'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->ady = ((char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: x of class  type_ground */
#ifndef TOLUA_DISABLE_tolua_get_type_ground_x
static int tolua_get_type_ground_x(lua_State* tolua_S)
{
  type_ground* self = (type_ground*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: x of class  type_ground */
#ifndef TOLUA_DISABLE_tolua_set_type_ground_x
static int tolua_set_type_ground_x(lua_State* tolua_S)
{
  type_ground* self = (type_ground*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->x = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: y of class  type_ground */
#ifndef TOLUA_DISABLE_tolua_get_type_ground_y
static int tolua_get_type_ground_y(lua_State* tolua_S)
{
  type_ground* self = (type_ground*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: y of class  type_ground */
#ifndef TOLUA_DISABLE_tolua_set_type_ground_y
static int tolua_set_type_ground_y(lua_State* tolua_S)
{
  type_ground* self = (type_ground*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->y = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: ground of class  type_ground */
#ifndef TOLUA_DISABLE_tolua_get_type_ground_ground
static int tolua_get_type_ground_ground(lua_State* tolua_S)
{
  type_ground* self = (type_ground*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ground'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->ground);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: ground of class  type_ground */
#ifndef TOLUA_DISABLE_tolua_set_type_ground_ground
static int tolua_set_type_ground_ground(lua_State* tolua_S)
{
  type_ground* self = (type_ground*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ground'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->ground = ((bool)  tolua_toboolean(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_CreateBullet */
#ifndef TOLUA_DISABLE_tolua_thg_C_CreateBullet00
static int tolua_thg_C_CreateBullet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,8,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,9,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,10,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,11,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,12,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,13,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,14,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,15,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double proc_type = ((double)  tolua_tonumber(tolua_S,1,0));
  double chr_obj_no = ((double)  tolua_tonumber(tolua_S,2,0));
  double chara_type = ((double)  tolua_tonumber(tolua_S,3,0));
  double blt_type = ((double)  tolua_tonumber(tolua_S,4,0));
  double obj_type = ((double)  tolua_tonumber(tolua_S,5,0));
  double posx = ((double)  tolua_tonumber(tolua_S,6,0));
  double posy = ((double)  tolua_tonumber(tolua_S,7,0));
  double vecx = ((double)  tolua_tonumber(tolua_S,8,0));
  double vecy = ((double)  tolua_tonumber(tolua_S,9,0));
  double addx = ((double)  tolua_tonumber(tolua_S,10,0));
  double addy = ((double)  tolua_tonumber(tolua_S,11,0));
  double hit_range = ((double)  tolua_tonumber(tolua_S,12,0));
  double extdata1 = ((double)  tolua_tonumber(tolua_S,13,0));
  double extdata2 = ((double)  tolua_tonumber(tolua_S,14,0));
  {
   int tolua_ret = (int)  C_CreateBullet(proc_type,chr_obj_no,chara_type,blt_type,obj_type,posx,posy,vecx,vecy,addx,addy,hit_range,extdata1,extdata2);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_CreateBullet'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_Shot */
#ifndef TOLUA_DISABLE_tolua_thg_C_Shot00
static int tolua_thg_C_Shot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,8,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,9,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,10,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int chr_obj_no = ((int)  tolua_tonumber(tolua_S,1,0));
  int proc_type = ((int)  tolua_tonumber(tolua_S,2,0));
  int chr_id = ((int)  tolua_tonumber(tolua_S,3,0));
  int blt_type = ((int)  tolua_tonumber(tolua_S,4,0));
  int vec_angle = ((int)  tolua_tonumber(tolua_S,5,0));
  int power = ((int)  tolua_tonumber(tolua_S,6,0));
  int frame = ((int)  tolua_tonumber(tolua_S,7,0));
  int indicator_angle = ((int)  tolua_tonumber(tolua_S,8,0));
  int indicator_power = ((int)  tolua_tonumber(tolua_S,9,0));
  {
   bool tolua_ret = (bool)  C_Shot(chr_obj_no,proc_type,chr_id,blt_type,vec_angle,power,frame,indicator_angle,indicator_power);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_Shot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_RemoveBullet */
#ifndef TOLUA_DISABLE_tolua_thg_C_RemoveBullet00
static int tolua_thg_C_RemoveBullet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double obj_no = ((double)  tolua_tonumber(tolua_S,1,0));
  int rm_type = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   bool tolua_ret = (bool)  C_RemoveBullet(obj_no,rm_type);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_RemoveBullet'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_IsRemovedObject */
#ifndef TOLUA_DISABLE_tolua_thg_C_IsRemovedObject00
static int tolua_thg_C_IsRemovedObject00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double obj_no = ((double)  tolua_tonumber(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  C_IsRemovedObject(obj_no);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_IsRemovedObject'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_BombObject */
#ifndef TOLUA_DISABLE_tolua_thg_C_BombObject00
static int tolua_thg_C_BombObject00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int scr_id = ((int)  tolua_tonumber(tolua_S,1,0));
  int blt_type = ((int)  tolua_tonumber(tolua_S,2,0));
  int blt_chr_no = ((int)  tolua_tonumber(tolua_S,3,0));
  int blt_no = ((int)  tolua_tonumber(tolua_S,4,0));
  int pos_x = ((int)  tolua_tonumber(tolua_S,5,0));
  int pos_y = ((int)  tolua_tonumber(tolua_S,6,0));
  int erase = ((int)  tolua_tonumber(tolua_S,7,1));
  {
   bool tolua_ret = (bool)  C_BombObject(scr_id,blt_type,blt_chr_no,blt_no,pos_x,pos_y,erase);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_BombObject'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_DamageCharaHP */
#ifndef TOLUA_DISABLE_tolua_thg_C_DamageCharaHP00
static int tolua_thg_C_DamageCharaHP00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int assailant_no = ((int)  tolua_tonumber(tolua_S,1,0));
  int victim_no = ((int)  tolua_tonumber(tolua_S,2,0));
  int hp = ((int)  tolua_tonumber(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  C_DamageCharaHP(assailant_no,victim_no,hp);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_DamageCharaHP'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_UpdateCharaStatus */
#ifndef TOLUA_DISABLE_tolua_thg_C_UpdateCharaStatus00
static int tolua_thg_C_UpdateCharaStatus00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int obj_no = ((int)  tolua_tonumber(tolua_S,1,0));
  int hp = ((int)  tolua_tonumber(tolua_S,2,0));
  int mv = ((int)  tolua_tonumber(tolua_S,3,0));
  int delay = ((int)  tolua_tonumber(tolua_S,4,0));
  int exp = ((int)  tolua_tonumber(tolua_S,5,0));
  {
   bool tolua_ret = (bool)  C_UpdateCharaStatus(obj_no,hp,mv,delay,exp);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_UpdateCharaStatus'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_UpdateCharaPos */
#ifndef TOLUA_DISABLE_tolua_thg_C_UpdateCharaPos00
static int tolua_thg_C_UpdateCharaPos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int obj_no = ((int)  tolua_tonumber(tolua_S,1,0));
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  C_UpdateCharaPos(obj_no,x,y);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_UpdateCharaPos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetBulletOptionData */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetBulletOptionData00
static int tolua_thg_C_SetBulletOptionData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int obj_no = ((int)  tolua_tonumber(tolua_S,1,0));
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  int data = ((int)  tolua_tonumber(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  C_SetBulletOptionData(obj_no,index,data);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetBulletOptionData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_GetBulletOptionData */
#ifndef TOLUA_DISABLE_tolua_thg_C_GetBulletOptionData00
static int tolua_thg_C_GetBulletOptionData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int obj_no = ((int)  tolua_tonumber(tolua_S,1,0));
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   unsigned int tolua_ret = (unsigned int)  C_GetBulletOptionData(obj_no,index);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_GetBulletOptionData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetCharaOptionData */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetCharaOptionData00
static int tolua_thg_C_SetCharaOptionData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int obj_no = ((int)  tolua_tonumber(tolua_S,1,0));
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  int data = ((int)  tolua_tonumber(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  C_SetCharaOptionData(obj_no,index,data);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetCharaOptionData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_GetCharaOptionData */
#ifndef TOLUA_DISABLE_tolua_thg_C_GetCharaOptionData00
static int tolua_thg_C_GetCharaOptionData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int obj_no = ((int)  tolua_tonumber(tolua_S,1,0));
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   unsigned int tolua_ret = (unsigned int)  C_GetCharaOptionData(obj_no,index);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_GetCharaOptionData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetBulletExtData1 */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetBulletExtData100
static int tolua_thg_C_SetBulletExtData100(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int obj_no = ((int)  tolua_tonumber(tolua_S,1,0));
  unsigned long extdata1 = ((unsigned long)  tolua_tonumber(tolua_S,2,0));
  {
   bool tolua_ret = (bool)  C_SetBulletExtData1(obj_no,extdata1);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetBulletExtData1'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetBulletExtData2 */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetBulletExtData200
static int tolua_thg_C_SetBulletExtData200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int obj_no = ((int)  tolua_tonumber(tolua_S,1,0));
  unsigned long extdata2 = ((unsigned long)  tolua_tonumber(tolua_S,2,0));
  {
   bool tolua_ret = (bool)  C_SetBulletExtData2(obj_no,extdata2);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetBulletExtData2'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_GetCharaExtData1 */
#ifndef TOLUA_DISABLE_tolua_thg_C_GetCharaExtData100
static int tolua_thg_C_GetCharaExtData100(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int obj_no = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   unsigned int tolua_ret = (unsigned int)  C_GetCharaExtData1(obj_no);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_GetCharaExtData1'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_GetCharaExtData2 */
#ifndef TOLUA_DISABLE_tolua_thg_C_GetCharaExtData200
static int tolua_thg_C_GetCharaExtData200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int obj_no = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   unsigned int tolua_ret = (unsigned int)  C_GetCharaExtData2(obj_no);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_GetCharaExtData2'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetCharaExtData1 */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetCharaExtData100
static int tolua_thg_C_SetCharaExtData100(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int obj_no = ((int)  tolua_tonumber(tolua_S,1,0));
  unsigned long extdata1 = ((unsigned long)  tolua_tonumber(tolua_S,2,0));
  {
   bool tolua_ret = (bool)  C_SetCharaExtData1(obj_no,extdata1);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetCharaExtData1'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetCharaExtData2 */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetCharaExtData200
static int tolua_thg_C_SetCharaExtData200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int obj_no = ((int)  tolua_tonumber(tolua_S,1,0));
  unsigned long extdata2 = ((unsigned long)  tolua_tonumber(tolua_S,2,0));
  {
   bool tolua_ret = (bool)  C_SetCharaExtData2(obj_no,extdata2);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetCharaExtData2'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_AddCharaItem */
#ifndef TOLUA_DISABLE_tolua_thg_C_AddCharaItem00
static int tolua_thg_C_AddCharaItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int obj_no = ((int)  tolua_tonumber(tolua_S,1,0));
  int item_flg = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   bool tolua_ret = (bool)  C_AddCharaItem(obj_no,item_flg);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_AddCharaItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetCharaState */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetCharaState00
static int tolua_thg_C_SetCharaState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int obj_no = ((int)  tolua_tonumber(tolua_S,1,0));
  int chr_stt = ((int)  tolua_tonumber(tolua_S,2,0));
  int val = ((int)  tolua_tonumber(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  C_SetCharaState(obj_no,chr_stt,val);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetCharaState'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_GetCharaState */
#ifndef TOLUA_DISABLE_tolua_thg_C_GetCharaState00
static int tolua_thg_C_GetCharaState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int obj_no = ((int)  tolua_tonumber(tolua_S,1,0));
  int chr_stt = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   int tolua_ret = (int)  C_GetCharaState(obj_no,chr_stt);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_GetCharaState'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_GetCharaItem */
#ifndef TOLUA_DISABLE_tolua_thg_C_GetCharaItem00
static int tolua_thg_C_GetCharaItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int obj_no = ((int)  tolua_tonumber(tolua_S,1,0));
  int item_index = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   unsigned long tolua_ret = (unsigned long)  C_GetCharaItem(obj_no,item_index);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_GetCharaItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_UpdateBulletState */
#ifndef TOLUA_DISABLE_tolua_thg_C_UpdateBulletState00
static int tolua_thg_C_UpdateBulletState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double obj_no = ((double)  tolua_tonumber(tolua_S,1,0));
  double state = ((double)  tolua_tonumber(tolua_S,2,0));
  {
   bool tolua_ret = (bool)  C_UpdateBulletState(obj_no,state);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_UpdateBulletState'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_UpdateBulletPosition */
#ifndef TOLUA_DISABLE_tolua_thg_C_UpdateBulletPosition00
static int tolua_thg_C_UpdateBulletPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double obj_no = ((double)  tolua_tonumber(tolua_S,1,0));
  double px = ((double)  tolua_tonumber(tolua_S,2,0));
  double py = ((double)  tolua_tonumber(tolua_S,3,0));
  double vx = ((double)  tolua_tonumber(tolua_S,4,0));
  double vy = ((double)  tolua_tonumber(tolua_S,5,0));
  double adx = ((double)  tolua_tonumber(tolua_S,6,0));
  double ady = ((double)  tolua_tonumber(tolua_S,7,0));
  {
   bool tolua_ret = (bool)  C_UpdateBulletPosition(obj_no,px,py,vx,vy,adx,ady);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_UpdateBulletPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_UpdateBulletVector */
#ifndef TOLUA_DISABLE_tolua_thg_C_UpdateBulletVector00
static int tolua_thg_C_UpdateBulletVector00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double obj_no = ((double)  tolua_tonumber(tolua_S,1,0));
  double vx = ((double)  tolua_tonumber(tolua_S,2,0));
  double vy = ((double)  tolua_tonumber(tolua_S,3,0));
  double adx = ((double)  tolua_tonumber(tolua_S,4,0));
  double ady = ((double)  tolua_tonumber(tolua_S,5,0));
  {
   bool tolua_ret = (bool)  C_UpdateBulletVector(obj_no,vx,vy,adx,ady);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_UpdateBulletVector'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_UpdateObjectType */
#ifndef TOLUA_DISABLE_tolua_thg_C_UpdateObjectType00
static int tolua_thg_C_UpdateObjectType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int obj_no = ((int)  tolua_tonumber(tolua_S,1,0));
  int type = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   bool tolua_ret = (bool)  C_UpdateObjectType(obj_no,type);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_UpdateObjectType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_PasteTextureOnStage */
#ifndef TOLUA_DISABLE_tolua_thg_C_PasteTextureOnStage00
static int tolua_thg_C_PasteTextureOnStage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int scr_id = ((int)  tolua_tonumber(tolua_S,1,0));
  int sx = ((int)  tolua_tonumber(tolua_S,2,0));
  int sy = ((int)  tolua_tonumber(tolua_S,3,0));
  int tx = ((int)  tolua_tonumber(tolua_S,4,0));
  int ty = ((int)  tolua_tonumber(tolua_S,5,0));
  int tw = ((int)  tolua_tonumber(tolua_S,6,0));
  int th = ((int)  tolua_tonumber(tolua_S,7,0));
  {
   C_PasteTextureOnStage(scr_id,sx,sy,tx,ty,tw,th);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_PasteTextureOnStage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_GetRand */
#ifndef TOLUA_DISABLE_tolua_thg_C_GetRand00
static int tolua_thg_C_GetRand00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int min = ((int)  tolua_tonumber(tolua_S,1,0));
  int max = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   int tolua_ret = (int)  C_GetRand(min,max);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_GetRand'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_GetEntityCharacters */
#ifndef TOLUA_DISABLE_tolua_thg_C_GetEntityCharacters00
static int tolua_thg_C_GetEntityCharacters00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int tolua_ret = (int)  C_GetEntityCharacters();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_GetEntityCharacters'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_GetLivingCharacters */
#ifndef TOLUA_DISABLE_tolua_thg_C_GetLivingCharacters00
static int tolua_thg_C_GetLivingCharacters00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int tolua_ret = (int)  C_GetLivingCharacters();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_GetLivingCharacters'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_GetWindValue */
#ifndef TOLUA_DISABLE_tolua_thg_C_GetWindValue00
static int tolua_thg_C_GetWindValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int tolua_ret = (int)  C_GetWindValue();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_GetWindValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetWindValue */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetWindValue00
static int tolua_thg_C_SetWindValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int wind = ((int)  tolua_tonumber(tolua_S,1,0));
  int dir = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   C_SetWindValue(wind,dir);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetWindValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_GetStageWidth */
#ifndef TOLUA_DISABLE_tolua_thg_C_GetStageWidth00
static int tolua_thg_C_GetStageWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int tolua_ret = (int)  C_GetStageWidth();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_GetStageWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_GetStageHeight */
#ifndef TOLUA_DISABLE_tolua_thg_C_GetStageHeight00
static int tolua_thg_C_GetStageHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int tolua_ret = (int)  C_GetStageHeight();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_GetStageHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_GetCharacterAtVector */
#ifndef TOLUA_DISABLE_tolua_thg_C_GetCharacterAtVector00
static int tolua_thg_C_GetCharacterAtVector00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int index = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   type_session tolua_ret = (type_session)  C_GetCharacterAtVector(index);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((type_session)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"type_session");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(type_session));
     tolua_pushusertype(tolua_S,tolua_obj,"type_session");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_GetCharacterAtVector'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_GetCharacterFromObjNo */
#ifndef TOLUA_DISABLE_tolua_thg_C_GetCharacterFromObjNo00
static int tolua_thg_C_GetCharacterFromObjNo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int obj_no = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   type_session tolua_ret = (type_session)  C_GetCharacterFromObjNo(obj_no);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((type_session)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"type_session");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(type_session));
     tolua_pushusertype(tolua_S,tolua_obj,"type_session");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_GetCharacterFromObjNo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_GetBulletInfo */
#ifndef TOLUA_DISABLE_tolua_thg_C_GetBulletInfo00
static int tolua_thg_C_GetBulletInfo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int obj_no = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   type_blt tolua_ret = (type_blt)  C_GetBulletInfo(obj_no);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((type_blt)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"type_blt");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(type_blt));
     tolua_pushusertype(tolua_S,tolua_obj,"type_blt");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_GetBulletInfo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_GetBulletAtkValue */
#ifndef TOLUA_DISABLE_tolua_thg_C_GetBulletAtkValue00
static int tolua_thg_C_GetBulletAtkValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int blt_no = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   int tolua_ret = (int)  C_GetBulletAtkValue(blt_no);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_GetBulletAtkValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_GetBulletExtData1 */
#ifndef TOLUA_DISABLE_tolua_thg_C_GetBulletExtData100
static int tolua_thg_C_GetBulletExtData100(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int obj_no = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   unsigned int tolua_ret = (unsigned int)  C_GetBulletExtData1(obj_no);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_GetBulletExtData1'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_GetBulletExtData2 */
#ifndef TOLUA_DISABLE_tolua_thg_C_GetBulletExtData200
static int tolua_thg_C_GetBulletExtData200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int obj_no = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   unsigned int tolua_ret = (unsigned int)  C_GetBulletExtData2(obj_no);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_GetBulletExtData2'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_UpdateBulletAngle */
#ifndef TOLUA_DISABLE_tolua_thg_C_UpdateBulletAngle00
static int tolua_thg_C_UpdateBulletAngle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double blt_no = ((double)  tolua_tonumber(tolua_S,1,0));
  double angle = ((double)  tolua_tonumber(tolua_S,2,0));
  {
   bool tolua_ret = (bool)  C_UpdateBulletAngle(blt_no,angle);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_UpdateBulletAngle'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetBulletTextureIndex */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetBulletTextureIndex00
static int tolua_thg_C_SetBulletTextureIndex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double blt_no = ((double)  tolua_tonumber(tolua_S,1,0));
  double index = ((double)  tolua_tonumber(tolua_S,2,0));
  {
   bool tolua_ret = (bool)  C_SetBulletTextureIndex(blt_no,index);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetBulletTextureIndex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetCharaTexture */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetCharaTexture00
static int tolua_thg_C_SetCharaTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int chr_no = ((int)  tolua_tonumber(tolua_S,1,0));
  int left = ((int)  tolua_tonumber(tolua_S,2,0));
  int top = ((int)  tolua_tonumber(tolua_S,3,0));
  int right = ((int)  tolua_tonumber(tolua_S,4,0));
  int bottom = ((int)  tolua_tonumber(tolua_S,5,0));
  {
   bool tolua_ret = (bool)  C_SetCharaTexture(chr_no,left,top,right,bottom);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetCharaTexture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_AddEffect */
#ifndef TOLUA_DISABLE_tolua_thg_C_AddEffect00
static int tolua_thg_C_AddEffect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,8,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double chr_id = ((double)  tolua_tonumber(tolua_S,1,0));
  double left = ((double)  tolua_tonumber(tolua_S,2,0));
  double top = ((double)  tolua_tonumber(tolua_S,3,0));
  double right = ((double)  tolua_tonumber(tolua_S,4,0));
  double bottom = ((double)  tolua_tonumber(tolua_S,5,0));
  double x = ((double)  tolua_tonumber(tolua_S,6,0));
  double y = ((double)  tolua_tonumber(tolua_S,7,0));
  double age = ((double)  tolua_tonumber(tolua_S,8,0));
  {
   int tolua_ret = (int)  C_AddEffect(chr_id,left,top,right,bottom,x,y,age);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_AddEffect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_RemoveEffect */
#ifndef TOLUA_DISABLE_tolua_thg_C_RemoveEffect00
static int tolua_thg_C_RemoveEffect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double effect_no = ((double)  tolua_tonumber(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  C_RemoveEffect(effect_no);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_RemoveEffect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetEffectVector */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetEffectVector00
static int tolua_thg_C_SetEffectVector00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double effect_no = ((double)  tolua_tonumber(tolua_S,1,0));
  double vx = ((double)  tolua_tonumber(tolua_S,2,0));
  double vy = ((double)  tolua_tonumber(tolua_S,3,0));
  double ax = ((double)  tolua_tonumber(tolua_S,4,0));
  double ay = ((double)  tolua_tonumber(tolua_S,5,0));
  double effect_time = ((double)  tolua_tonumber(tolua_S,6,0));
  {
   bool tolua_ret = (bool)  C_SetEffectVector(effect_no,vx,vy,ax,ay,effect_time);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetEffectVector'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetEffectAlpha */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetEffectAlpha00
static int tolua_thg_C_SetEffectAlpha00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double effect_no = ((double)  tolua_tonumber(tolua_S,1,0));
  double alpha = ((double)  tolua_tonumber(tolua_S,2,0));
  {
   bool tolua_ret = (bool)  C_SetEffectAlpha(effect_no,alpha);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetEffectAlpha'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetEffectFade */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetEffectFade00
static int tolua_thg_C_SetEffectFade00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double effect_no = ((double)  tolua_tonumber(tolua_S,1,0));
  double fade = ((double)  tolua_tonumber(tolua_S,2,0));
  double effect_time = ((double)  tolua_tonumber(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  C_SetEffectFade(effect_no,fade,effect_time);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetEffectFade'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetEffectFadeInOut */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetEffectFadeInOut00
static int tolua_thg_C_SetEffectFadeInOut00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double effect_no = ((double)  tolua_tonumber(tolua_S,1,0));
  double fadeinout = ((double)  tolua_tonumber(tolua_S,2,0));
  {
   bool tolua_ret = (bool)  C_SetEffectFadeInOut(effect_no,fadeinout);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetEffectFadeInOut'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetEffectRotate */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetEffectRotate00
static int tolua_thg_C_SetEffectRotate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double effect_no = ((double)  tolua_tonumber(tolua_S,1,0));
  double rot = ((double)  tolua_tonumber(tolua_S,2,0));
  {
   bool tolua_ret = (bool)  C_SetEffectRotate(effect_no,rot);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetEffectRotate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetEffectRotation */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetEffectRotation00
static int tolua_thg_C_SetEffectRotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double effect_no = ((double)  tolua_tonumber(tolua_S,1,0));
  double rot = ((double)  tolua_tonumber(tolua_S,2,0));
  double effect_time = ((double)  tolua_tonumber(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  C_SetEffectRotation(effect_no,rot,effect_time);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetEffectRotation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetEffectScale */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetEffectScale00
static int tolua_thg_C_SetEffectScale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double effect_no = ((double)  tolua_tonumber(tolua_S,1,0));
  double scalex = ((double)  tolua_tonumber(tolua_S,2,0));
  double scaley = ((double)  tolua_tonumber(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  C_SetEffectScale(effect_no,scalex,scaley);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetEffectScale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetEffectScalling */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetEffectScalling00
static int tolua_thg_C_SetEffectScalling00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double effect_no = ((double)  tolua_tonumber(tolua_S,1,0));
  double scalex = ((double)  tolua_tonumber(tolua_S,2,0));
  double scaley = ((double)  tolua_tonumber(tolua_S,3,0));
  double effect_time = ((double)  tolua_tonumber(tolua_S,4,0));
  {
   bool tolua_ret = (bool)  C_SetEffectScalling(effect_no,scalex,scaley,effect_time);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetEffectScalling'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetEffectAnimation */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetEffectAnimation00
static int tolua_thg_C_SetEffectAnimation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double effect_no = ((double)  tolua_tonumber(tolua_S,1,0));
  double atime = ((double)  tolua_tonumber(tolua_S,2,0));
  double acount = ((double)  tolua_tonumber(tolua_S,3,0));
  bool loop = ((bool)  tolua_toboolean(tolua_S,4,0));
  double effect_time = ((double)  tolua_tonumber(tolua_S,5,0));
  {
   bool tolua_ret = (bool)  C_SetEffectAnimation(effect_no,atime,acount,loop,effect_time);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetEffectAnimation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetEffectTexture */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetEffectTexture00
static int tolua_thg_C_SetEffectTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int effect_no = ((int)  tolua_tonumber(tolua_S,1,0));
  double left = ((double)  tolua_tonumber(tolua_S,2,0));
  double top = ((double)  tolua_tonumber(tolua_S,3,0));
  double right = ((double)  tolua_tonumber(tolua_S,4,0));
  double bottom = ((double)  tolua_tonumber(tolua_S,5,0));
  {
   bool tolua_ret = (bool)  C_SetEffectTexture(effect_no,left,top,right,bottom);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetEffectTexture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_AddBGEffect */
#ifndef TOLUA_DISABLE_tolua_thg_C_AddBGEffect00
static int tolua_thg_C_AddBGEffect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,8,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double chr_id = ((double)  tolua_tonumber(tolua_S,1,0));
  double left = ((double)  tolua_tonumber(tolua_S,2,0));
  double top = ((double)  tolua_tonumber(tolua_S,3,0));
  double right = ((double)  tolua_tonumber(tolua_S,4,0));
  double bottom = ((double)  tolua_tonumber(tolua_S,5,0));
  double x = ((double)  tolua_tonumber(tolua_S,6,0));
  double y = ((double)  tolua_tonumber(tolua_S,7,0));
  double age = ((double)  tolua_tonumber(tolua_S,8,0));
  {
   int tolua_ret = (int)  C_AddBGEffect(chr_id,left,top,right,bottom,x,y,age);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_AddBGEffect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_RemoveBGEffect */
#ifndef TOLUA_DISABLE_tolua_thg_C_RemoveBGEffect00
static int tolua_thg_C_RemoveBGEffect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double effect_no = ((double)  tolua_tonumber(tolua_S,1,0));
  {
   bool tolua_ret = (bool)  C_RemoveBGEffect(effect_no);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_RemoveBGEffect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetBGEffectVector */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetBGEffectVector00
static int tolua_thg_C_SetBGEffectVector00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double effect_no = ((double)  tolua_tonumber(tolua_S,1,0));
  double vx = ((double)  tolua_tonumber(tolua_S,2,0));
  double vy = ((double)  tolua_tonumber(tolua_S,3,0));
  double ax = ((double)  tolua_tonumber(tolua_S,4,0));
  double ay = ((double)  tolua_tonumber(tolua_S,5,0));
  double effect_time = ((double)  tolua_tonumber(tolua_S,6,0));
  {
   bool tolua_ret = (bool)  C_SetBGEffectVector(effect_no,vx,vy,ax,ay,effect_time);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetBGEffectVector'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetBGEffectAlpha */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetBGEffectAlpha00
static int tolua_thg_C_SetBGEffectAlpha00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double effect_no = ((double)  tolua_tonumber(tolua_S,1,0));
  double alpha = ((double)  tolua_tonumber(tolua_S,2,0));
  {
   bool tolua_ret = (bool)  C_SetBGEffectAlpha(effect_no,alpha);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetBGEffectAlpha'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetBGEffectFade */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetBGEffectFade00
static int tolua_thg_C_SetBGEffectFade00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double effect_no = ((double)  tolua_tonumber(tolua_S,1,0));
  double fade = ((double)  tolua_tonumber(tolua_S,2,0));
  double effect_time = ((double)  tolua_tonumber(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  C_SetBGEffectFade(effect_no,fade,effect_time);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetBGEffectFade'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetBGEffectFadeInOut */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetBGEffectFadeInOut00
static int tolua_thg_C_SetBGEffectFadeInOut00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double effect_no = ((double)  tolua_tonumber(tolua_S,1,0));
  double fadeinout = ((double)  tolua_tonumber(tolua_S,2,0));
  {
   bool tolua_ret = (bool)  C_SetBGEffectFadeInOut(effect_no,fadeinout);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetBGEffectFadeInOut'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetBGEffectRotate */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetBGEffectRotate00
static int tolua_thg_C_SetBGEffectRotate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double effect_no = ((double)  tolua_tonumber(tolua_S,1,0));
  double rot = ((double)  tolua_tonumber(tolua_S,2,0));
  {
   bool tolua_ret = (bool)  C_SetBGEffectRotate(effect_no,rot);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetBGEffectRotate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetBGEffectRotation */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetBGEffectRotation00
static int tolua_thg_C_SetBGEffectRotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double effect_no = ((double)  tolua_tonumber(tolua_S,1,0));
  double rot = ((double)  tolua_tonumber(tolua_S,2,0));
  double effect_time = ((double)  tolua_tonumber(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  C_SetBGEffectRotation(effect_no,rot,effect_time);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetBGEffectRotation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetBGEffectScale */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetBGEffectScale00
static int tolua_thg_C_SetBGEffectScale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double effect_no = ((double)  tolua_tonumber(tolua_S,1,0));
  double scalex = ((double)  tolua_tonumber(tolua_S,2,0));
  double scaley = ((double)  tolua_tonumber(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  C_SetBGEffectScale(effect_no,scalex,scaley);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetBGEffectScale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetBGEffectScalling */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetBGEffectScalling00
static int tolua_thg_C_SetBGEffectScalling00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double effect_no = ((double)  tolua_tonumber(tolua_S,1,0));
  double scalex = ((double)  tolua_tonumber(tolua_S,2,0));
  double scaley = ((double)  tolua_tonumber(tolua_S,3,0));
  double effect_time = ((double)  tolua_tonumber(tolua_S,4,0));
  {
   bool tolua_ret = (bool)  C_SetBGEffectScalling(effect_no,scalex,scaley,effect_time);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetBGEffectScalling'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetBGEffectAnimation */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetBGEffectAnimation00
static int tolua_thg_C_SetBGEffectAnimation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double effect_no = ((double)  tolua_tonumber(tolua_S,1,0));
  double atime = ((double)  tolua_tonumber(tolua_S,2,0));
  double acount = ((double)  tolua_tonumber(tolua_S,3,0));
  bool loop = ((bool)  tolua_toboolean(tolua_S,4,0));
  double effect_time = ((double)  tolua_tonumber(tolua_S,5,0));
  {
   bool tolua_ret = (bool)  C_SetBGEffectAnimation(effect_no,atime,acount,loop,effect_time);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetBGEffectAnimation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetBGEffectTexture */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetBGEffectTexture00
static int tolua_thg_C_SetBGEffectTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int effect_no = ((int)  tolua_tonumber(tolua_S,1,0));
  double left = ((double)  tolua_tonumber(tolua_S,2,0));
  double top = ((double)  tolua_tonumber(tolua_S,3,0));
  double right = ((double)  tolua_tonumber(tolua_S,4,0));
  double bottom = ((double)  tolua_tonumber(tolua_S,5,0));
  {
   bool tolua_ret = (bool)  C_SetBGEffectTexture(effect_no,left,top,right,bottom);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetBGEffectTexture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetCameraFocusToBullet */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetCameraFocusToBullet00
static int tolua_thg_C_SetCameraFocusToBullet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int blt_no = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   C_SetCameraFocusToBullet(blt_no);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetCameraFocusToBullet'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_SetCameraFocusToChara */
#ifndef TOLUA_DISABLE_tolua_thg_C_SetCameraFocusToChara00
static int tolua_thg_C_SetCameraFocusToChara00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int obj_no = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   C_SetCameraFocusToChara(obj_no);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_SetCameraFocusToChara'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_HideStage */
#ifndef TOLUA_DISABLE_tolua_thg_C_HideStage00
static int tolua_thg_C_HideStage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   C_HideStage();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_HideStage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_ShowStage */
#ifndef TOLUA_DISABLE_tolua_thg_C_ShowStage00
static int tolua_thg_C_ShowStage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   C_ShowStage();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_ShowStage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_RegistSoundSE */
#ifndef TOLUA_DISABLE_tolua_thg_C_RegistSoundSE00
static int tolua_thg_C_RegistSoundSE00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  char* rc_name = ((char*)  tolua_tostring(tolua_S,1,0));
  {
   C_RegistSoundSE(rc_name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_RegistSoundSE'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_PlaySoundSE */
#ifndef TOLUA_DISABLE_tolua_thg_C_PlaySoundSE00
static int tolua_thg_C_PlaySoundSE00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  char* rc_name = ((char*)  tolua_tostring(tolua_S,1,0));
  int loop = ((int)  tolua_tonumber(tolua_S,2,0));
  double fade = ((double)  tolua_tonumber(tolua_S,3,0));
  {
   C_PlaySoundSE(rc_name,loop,fade);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_PlaySoundSE'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_GetMyCharaNo */
#ifndef TOLUA_DISABLE_tolua_thg_C_GetMyCharaNo00
static int tolua_thg_C_GetMyCharaNo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int tolua_ret = (int)  C_GetMyCharaNo();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_GetMyCharaNo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_AddMsgLog */
#ifndef TOLUA_DISABLE_tolua_thg_C_AddMsgLog00
static int tolua_thg_C_AddMsgLog00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  char* str = ((char*)  tolua_tostring(tolua_S,1,0));
  {
   C_AddMsgLog(str);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_AddMsgLog'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_GetAngle */
#ifndef TOLUA_DISABLE_tolua_thg_C_GetAngle00
static int tolua_thg_C_GetAngle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int x = ((int)  tolua_tonumber(tolua_S,1,0));
  int y = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   int tolua_ret = (int)  C_GetAngle(x,y);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_GetAngle'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_GetScrIDFromChrNo */
#ifndef TOLUA_DISABLE_tolua_thg_C_GetScrIDFromChrNo00
static int tolua_thg_C_GetScrIDFromChrNo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int obj_no = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   int tolua_ret = (int)  C_GetScrIDFromChrNo(obj_no);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_GetScrIDFromChrNo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_GetUserTeamNo */
#ifndef TOLUA_DISABLE_tolua_thg_C_GetUserTeamNo00
static int tolua_thg_C_GetUserTeamNo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int obj_no = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   int tolua_ret = (int)  C_GetUserTeamNo(obj_no);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_GetUserTeamNo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_GetUserName */
#ifndef TOLUA_DISABLE_tolua_thg_C_GetUserName00
static int tolua_thg_C_GetUserName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int obj_no = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   char* tolua_ret = (char*)  C_GetUserName(obj_no);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_GetUserName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_DbgOutputStr */
#ifndef TOLUA_DISABLE_tolua_thg_C_DbgOutputStr00
static int tolua_thg_C_DbgOutputStr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  char* str = ((char*)  tolua_tostring(tolua_S,1,0));
  {
   C_DbgOutputStr(str);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_DbgOutputStr'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_DbgOutputNum */
#ifndef TOLUA_DISABLE_tolua_thg_C_DbgOutputNum00
static int tolua_thg_C_DbgOutputNum00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int num = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   C_DbgOutputNum(num);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_DbgOutputNum'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_DbgAddLogNum */
#ifndef TOLUA_DISABLE_tolua_thg_C_DbgAddLogNum00
static int tolua_thg_C_DbgAddLogNum00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int num = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   C_DbgAddLogNum(num);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_DbgAddLogNum'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_GetRandomStagePos */
#ifndef TOLUA_DISABLE_tolua_thg_C_GetRandomStagePos00
static int tolua_thg_C_GetRandomStagePos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   type_ground tolua_ret = (type_ground)  C_GetRandomStagePos();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((type_ground)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"type_ground");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(type_ground));
     tolua_pushusertype(tolua_S,tolua_obj,"type_ground");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_GetRandomStagePos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: C_GetGroundPos */
#ifndef TOLUA_DISABLE_tolua_thg_C_GetGroundPos00
static int tolua_thg_C_GetGroundPos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int x = ((int)  tolua_tonumber(tolua_S,1,0));
  int y = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   type_ground tolua_ret = (type_ground)  C_GetGroundPos(x,y);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((type_ground)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"type_ground");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(type_ground));
     tolua_pushusertype(tolua_S,tolua_obj,"type_ground");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'C_GetGroundPos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_thg_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_constant(tolua_S,"MAX_SHOT_INDICATOR",MAX_SHOT_INDICATOR);
  tolua_constant(tolua_S,"CHARA_FACE_FINE",CHARA_FACE_FINE);
  tolua_constant(tolua_S,"CHARA_FACE_NORMAL",CHARA_FACE_NORMAL);
  tolua_constant(tolua_S,"CHARA_FACE_HURT",CHARA_FACE_HURT);
  tolua_constant(tolua_S,"BLT_PROC_TYPE_SCR_CHARA",BLT_PROC_TYPE_SCR_CHARA);
  tolua_constant(tolua_S,"BLT_PROC_TYPE_SCR_STAGE",BLT_PROC_TYPE_SCR_STAGE);
  tolua_constant(tolua_S,"BLT_PROC_TYPE_SCR_SPELL",BLT_PROC_TYPE_SCR_SPELL);
  tolua_constant(tolua_S,"BLT_PROC_TYPE_ITEM",BLT_PROC_TYPE_ITEM);
  tolua_constant(tolua_S,"DEF_BLT_TYPE_SPELL",DEF_BLT_TYPE_SPELL);
  tolua_constant(tolua_S,"DEF_BLT_TRG_TYPE_START_INDEX",DEF_BLT_TRG_TYPE_START_INDEX);
  tolua_constant(tolua_S,"DEF_BLT_TRG_TYPE_REVERSE",DEF_BLT_TRG_TYPE_REVERSE);
  tolua_constant(tolua_S,"DEF_BLT_TRG_TYPE_BLIND",DEF_BLT_TRG_TYPE_BLIND);
  tolua_constant(tolua_S,"DEF_BLT_TRG_TYPE_REPAIR",DEF_BLT_TRG_TYPE_REPAIR);
  tolua_constant(tolua_S,"DEF_BLT_TRG_TYPE_TELEPORT",DEF_BLT_TRG_TYPE_TELEPORT);
  tolua_constant(tolua_S,"DEF_BLT_TRG_TYPE_DRAIN",DEF_BLT_TRG_TYPE_DRAIN);
  tolua_constant(tolua_S,"DEF_BLT_TRG_TYPE_FETCH",DEF_BLT_TRG_TYPE_FETCH);
  tolua_constant(tolua_S,"DEF_BLT_TRG_TYPE_EXCHANGE",DEF_BLT_TRG_TYPE_EXCHANGE);
  tolua_constant(tolua_S,"DEF_BLT_TRG_TYPE_NOANGLE",DEF_BLT_TRG_TYPE_NOANGLE);
  tolua_constant(tolua_S,"DEF_BLT_TRG_TYPE_NOMOVE",DEF_BLT_TRG_TYPE_NOMOVE);
  tolua_constant(tolua_S,"STAGE_OBJ_NO",STAGE_OBJ_NO);
  tolua_constant(tolua_S,"DEF_STAGE_OBJ_NO",DEF_STAGE_OBJ_NO);
  tolua_constant(tolua_S,"DEF_STAGE_ID",DEF_STAGE_ID);
  tolua_constant(tolua_S,"DEF_SYSTEM_ID",DEF_SYSTEM_ID);
  tolua_constant(tolua_S,"OBJ_TYPE_BLT",OBJ_TYPE_BLT);
  tolua_constant(tolua_S,"OBJ_TYPE_SOLID",OBJ_TYPE_SOLID);
  tolua_constant(tolua_S,"OBJ_TYPE_GAS",OBJ_TYPE_GAS);
  tolua_constant(tolua_S,"OBJ_TYPE_LIQUID",OBJ_TYPE_LIQUID);
  tolua_constant(tolua_S,"OBJ_TYPE_STAGE",OBJ_TYPE_STAGE);
  tolua_constant(tolua_S,"OBJ_TYPE_TACTIC",OBJ_TYPE_TACTIC);
  tolua_constant(tolua_S,"OBJ_TYPE_SLIDE",OBJ_TYPE_SLIDE);
  tolua_constant(tolua_S,"OBJ_STATE_MAIN_WAIT_FLG",OBJ_STATE_MAIN_WAIT_FLG);
  tolua_constant(tolua_S,"OBJ_STATE_MAIN_ACTIVE_FLG",OBJ_STATE_MAIN_ACTIVE_FLG);
  tolua_constant(tolua_S,"OBJ_STATE_MAIN_DEAD_FLG",OBJ_STATE_MAIN_DEAD_FLG);
  tolua_constant(tolua_S,"OBJ_STATE_MAIN_DROP_FLG",OBJ_STATE_MAIN_DROP_FLG);
  tolua_constant(tolua_S,"OBJ_STATE_MAIN_PAUSE_FLG",OBJ_STATE_MAIN_PAUSE_FLG);
  tolua_constant(tolua_S,"OBJ_STATE_MAIN_SHOTPOWER_FLG",OBJ_STATE_MAIN_SHOTPOWER_FLG);
  tolua_constant(tolua_S,"OBJ_STATE_MAIN_TRIGGER_BULLET_FLG",OBJ_STATE_MAIN_TRIGGER_BULLET_FLG);
  tolua_constant(tolua_S,"OBJ_STATE_MAIN_TRIGGER_SPELL_FLG",OBJ_STATE_MAIN_TRIGGER_SPELL_FLG);
  tolua_constant(tolua_S,"OBJ_STATE_MAIN_SHOOTING_FLG",OBJ_STATE_MAIN_SHOOTING_FLG);
  tolua_constant(tolua_S,"OBJ_STATE_MAIN_SHOT_FLG",OBJ_STATE_MAIN_SHOT_FLG);
  tolua_constant(tolua_S,"OBJ_STATE_MAIN_GALLERY_FLG",OBJ_STATE_MAIN_GALLERY_FLG);
  tolua_constant(tolua_S,"DEF_STATE_EMPTY",DEF_STATE_EMPTY);
  tolua_constant(tolua_S,"DEF_STATE_WAIT",DEF_STATE_WAIT);
  tolua_constant(tolua_S,"DEF_STATE_ACTIVE",DEF_STATE_ACTIVE);
  tolua_constant(tolua_S,"DEF_STATE_DEAD",DEF_STATE_DEAD);
  tolua_constant(tolua_S,"DEF_STATE_DROP",DEF_STATE_DROP);
  tolua_constant(tolua_S,"DEF_STATE_SHOTPOWER",DEF_STATE_SHOTPOWER);
  tolua_constant(tolua_S,"DEF_STATE_TRIGGER_BULLET",DEF_STATE_TRIGGER_BULLET);
  tolua_constant(tolua_S,"DEF_STATE_TRIGGER_SPELL",DEF_STATE_TRIGGER_SPELL);
  tolua_constant(tolua_S,"DEF_STATE_SHOT",DEF_STATE_SHOT);
  tolua_constant(tolua_S,"DEF_STATE_GALLERY",DEF_STATE_GALLERY);
  tolua_constant(tolua_S,"OBJ_STATE_NONE",OBJ_STATE_NONE);
  tolua_constant(tolua_S,"OBJ_STATE_ROOM",OBJ_STATE_ROOM);
  tolua_constant(tolua_S,"OBJ_STATE_LOAD",OBJ_STATE_LOAD);
  tolua_constant(tolua_S,"OBJ_STATE_GAME",OBJ_STATE_GAME);
  tolua_constant(tolua_S,"OBJ_STATE_RESULT",OBJ_STATE_RESULT);
  tolua_constant(tolua_S,"DEF_MAX_SHOT_POWER",DEF_MAX_SHOT_POWER);
  tolua_constant(tolua_S,"FPS",FPS);
  tolua_constant(tolua_S,"FPSO",FPSO);
  tolua_constant(tolua_S,"MAX_WIND_VALUE",MAX_WIND_VALUE);
  tolua_constant(tolua_S,"MIN_WIND_VALUE",MIN_WIND_VALUE);
  tolua_constant(tolua_S,"GAME_ITEM_MOVE_UP",GAME_ITEM_MOVE_UP);
  tolua_constant(tolua_S,"GAME_ITEM_DOUBLE_SHOT",GAME_ITEM_DOUBLE_SHOT);
  tolua_constant(tolua_S,"GAME_ITEM_STEALTH",GAME_ITEM_STEALTH);
  tolua_constant(tolua_S,"GAME_ITEM_REVERSE",GAME_ITEM_REVERSE);
  tolua_constant(tolua_S,"GAME_ITEM_BLIND",GAME_ITEM_BLIND);
  tolua_constant(tolua_S,"GAME_ITEM_SHIELD",GAME_ITEM_SHIELD);
  tolua_constant(tolua_S,"GAME_ITEM_POWER",GAME_ITEM_POWER);
  tolua_constant(tolua_S,"GAME_ITEM_NOANGLE",GAME_ITEM_NOANGLE);
  tolua_constant(tolua_S,"GAME_ITEM_NOMOVE",GAME_ITEM_NOMOVE);
  tolua_constant(tolua_S,"GAME_ITEM_REPAIR",GAME_ITEM_REPAIR);
  tolua_constant(tolua_S,"GAME_ITEM_REPAIR_BULLET",GAME_ITEM_REPAIR_BULLET);
  tolua_constant(tolua_S,"GAME_ITEM_TELEPORT",GAME_ITEM_TELEPORT);
  tolua_constant(tolua_S,"GAME_ITEM_REPAIR_BIG",GAME_ITEM_REPAIR_BIG);
  tolua_constant(tolua_S,"GAME_ITEM_REPAIR_TEAM",GAME_ITEM_REPAIR_TEAM);
  tolua_constant(tolua_S,"GAME_ITEM_DRAIN",GAME_ITEM_DRAIN);
  tolua_constant(tolua_S,"GAME_ITEM_FETCH",GAME_ITEM_FETCH);
  tolua_constant(tolua_S,"GAME_ITEM_EXCHANGE",GAME_ITEM_EXCHANGE);
  tolua_constant(tolua_S,"GAME_ITEM_WIND_CHANGE",GAME_ITEM_WIND_CHANGE);
  tolua_constant(tolua_S,"GAME_ITEM_STEAL",GAME_ITEM_STEAL);
  tolua_constant(tolua_S,"CHARA_STATE_STEALTH",CHARA_STATE_STEALTH);
  tolua_constant(tolua_S,"CHARA_STATE_BLIND",CHARA_STATE_BLIND);
  tolua_constant(tolua_S,"CHARA_STATE_PAIN",CHARA_STATE_PAIN);
  tolua_constant(tolua_S,"CHARA_STATE_SHIELD",CHARA_STATE_SHIELD);
  tolua_constant(tolua_S,"CHARA_STATE_POWER",CHARA_STATE_POWER);
  tolua_constant(tolua_S,"CHARA_STATE_UPMOVE",CHARA_STATE_UPMOVE);
  tolua_constant(tolua_S,"CHARA_STATE_NOMOVE",CHARA_STATE_NOMOVE);
  tolua_constant(tolua_S,"CHARA_STATE_NOANGLE",CHARA_STATE_NOANGLE);
  tolua_constant(tolua_S,"CHARA_STATE_DOUBLE",CHARA_STATE_DOUBLE);
  tolua_constant(tolua_S,"CHARA_STATE_REVERSE",CHARA_STATE_REVERSE);
  tolua_constant(tolua_S,"CHARA_STATE_ITEM_REVERSE",CHARA_STATE_ITEM_REVERSE);
  tolua_constant(tolua_S,"CHARA_STATE_ITEM_BLIND",CHARA_STATE_ITEM_BLIND);
  tolua_constant(tolua_S,"CHARA_STATE_ITEM_REPAIRBLT",CHARA_STATE_ITEM_REPAIRBLT);
  tolua_constant(tolua_S,"CHARA_STATE_ITEM_TELEPORT",CHARA_STATE_ITEM_TELEPORT);
  tolua_constant(tolua_S,"CHARA_STATE_ITEM_DRAIN",CHARA_STATE_ITEM_DRAIN);
  tolua_constant(tolua_S,"CHARA_STATE_ITEM_FETCH",CHARA_STATE_ITEM_FETCH);
  tolua_constant(tolua_S,"CHARA_STATE_ITEM_EXCHANGE",CHARA_STATE_ITEM_EXCHANGE);
  tolua_constant(tolua_S,"CHARA_STATE_ITEM_NOANGLE",CHARA_STATE_ITEM_NOANGLE);
  tolua_constant(tolua_S,"CHARA_STATE_ITEM_NOMOVE",CHARA_STATE_ITEM_NOMOVE);
  tolua_constant(tolua_S,"CHARA_STATE_ITEM_STEAL",CHARA_STATE_ITEM_STEAL);
  tolua_constant(tolua_S,"LOBBY_W",LOBBY_W);
  tolua_constant(tolua_S,"LOBBY_WATCHER_SPOT_W",LOBBY_WATCHER_SPOT_W);
  tolua_constant(tolua_S,"LOBBY_H",LOBBY_H);
  tolua_constant(tolua_S,"OBJ_STATE_EMPTY",OBJ_STATE_EMPTY);
  tolua_constant(tolua_S,"OBJ_STATE_ROOM_READY",OBJ_STATE_ROOM_READY);
  tolua_constant(tolua_S,"OBJ_STATE_ROOM_MOVE",OBJ_STATE_ROOM_MOVE);
  tolua_constant(tolua_S,"OBJ_STATE_ROOM_READY_OK",OBJ_STATE_ROOM_READY_OK);
  tolua_constant(tolua_S,"OBJ_STATE_LOADING",OBJ_STATE_LOADING);
  tolua_constant(tolua_S,"OBJ_STATE_LOADCOMPLETE",OBJ_STATE_LOADCOMPLETE);
  tolua_constant(tolua_S,"OBJ_STATE_MAIN_PAUSE",OBJ_STATE_MAIN_PAUSE);
  tolua_constant(tolua_S,"OBJ_STATE_MAIN_WAIT",OBJ_STATE_MAIN_WAIT);
  tolua_constant(tolua_S,"OBJ_STATE_MAIN_ACTIVE",OBJ_STATE_MAIN_ACTIVE);
  tolua_constant(tolua_S,"OBJ_STATE_MAIN_DEAD",OBJ_STATE_MAIN_DEAD);
  tolua_constant(tolua_S,"OBJ_STATE_MAIN_DROP",OBJ_STATE_MAIN_DROP);
  tolua_constant(tolua_S,"OBJ_STATE_MAIN_GALLERY",OBJ_STATE_MAIN_GALLERY);
  tolua_constant(tolua_S,"OBJ_STATE_RESULT_CONFIRMING",OBJ_STATE_RESULT_CONFIRMING);
  tolua_constant(tolua_S,"OBJ_STATE_RESULT_CONFIRMED",OBJ_STATE_RESULT_CONFIRMED);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"type_session","type_session","",tolua_collect_type_session);
  #else
  tolua_cclass(tolua_S,"type_session","type_session","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"type_session");
   tolua_variable(tolua_S,"obj_state",tolua_get_type_session_obj_state,tolua_set_type_session_obj_state);
   tolua_variable(tolua_S,"obj_type",tolua_get_type_session_unsigned_obj_type,tolua_set_type_session_unsigned_obj_type);
   tolua_variable(tolua_S,"vx",tolua_get_type_session_vx,tolua_set_type_session_vx);
   tolua_variable(tolua_S,"vy",tolua_get_type_session_vy,tolua_set_type_session_vy);
   tolua_variable(tolua_S,"ax",tolua_get_type_session_ax,tolua_set_type_session_ax);
   tolua_variable(tolua_S,"ay",tolua_get_type_session_ay,tolua_set_type_session_ay);
   tolua_variable(tolua_S,"angle",tolua_get_type_session_angle,tolua_set_type_session_angle);
   tolua_variable(tolua_S,"obj_no",tolua_get_type_session_obj_no,tolua_set_type_session_obj_no);
   tolua_variable(tolua_S,"sess_index",tolua_get_type_session_unsigned_sess_index,tolua_set_type_session_unsigned_sess_index);
   tolua_variable(tolua_S,"proc_flg",tolua_get_type_session_unsigned_proc_flg,tolua_set_type_session_unsigned_proc_flg);
   tolua_variable(tolua_S,"frame_count",tolua_get_type_session_unsigned_frame_count,tolua_set_type_session_unsigned_frame_count);
   tolua_variable(tolua_S,"turn_count",tolua_get_type_session_unsigned_turn_count,tolua_set_type_session_unsigned_turn_count);
   tolua_variable(tolua_S,"scrinfo",tolua_get_type_session_scrinfo_ptr,tolua_set_type_session_scrinfo_ptr);
   tolua_variable(tolua_S,"extdata1",tolua_get_type_session_unsigned_extdata1,tolua_set_type_session_unsigned_extdata1);
   tolua_variable(tolua_S,"extdata2",tolua_get_type_session_unsigned_extdata2,tolua_set_type_session_unsigned_extdata2);
   tolua_array(tolua_S,"option",tolua_get_thg_type_session_option,tolua_set_thg_type_session_option);
   tolua_array(tolua_S,"name",tolua_get_thg_type_session_name,tolua_set_thg_type_session_name);
   tolua_variable(tolua_S,"name_len",tolua_get_type_session_unsigned_name_len,tolua_set_type_session_unsigned_name_len);
   tolua_variable(tolua_S,"lx",tolua_get_type_session_lx,tolua_set_type_session_lx);
   tolua_variable(tolua_S,"ly",tolua_get_type_session_ly,tolua_set_type_session_ly);
   tolua_variable(tolua_S,"sock",tolua_get_type_session_sock,tolua_set_type_session_sock);
   tolua_variable(tolua_S,"addr",tolua_get_type_session_addr,tolua_set_type_session_addr);
   tolua_variable(tolua_S,"sessionkey",tolua_get_type_session_sessionkey,tolua_set_type_session_sessionkey);
   tolua_variable(tolua_S,"clientver",tolua_get_type_session_clientver,tolua_set_type_session_clientver);
   tolua_variable(tolua_S,"port",tolua_get_type_session_port,tolua_set_type_session_port);
   tolua_variable(tolua_S,"cost",tolua_get_type_session_unsigned_cost,tolua_set_type_session_unsigned_cost);
   tolua_array(tolua_S,"items",tolua_get_thg_type_session_items,tolua_set_thg_type_session_items);
   tolua_variable(tolua_S,"dir",tolua_get_type_session_dir,tolua_set_type_session_dir);
   tolua_variable(tolua_S,"master",tolua_get_type_session_unsigned_master,tolua_set_type_session_unsigned_master);
   tolua_variable(tolua_S,"game_ready",tolua_get_type_session_unsigned_game_ready,tolua_set_type_session_unsigned_game_ready);
   tolua_variable(tolua_S,"team_no",tolua_get_type_session_unsigned_team_no,tolua_set_type_session_unsigned_team_no);
   tolua_variable(tolua_S,"MV_c",tolua_get_type_session_MV_c,tolua_set_type_session_MV_c);
   tolua_variable(tolua_S,"MV_m",tolua_get_type_session_MV_m,tolua_set_type_session_MV_m);
   tolua_variable(tolua_S,"HP_c",tolua_get_type_session_HP_c,tolua_set_type_session_HP_c);
   tolua_variable(tolua_S,"HP_m",tolua_get_type_session_HP_m,tolua_set_type_session_HP_m);
   tolua_variable(tolua_S,"delay",tolua_get_type_session_delay,tolua_set_type_session_delay);
   tolua_variable(tolua_S,"EXP_c",tolua_get_type_session_EXP_c,tolua_set_type_session_EXP_c);
   tolua_variable(tolua_S,"live_count",tolua_get_type_session_unsigned_live_count,tolua_set_type_session_unsigned_live_count);
   tolua_variable(tolua_S,"entity",tolua_get_type_session_unsigned_entity,tolua_set_type_session_unsigned_entity);
   tolua_variable(tolua_S,"chara_type",tolua_get_type_session_unsigned_chara_type,tolua_set_type_session_unsigned_chara_type);
   tolua_variable(tolua_S,"chara_state",tolua_get_type_session_chara_state,tolua_set_type_session_chara_state);
   tolua_variable(tolua_S,"connect_state",tolua_get_type_session_unsigned_connect_state,tolua_set_type_session_unsigned_connect_state);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"type_blt","type_blt","",tolua_collect_type_blt);
  #else
  tolua_cclass(tolua_S,"type_blt","type_blt","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"type_blt");
   tolua_variable(tolua_S,"obj_state",tolua_get_type_blt_obj_state,tolua_set_type_blt_obj_state);
   tolua_variable(tolua_S,"obj_type",tolua_get_type_blt_unsigned_obj_type,tolua_set_type_blt_unsigned_obj_type);
   tolua_variable(tolua_S,"vx",tolua_get_type_blt_vx,tolua_set_type_blt_vx);
   tolua_variable(tolua_S,"vy",tolua_get_type_blt_vy,tolua_set_type_blt_vy);
   tolua_variable(tolua_S,"ax",tolua_get_type_blt_ax,tolua_set_type_blt_ax);
   tolua_variable(tolua_S,"ay",tolua_get_type_blt_ay,tolua_set_type_blt_ay);
   tolua_variable(tolua_S,"angle",tolua_get_type_blt_angle,tolua_set_type_blt_angle);
   tolua_variable(tolua_S,"obj_no",tolua_get_type_blt_obj_no,tolua_set_type_blt_obj_no);
   tolua_variable(tolua_S,"sess_index",tolua_get_type_blt_unsigned_sess_index,tolua_set_type_blt_unsigned_sess_index);
   tolua_variable(tolua_S,"proc_flg",tolua_get_type_blt_unsigned_proc_flg,tolua_set_type_blt_unsigned_proc_flg);
   tolua_variable(tolua_S,"frame_count",tolua_get_type_blt_unsigned_frame_count,tolua_set_type_blt_unsigned_frame_count);
   tolua_variable(tolua_S,"turn_count",tolua_get_type_blt_unsigned_turn_count,tolua_set_type_blt_unsigned_turn_count);
   tolua_variable(tolua_S,"scrinfo",tolua_get_type_blt_scrinfo_ptr,tolua_set_type_blt_scrinfo_ptr);
   tolua_variable(tolua_S,"extdata1",tolua_get_type_blt_unsigned_extdata1,tolua_set_type_blt_unsigned_extdata1);
   tolua_variable(tolua_S,"extdata2",tolua_get_type_blt_unsigned_extdata2,tolua_set_type_blt_unsigned_extdata2);
   tolua_array(tolua_S,"option",tolua_get_thg_type_blt_option,tolua_set_thg_type_blt_option);
   tolua_variable(tolua_S,"proc_type",tolua_get_type_blt_unsigned_proc_type,tolua_set_type_blt_unsigned_proc_type);
   tolua_variable(tolua_S,"hit_range",tolua_get_type_blt_unsigned_hit_range,tolua_set_type_blt_unsigned_hit_range);
   tolua_variable(tolua_S,"chr_obj_no",tolua_get_type_blt_chr_obj_no,tolua_set_type_blt_chr_obj_no);
   tolua_variable(tolua_S,"chara_type",tolua_get_type_blt_unsigned_chara_type,tolua_set_type_blt_unsigned_chara_type);
   tolua_variable(tolua_S,"bullet_type",tolua_get_type_blt_unsigned_bullet_type,tolua_set_type_blt_unsigned_bullet_type);
   tolua_variable(tolua_S,"tex_index",tolua_get_type_blt_unsigned_tex_index,tolua_set_type_blt_unsigned_tex_index);
   tolua_variable(tolua_S,"bx",tolua_get_type_blt_bx,tolua_set_type_blt_bx);
   tolua_variable(tolua_S,"by",tolua_get_type_blt_by,tolua_set_type_blt_by);
   tolua_variable(tolua_S,"adx",tolua_get_type_blt_adx,tolua_set_type_blt_adx);
   tolua_variable(tolua_S,"ady",tolua_get_type_blt_ady,tolua_set_type_blt_ady);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"type_ground","type_ground","",tolua_collect_type_ground);
  #else
  tolua_cclass(tolua_S,"type_ground","type_ground","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"type_ground");
   tolua_variable(tolua_S,"x",tolua_get_type_ground_x,tolua_set_type_ground_x);
   tolua_variable(tolua_S,"y",tolua_get_type_ground_y,tolua_set_type_ground_y);
   tolua_variable(tolua_S,"ground",tolua_get_type_ground_ground,tolua_set_type_ground_ground);
  tolua_endmodule(tolua_S);
  tolua_function(tolua_S,"C_CreateBullet",tolua_thg_C_CreateBullet00);
  tolua_function(tolua_S,"C_Shot",tolua_thg_C_Shot00);
  tolua_function(tolua_S,"C_RemoveBullet",tolua_thg_C_RemoveBullet00);
  tolua_function(tolua_S,"C_IsRemovedObject",tolua_thg_C_IsRemovedObject00);
  tolua_function(tolua_S,"C_BombObject",tolua_thg_C_BombObject00);
  tolua_function(tolua_S,"C_DamageCharaHP",tolua_thg_C_DamageCharaHP00);
  tolua_function(tolua_S,"C_UpdateCharaStatus",tolua_thg_C_UpdateCharaStatus00);
  tolua_function(tolua_S,"C_UpdateCharaPos",tolua_thg_C_UpdateCharaPos00);
  tolua_function(tolua_S,"C_SetBulletOptionData",tolua_thg_C_SetBulletOptionData00);
  tolua_function(tolua_S,"C_GetBulletOptionData",tolua_thg_C_GetBulletOptionData00);
  tolua_function(tolua_S,"C_SetCharaOptionData",tolua_thg_C_SetCharaOptionData00);
  tolua_function(tolua_S,"C_GetCharaOptionData",tolua_thg_C_GetCharaOptionData00);
  tolua_function(tolua_S,"C_SetBulletExtData1",tolua_thg_C_SetBulletExtData100);
  tolua_function(tolua_S,"C_SetBulletExtData2",tolua_thg_C_SetBulletExtData200);
  tolua_function(tolua_S,"C_GetCharaExtData1",tolua_thg_C_GetCharaExtData100);
  tolua_function(tolua_S,"C_GetCharaExtData2",tolua_thg_C_GetCharaExtData200);
  tolua_function(tolua_S,"C_SetCharaExtData1",tolua_thg_C_SetCharaExtData100);
  tolua_function(tolua_S,"C_SetCharaExtData2",tolua_thg_C_SetCharaExtData200);
  tolua_function(tolua_S,"C_AddCharaItem",tolua_thg_C_AddCharaItem00);
  tolua_function(tolua_S,"C_SetCharaState",tolua_thg_C_SetCharaState00);
  tolua_function(tolua_S,"C_GetCharaState",tolua_thg_C_GetCharaState00);
  tolua_function(tolua_S,"C_GetCharaItem",tolua_thg_C_GetCharaItem00);
  tolua_function(tolua_S,"C_UpdateBulletState",tolua_thg_C_UpdateBulletState00);
  tolua_function(tolua_S,"C_UpdateBulletPosition",tolua_thg_C_UpdateBulletPosition00);
  tolua_function(tolua_S,"C_UpdateBulletVector",tolua_thg_C_UpdateBulletVector00);
  tolua_function(tolua_S,"C_UpdateObjectType",tolua_thg_C_UpdateObjectType00);
  tolua_function(tolua_S,"C_PasteTextureOnStage",tolua_thg_C_PasteTextureOnStage00);
  tolua_function(tolua_S,"C_GetRand",tolua_thg_C_GetRand00);
  tolua_function(tolua_S,"C_GetEntityCharacters",tolua_thg_C_GetEntityCharacters00);
  tolua_function(tolua_S,"C_GetLivingCharacters",tolua_thg_C_GetLivingCharacters00);
  tolua_function(tolua_S,"C_GetWindValue",tolua_thg_C_GetWindValue00);
  tolua_function(tolua_S,"C_SetWindValue",tolua_thg_C_SetWindValue00);
  tolua_function(tolua_S,"C_GetStageWidth",tolua_thg_C_GetStageWidth00);
  tolua_function(tolua_S,"C_GetStageHeight",tolua_thg_C_GetStageHeight00);
  tolua_function(tolua_S,"C_GetCharacterAtVector",tolua_thg_C_GetCharacterAtVector00);
  tolua_function(tolua_S,"C_GetCharacterFromObjNo",tolua_thg_C_GetCharacterFromObjNo00);
  tolua_function(tolua_S,"C_GetBulletInfo",tolua_thg_C_GetBulletInfo00);
  tolua_function(tolua_S,"C_GetBulletAtkValue",tolua_thg_C_GetBulletAtkValue00);
  tolua_function(tolua_S,"C_GetBulletExtData1",tolua_thg_C_GetBulletExtData100);
  tolua_function(tolua_S,"C_GetBulletExtData2",tolua_thg_C_GetBulletExtData200);
  tolua_function(tolua_S,"C_UpdateBulletAngle",tolua_thg_C_UpdateBulletAngle00);
  tolua_function(tolua_S,"C_SetBulletTextureIndex",tolua_thg_C_SetBulletTextureIndex00);
  tolua_function(tolua_S,"C_SetCharaTexture",tolua_thg_C_SetCharaTexture00);
  tolua_function(tolua_S,"C_AddEffect",tolua_thg_C_AddEffect00);
  tolua_function(tolua_S,"C_RemoveEffect",tolua_thg_C_RemoveEffect00);
  tolua_function(tolua_S,"C_SetEffectVector",tolua_thg_C_SetEffectVector00);
  tolua_function(tolua_S,"C_SetEffectAlpha",tolua_thg_C_SetEffectAlpha00);
  tolua_function(tolua_S,"C_SetEffectFade",tolua_thg_C_SetEffectFade00);
  tolua_function(tolua_S,"C_SetEffectFadeInOut",tolua_thg_C_SetEffectFadeInOut00);
  tolua_function(tolua_S,"C_SetEffectRotate",tolua_thg_C_SetEffectRotate00);
  tolua_function(tolua_S,"C_SetEffectRotation",tolua_thg_C_SetEffectRotation00);
  tolua_function(tolua_S,"C_SetEffectScale",tolua_thg_C_SetEffectScale00);
  tolua_function(tolua_S,"C_SetEffectScalling",tolua_thg_C_SetEffectScalling00);
  tolua_function(tolua_S,"C_SetEffectAnimation",tolua_thg_C_SetEffectAnimation00);
  tolua_function(tolua_S,"C_SetEffectTexture",tolua_thg_C_SetEffectTexture00);
  tolua_function(tolua_S,"C_AddBGEffect",tolua_thg_C_AddBGEffect00);
  tolua_function(tolua_S,"C_RemoveBGEffect",tolua_thg_C_RemoveBGEffect00);
  tolua_function(tolua_S,"C_SetBGEffectVector",tolua_thg_C_SetBGEffectVector00);
  tolua_function(tolua_S,"C_SetBGEffectAlpha",tolua_thg_C_SetBGEffectAlpha00);
  tolua_function(tolua_S,"C_SetBGEffectFade",tolua_thg_C_SetBGEffectFade00);
  tolua_function(tolua_S,"C_SetBGEffectFadeInOut",tolua_thg_C_SetBGEffectFadeInOut00);
  tolua_function(tolua_S,"C_SetBGEffectRotate",tolua_thg_C_SetBGEffectRotate00);
  tolua_function(tolua_S,"C_SetBGEffectRotation",tolua_thg_C_SetBGEffectRotation00);
  tolua_function(tolua_S,"C_SetBGEffectScale",tolua_thg_C_SetBGEffectScale00);
  tolua_function(tolua_S,"C_SetBGEffectScalling",tolua_thg_C_SetBGEffectScalling00);
  tolua_function(tolua_S,"C_SetBGEffectAnimation",tolua_thg_C_SetBGEffectAnimation00);
  tolua_function(tolua_S,"C_SetBGEffectTexture",tolua_thg_C_SetBGEffectTexture00);
  tolua_function(tolua_S,"C_SetCameraFocusToBullet",tolua_thg_C_SetCameraFocusToBullet00);
  tolua_function(tolua_S,"C_SetCameraFocusToChara",tolua_thg_C_SetCameraFocusToChara00);
  tolua_function(tolua_S,"C_HideStage",tolua_thg_C_HideStage00);
  tolua_function(tolua_S,"C_ShowStage",tolua_thg_C_ShowStage00);
  tolua_function(tolua_S,"C_RegistSoundSE",tolua_thg_C_RegistSoundSE00);
  tolua_function(tolua_S,"C_PlaySoundSE",tolua_thg_C_PlaySoundSE00);
  tolua_function(tolua_S,"C_GetMyCharaNo",tolua_thg_C_GetMyCharaNo00);
  tolua_function(tolua_S,"C_AddMsgLog",tolua_thg_C_AddMsgLog00);
  tolua_function(tolua_S,"C_GetAngle",tolua_thg_C_GetAngle00);
  tolua_function(tolua_S,"C_GetScrIDFromChrNo",tolua_thg_C_GetScrIDFromChrNo00);
  tolua_function(tolua_S,"C_GetUserTeamNo",tolua_thg_C_GetUserTeamNo00);
  tolua_function(tolua_S,"C_GetUserName",tolua_thg_C_GetUserName00);
  tolua_function(tolua_S,"C_DbgOutputStr",tolua_thg_C_DbgOutputStr00);
  tolua_function(tolua_S,"C_DbgOutputNum",tolua_thg_C_DbgOutputNum00);
  tolua_function(tolua_S,"C_DbgAddLogNum",tolua_thg_C_DbgAddLogNum00);
  tolua_function(tolua_S,"C_GetRandomStagePos",tolua_thg_C_GetRandomStagePos00);
  tolua_function(tolua_S,"C_GetGroundPos",tolua_thg_C_GetGroundPos00);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_thg (lua_State* tolua_S) {
 return tolua_thg_open(tolua_S);
};
#endif

