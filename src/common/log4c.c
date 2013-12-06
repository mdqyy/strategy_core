#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define _L4C_NOPTHREAD
#ifndef _L4C_NOPTHREAD
#include <semaphore.h>
#include <pthread.h>
#else

#define pthread_mutex_t void*
#define pthread_key_t void*
#define pthread_mutexattr_t void*
#define PTHREAD_MUTEX_INITIALIZER NULL

char g_l4c_cfg_path[256]="l4c.cfg";

void set_cfg_path(const char *l4c_cfg_path) {
    if(strncmp("l4c.cfg", l4c_cfg_path, 7) != 0)
        strcpy(g_l4c_cfg_path, l4c_cfg_path);
}

int pthread_key_create(pthread_key_t* key, void (*destructor)(void*)) {
    return 0;
}

int pthread_key_delete(pthread_key_t key) {
    return 0;
}

int pthread_setspecific(pthread_key_t key, const void* value) {
    return 0;
}

void * pthread_getspecific(pthread_key_t key) {
    return NULL;
}

int pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t* attr) {
    return 0;
}

int pthread_mutex_destroy(pthread_mutex_t* mutex) {
    return 0;
}

int pthread_mutex_lock(pthread_mutex_t* mutex) {
    return 0;
}

int pthread_mutex_unlock(pthread_mutex_t* mutex) {
    return 0;
}
#endif

#ifdef WIN32
#include <windows.h>
#define GETPID GetCurrentProcessId

static int readlink(const char* p, char* d, int len) {
    return -1;
}

#else
#include <sys/time.h>
#include <unistd.h>
#define GETPID getpid


static int GetModuleFileName(const char* s, const char* s1, int len) {
    return 0;
}

#endif

#include <strategy_core/common/log4c.h>

#define _MAX_PATH_LEN           128
#define _MAX_TAG_LEN            128
#define _MAX_VALUE_LEN          1024
#define _MAX_DATE_FRAGMENT_NUM  64
#ifndef min
#define min(a, b) (((a)<(b))?(a):(b))
#endif

#ifndef _PROCPATH
#define _PROCPATH ""
#endif

typedef enum outputtype {
    OT_NULL   = 0,
    OT_STDOUT,
    OT_STDERR,
    OT_FILE,
    OT_SYSLOG
} e_outputtype;

typedef enum truncatetype {
    TT_NEVER    = 0,
    TT_DALIY    = 1,
    TT_ROLLING  = 2,
    TT_INTERVAL = 4

} e_truncate_type;

typedef enum fmttype {
    FT_NORMAL    = 0,
    FT_USER      = 1,
    FT_DATE_YEAR,
    FT_DATE_MONTH,
    FT_DATE_DAY,
    FT_DATE_HOUR,
    FT_DATE_MINUTE,
    FT_DATE_SECOND,
    FT_DATE_MILLISECOND,
    FT_NEWLINE,
    FT_DATE,
    FT_LINENO,
    FT_FUNCNAME,
    //FT_CLASSNAME,
    FT_FILENAME,
    FT_THREADID,
    FT_PROCESSID,
    FT_LEVEL,
    FT_REALTIME_S,
    FT_REALTIME_MS,
    FT_PERCENT,
    FT_APPNAME,
    FT_IDEN,
    FT_V0,
    FT_V1,
    FT_V2,
    FT_V3,
    FT_V4,
    FT_V5,
    FT_V6,
    FT_V7,
    FT_V8,
    FT_V9
} e_fmttype;

typedef enum flushtype {
    HT_EVERY = 1,
    HT_LINE,
    HT_CLOSE
} e_flushtype;


typedef struct logcfg {
    e_outputtype output_type;               // 主输出类型
    e_loglevel   lowest_level;              // 主最低可显示级别
    e_flushtype  flush_type;                // 刷新类型
    char         log_fmt[_MAX_VALUE_LEN];   // 日志格式
    char         log_path[_MAX_PATH_LEN];   // 日志路径
    char         log_name[_MAX_PATH_LEN];   // 日志名
    size_t       max_filesize;  // 最大文件大小
    time_t       interval;      // 新旧日志文件间隔
    int          auto_newline;  // 是否每次打印日志后自动换行
    size_t       max_buf;       // 使用的buf大小
    int          filelast;      // 是否文件持续打开
    unsigned     truncate_type; // 标识哪些情况下应创建新的日志文件
    int          open_clear;    // 是否文件打开的时候清空文件
    size_t       path_len;      // 路径字符串长度
    int          need_lock;     // 是否需要锁保护
    int          muti_thread;   // 是否多线程程序
} t_logcfg;

typedef struct fmtfragment {
    char      *fmt_fragment;
    unsigned  size;
    e_fmttype ft;
} t_fmtfragment;

typedef struct _log {
    t_logcfg      lc;
    char          *buf;
    FILE          *fp;
    time_t        last_log;
    t_fmtfragment *main_fmt;
    unsigned      mf_num;
    t_fmtfragment *filename_fmt;
    unsigned      ff_num;
    char          *file_nameparam;
    size_t        total_write;
    const char    *app_name;
    char          *iden;
    size_t        iden_size;
    pthread_mutex_t lock;
} t_log;

static pthread_key_t   _logkey;//线程私有存储
static pthread_mutex_t _loglock = PTHREAD_MUTEX_INITIALIZER; //保证key初始化的唯一

static char       *_level_name[] = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL", "BULLETIN", "NONE"};
static char       gc_szapp_full_path[1024] = "";
static const char *gscc_papp_name;

static t_loghndl  *_plh;
static t_logparam _lp;
static int        gsn_is_first = 1;
static int        gsn_isinit = 0;

static void lrtrim(char* str) {
    char *copied, *tail = NULL;
    if(str == NULL)
        return;

    for(copied = str; *str; ++str) {
        if(*str != ' ' && *str != '\t') {
            *copied++ = *str;
            tail = copied;
        } else {
            if(tail)
                *copied++ = *str;
        }
    }

    if(tail)
        *tail = 0;
    else
        *copied = 0;
    return;
}

