#ifndef TS_INPUT_H
#define TS_INPUT_H

// ---------------------------------------------------------------------------
// Input.h -- a scripted key/mouse source for the model's Win32 layer.
//
// PROVENANCE. This header is OUR model, not recovered data. GAME.EXE imports
// GetAsyncKeyState, GetKeyState, GetCursorPos and SetCursorPos (USER32) and
// drives its UI from window messages; it does NOT import DirectInput. The
// current stubs return 0 from both key-state calls and keep a cursor that
// nothing can move, so no key or click can ever reach the game.
//
// WHAT THIS IS. A deterministic input device: a timeline of events parsed from
// a small text script, a key/mouse state table the Win32 stubs read, and the
// WM_KEYDOWN / WM_KEYUP / WM_CHAR / WM_MOUSEMOVE / WM_LBUTTONDOWN / ... messages
// those events synthesise. Time is the model's own millisecond clock
// (Win32Stubs' tick(), advanced by Sleep), so a script is reproducible: the
// same script and the same instruction stream produce the same input.
//
// Script format (one event per line, `#` starts a comment):
//
//     @0    mousemove 320 240
//     @100  lbuttondown
//     @150  lbuttonup
//     @200  keydown 0x41          # 'A', a virtual-key code
//     @250  keyup   0x41
//     @300  text    0x41          # WM_CHAR
//     @400  rbuttondown
//     @450  rbuttonup
//     @500  mbuttondown
//     @550  mbuttonup
//     @600  wheel   -120
//
// Times are milliseconds since the script started; they need not be sorted
// (load_script sorts stably). Unknown commands and malformed lines make
// load_script return false and leave last_error() naming the line.
//
// INTEGRATION. This header does not include Win32Stubs.h. The stub bodies call
// the accessors below; the one-line bodies are listed in tools/test_runtime.py
// and in the report that accompanies this file.
//
// Header-only, C++03.
// ---------------------------------------------------------------------------

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Machine.h"

namespace ts {
namespace input {

enum {
    MAX_EVENTS   = 1024,
    MAX_MESSAGES = 1024,
    VK_LBUTTON   = 0x01,
    VK_RBUTTON   = 0x02,
    VK_CANCEL    = 0x03,
    VK_MBUTTON   = 0x04,
    VK_BACK      = 0x08,
    VK_TAB       = 0x09,
    VK_RETURN    = 0x0d,
    VK_SHIFT     = 0x10,
    VK_CONTROL   = 0x11,
    VK_MENU      = 0x12,
    VK_PAUSE     = 0x13,
    VK_CAPITAL   = 0x14,
    VK_ESCAPE    = 0x1b,
    VK_SPACE     = 0x20,
    VK_PRIOR     = 0x21,
    VK_NEXT      = 0x22,
    VK_END       = 0x23,
    VK_HOME      = 0x24,
    VK_LEFT      = 0x25,
    VK_UP        = 0x26,
    VK_RIGHT     = 0x27,
    VK_DOWN      = 0x28,
    VK_INSERT    = 0x2d,
    VK_DELETE    = 0x2e,
    VK_NUMLOCK   = 0x90,
    VK_SCROLL    = 0x91,

    MK_LBUTTON   = 0x0001,
    MK_RBUTTON   = 0x0002,
    MK_SHIFT     = 0x0004,
    MK_CONTROL   = 0x0008,
    MK_MBUTTON   = 0x0010,

