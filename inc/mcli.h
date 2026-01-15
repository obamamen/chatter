/* ================================== *\
 @file     mcli.h
 @project  mcli
 @author   moosm
 @date     1/12/2026
*\ ================================== */

#ifndef MCLI_MCLI_H
#define MCLI_MCLI_H

// ================================================
// PLATFORM DETECTION
//

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#   define MCLI_WINDOWS
#elif defined(__unix__) || defined(__unix) || defined(__linux__) || defined(__APPLE__)
#   define MCLI_UNIX
#else
#   error PLATFORM_NOT_SUPPORTED
#endif

#ifdef MCLI_WINDOWS
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#   include <conio.h>
#endif

#ifdef MCLI_UNIX
#   include <termios.h>
#   include <unistd.h>
#   include <sys/select.h>
#   include <sys/ioctl.h>
#endif

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


///////////////////////////////////////
//             INTERFACE
///////////////////////////////////////

// ================================================
// COLOR & STYLE CODES
//

#define MCLI_RESET              "\033[0m"

#define MCLI_FG_RED             "\033[31m"
#define MCLI_FG_GREEN           "\033[32m"
#define MCLI_FG_YELLOW          "\033[33m"
#define MCLI_FG_BLUE            "\033[34m"
#define MCLI_FG_MAGENTA         "\033[35m"
#define MCLI_FG_CYAN            "\033[36m"
#define MCLI_FG_WHITE           "\033[37m"
#define MCLI_FG_DEFAULT         "\033[39m"

#define MCLI_BG_RED             "\033[41m"
#define MCLI_BG_GREEN           "\033[42m"
#define MCLI_BG_YELLOW          "\033[43m"
#define MCLI_BG_BLUE            "\033[44m"
#define MCLI_BG_DEFAULT         "\033[49m"

#define MCLI_BOLD               "\033[1m"
#define MCLI_DIM                "\033[2m"
#define MCLI_UNDERLINE          "\033[4m"
#define MCLI_REVERSE            "\033[7m"

#define MCLI_ATTR_RESET         "\033[22;24;27m"


// ================================================
// CHARACTER CODES (ASCII)
//

#define MCLI_CHAR_NONE          0

#define MCLI_CHAR_NUL           0
#define MCLI_CHAR_BEL           7
#define MCLI_CHAR_BS            8
#define MCLI_CHAR_TAB           9
#define MCLI_CHAR_LF            10
#define MCLI_CHAR_CR            13
#define MCLI_CHAR_ESC           27
#define MCLI_CHAR_DEL           127

#define MCLI_ASCII_MIN          0
#define MCLI_ASCII_MAX          127

#define MCLI_ASCII_MIN_PRINT    32
#define MCLI_ASCII_MAX_PRINT    126


// ================================================
// KEY CODES (NON ASCII)
//

#define MCLI_KEY_NONE           0
#define MCLI_KEY_BASE           256

#define MCLI_KEY_UP             (MCLI_KEY_BASE + 1)
#define MCLI_KEY_DOWN           (MCLI_KEY_BASE + 2)
#define MCLI_KEY_LEFT           (MCLI_KEY_BASE + 3)
#define MCLI_KEY_RIGHT          (MCLI_KEY_BASE + 4)

#define MCLI_KEY_HOME           (MCLI_KEY_BASE + 5)
#define MCLI_KEY_END            (MCLI_KEY_BASE + 6)
#define MCLI_KEY_PAGE_UP        (MCLI_KEY_BASE + 7)
#define MCLI_KEY_PAGE_DOWN      (MCLI_KEY_BASE + 8)

#define MCLI_KEY_INSERT         (MCLI_KEY_BASE + 9)
#define MCLI_KEY_DELETE         (MCLI_KEY_BASE + 10)

#define MCLI_KEY_F1             (MCLI_KEY_BASE + 11)
#define MCLI_KEY_F2             (MCLI_KEY_BASE + 12)
#define MCLI_KEY_F3             (MCLI_KEY_BASE + 13)
#define MCLI_KEY_F4             (MCLI_KEY_BASE + 14)
#define MCLI_KEY_F5             (MCLI_KEY_BASE + 15)
#define MCLI_KEY_F6             (MCLI_KEY_BASE + 16)
#define MCLI_KEY_F7             (MCLI_KEY_BASE + 17)
#define MCLI_KEY_F8             (MCLI_KEY_BASE + 18)
#define MCLI_KEY_F9             (MCLI_KEY_BASE + 19)
#define MCLI_KEY_F10            (MCLI_KEY_BASE + 20)
#define MCLI_KEY_F11            (MCLI_KEY_BASE + 21)
#define MCLI_KEY_F12            (MCLI_KEY_BASE + 22)


