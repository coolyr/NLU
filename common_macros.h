#ifndef DIALOG_MACROS_H_
#define DIALOG_MACROS_H_

/* Namespaces */
#define DIALOG_SEG_BEGIN_NAMESPACE(x) namespace dialog { namespace x {
#define DIALOG_SEG_END_NAMESPACE(x) } }
#define DIALOG_SEG_USE_NAMESPACE(x) using namespace dialog::x
#define BEGIN_DIALOG_SEG_NAMESPACE namespace dialog {
#define END_DIALOG_SEG_NAMESPACE }

#define ALIAS_NAMESAPCE(x, y) namespace dialog { namespace x = y; }
#define DIALOG_SEG_NS(x) dialog::x

/* Compiler */
#define DIALOG_SEG_LIKELY(x) __builtin_expect(!!(x), 1)
#define DIALOG_SEG_UNLIKELY(x) __builtin_expect(!!(x), 0)

/* Assignment */
#define COPY_CONSTRUCTOR(T) \
    T(const T &); \
    T & operator=(const T &);
#define DISALLOW_COPY_AND_ASSIGN(TypeName) COPY_CONSTRUCTOR(TypeName)

/* Resource Management */
#define SAFE_FREE(ptr) do {\
    if ((ptr) != NULL) {\
        free(ptr);\
        (ptr) = NULL;\
    }\
} while (false)

#define SAFE_DELETE(ptr) do {\
    if ((ptr) != NULL) {\
        delete (ptr);\
        (ptr) = NULL;\
    }\
} while (false)

#define SAFE_DELETE_ARRAY(ptr) do {\
    if ((ptr) != NULL) {\
        delete[] (ptr);\
        (ptr) = NULL;\
    }\
} while (false)

#define SAFE_CLOSE(fd) do {\
    if ((fd) != -1) {\
        close(fd);\
        (fd) = -1;\
    }\
} while (false)

#define SAFE_FCLOSE(fp) do {\
    if ((fp) != NULL) {\
        fclose(fp);\
        (fp) = NULL;\
    }\
} while (false)

/* Checker */
#define CHECK_PARAM(cond, ret) \
    if (cond) {\
        return ret;\
    }

#endif  // DIALOG_MACROS_H_

