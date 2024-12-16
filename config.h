/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx = 3; /* border pixel of windows */
static const unsigned int gappx = 10;   /* gaps between windows */
static const unsigned int snap = 32;    /* snap pixel */
static const int showbar = 1;           /* 0 means no bar */
static const int topbar = 1;            /* 0 means bottom bar */
static const char *fonts[] = {"JetBrainsMono Nerd Font:size=12",
                              "JoyPixels:size=12:antialias=true:autohint:true"};
static const char dmenufont[] = "JetBrainsMono Nerd Font:size=12";
static const char col_gray1[] = "#1d2021";
static const char col_gray2[] = "#3c3836";
static const char col_gray3[] = "#ebdbb2";
static const char col_gray4[] = "#1d2021";
static const char col_cyan[] = "#689d6a";
static const char *colors[][3] = {
    /*               fg         bg         border   */
    [SchemeNorm] = {col_gray3, col_gray1, col_gray2},
    [SchemeSel] = {col_gray4, col_cyan, col_cyan},
};

/* tagging */
static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     isfloating   monitor */
    // { "Gimp",     NULL,       NULL,       0,            1,           -1 },
    {"Firefox", NULL, NULL, 1 << 8, 0, -1},
};

/* layout(s) */
static const float mfact = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints =
    1; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen =
    1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
    /* symbol     arrange function */
    {"[]=", tile}, /* first entry is default */
    {"><>", NULL}, /* no layout function means floating behavior */
    {"[M]", monocle},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                      \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
      {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},               \
      {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                        \
      {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

#include "movestack.c"
#include <X11/XF86keysym.h>

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
  {                                                                            \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                       \
  }

/* commands */
static char dmenumon[2] =
    "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {
    "dmenu_run", "-m",  dmenumon,  "-p",  "Run: ",  "-fn", dmenufont, "-nb",
    col_gray1,   "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL};

static const Key keys[] = {
    /* modifier                     key        function        argument */
    {MODKEY, XK_d, spawn, {.v = dmenucmd}},
    {MODKEY, XK_Return, spawn, SHCMD("$TERMINAL")},
    {MODKEY, XK_z, togglebar, {0}},
    {MODKEY, XK_j, focusstack, {.i = +1}},
    {MODKEY, XK_k, focusstack, {.i = -1}},
    {MODKEY, XK_i, incnmaster, {.i = +1}},
    {MODKEY, XK_r, incnmaster, {.i = -1}},
    {MODKEY, XK_h, setmfact, {.f = -0.05}},
    {MODKEY, XK_l, setmfact, {.f = +0.05}},
    {MODKEY | ShiftMask, XK_j, movestack, {.i = +1}},
    {MODKEY | ShiftMask, XK_k, movestack, {.i = -1}},
    {MODKEY | ShiftMask, XK_Return, zoom, {0}},
    {MODKEY | ShiftMask, XK_Tab, view, {0}},
    {MODKEY, XK_q, killclient, {0}},
    {MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
    {MODKEY, XK_f, setlayout, {.v = &layouts[1]}},
    {MODKEY, XK_m, setlayout, {.v = &layouts[2]}},
    {MODKEY | ShiftMask, XK_f, fullscreen, {0}},
    {MODKEY, XK_space, setlayout, {0}},
    {MODKEY | ShiftMask, XK_space, togglefloating, {0}},
    {MODKEY, XK_0, view, {.ui = ~0}},
    {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
    {MODKEY, XK_comma, focusmon, {.i = -1}},
    {MODKEY, XK_period, focusmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},
    {MODKEY, XK_c, movecenter, {0}},
    {MODKEY, XK_minus, setgaps, {.i = -1}},
    {MODKEY, XK_equal, setgaps, {.i = +1}},
    {MODKEY | ShiftMask, XK_equal, setgaps, {.i = 0}},

    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5) TAGKEYS(XK_7, 6) TAGKEYS(XK_8, 7)
            TAGKEYS(XK_9, 8)

    // { 0,				XF86XK_AudioMute, spawn,   SHCMD("pactl
    // set-sink-mute @DEFAULT_SINK@ toggle") }, { 0,
    // XF86XK_AudioMicMute, spawn,   SHCMD("pactl set-source-mute
    // @DEFAULT_SOURCE@ toggle") }, { 0,
    // XF86XK_AudioRaiseVolume, spawn,   SHCMD("pactl set-sink-volume
    // @DEFAULT_SINK@ +5%") }, { 0,
    // XF86XK_AudioLowerVolume, spawn,   SHCMD("pactl set-sink-volume
    // @DEFAULT_SINK@ -5%") }, { 0,
    // XF86XK_MonBrightnessUp, spawn,    SHCMD("brightnessctl s +5%") }, { 0,
    // XF86XK_MonBrightnessDown, spawn,  SHCMD("brightnessctl s 5%-") },
    {0, XF86XK_AudioMute, spawn, SHCMD("~/.config/dunst/dunstify_vol -t")},
    {MODKEY | ShiftMask, XK_o, spawn, SHCMD("~/.config/dunst/dunstify_vol -t")},
    {0, XF86XK_AudioMicMute, spawn, SHCMD("~/.config/dunst/dunstify_vol -y")},
    {MODKEY | ShiftMask, XK_m, spawn, SHCMD("~/.config/dunst/dunstify_vol -y")},
    {0, XF86XK_AudioRaiseVolume, spawn,
     SHCMD("~/.config/dunst/dunstify_vol -i 10")},
    {0, XF86XK_AudioLowerVolume, spawn,
     SHCMD("~/.config/dunst/dunstify_vol -d 10")},
    {0, XF86XK_MonBrightnessUp, spawn, SHCMD("~/.config/dunst/dunstify_br -i")},
    {0, XF86XK_MonBrightnessDown, spawn,
     SHCMD("~/.config/dunst/dunstify_br -d")},
    {0, XF86XK_AudioPlay, spawn, SHCMD("playerctl play-pause")},
    {0, XF86XK_AudioPause, spawn, SHCMD("playerctl play-pause")},
    {MODKEY, XK_p, spawn, SHCMD("playerctl play-pause")},
    {0, XF86XK_AudioNext, spawn, SHCMD("playerctl next")},
    {0, XF86XK_AudioPrev, spawn, SHCMD("playerctl previous")},
    {0, XF86XK_TouchpadToggle, spawn, SHCMD("~/.local/bin/toggletouchpad")},
    {MODKEY, XK_b, spawn, SHCMD("$BROWSER")},
    {MODKEY | ShiftMask, XK_b, spawn, SHCMD("chromium")},
    {MODKEY | ShiftMask, XK_l, spawn, SHCMD("slock")},
    {MODKEY | ShiftMask, XK_c, spawn, SHCMD("~/.local/bin/color-picker")},
    {MODKEY | ShiftMask, XK_e, spawn, SHCMD("~/.local/bin/emoji")},
    {MODKEY, XK_Print, spawn, SHCMD("~/.local/bin/ss")},
    {MODKEY | ShiftMask, XK_s, spawn, SHCMD("~/.local/bin/ss")},
    {MODKEY | ShiftMask, XK_Print, spawn, SHCMD("~/.local/bin/ss -s")},
    {MODKEY | ControlMask | ShiftMask, XK_s, spawn,
     SHCMD("~/.local/bin/ss -s")},
    {MODKEY, XK_v, spawn, SHCMD("~/.local/bin/sclip")},
    {MODKEY | ShiftMask, XK_p, spawn, SHCMD("~/.local/bin/passmenu")},
    {MODKEY | ShiftMask, XK_q, spawn, SHCMD("~/.local/bin/system-action")},
    {MODKEY | ControlMask, XK_q, quit, {0}},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask      button          function argument */
    {ClkLtSymbol, 0, Button1, setlayout, {0}},
    {ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
    {ClkWinTitle, 0, Button2, zoom, {0}},
    {ClkStatusText, 0, Button2, spawn, SHCMD("$TERMINAL")},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