static const char* get_progname() {
    const char *s1 = NULL, *s2 = NULL;
    if(gscc_papp_name == 0) {
        if(GetModuleFileName(NULL, gc_szapp_full_path, 1024) != 0)
            ;
        else if(strlen(_PROCPATH) != 0) {
            char procpath[1024];
            sprintf(procpath, "/proc/%d/%s", GETPID(), _PROCPATH);
            if(readlink(procpath, gc_szapp_full_path, 1024) < 0) {
                strncpy(gc_szapp_full_path, getenv("_"), 1023);
                gc_szapp_full_path[1023] = 0;
            }
        } else {
            if(getenv("_") == NULL)
                return "";
            strncpy(gc_szapp_full_path, getenv("_"), 1023);
            gc_szapp_full_path[1023] = 0;
        }

        if(gc_szapp_full_path == NULL)
            return "";

        s1 = strrchr(gc_szapp_full_path, '\\');
        s2 = strrchr(gc_szapp_full_path, '/');
        if(s1 < s2)
            s1 = s2;
        if(s1 == NULL)
            s1 = gc_szapp_full_path;
        else
            ++s1;
        gscc_papp_name = s1;
    }
    return gscc_papp_name;
}

static int strvalid(const char* word, int level, unsigned len) {
    unsigned i = 0;
    for(; i < 1024 && i < len; ++i) {
        if(word[i] == 0)
            break;

        if(word[i] >= '0' && word[i] <= '9')
            continue;

        if(level >= 1 && ((word[i] >= 'A' && word[i] <= 'Z') || (word[i] >= 'a' && word[i] <= 'z') || word[i] == '_'))
            continue;
        return 0;
    }
    return 1;
}

static int _pow(int a, int b) {
    int i = 0, total = 1;
    for(; i < b; ++i)
        total *= a;
    return total;
}

static int is_maintag(char* line, const char* main_tag) {
    int f = -1, t = -1, i = 0;
    size_t len = strlen(line), mainlen = strlen(main_tag);;
    for(; i < (int)len; ++i) {
        if(line[i] == ' ' || line[i] == '\t')
            continue;
        if(line[i] == '[') {
            f = i + 1;
            break;
        } else if(line[i] == '#')
            return 0;
        else
            return -1;
    }

    if(f == -1)
        return 0;

    for(i = (int)len - 1; i >= 0; --i) {
        if(line[i] == ' ' || line[i] == '\t')
            continue;

        if(line[i] == ']') {
            t = i - 1;
            break;
        } else
            break;
    }

    if(t == -1)
        return 0;

    if(memcmp(main_tag, line + f, mainlen) == 0 && mainlen == t - f + 1)
        return 2;
    else
        return 1;
}

char* l4c_getcfgstr(const char* path, const char* main_tag, const char* sub_tag, char* value, size_t len, const char* def) {
    int tv = 0;
    int pstat = -1;
    size_t sub_len = 0;
    size_t line_len = 0;
    FILE* fp = NULL;
    char line[_MAX_VALUE_LEN];

    sub_len = strlen(sub_tag);
    fp = fopen(path, "r");
    if(fp == NULL)
        goto ERREND;

    while(fgets(line, _MAX_VALUE_LEN - 1, fp) != NULL) {
        char* zs = NULL;

        line[_MAX_VALUE_LEN-1] = 0;
        line_len = strlen(line);

        if(line_len - 1 >= 0 && line[line_len-1] == '\n')
            line[--line_len] = 0;

        if(line_len - 1 >= 0 && line[line_len-1] == '\r')
            line[--line_len] = 0;

        if(line_len - 1 >= 0 && line[line_len-1] == '\n')
            line[--line_len] = 0;

        if(line_len - 1 >= 0 && line[line_len-1] == '\r')
            line[--line_len] = 0;

        tv = is_maintag(line, main_tag);
        if(tv == 0)
            continue;
        else if(tv > 0) {
            if(pstat < 0) {
                if(tv > 1)
                    ++pstat;
                continue;
            } else
                goto ERREND;
        } else {
            if(pstat < 0)
                continue;
            else {
                char* s = strchr(line, '=');
                if(s == NULL)
                    continue;
                *s = 0;
                ++s;
                lrtrim(line);
                if(memcmp(line, sub_tag, sub_len + 1) == 0) {
                    strncpy(value, s, len - 1);
                    value[len-1] = 0;
                    return value;
                } else
                    continue;
            }
        }
    }

ERREND:
    strncpy(value, def, len - 1);
    value[len-1] = 0;
    return value;
}

int l4c_getcfgint(const char* path, const char* main_tag, const char* sub_tag, int defint) {
    char buf[_MAX_VALUE_LEN];
    char def[128];
    sprintf(def, "%d", defint);
    l4c_getcfgstr(path, main_tag, sub_tag, buf, _MAX_VALUE_LEN, def);

    if(strvalid(buf, 0, _MAX_VALUE_LEN)) {
        size_t len = strlen(buf);
        if(len < 10 || (len == 10 && strcmp(buf, "2147483647") < 0))
            return atoi(buf);
        else
            return defint;
    } else
        return defint;
}


static unsigned transtt(char* value) {
    char *tt, *last_tt = value;
    unsigned type = 0;

    while((tt = strchr(value, ',')) || last_tt) {
        if(tt)
            tt[0] = 0;

        lrtrim(last_tt);
        switch(last_tt[0]) {
        case 'N':
            if(strcmp(last_tt, "NEVER") == 0)    type = type | TT_NEVER;
            break;
        case 'D':
            if(strcmp(last_tt, "DALIY") == 0)    type = type | TT_DALIY;
            break;
        case 'R':
            if(strcmp(last_tt, "ROLLING") == 0)  type = type | TT_ROLLING;
            break;
        case 'I':
            if(strcmp(last_tt, "INTERVAL") == 0) type = type | TT_INTERVAL;
            break;
        }

        if(tt)
            last_tt = tt + 1;
        else
            last_tt = 0;
    }
    return type;
}