    WM_KEYDOWN      = 0x0100,
    WM_KEYUP        = 0x0101,
    WM_CHAR         = 0x0102,
    WM_SYSKEYDOWN   = 0x0104,
    WM_SYSKEYUP     = 0x0105,
    WM_MOUSEMOVE    = 0x0200,
    WM_LBUTTONDOWN  = 0x0201,
    WM_LBUTTONUP    = 0x0202,
    WM_LBUTTONDBLCLK= 0x0203,
    WM_RBUTTONDOWN  = 0x0204,
    WM_RBUTTONUP    = 0x0205,
    WM_MBUTTONDOWN  = 0x0207,
    WM_MBUTTONUP    = 0x0208,
    WM_MOUSEWHEEL   = 0x020a
};

enum Kind {
    EV_NONE = 0,
    EV_KEYDOWN,
    EV_KEYUP,
    EV_TEXT,
    EV_MOUSEMOVE,
    EV_LBUTTONDOWN,
    EV_LBUTTONUP,
    EV_RBUTTONDOWN,
    EV_RBUTTONUP,
    EV_MBUTTONDOWN,
    EV_MBUTTONUP,
    EV_WHEEL
};

struct Event {
    u32 at_ms;
    u16 kind;
    u16 a;      // virtual key or character
    i32 b, c;   // mouse coordinates or wheel delta
    Event() : at_ms(0), kind(EV_NONE), a(0), b(0), c(0) {}
};

struct Message {
    u32 msg, wparam, lparam;
    Message() : msg(0), wparam(0), lparam(0) {}
};

struct State {
    bool down[256];
    bool latch[256];      // pressed since the last GetAsyncKeyState
    bool toggle[256];     // CAPITAL/NUMLOCK/SCROLL toggle state
    u8   kbd[256];        // GetKeyboardState image (0x80 = down)
    i32  mouse_x, mouse_y;
    u32  buttons;         // MK_* bits
    u32  now_ms;
    unsigned next_event;
    unsigned nevents;
    Event events[MAX_EVENTS];
    unsigned msg_head, msg_tail;
    Message messages[MAX_MESSAGES];
    char error[160];

