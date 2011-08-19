#ifndef DDEBUG
#define DDEBUG 0
#endif
#include "ddebug.h"

#include "ngx_http_lua_time.h"


int
ngx_http_lua_ngx_today(lua_State *L)
{
    ngx_http_request_t      *r;
    time_t                   now;
    ngx_tm_t                 tm;
    u_char                   buf[sizeof("2010-11-19") - 1];

    lua_getglobal(L, GLOBALS_SYMBOL_REQUEST);
    r = lua_touserdata(L, -1);
    lua_pop(L, 1);

    if (r == NULL) {
        return luaL_error(L, "no request object found");
    }

    if (lua_gettop(L) > 0) {
        return luaL_error(L, "shouldn't have argument");
    }

    now = ngx_time();
    ngx_gmtime(now + ngx_cached_time->gmtoff * 60, &tm);

    ngx_sprintf(buf, "%04d-%02d-%02d", tm.ngx_tm_year, tm.ngx_tm_mon,
            tm.ngx_tm_mday);

    lua_pushlstring(L, (char *) buf, sizeof(buf));

    return 1;
}


int
ngx_http_lua_ngx_localtime(lua_State *L)
{
    ngx_http_request_t      *r;
    ngx_tm_t                 tm;

    u_char buf[sizeof("2010-11-19 20:56:31") - 1];

    lua_getglobal(L, GLOBALS_SYMBOL_REQUEST);
    r = lua_touserdata(L, -1);
    lua_pop(L, 1);

    if (r == NULL) {
        return luaL_error(L, "no request object found");
    }

    if (lua_gettop(L) > 0) {
        return luaL_error(L, "shouldn't have argument");
    }

    ngx_gmtime(ngx_time() + ngx_cached_time->gmtoff * 60, &tm);

    ngx_sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d", tm.ngx_tm_year,
            tm.ngx_tm_mon, tm.ngx_tm_mday, tm.ngx_tm_hour, tm.ngx_tm_min,
            tm.ngx_tm_sec);

    lua_pushlstring(L, (char *) buf, sizeof(buf));

    return 1;
}


int
ngx_http_lua_ngx_time(lua_State *L)
{
    ngx_http_request_t      *r;

    lua_getglobal(L, GLOBALS_SYMBOL_REQUEST);
    r = lua_touserdata(L, -1);
    lua_pop(L, 1);

    if (r == NULL) {
        return luaL_error(L, "no request object found");
    }

    if (lua_gettop(L) > 0) {
        return luaL_error(L, "shouldn't have argument");
    }

    lua_pushnumber(L, (lua_Number) ngx_time());

    return 1;
}


int
ngx_http_lua_ngx_utctime(lua_State *L)
{
    ngx_http_request_t      *r;
    ngx_tm_t                 tm;

    u_char buf[sizeof("2010-11-19 20:56:31") - 1];

    lua_getglobal(L, GLOBALS_SYMBOL_REQUEST);
    r = lua_touserdata(L, -1);
    lua_pop(L, 1);

    if (r == NULL) {
        return luaL_error(L, "no request object found");
    }

    if (lua_gettop(L) > 0) {
        return luaL_error(L, "shouldn't have argument");
    }

    ngx_gmtime(ngx_time(), &tm);

    ngx_sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d", tm.ngx_tm_year,
            tm.ngx_tm_mon, tm.ngx_tm_mday, tm.ngx_tm_hour, tm.ngx_tm_min,
            tm.ngx_tm_sec);

    lua_pushlstring(L, (char *) buf, sizeof(buf));

    return 1;
}


int
ngx_http_lua_ngx_cookie_time(lua_State *L)
{
    time_t                               t;
    u_char                              *p;

    u_char   buf[sizeof("Mon, 28 Sep 1970 06:00:00 GMT") - 1];

    if (lua_gettop(L) != 1) {
        return luaL_error(L, "expecting one argument");
    }

    t = (time_t) luaL_checknumber(L, 1);

    p = buf;
    p = ngx_http_cookie_time(p, t);

    lua_pushlstring(L, (char *) buf, p - buf);

    return 1;
}


int
ngx_http_lua_ngx_http_time(lua_State *L)
{
    time_t                               t;
    u_char                              *p;

    u_char   buf[sizeof("Mon, 28 Sep 1970 06:00:00 GMT") - 1];

    if (lua_gettop(L) != 1) {
        return luaL_error(L, "expecting one argument");
    }

    t = (time_t) luaL_checknumber(L, 1);

    p = buf;
    p = ngx_http_time(p, t);

    lua_pushlstring(L, (char *) buf, p - buf);

    return 1;
}


int
ngx_http_lua_ngx_parse_http_time(lua_State *L)
{
    u_char                              *p;
    size_t                               len;
    time_t                               time;

    if (lua_gettop(L) != 1) {
        return luaL_error(L, "expecting one argument");
    }

    p = (u_char *) luaL_checklstring(L, 1, &len);

    time = ngx_http_parse_time(p, len);
    if (time == NGX_ERROR) {
        lua_pushnil(L);
        return 1;
    }

    lua_pushnumber(L, (lua_Number) time);

    return 1;
}