static t_fmtfragment* ans_datefmt(char* date_fmt, size_t len) {
    t_fmtfragment* pdf;
    unsigned i = 0, cur_idx = 0, cn = 0;
    e_fmttype last_ft = FT_NORMAL, cur_ft = FT_NORMAL;

    if(len == 0)
        return NULL;

    pdf = (t_fmtfragment*)malloc(sizeof(t_fmtfragment) * _MAX_DATE_FRAGMENT_NUM);
    memset(pdf, 0, sizeof(t_fmtfragment)*_MAX_DATE_FRAGMENT_NUM);
    pdf[0].fmt_fragment = date_fmt;

    for(i = 0; i < _MAX_VALUE_LEN - 1 && date_fmt[i] != 0 && i < len; ++i) {
        switch(date_fmt[i]) {
        case 'y':
            cur_ft = FT_DATE_YEAR;
            break;
        case 'M':
            cur_ft = FT_DATE_MONTH;
            break;
        case 'd':
            cur_ft = FT_DATE_DAY;
            break;
        case 'H':
            cur_ft = FT_DATE_HOUR;
            break;
        case 'm':
            cur_ft = FT_DATE_MINUTE;
            break;
        case 's':
            cur_ft = FT_DATE_SECOND;
            break;
        case 'S':
            cur_ft = FT_DATE_MILLISECOND;
            break;
        default:
            cur_ft = FT_NORMAL;
        }

        if(cur_ft == last_ft) {
            ++pdf[cur_idx].size;
            continue;
        } else {
            ++cur_idx;
            if(cur_idx >= _MAX_DATE_FRAGMENT_NUM) {
                free(pdf);
                return NULL;
            }

            last_ft = cur_ft;
            pdf[cur_idx].fmt_fragment = date_fmt + i;
            pdf[cur_idx].ft = cur_ft;
            ++pdf[cur_idx].size;
        }
    }
    return pdf;
}


//分析格式
static unsigned ans_fmt(char* format, t_fmtfragment** pff1, char* mask) {
    unsigned i, num = 0, cur_idx = 0, cn = 0, jo = 0;
    t_fmtfragment* pff;
    char sh, last_c = 0;

    for(i = 0; i < _MAX_VALUE_LEN && format[i] != 0; ++i) {
        if(format[i] == '%')
            ++num;
    }

    pff = (t_fmtfragment*)malloc(sizeof(t_fmtfragment) * (num * 2 + 2));
    memset(pff, 0, sizeof(t_fmtfragment)*(num * 2 + 2));
    pff[0].fmt_fragment = format;

    for(i = 0; i < _MAX_VALUE_LEN && format[i] != 0; ++i) {
        if(i > 0)
            last_c = format[i-1];

        if(last_c == '%')
            ++jo;
        else
            jo = 0;

        if(format[i] == '%' && (jo % 2) == 0) {
            if(!(pff[cur_idx].fmt_fragment == NULL && pff[cur_idx].size == 0 && pff[cur_idx].ft == FT_NORMAL))
                ++cur_idx;
            continue;
        }

        if(i == 0) {
            ++pff[cur_idx].size;
            continue;
        }

        if((jo % 2) == 0 && (format[i] != '%')) {
            if(pff[cur_idx].size == 0)
                pff[cur_idx].fmt_fragment = format + i;
            ++pff[cur_idx].size;
            continue;
        }


        if(mask) {
            if(mask[format[i]])
                sh = 0;
            else
                sh = format[i];
        } else
            sh = format[i];

        switch(sh) {
        case 'n':
            pff[cur_idx].ft = FT_NEWLINE;
            pff[cur_idx].size = 1;
            ++cur_idx;
            break;
        case 'd': {
            char *s1, *s2;
            s1 = strchr(format + i, '%');
            s2 = strchr(format + i, '}');

            if((s1 != NULL && s1 < s2) || (format[i+1] != '{') || s2 == NULL) {
                pff[cur_idx].fmt_fragment = format + i;
                ++pff[cur_idx].size;
                break;
            } else {
                t_fmtfragment* ppff = ans_datefmt(format + i + 2, s2 - (format + i + 2));
                if(ppff == NULL) {
                    pff[cur_idx].fmt_fragment = format + i;
                    ++pff[cur_idx].size;
                    break;
                } else {
                    pff[cur_idx].fmt_fragment = (char*)ppff;
                    pff[cur_idx].ft = FT_DATE;
                    i += (unsigned)(s2 - (format + i));
                    ++cur_idx;
                    break;
                }
            }
        }
        //case 'c':pff[cur_idx].ft=FT_CLASSNAME;pff[cur_idx].size=1;cur_idx++;break;
        case 'l':
            pff[cur_idx].ft = FT_LEVEL;
            pff[cur_idx].size = 10;
            ++cur_idx;
            break;
        case 'r':
            pff[cur_idx].ft = FT_REALTIME_MS;
            pff[cur_idx].size = 4;
            ++cur_idx;
            break;
        case 's':
            pff[cur_idx].ft = FT_REALTIME_S;
            pff[cur_idx].size = 12;
            ++cur_idx;
            break;
        case 'p':
            pff[cur_idx].ft = FT_PROCESSID;
            pff[cur_idx].size = 11;
            ++cur_idx;
            break;
        case 'f':
            pff[cur_idx].ft = FT_FILENAME;
            pff[cur_idx].size = 1;
            ++cur_idx;
            break;
        case 'g':
            pff[cur_idx].ft = FT_FUNCNAME;
            pff[cur_idx].size = 1;
            ++cur_idx;
            break;
        case 'h':
            pff[cur_idx].ft = FT_LINENO;
            pff[cur_idx].size = 10;
            ++cur_idx;
            break;
        case 'm':
            pff[cur_idx].ft = FT_USER;
            pff[cur_idx].size = 1;
            ++cur_idx;
            break;
        case 'a':
            pff[cur_idx].ft = FT_APPNAME;
            pff[cur_idx].size = 1;
            ++cur_idx;
            break;
        case 'i':
            pff[cur_idx].ft = FT_IDEN;
            pff[cur_idx].size = 1;
            ++cur_idx;
            break;
        case '0':
            pff[cur_idx].ft = FT_V0;
            pff[cur_idx].size = 1;
            ++cur_idx;
            break;
        case '1':
            pff[cur_idx].ft = FT_V1;
            pff[cur_idx].size = 1;
            ++cur_idx;
            break;
        case '2':
            pff[cur_idx].ft = FT_V2;
            pff[cur_idx].size = 1;
            ++cur_idx;
            break;
        case '3':
            pff[cur_idx].ft = FT_V3;
            pff[cur_idx].size = 1;
            ++cur_idx;
            break;
        case '4':
            pff[cur_idx].ft = FT_V4;
            pff[cur_idx].size = 1;
            ++cur_idx;
            break;
        case '5':
            pff[cur_idx].ft = FT_V5;
            pff[cur_idx].size = 1;
            ++cur_idx;
            break;
        case '6':
            pff[cur_idx].ft = FT_V6;
            pff[cur_idx].size = 1;
            ++cur_idx;
            break;
        case '7':
            pff[cur_idx].ft = FT_V7;
            pff[cur_idx].size = 1;
            ++cur_idx;
            break;
        case '8':
            pff[cur_idx].ft = FT_V8;
            pff[cur_idx].size = 1;
            ++cur_idx;
            break;
        case '9':
            pff[cur_idx].ft = FT_V9;
            pff[cur_idx].size = 1;
            ++cur_idx;
            break;
        case '%':
            pff[cur_idx].ft = FT_PERCENT;
            pff[cur_idx].size = 1;
            ++cur_idx;
            break;
        default:
            if(pff[cur_idx].size == 0) {
                pff[cur_idx].fmt_fragment = format + i - 1;
                ++pff[cur_idx].size;
            }
            ++pff[cur_idx].size;
        }

        if(cur_idx > num * 2 + 1) {
            free(pff);
            return 0;
        }
    }

    if(pff[cur_idx].size == 0 && pff[cur_idx].fmt_fragment == NULL && pff[cur_idx].ft == FT_NORMAL)
        cur_idx--;

    *pff1 = pff;
    return cur_idx + 1;
}