// ================================================
// CORE TYPES
//

enum mcli_result
{
    mcli_ok = 0,
    mcli_error = 1,
};
typedef enum mcli_result mcli_result_t;


// ================================================
// INITIALIZATION & CLEANUP
//

// ----------------------------------------------------------------
// initialize the library.
//
// must be called before any mcli functions are called.
// ----------------------------------------------------------------
// returns: mcli_ok on success, mcli_error on failure.
mcli_result_t mcli_initialize(void);

// ----------------------------------------------------------------
// shuts down / cleans-up the library.
//
// must be called after all mcli functions.
// ----------------------------------------------------------------
// returns: mcli_ok on success, mcli_error on failure.
mcli_result_t mcli_cleanup(void);


// ================================================
// RAW MODE CONTROL
//

// ----------------------------------------------------------------
// enable/disable raw mode.
//
// no echo and no line buffering.
// ----------------------------------------------------------------
// returns: mcli_ok on success, mcli_error on failure.
mcli_result_t mcli_raw_mode(bool enable);


// ================================================
// LOW-LEVEL INPUT
//

// ----------------------------------------------------------------
// checks if input is available.
//
// (non blocking)
// ----------------------------------------------------------------
bool mcli_kbhit(void);

// ----------------------------------------------------------------
// get single char.
//
// (blocking)
// ----------------------------------------------------------------
int mcli_getchar(void);

// ----------------------------------------------------------------
// get single char.
//
// (non blocking)
// ----------------------------------------------------------------
// returns: -1 if none.
int mcli_getchar_nb(void);


// ================================================
// HIGH-LEVEL INPUT
//

// ----------------------------------------------------------------
// get key (ASCII char or MCLI_KEY_*)
//
// (blocking)
// ----------------------------------------------------------------
int mcli_getkey(void);

// ----------------------------------------------------------------
// get key (ASCII char or MCLI_KEY_*)
//
// non-blocking, returns MCLI_KEY_NONE if none
// ----------------------------------------------------------------
int mcli_getkey_nb(void);


// ================================================
// TERMINAL INFO
//

// ----------------------------------------------------------------
// get terminal size.
// ----------------------------------------------------------------
// returns: mcli_ok on success, mcli_error on failure.
mcli_result_t mcli_term_size(int* rows, int* cols);


// ================================================
// CURSOR & SCREEN CONTROL
//

// ----------------------------------------------------------------
// moves cursor to absolute (col, row)
//
// 1 indexed.
// ----------------------------------------------------------------
void mcli_cursor_move(int row, int col);


// ----------------------------------------------------------------
// functions for moving the cursor relatively.
// ----------------------------------------------------------------
void mcli_cursor_up(int n);
void mcli_cursor_down(int n);
void mcli_cursor_forward(int n);
void mcli_cursor_back(int n);

// ----------------------------------------------------------------
// moves cursor to begin of line.
// ----------------------------------------------------------------
void mcli_cursor_home(void);

// ----------------------------------------------------------------
// cursor visibility functions.
// ----------------------------------------------------------------
void mcli_cursor_show(void);
void mcli_cursor_hide(void);

// ----------------------------------------------------------------
// position saving / loading.
// ----------------------------------------------------------------
void mcli_cursor_save(void);
void mcli_cursor_restore(void);

// ----------------------------------------------------------------
// clearing operations.
// ----------------------------------------------------------------
void mcli_clear_screen(void);
void mcli_clear_line(void);
void mcli_clear_to_end(void);
void mcli_clear_to_start(void);

// ================================================
// KEY INFO
//

static inline bool mcli_is_ascii(int ch)
{
    return ch >= MCLI_ASCII_MIN && ch <= MCLI_ASCII_MAX;
}

static inline bool mcli_is_printable(int ch)
{
    return ch >= MCLI_ASCII_MIN_PRINT && ch <= MCLI_ASCII_MAX_PRINT;
}