    State() : mouse_x(0), mouse_y(0), buttons(0), now_ms(0),
              next_event(0), nevents(0), msg_head(0), msg_tail(0) {
        for (unsigned i = 0; i < 256u; ++i) {
            down[i] = false; latch[i] = false; toggle[i] = false; kbd[i] = 0;
        }
        error[0] = 0;
    }
};

inline State &st() { static State s; return s; }

inline const char *last_error() { return st().error; }

inline void set_error(const char *what, int line) {
    if (line > 0)
        std::sprintf(st().error, "%s at line %d", what, line);
    else
        std::sprintf(st().error, "%s", what);
}

inline void reset() {
    State &s = st();
    s = State();
}

// -- script parsing ----------------------------------------------------------
inline const char *skip_ws(const char *p) {
    while (*p == ' ' || *p == '\t' || *p == '\r') ++p;
    return p;
}

inline bool token(const char **pp, char *out, unsigned cap) {
    const char *p = skip_ws(*pp);
    unsigned n = 0;
    while (*p && *p != ' ' && *p != '\t' && *p != '\r' && *p != '\n' && *p != '#') {
        if (n + 1u < cap) out[n++] = *p;
        ++p;
    }
    out[n] = 0;
    *pp = p;
    return n != 0u;
}

inline bool number(const char **pp, i32 *out) {
    const char *p = skip_ws(*pp);
    if (!*p) return false;
    char *end = 0;
    long v = std::strtol(p, &end, 0);
    if (end == p) return false;
    *out = (i32)v;
    *pp = end;
    return true;
}

// Stable insertion sort by time: a script may be written out of order, but
// events at the same millisecond keep the order they were written in.
inline void sort_events(Event *e, unsigned n) {
    for (unsigned i = 1; i < n; ++i) {
        Event v = e[i];
        unsigned j = i;
        while (j > 0u && e[j - 1u].at_ms > v.at_ms) {
            e[j] = e[j - 1u];
            --j;
        }
        e[j] = v;
    }
}

inline bool load_script(const char *text) {
    State &s = st();
    reset();
    if (!text) { set_error("null script", 0); return false; }
    const char *p = text;
    int line = 0;
    while (*p) {
        ++line;
        p = skip_ws(p);
        if (*p == '#' || *p == '\n' || *p == 0) {
            while (*p && *p != '\n') ++p;
            if (*p == '\n') ++p;
            continue;
        }
        if (*p != '@') { set_error("expected @<ms>", line); return false; }
        ++p;
        i32 at = 0;
        if (!number(&p, &at) || at < 0) { set_error("bad time", line); return false; }
        char cmd[32];
        if (!token(&p, cmd, sizeof(cmd))) { set_error("missing command", line); return false; }
        if (s.nevents >= (unsigned)MAX_EVENTS) { set_error("too many events", line); return false; }

        Event ev;
        ev.at_ms = (u32)at;
        if (!std::strcmp(cmd, "keydown") || !std::strcmp(cmd, "keyup")) {
            i32 vk = 0;
            if (!number(&p, &vk) || vk < 0 || vk > 255) { set_error("bad virtual key", line); return false; }
            ev.kind = (u16)(cmd[3] == 'd' ? EV_KEYDOWN : EV_KEYUP);
            ev.a = (u16)vk;
        } else if (!std::strcmp(cmd, "text")) {
            i32 ch = 0;
            if (!number(&p, &ch) || ch < 0 || ch > 0xffff) { set_error("bad character", line); return false; }
            ev.kind = EV_TEXT;
            ev.a = (u16)ch;
        } else if (!std::strcmp(cmd, "mousemove")) {
            i32 x = 0, y = 0;
            if (!number(&p, &x) || !number(&p, &y)) { set_error("mousemove needs x y", line); return false; }
            ev.kind = EV_MOUSEMOVE;
            ev.b = x; ev.c = y;
        } else if (!std::strcmp(cmd, "lbuttondown"))  { ev.kind = EV_LBUTTONDOWN;
        } else if (!std::strcmp(cmd, "lbuttonup"))    { ev.kind = EV_LBUTTONUP;
        } else if (!std::strcmp(cmd, "rbuttondown"))  { ev.kind = EV_RBUTTONDOWN;
        } else if (!std::strcmp(cmd, "rbuttonup"))    { ev.kind = EV_RBUTTONUP;
        } else if (!std::strcmp(cmd, "mbuttondown"))  { ev.kind = EV_MBUTTONDOWN;
        } else if (!std::strcmp(cmd, "mbuttonup"))    { ev.kind = EV_MBUTTONUP;
        } else if (!std::strcmp(cmd, "wheel")) {
            i32 d = 0;
            if (!number(&p, &d)) { set_error("wheel needs a delta", line); return false; }
            ev.kind = EV_WHEEL;
            ev.b = d;
        } else {
            set_error("unknown command", line);
            return false;
        }
        // Anything left on the line other than a comment is an error: a typo
        // in an argument must not silently become a shorter event.
        p = skip_ws(p);
        if (*p && *p != '#' && *p != '\n') { set_error("trailing text", line); return false; }
        while (*p && *p != '\n') ++p;
        if (*p == '\n') ++p;
        s.events[s.nevents++] = ev;
    }
    sort_events(s.events, s.nevents);
    s.error[0] = 0;
    return true;
}

inline bool load_file(const char *path) {
    std::FILE *f = std::fopen(path, "rb");
    if (!f) { set_error("cannot open script", 0); return false; }
    char buf[8192];
    unsigned n = (unsigned)std::fread(buf, 1, sizeof(buf) - 1u, f);
    std::fclose(f);
    buf[n] = 0;
    return load_script(buf);
}

// -- key/mouse state ---------------------------------------------------------
// Scan codes for the US layout. The game's WM_KEYDOWN handler may read the
// scan code out of lParam bits 16..23; a zero there is "no key", so the common
// keys are mapped rather than left at zero.
inline u8 scancode(u32 vk) {
    static const u8 letters[26] = {
        0x1e, 0x30, 0x2e, 0x20, 0x12, 0x21, 0x22, 0x23, 0x17, 0x24,
        0x25, 0x26, 0x32, 0x31, 0x18, 0x19, 0x10, 0x13, 0x1f, 0x14,
        0x16, 0x2f, 0x11, 0x2d, 0x15, 0x2c
    };
    if (vk >= 'A' && vk <= 'Z') return letters[vk - 'A'];
    if (vk >= '0' && vk <= '9') return (u8)(vk == '0' ? 0x0b : (vk - '0' + 0x01));
    switch (vk) {
    case VK_ESCAPE: return 0x01;
    case VK_BACK:   return 0x0e;
    case VK_TAB:    return 0x0f;
    case VK_RETURN: return 0x1c;
    case VK_CONTROL:return 0x1d;
    case VK_MENU:   return 0x38;
    case VK_SPACE:  return 0x39;
    case VK_CAPITAL:return 0x3a;
    case VK_SHIFT:  return 0x2a;
    case VK_PAUSE:  return 0x45;
    case VK_HOME:   return 0x47;
    case VK_PRIOR:  return 0x49;
    case VK_END:    return 0x4f;
    case VK_NEXT:   return 0x51;
    case VK_INSERT: return 0x52;
    case VK_DELETE: return 0x53;
    case VK_UP:     return 0x48;
    case VK_LEFT:   return 0x4b;
    case VK_RIGHT:  return 0x4d;
    case VK_DOWN:   return 0x50;
    case VK_NUMLOCK:return 0x45;
    case VK_SCROLL: return 0x46;
    default: break;
    }
    if (vk >= 0x70 && vk <= 0x7b) return (u8)(0x3b + (vk - 0x70));   // F1..F12
    return 0u;
}

inline void push_message(u32 msg, u32 wp, u32 lp) {
    State &s = st();
    if (s.msg_tail - s.msg_head >= (unsigned)MAX_MESSAGES) return;
    s.messages[s.msg_tail % (unsigned)MAX_MESSAGES].msg = msg;
    s.messages[s.msg_tail % (unsigned)MAX_MESSAGES].wparam = wp;
    s.messages[s.msg_tail % (unsigned)MAX_MESSAGES].lparam = lp;
    ++s.msg_tail;
}

inline u32 make_lparam(i32 x, i32 y) {
    return ((u32)x & 0xffffu) | (((u32)y & 0xffffu) << 16);
}

inline void set_down(u32 vk, bool down_now) {
    State &s = st();
    if (vk > 255u) return;
    if (down_now && !s.down[vk]) {
        s.latch[vk] = true;
        if (vk == VK_CAPITAL || vk == VK_NUMLOCK || vk == VK_SCROLL)
            s.toggle[vk] = !s.toggle[vk];
    }
    s.down[vk] = down_now;
    s.kbd[vk] = down_now ? 0x80u : 0x00u;
}

inline void apply(const Event &e) {
    State &s = st();
    switch (e.kind) {
    case EV_KEYDOWN:
        set_down(e.a, true);
        push_message(WM_KEYDOWN, e.a,
                     1u | ((u32)scancode(e.a) << 16));
        break;
    case EV_KEYUP:
        set_down(e.a, false);
        push_message(WM_KEYUP, e.a,
                     1u | ((u32)scancode(e.a) << 16) | (1u << 30) | (1u << 31));
        break;
    case EV_TEXT:
        push_message(WM_CHAR, e.a, 1u);
        break;
    case EV_MOUSEMOVE:
        s.mouse_x = e.b; s.mouse_y = e.c;
        push_message(WM_MOUSEMOVE, s.buttons, make_lparam(e.b, e.c));
        break;
    case EV_LBUTTONDOWN:
        s.buttons |= MK_LBUTTON; set_down(VK_LBUTTON, true);
        push_message(WM_LBUTTONDOWN, MK_LBUTTON, make_lparam(s.mouse_x, s.mouse_y));
        break;
    case EV_LBUTTONUP:
        s.buttons &= ~(u32)MK_LBUTTON; set_down(VK_LBUTTON, false);
        push_message(WM_LBUTTONUP, 0u, make_lparam(s.mouse_x, s.mouse_y));
        break;
    case EV_RBUTTONDOWN:
        s.buttons |= MK_RBUTTON; set_down(VK_RBUTTON, true);
        push_message(WM_RBUTTONDOWN, MK_RBUTTON, make_lparam(s.mouse_x, s.mouse_y));
        break;
    case EV_RBUTTONUP:
        s.buttons &= ~(u32)MK_RBUTTON; set_down(VK_RBUTTON, false);
        push_message(WM_RBUTTONUP, 0u, make_lparam(s.mouse_x, s.mouse_y));
        break;
    case EV_MBUTTONDOWN:
        s.buttons |= MK_MBUTTON; set_down(VK_MBUTTON, true);
        push_message(WM_MBUTTONDOWN, MK_MBUTTON, make_lparam(s.mouse_x, s.mouse_y));
        break;
    case EV_MBUTTONUP:
        s.buttons &= ~(u32)MK_MBUTTON; set_down(VK_MBUTTON, false);
        push_message(WM_MBUTTONUP, 0u, make_lparam(s.mouse_x, s.mouse_y));
        break;
    case EV_WHEEL:
        push_message(WM_MOUSEWHEEL,
                     ((u32)(u16)e.b << 16) | s.buttons,
                     make_lparam(s.mouse_x, s.mouse_y));
        break;
    default:
        break;
    }
}

// Apply every event due at or before `now_ms`, exactly once, in time order.
inline void advance(u32 now_ms) {
    State &s = st();
    s.now_ms = now_ms;
    while (s.next_event < s.nevents && s.events[s.next_event].at_ms <= now_ms) {
        apply(s.events[s.next_event]);
        ++s.next_event;
    }
}

// The next synthesised window message, or false when the timeline has none
// left. The caller (PeekMessageA) decides which window it belongs to.
inline bool pop_message(u32 *msg, u32 *wparam, u32 *lparam) {
    State &s = st();
    if (s.msg_head == s.msg_tail) return false;
    Message &m = s.messages[s.msg_head % (unsigned)MAX_MESSAGES];
    if (msg) *msg = m.msg;
    if (wparam) *wparam = m.wparam;
    if (lparam) *lparam = m.lparam;
    ++s.msg_head;
    return true;
}

inline unsigned pending_messages() {
    State &s = st();
    return s.msg_tail - s.msg_head;
}

// -- the accessors the Win32 stubs call --------------------------------------
// GetAsyncKeyState: high bit = currently down, low bit = pressed since the
// previous call. The latch is cleared by the read, as Windows does.
inline u32 async_key_state(u32 vk) {
#ifdef TS_MUTATION_KEY_ZERO
    (void)vk;
    return 0u;
#else
    State &s = st();
    if (vk > 255u) return 0u;
    u32 r = s.down[vk] ? 0x8000u : 0u;
    if (s.latch[vk]) { r |= 0x0001u; s.latch[vk] = false; }
    return r;
#endif
}

// GetKeyState: high bit = down, low bit = toggle state for the toggle keys.
inline u32 key_state(u32 vk) {
    State &s = st();
    if (vk > 255u) return 0u;
    u32 r = s.down[vk] ? 0x8000u : 0u;
    if (s.toggle[vk]) r |= 0x0001u;
    return r;
}

// GetKeyboardState(LPBYTE): 256 bytes, 0x80 for a down key.
inline void keyboard_state(u32 va) {
    State &s = st();
    if (!va) return;
    for (u32 i = 0; i < 256u; ++i) st8(va + i, s.kbd[i]);
}

inline void cursor_pos(u32 va) {
    State &s = st();
    if (!va) return;
    st32(va + 0u, (u32)s.mouse_x);
    st32(va + 4u, (u32)s.mouse_y);
}

inline void set_cursor(i32 x, i32 y) {
    st().mouse_x = x;
    st().mouse_y = y;
}

inline u32 mouse_buttons() { return st().buttons; }
inline i32 mouse_x() { return st().mouse_x; }
inline i32 mouse_y() { return st().mouse_y; }
inline bool is_down(u32 vk) { return vk < 256u && st().down[vk]; }

}  // namespace input
}  // namespace ts

#endif  // TS_INPUT_H