static time_t local_time(time_t rawtime, int tmz[7]) {
#ifdef WIN32
    struct tm        timeinfo;
    SYSTEMTIME        curDateTime;

    time_t now = time(NULL);
    GetLocalTime(&curDateTime);

    if(rawtime != 0) {
        localtime_s(&timeinfo, &rawtime);
        tmz[0] = 1900 + timeinfo.tm_year;
        tmz[1] = 1 + timeinfo.tm_mon;
        tmz[2] = timeinfo.tm_mday;
        tmz[3] = timeinfo.tm_hour;
        tmz[4] = timeinfo.tm_min;
        tmz[5] = timeinfo.tm_sec;
        tmz[6] = curDateTime.wMilliseconds;
        return rawtime;
    } else {
        tmz[0] = curDateTime.wYear;
        tmz[1] = curDateTime.wMonth;
        tmz[2] = curDateTime.wDay;
        tmz[3] = curDateTime.wHour;
        tmz[4] = curDateTime.wMinute;
        tmz[5] = curDateTime.wSecond;
        tmz[6] = curDateTime.wMilliseconds;
        return now;
    }

#else
    struct timeval ts;
    struct tm* timeinfo;

    if(rawtime == 0) {
        gettimeofday(&ts, NULL);
    }

    else {
        ts.tv_sec = rawtime;
        ts.tv_usec = 0;
    }

    timeinfo = localtime(&ts.tv_sec);
    tmz[0] = 1900 + timeinfo->tm_year;
    tmz[1] = 1 + timeinfo->tm_mon;
    tmz[2] = timeinfo->tm_mday;
    tmz[3] = timeinfo->tm_hour;
    tmz[4] = timeinfo->tm_min;
    tmz[5] = timeinfo->tm_sec;
    tmz[6] = ts.tv_usec / 1000;

    return ts.tv_sec;
#endif
}