static inline bool mcli_is_backspace(int key)
{
    return key == MCLI_CHAR_BS || key == MCLI_CHAR_DEL;
}

static inline bool mcli_is_enter(int key)
{
    return key == MCLI_CHAR_LF || key == MCLI_CHAR_CR;
}

static inline bool mcli_is_tab(int key)
{
    return key == MCLI_CHAR_TAB;
}

static inline bool mcli_is_escape(int key)
{
    return key == MCLI_CHAR_ESC;
}

///////////////////////////////////////
//              SOURCE
///////////////////////////////////////
#if defined(MCLI_SOURCE)

#ifdef MCLI_UNIX
static struct termios g_orig_termios;
static bool g_raw_mode_active = false;
#endif


mcli_result_t mcli_initialize(void)
{
#ifdef MCLI_WINDOWS
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        return mcli_error;
    }

    DWORD mode = 0;
    if (!GetConsoleMode(hOut, &mode))
    {
        return mcli_error;
    }

    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    const WINBOOL r = SetConsoleMode(hOut, mode);
    if (r != FALSE)
    {
        return mcli_ok;
    }
    return mcli_error;

#elif defined(MCLI_UNIX)
    return mcli_ok;
#endif
}


mcli_result_t mcli_cleanup(void)
{
#ifdef MCLI_UNIX
    if (g_raw_mode_active)
    {
        mcli_raw_mode(false);
    }
#endif
    return mcli_ok;
}


mcli_result_t mcli_raw_mode(bool enable)
{
#ifdef MCLI_WINDOWS
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    if (hIn == INVALID_HANDLE_VALUE)
    {
        return mcli_error;
    }

    DWORD mode;
    if (!GetConsoleMode(hIn, &mode))
    {
        return mcli_error;
    }

    if (enable)
    {
        mode &= ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
    }
    else
    {
        mode |= (ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
    }

    if (!SetConsoleMode(hIn, mode))
    {
        return mcli_error;
    }

    return mcli_ok;

#elif defined(MCLI_UNIX)
    if (enable)
    {
        if (tcgetattr(STDIN_FILENO, &g_orig_termios) != 0)
        {
            return mcli_error;
        }

        struct termios raw = g_orig_termios;
        raw.c_lflag &= ~(ECHO | ICANON);
        raw.c_cc[VMIN] = 0;
        raw.c_cc[VTIME] = 0;

        if (tcsetattr(STDIN_FILENO, TCSANOW, &raw) != 0)
        {
            return mcli_error;
        }

        g_raw_mode_active = true;
    }
    else
    {
        if (g_raw_mode_active)
        {
            if (tcsetattr(STDIN_FILENO, TCSANOW, &g_orig_termios) != 0)
            {
                return mcli_error;
            }
            g_raw_mode_active = false;
        }
    }

    return mcli_ok;
#endif
}


bool mcli_kbhit(void)
{
#ifdef MCLI_WINDOWS
    return _kbhit() != 0;

#elif defined(MCLI_UNIX)
    struct timeval tv = { 0, 0 };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
#endif
}


int mcli_getchar(void)
{
#ifdef MCLI_WINDOWS
    return _getch();

#elif defined(MCLI_UNIX)
    return getchar();
#endif
}


int mcli_getchar_nb(void)
{
#ifdef MCLI_WINDOWS
    if (!_kbhit())
    {
        return -1;
    }
    return _getch();

#elif defined(MCLI_UNIX)
    if (!mcli_kbhit())
    {
        return -1;
    }
    return getchar();
#endif
}


#ifdef MCLI_WINDOWS

static int mcli_decode_key(void)
{
    const int ch = _getch();

    if (ch == 0 || ch == 224)
    {
        switch (_getch())
        {
            case 72:  return MCLI_KEY_UP;
            case 80:  return MCLI_KEY_DOWN;
            case 75:  return MCLI_KEY_LEFT;
            case 77:  return MCLI_KEY_RIGHT;

            case 71:  return MCLI_KEY_HOME;
            case 79:  return MCLI_KEY_END;
            case 73:  return MCLI_KEY_PAGE_UP;
            case 81:  return MCLI_KEY_PAGE_DOWN;

            case 82:  return MCLI_KEY_INSERT;
            case 83:  return MCLI_KEY_DELETE;

            case 59:  return MCLI_KEY_F1;
            case 60:  return MCLI_KEY_F2;
            case 61:  return MCLI_KEY_F3;
            case 62:  return MCLI_KEY_F4;
            case 63:  return MCLI_KEY_F5;
            case 64:  return MCLI_KEY_F6;
            case 65:  return MCLI_KEY_F7;
            case 66:  return MCLI_KEY_F8;
            case 67:  return MCLI_KEY_F9;
            case 68:  return MCLI_KEY_F10;
            case 133: return MCLI_KEY_F11;
            case 134: return MCLI_KEY_F12;

            default:  return MCLI_KEY_NONE;
        }
    }

    return ch;
}

#endif


#ifdef MCLI_UNIX

static int mcli_decode_key(void)
{
    int ch = getchar();

    if (ch != MCLI_KEY_ESC)
        return ch;

    if (!mcli_kbhit())
        return MCLI_KEY_ESC;

    int seq1 = getchar();
    if (seq1 != '[')
        return MCLI_KEY_ESC;

    int seq2 = getchar();

    switch (seq2)
    {
        case 'A': return MCLI_KEY_UP;
        case 'B': return MCLI_KEY_DOWN;
        case 'C': return MCLI_KEY_RIGHT;
        case 'D': return MCLI_KEY_LEFT;

        case 'H': return MCLI_KEY_HOME;
        case 'F': return MCLI_KEY_END;

        case '3':
            if (getchar() == '~') return MCLI_KEY_DELETE;
            break;

        case '5':
            if (getchar() == '~') return MCLI_KEY_PAGE_UP;
            break;

        case '6':
            if (getchar() == '~') return MCLI_KEY_PAGE_DOWN;
            break;
    }

    return MCLI_KEY_NONE;
}

#endif


int mcli_getkey(void)
{
    return mcli_decode_key();
}

int mcli_getkey_nb(void)
{
    if (!mcli_kbhit())
        return MCLI_KEY_NONE;

    return mcli_getkey();
}


mcli_result_t mcli_term_size(int* rows, int* cols)
{
    if (!rows || !cols)
        return mcli_error;

#ifdef MCLI_WINDOWS
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    if (GetConsoleScreenBufferInfo(hOut, &csbi))
    {
        *cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        *rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        return mcli_ok;
    }
    return mcli_error;

#elif defined(MCLI_UNIX)
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0)
    {
        *rows = ws.ws_row;
        *cols = ws.ws_col;
        return mcli_ok;
    }
    return mcli_error;
#endif
}


