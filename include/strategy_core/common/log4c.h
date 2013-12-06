//==============================================================================
/*!
 *  \file log4c.h
 *
 *  \brief log4c.h
 *
 *  \author chenqinyu
 *          contact:
 *              qinyu.chen@corp.elong.com
 *              superthierry14@gmail.com
 *  \date 2013-07
 *
 *  \par http://www.elong.com<BR>
 */
//==============================================================================

#ifndef STRATEGY_CORE_COMMON_LOG4C_H_
#define STRATEGY_CORE_COMMON_LOG4C_H_

#include <stdarg.h>
#include <stdio.h>

#ifndef NULL
#define NULL 0
#endif

#define LOGERR_BASE         -1001
#define LOGERR_OUTPUTTYPE   LOGERR_BASE - 1
#define LOGERR_MALLOC       LOGERR_BASE - 2
#define LOGERR_LOGTOOLONG   LOGERR_BASE - 3
#define LOGERR_OPENFILE     LOGERR_BASE - 4
#define LOGERR_ANS          LOGERR_BASE - 5
#define LOGERR_LOADDEF      LOGERR_BASE - 6
#define LOGERR_HANDLE       LOGERR_BASE - 7


#ifdef __GNUC__
#define LVTRACE        (t_logparam){_LTRACE,    __LINE__, __FUNCTION__, __FILE__}
#define LVDEBUG        (t_logparam){_LDEBUG,    __LINE__, __FUNCTION__, __FILE__}
#define LVINFO         (t_logparam){_LINFO,     __LINE__, __FUNCTION__, __FILE__}
#define LVWARN         (t_logparam){_LWARN,     __LINE__, __FUNCTION__, __FILE__}
#define LVERROR        (t_logparam){_LERROR,    __LINE__, __FUNCTION__, __FILE__}
#define LVFATAL        (t_logparam){_LFATAL,    __LINE__, __FUNCTION__, __FILE__}
#define LVBULLETIN     (t_logparam){_LBULLETIN, __LINE__, __FUNCTION__, __FILE__}
#define LVUSERLEVEL(a) (t_logparam){(((unsigned)(a))&0x00FFFFFF)+10, __LINE__, __FUNCTION__, __FILE__}
#else

#ifdef SOLARIS
#define __FUNCTION__ __func__
#endif

#define LVTRACE        tmp_fetch(_LTRACE,    __LINE__, __FUNCTION__, __FILE__)
#define LVDEBUG        tmp_fetch(_LDEBUG,    __LINE__, __FUNCTION__, __FILE__)
#define LVINFO         tmp_fetch(_LINFO,     __LINE__, __FUNCTION__, __FILE__)
#define LVWARN         tmp_fetch(_LWARN,     __LINE__, __FUNCTION__, __FILE__)
#define LVERROR        tmp_fetch(_LERROR,    __LINE__, __FUNCTION__, __FILE__)
#define LVFATAL        tmp_fetch(_LFATAL,    __LINE__, __FUNCTION__, __FILE__)
#define LVBULLETIN     tmp_fetch(_LBULLETIN, __LINE__, __FUNCTION__, __FILE__)
#define LVUSERLEVEL(a) tmp_fetch((((unsigned)(a))&0x00FFFFFF)+10, __LINE__, __FUNCTION__, __FILE__)
#endif

#define L4C_TRACE      tmp_set(_LTRACE,    __LINE__, __FUNCTION__, __FILE__), l4c_logdef
#define L4C_DEBUG      tmp_set(_LDEBUG,    __LINE__, __FUNCTION__, __FILE__), l4c_logdef
#define L4C_INFO       tmp_set(_LINFO,     __LINE__, __FUNCTION__, __FILE__), l4c_logdef
#define L4C_WARN       tmp_set(_LWARN,     __LINE__, __FUNCTION__, __FILE__), l4c_logdef
#define L4C_ERROR      tmp_set(_LERROR,    __LINE__, __FUNCTION__, __FILE__), l4c_logdef
#define L4C_FATAL      tmp_set(_LFATAL,    __LINE__, __FUNCTION__, __FILE__), l4c_logdef
#define L4C_BULLETIN   tmp_set(_LBULLETIN, __LINE__, __FUNCTION__, __FILE__), l4c_logdef

typedef enum loglevel {
    _LALL      = 0,
    _LTRACE    = 0,
    _LDEBUG,
    _LINFO,
    _LWARN,
    _LERROR,
    _LFATAL,
    _LBULLETIN,
    _LNONE
} e_loglevel;

typedef struct _loghndl {
    char s[1500];
} t_loghndl;


typedef struct logparam {
    e_loglevel log_level;
    int        line_no;
    const char *func_name;
    const char *file_name;
} t_logparam;

#ifdef __cplusplus
extern "C" {
#endif
    char* l4c_getcfgstr(const char* path, const char* main_tag, const char* sub_tag, char* value, size_t len, const char* def);
    int l4c_getcfgint(const char* path, const char* main_tag, const char* sub_tag, int defint);
    t_logparam tmp_fetch(int ll, int line_no, const char* func_name, const char* file_name);
    int l4c_open(t_loghndl* plh, const char* cfgpath, const char* main_tag, const char* param, const char* iden);
    void l4c_close(t_loghndl* plh);
    const char* l4c_strerr(int errcode);
    int l4c_vlog(t_logparam lp, t_loghndl* plh, const char* fmt, va_list vl);
    int l4c_log(t_logparam lp, t_loghndl* plh, const char* fmt, ...);
    int l4c_vlogex(t_logparam lp, t_loghndl* plh, const char* var[], const char* fmt, va_list vl);
    int l4c_logex(t_logparam lp, t_loghndl* plh, const char* var[], const char* fmt, ...);
    int l4c_vlogdef(const char* fmt, va_list vl);
    int l4c_logdef(const char* fmt, ...);
    int l4c_vlogdefex(const char* var[], const char* fmt, va_list vl);
    int l4c_logdefex(const char* var[], const char* fmt, ...);
    void tmp_set(int ll, int line_no, const char* func_name, const char* file_name);
    int l4c_idenset(t_loghndl* plh, const char* iden);
    int l4c_lvset(t_logparam lp, t_loghndl* plh);
    t_logparam l4c_lvget(t_loghndl* plh);
    int l4c_lvcmp(t_logparam lp1, t_logparam lp2);

    void set_cfg_path(const char *l4c_cfg_path);
#ifdef __cplusplus
}
#endif
#endif //STRATEGY_CORE_COMMON_LOG4C_H_