static size_t sprint_fmt(time_t now, int tmz[], char* out_buf_str, size_t len, t_fmtfragment* pff, unsigned num, t_logparam* lp, const char* appname, const char* iden, const char* var[], const char* pam, va_list arg) {
#define __VARSTUFF(a) case FT_V ## a : if(var!=NULL){if(var[a]==NULL) v="";else v=var[a];}else v="";sc_size=strlen(v);if(sc_size+buf_len>=len){out_buf_str[buf_len]=0;return buf_len;} memcpy(buf_cur_pos, v, sc_size);buf_len+=sc_size;break;

    unsigned i = 0;
    char sc[_MAX_VALUE_LEN];
    const char* v;
    char datafmt[] = "%04d";
    size_t sc_size = 0, buf_len = 0;

    for(i = 0; i < num; ++i) {
        char* buf_cur_pos = out_buf_str + buf_len;
        if(pff[i].size == 0 && pff[i].fmt_fragment == NULL && pff[i].ft == FT_NORMAL)
            break;

        if(buf_len + pff[i].size >= len)
            break;

        switch(pff[i].ft) {
        case FT_NORMAL:
            memcpy(buf_cur_pos, pff[i].fmt_fragment, pff[i].size);
            buf_len += pff[i].size;
            break;
        case FT_NEWLINE:
            *buf_cur_pos = '\n';
            buf_len += 1;
            break;
        case FT_DATE:
            buf_len += sprint_fmt(now, tmz, buf_cur_pos, len - buf_len, (t_fmtfragment*)pff[i].fmt_fragment, _MAX_DATE_FRAGMENT_NUM, lp, appname, iden, var, pam, arg);
            break;
        case FT_DATE_YEAR:
            ;
        case FT_DATE_MONTH:
            ;
        case FT_DATE_DAY:
            ;
        case FT_DATE_HOUR:
            ;
        case FT_DATE_MINUTE:
            ;
        case FT_DATE_SECOND:
            ;
        case FT_DATE_MILLISECOND:
            datafmt[2] = (pff[i].size % 10) + '0';
            sprintf(sc, datafmt, (tmz[pff[i].ft - FT_DATE_YEAR]) % (_pow(10, (pff[i].size))));
            sc_size = strlen(sc);
            memcpy(buf_cur_pos, sc, sc_size);
            buf_len += sc_size;
            break;
        case FT_LINENO:
            sprintf(sc, "%d", lp->line_no);
            sc_size = strlen(sc);
            memcpy(buf_cur_pos, sc, sc_size);
            buf_len += sc_size;
            break;
        case FT_FUNCNAME:
            sc_size = strlen(lp->func_name);
            if(sc_size + buf_len >= len) {
                out_buf_str[buf_len] = 0;
                return buf_len;
            }

            memcpy(buf_cur_pos, lp->func_name, sc_size);
            buf_len += sc_size;
            break;
            //case FT_CLASSNAME: sc_size=strlen(lp->class_name);if(sc_size+total>=len){buf[total]=0;return total;} memcpy(cur, lp->class_name, sc_size);total+=sc_size;break;
        case FT_FILENAME:
            sc_size = strlen(lp->file_name);
            if(sc_size + buf_len >= len) {
                out_buf_str[buf_len] = 0;
                return buf_len;
            }

            memcpy(buf_cur_pos, lp->file_name, sc_size);
            buf_len += sc_size;
            break;
        case FT_APPNAME:
            sc_size = strlen(appname);
            if(sc_size + buf_len >= len) {
                out_buf_str[buf_len] = 0;
                return buf_len;
            }

            memcpy(buf_cur_pos, appname, sc_size);
            buf_len += sc_size;
            break;
        case FT_PROCESSID:
            sprintf(sc, "%d", GETPID());
            sc_size = strlen(sc);
            memcpy(buf_cur_pos, sc, sc_size);
            buf_len += sc_size;
            break;
        case FT_LEVEL :
            if(lp->log_level < _LNONE && lp->log_level >= 0) {
                sc_size = strlen(_level_name[lp->log_level]);
                memcpy(buf_cur_pos, _level_name[lp->log_level], sc_size);
            } else {
                sprintf(sc, "LEVEL[%d]", lp->log_level);
                sc_size = strlen(sc);
                memcpy(buf_cur_pos, sc, sc_size);
            }
            buf_len += sc_size;
            break;
        case FT_REALTIME_S:
            sprintf(sc, "%011d", (int)now);
            sc_size = strlen(sc);
            memcpy(buf_cur_pos, sc, sc_size);
            buf_len += sc_size;
            break;
        case FT_REALTIME_MS:
            sprintf(sc, "%03d", tmz[6]);
            sc_size = strlen(sc);
            memcpy(buf_cur_pos, sc, sc_size);
            buf_len += sc_size;
            break;
        case FT_USER:
            sc_size = vsnprintf(buf_cur_pos, len - buf_len, pam, arg);
            if(sc_size + buf_len >= len) {
                out_buf_str[len-1] = 0;
                return len - 1;
            }
            buf_len += sc_size;
            break;
        case FT_IDEN:
            sc_size = strlen(iden);
            if(sc_size + buf_len >= len) {
                out_buf_str[buf_len] = 0;
                return buf_len;
            }

            memcpy(buf_cur_pos, iden, sc_size);
            buf_len += sc_size;
            break;
        case FT_PERCENT:
            *buf_cur_pos = '%';
            buf_len += 1;
            break;
            __VARSTUFF(0)
            __VARSTUFF(1)
            __VARSTUFF(2)
            __VARSTUFF(3)
            __VARSTUFF(4)
            __VARSTUFF(5)
            __VARSTUFF(6)
            __VARSTUFF(7)
            __VARSTUFF(8)
            __VARSTUFF(9)
        }

        if(buf_len >= len) {
            out_buf_str[len-1] = 0;
            return len - 1;
        }
    }

    out_buf_str[buf_len] = 0;
    return buf_len;
}


static size_t getfilesize(const char* filename) {
    struct stat file_stat;
    if(stat(filename, &file_stat) < 0)
        return 0;
    else
        return (size_t)file_stat.st_size;
}


static void openlogfile(t_log* pl, ...) {
    t_logparam lp;
    size_t fn_len;
    va_list argptr;
    char open_type[] = "a+";
    time_t now;
    int tmz[7], tmz_now[7];

    if(pl->fp) {
        int need_close = 0;
        now = time(NULL);

        if(pl->lc.output_type != OT_FILE)
            return;

        if(pl->lc.truncate_type & TT_DALIY) {
            local_time(pl->last_log, tmz);
            local_time(now, tmz_now);
            if(tmz[0] != tmz_now[0] || tmz[1] != tmz_now[1] || tmz[2] != tmz_now[2])
                need_close = 1;
        }

        if((pl->lc.truncate_type & TT_ROLLING) && (pl->total_write >= pl->lc.max_filesize && now - pl->last_log > 0))
            need_close = 1;

        if((pl->lc.truncate_type & TT_INTERVAL) && now - pl->last_log > 0)
            need_close = 1;

        if(need_close)
            fclose(pl->fp);
        else
            return;
    }

    if(pl->lc.path_len >= pl->lc.max_buf - 1)
        return;

    strncpy(pl->buf, pl->lc.log_path, pl->lc.max_buf - 1);

    va_start(argptr, pl);
    now = local_time(0, tmz_now);
    fn_len = sprint_fmt(now, tmz_now, pl->buf + pl->lc.path_len, pl->lc.max_buf - pl->lc.path_len, pl->filename_fmt, pl->ff_num, &lp, pl->app_name, pl->iden, NULL, pl->file_nameparam, argptr);
    va_end(argptr);

    if(pl->lc.filelast)
        pl->total_write = getfilesize(pl->buf);

    if(pl->lc.open_clear)
        open_type[1] = 0;

    pl->fp = fopen(pl->buf, open_type);
}