// ================================================
// CURSOR & SCREEN CONTROL
//

void mcli_cursor_move(int row, int col)
{
    printf("\033[%d;%dH", row, col);
    fflush(stdout);
}


void mcli_cursor_up(int n)
{
    if (n > 0)
    {
        printf("\033[%dA", n);
        fflush(stdout);
    }
}


void mcli_cursor_down(int n)
{
    if (n > 0)
    {
        printf("\033[%dB", n);
        fflush(stdout);
    }
}


void mcli_cursor_forward(int n)
{
    if (n > 0)
    {
        printf("\033[%dC", n);
        fflush(stdout);
    }
}


void mcli_cursor_back(int n)
{
    if (n > 0)
    {
        printf("\033[%dD", n);
        fflush(stdout);
    }
}


void mcli_cursor_home(void)
{
    printf("\r");
    fflush(stdout);
}


void mcli_cursor_show(void)
{
    printf("\033[?25h");
    fflush(stdout);
}


void mcli_cursor_hide(void)
{
    printf("\033[?25l");
    fflush(stdout);
}


void mcli_cursor_save(void)
{
    printf("\033[s");
    fflush(stdout);
}


void mcli_cursor_restore(void)
{
    printf("\033[u");
    fflush(stdout);
}


void mcli_clear_screen(void)
{
    printf("\033[2J\033[H");
    fflush(stdout);
}


void mcli_clear_line(void)
{
    printf("\033[2K");
    fflush(stdout);
}


void mcli_clear_to_end(void)
{
    printf("\033[0K");
    fflush(stdout);
}


void mcli_clear_to_start(void)
{
    printf("\033[1K");
    fflush(stdout);
}

#endif // MCLI_SOURCE

#endif // MCLI_MCLI_H