const char* l4c_strerr(int errcode) {
    switch(errcode) {
    case LOGERR_OUTPUTTYPE    :
        return "[LOGERR_OUTPUTTYPE]输出类型不正确";
    case LOGERR_MALLOC        :
        return "[LOGERR_MALLOC]内存分配失败";
    case LOGERR_ANS            :
        return "[LOGERR_ANS]日志格式解析错误";
    case LOGERR_LOGTOOLONG    :
        return "[LOGERR_LOGTOOLONG]日志内容过长";
    case LOGERR_OPENFILE    :
        return "[LOGERR_OPENFILE]日志文件打开失败";
    case LOGERR_LOADDEF        :
        return "[LOGERR_LOADDEF]加载默认配置失败";
    case LOGERR_HANDLE        :
        return "[LOGERR_LOADDEF]日志句柄无效";
    case 0                    :
        return "正确";
    default:
        return "未知";
    }
}

void l4c_close(t_loghndl* plh) {
    unsigned i = 0;
    t_log* pl = (t_log*)plh;
    if(pl->fp != NULL && pl->lc.output_type == OT_FILE)
        fclose(pl->fp);

    if(pl->buf)
        free(pl->buf);

    if(pl->lc.need_lock)
        pthread_mutex_destroy(&pl->lock);

    if(pl->file_nameparam)
        free(pl->file_nameparam);

    if(pl->filename_fmt) {
        for(i = 0; i < pl->ff_num; ++i) {
            if(pl->filename_fmt[i].ft == FT_DATE && pl->filename_fmt[i].fmt_fragment != NULL)
                free(pl->filename_fmt[i].fmt_fragment);
        }
        free(pl->filename_fmt);
    }

    if(pl->main_fmt) {
        for(i = 0; i < pl->mf_num; ++i) {
            if(pl->main_fmt[i].ft == FT_DATE && pl->main_fmt[i].fmt_fragment != NULL)
                free(pl->main_fmt[i].fmt_fragment);
        }
        free(pl->main_fmt);
    }

    if(pl->iden)
        free((void*)pl->iden);
    memset(pl, 0, sizeof(t_log));
}

int l4c_open(t_loghndl* plh, const char* cfgpath, const char* main_tag, const char* fileparam, const char* iden) {
    char value[_MAX_VALUE_LEN];
    char mask[256];
    t_log* pl = (t_log*)plh;
    memset(mask, 1, 26);
    mask['m'] = 0;
    mask['d'] = 0;
    mask['r'] = 0;
    mask['p'] = 0;
    mask['a'] = 0;

    memset(pl, 0, sizeof(t_log));
    l4c_getcfgstr(cfgpath, main_tag, "OutputType", value, _MAX_VALUE_LEN, "");
    lrtrim(value);

    if(value[0] == 0)
        return LOGERR_OUTPUTTYPE;

    pl->lc.output_type = (e_outputtype)-1;
    switch(value[strlen(value)-1]) {
    case 'L':
        if(strcmp(value, "NULL") == 0)    pl->lc.output_type = OT_NULL;
        break;
    case 'T':
        if(strcmp(value, "STDOUT") == 0)  pl->lc.output_type = OT_STDOUT;
        break;
    case 'R':
        if(strcmp(value, "STDERR") == 0)  pl->lc.output_type = OT_STDERR;
        break;
    case 'E':
        if(strcmp(value, "FILE") == 0)    pl->lc.output_type = OT_FILE;
        break;
    case 'G':
        if(strcmp(value, "SYSLOG") == 0)  pl->lc.output_type = OT_SYSLOG;
        break;
    default:
        ;
    }

    if(pl->lc.output_type == -1)
        return LOGERR_OUTPUTTYPE;

    l4c_getcfgstr(cfgpath, main_tag, "Level", value, _MAX_VALUE_LEN, "");
    lrtrim(value);
    if(value[0] == 0)
        return LOGERR_OUTPUTTYPE;

    pl->lc.lowest_level = (e_loglevel) - 1;
    switch(value[0]) {
    case 'A':
        if(strcmp(value, "ALL"     ) == 0) pl->lc.lowest_level = _LALL;
        break;
    case 'T':
        if(strcmp(value, "TRACE"   ) == 0) pl->lc.lowest_level = _LTRACE;
        break;
    case 'D':
        if(strcmp(value, "DEBUG"   ) == 0) pl->lc.lowest_level = _LDEBUG;
        break;
    case 'I':
        if(strcmp(value, "INFO"    ) == 0) pl->lc.lowest_level = _LINFO;
        break;
    case 'W':
        if(strcmp(value, "WARN"    ) == 0) pl->lc.lowest_level = _LWARN;
        break;
    case 'E':
        if(strcmp(value, "ERROR"   ) == 0) pl->lc.lowest_level = _LERROR;
        break;
    case 'F':
        if(strcmp(value, "FATAL"   ) == 0) pl->lc.lowest_level = _LFATAL;
        break;
    case 'B':
        if(strcmp(value, "BULLETIN") == 0) pl->lc.lowest_level = _LBULLETIN;
        break;
    case 'N':
        if(strcmp(value, "NONE"    ) == 0) pl->lc.lowest_level = _LNONE;
        break;
    default:
        ;
    }

    if(pl->lc.lowest_level == (e_loglevel) - 1)
        return LOGERR_OUTPUTTYPE;

    l4c_getcfgstr(cfgpath, main_tag, "FlushType", value, _MAX_VALUE_LEN, "");
    lrtrim(value);
    if(value[0] == 0)
        strcpy(value, "CLOSE");

    pl->lc.flush_type = (e_flushtype)-1;
    switch(value[0]) {
    case 'E':
        if(strcmp(value, "EVERY") == 0) pl->lc.flush_type = HT_EVERY;
        break;
    case 'L':
        if(strcmp(value, "LINE" ) == 0) pl->lc.flush_type = HT_LINE;
        break;
    case 'C':
        if(strcmp(value, "CLOSE") == 0) pl->lc.flush_type = HT_CLOSE;
        break;
    default:
        ;
    }

    if(pl->lc.flush_type == -1)
        pl->lc.flush_type = HT_CLOSE;

    l4c_getcfgstr(cfgpath, main_tag, "TruncateType", value, _MAX_VALUE_LEN, "NEVER");
    pl->lc.truncate_type = transtt(value);

    l4c_getcfgstr(cfgpath, main_tag, "Format", pl->lc.log_fmt, _MAX_VALUE_LEN, "%m");
    l4c_getcfgstr(cfgpath, main_tag, "LogPath", pl->lc.log_path, _MAX_PATH_LEN, "");
    l4c_getcfgstr(cfgpath, main_tag, "LogName", pl->lc.log_name, _MAX_PATH_LEN, "a.log");

    pl->lc.max_filesize = l4c_getcfgint(cfgpath, main_tag, "MaxFileSize", -1);
    pl->lc.interval = l4c_getcfgint(cfgpath, main_tag, "Interval", -1);
    pl->lc.auto_newline = l4c_getcfgint(cfgpath, main_tag, "AutoNewline", 1);
    if(pl->lc.auto_newline) {
        if(strlen(pl->lc.log_fmt) < _MAX_VALUE_LEN - 2)
            strcat(pl->lc.log_fmt, "%n");
    }

    pl->lc.max_buf = l4c_getcfgint(cfgpath, main_tag, "BufferSize", 4 * 1024);
    pl->lc.filelast = l4c_getcfgint(cfgpath, main_tag, "LogLastingOpen", 1);
    pl->lc.need_lock = l4c_getcfgint(cfgpath, main_tag, "NeedLock", 0);
    if(pl->lc.need_lock)
        pthread_mutex_init(&pl->lock, NULL);

    pl->lc.muti_thread = l4c_getcfgint(cfgpath, main_tag, "MutiThread", 1);
    if(!pl->lc.muti_thread)
        pl->lc.need_lock = 0;

#ifdef _L4C_NOPTHREAD
    pl->lc.need_lock = 0;
    pl->lc.muti_thread = 0;
#endif
    pl->lc.open_clear = l4c_getcfgint(cfgpath, main_tag, "LogOpenClear", 0);
    pl->lc.path_len = strlen(pl->lc.log_path);

    pl->iden = (char*)malloc(_MAX_VALUE_LEN);
    if(pl->iden == NULL)
        return LOGERR_MALLOC;

    strncpy(pl->iden, iden, _MAX_VALUE_LEN - 1);
    pl->iden[_MAX_VALUE_LEN-1] = 0;
    pl->iden_size = strlen(pl->iden);
    pl->buf = (char*)malloc(pl->lc.max_buf);
    if(pl->buf == NULL) {
        l4c_close(plh);
        return LOGERR_MALLOC;
    }

    if(fileparam == NULL)
        fileparam = "";

    pl->file_nameparam = (char*)malloc(strlen(fileparam) + 16);
    if(pl->file_nameparam == NULL) {
        l4c_close(plh);
        return LOGERR_MALLOC;
    }

    strcpy(pl->file_nameparam, fileparam);

    pl->mf_num = ans_fmt(pl->lc.log_fmt, &pl->main_fmt, NULL);
    if(!pl->mf_num) {
        l4c_close(plh);
        return LOGERR_ANS;
    }

    pl->ff_num = ans_fmt(pl->lc.log_name, &pl->filename_fmt, mask);
    if(!pl->ff_num) {
        l4c_close(plh);
        return LOGERR_ANS;
    }

    pl->app_name = get_progname();
    return 0;
}

static void dstmem(void* p) {
    free(p);
}

static t_loghndl *defhandle() {
    t_loghndl *plh = NULL;
    if(gsn_is_first) {
        t_log *pl;
        pthread_mutex_lock(&_loglock);
        if(!gsn_is_first) {
            pthread_mutex_unlock(&_loglock);
            goto notfirst;
        }

        plh = (t_loghndl *)malloc(sizeof(t_log));
        if(plh == NULL) {
            gsn_is_first = 0;
            pthread_mutex_unlock(&_loglock);
            return NULL;
        }

        pl = (t_log *)plh;
        if(l4c_open(plh, g_l4c_cfg_path, ".DEFAULT", "", "") != 0) {
            if(getenv("L4C_CFG") == NULL) {
                l4c_close(plh);
                gsn_is_first = 0;
                pthread_mutex_unlock(&_loglock);
                return NULL;
            }

            if(l4c_open(plh, getenv("L4C_CFG"), ".DEFAULT", "", "") != 0) {
                l4c_close(plh);
                gsn_is_first = 0;
                pthread_mutex_unlock(&_loglock);
                return NULL;
            }
        }

        if(!pl->lc.need_lock && pl->lc.muti_thread) { //如果是多线程程序，则强制加锁
            pthread_mutex_init(&pl->lock, NULL);
            pl->lc.need_lock = 1;
        }

        if(pl->lc.muti_thread) {
            if(!pthread_key_create(&_logkey, dstmem))
                gsn_isinit = 1;
        }

        _plh = plh;
        gsn_is_first = 0;
        pthread_mutex_unlock(&_loglock);
    }

notfirst:
    if(plh == NULL)
        plh = _plh;
    return plh;
}

static int vlogr(t_logparam lp, t_loghndl* plh, const char* var[], const char* fmt, va_list vl) {
    int tmz[7];
    size_t write_size;
    t_log* pl = (t_log*)plh;
    time_t now;

    if(pl->lc.output_type == OT_NULL || lp.log_level < pl->lc.lowest_level)
        return 0;

    if(pl->lc.need_lock)
        pthread_mutex_lock(&pl->lock);

    if(pl->lc.output_type == OT_STDOUT)
        pl->fp = stdout;
    else if(pl->lc.output_type == OT_FILE) {
        openlogfile(pl);
        if(pl->fp == NULL) {
            if(pl->lc.need_lock)
                pthread_mutex_unlock(&pl->lock);
            return LOGERR_OPENFILE;
        }
    } else if(pl->lc.output_type == OT_STDERR)
        pl->fp = stderr;
    else
        pl->fp = NULL;

    now = local_time(0, tmz);
    write_size = sprint_fmt(now, tmz, pl->buf, pl->lc.max_buf, pl->main_fmt, pl->mf_num, &lp, pl->app_name, pl->iden, var, fmt, vl);
    if(pl->fp)
        fputs(pl->buf, pl->fp);

    pl->total_write += write_size;
    if(pl->lc.output_type == OT_FILE) {
        if(pl->lc.filelast) {
            if(pl->lc.flush_type == HT_EVERY)
                fflush(pl->fp);
            else if(pl->lc.flush_type == HT_LINE) {
                if(strchr(pl->buf, '\n'))
                    fflush(pl->fp);
            }
        } else {
            fclose(pl->fp);
            pl->fp = NULL;
            pl->total_write = 0;
        }
    }

    if(pl->lc.need_lock)
        pthread_mutex_unlock(&pl->lock);
    return (int)write_size;
}

int l4c_vlog(t_logparam lp, t_loghndl* plh, const char* fmt, va_list vl) {
    if(plh == NULL)
        return LOGERR_HANDLE;
    return vlogr(lp, plh, NULL, fmt, vl);
}

int l4c_vlogex(t_logparam lp, t_loghndl* plh, const char* var[], const char* fmt, va_list vl) {
    if(plh == NULL)
        return LOGERR_HANDLE;
    return vlogr(lp, plh, var, fmt, vl);
}

t_logparam tmp_fetch(int ll, int line_no, const char* func_name, const char* file_name) {
    t_logparam p;
    p.file_name = file_name;
    p.func_name = func_name;
    p.line_no = line_no;
    p.log_level = (e_loglevel)ll;
    return p;
}

int l4c_log(t_logparam lp, t_loghndl* plh, const char* fmt, ...) {
    int ret;
    va_list argptr;

    va_start(argptr, fmt);
    ret = l4c_vlog(lp, plh, fmt, argptr);
    va_end(argptr);
    return ret;
}

int l4c_logex(t_logparam lp, t_loghndl* plh, const char* var[], const char* fmt, ...) {
    int ret;
    va_list argptr;

    va_start(argptr, fmt);
    ret = l4c_vlogex(lp, plh, var, fmt, argptr);
    va_end(argptr);
    return ret;
}

int l4c_vlogdef(const char* fmt, va_list vl) {
    t_logparam lp = {(e_loglevel)0, 0, "", ""};
    t_logparam* pla;
    t_log* pl;

    t_loghndl* plh = defhandle();
    if(plh == NULL)
        return LOGERR_LOADDEF;

    pl = (t_log*)plh;
    if(pl->lc.muti_thread)
        pla = (t_logparam*)pthread_getspecific(_logkey);
    else
        pla = &_lp;

    if(pla == NULL)
        pla = &lp;
    return vlogr(*pla, plh, NULL, fmt, vl);
}

int l4c_vlogdefex(const char* var[], const char* fmt, va_list vl) {
    t_logparam lp = {(e_loglevel)0, 0, "", ""};
    t_logparam* pla;
    t_log* pl;

    t_loghndl* plh = defhandle();
    if(plh == NULL)
        return LOGERR_LOADDEF;

    pl = (t_log*)plh;
    if(pl->lc.muti_thread)
        pla = (t_logparam*)pthread_getspecific(_logkey);
    else
        pla = &_lp;

    if(pla == NULL)
        pla = &lp;
    return vlogr(*pla, plh, var, fmt, vl);
}

void tmp_set(int log_level, int line_no, const char* func_name, const char* file_name) {
    t_logparam *pla = NULL;
    t_log *pl = NULL;

    pl = (t_log*)defhandle();
    if(pl == NULL)
        return;

    if(pl->lc.muti_thread)
        pla = (t_logparam*)pthread_getspecific(_logkey);
    else
        pla = &_lp;

    if(gsn_isinit && pl->lc.muti_thread && pla == NULL) {
        pla = (t_logparam*)malloc(sizeof(t_logparam));
        if(pla == NULL)
            return ;

        if(pthread_setspecific(_logkey, pla))
            free(pla);
    }

    if(pla) {
        pla->file_name = file_name;
        pla->func_name = func_name;
        pla->line_no = line_no;
        pla->log_level = (e_loglevel)log_level;
    }
}

int l4c_logdef(const char* fmt, ...) {
    int ret;
    va_list argptr;

    va_start(argptr, fmt);
    ret = l4c_vlogdef(fmt, argptr);
    va_end(argptr);
    return ret;
}

int l4c_logdefex(const char* var[], const char* fmt, ...) {
    int ret;
    va_list argptr;

    va_start(argptr, fmt);
    ret = l4c_vlogdefex(var, fmt, argptr);
    va_end(argptr);
    return ret;
}


int l4c_idenset(t_loghndl* plh, const char* iden) {
    t_log* pl = (t_log*)plh;

    if(plh == NULL) {
        plh = defhandle();
        if(plh == NULL)
            return LOGERR_LOADDEF;
        pl = (t_log*)plh;
    }

    if(pl->lc.need_lock)
        pthread_mutex_lock(&pl->lock);

    strncpy(pl->iden, iden, _MAX_VALUE_LEN - 1);
    pl->iden[_MAX_VALUE_LEN-1] = 0;
    pl->iden_size = strlen(pl->iden);

    if(pl->lc.need_lock)
        pthread_mutex_unlock(&pl->lock);

    return 0;
}

int l4c_lvset(t_logparam lp, t_loghndl* plh) {
    t_log* pl = (t_log*)plh;
    if(plh == NULL) {
        plh = defhandle();
        if(plh == NULL)
            return LOGERR_LOADDEF;
        pl = (t_log*)plh;
    }

    if(pl->lc.need_lock)
        pthread_mutex_lock(&pl->lock);

    pl->lc.lowest_level = lp.log_level;
    if(pl->lc.need_lock)
        pthread_mutex_unlock(&pl->lock);
    return 0;
}

t_logparam l4c_lvget(t_loghndl* plh) {
    t_logparam lp = {(e_loglevel)0, 0, "", ""};
    t_log* pl = (t_log*)plh;
    if(plh == NULL) {
        plh = defhandle();
        if(plh == NULL)
            return lp;
        pl = (t_log*)plh;
    }

    lp.log_level = pl->lc.lowest_level;
    return lp;
}

int l4c_lvcmp(t_logparam lp1, t_logparam lp2) {
    if(lp1.log_level == lp2.log_level)
        return 0;
    else if(lp1.log_level > lp2.log_level)
        return 1;
    else
        return -1;
}
