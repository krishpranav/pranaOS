/*
 * Copyright (c) 2021 krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <AK/Debug.h>
#include <AK/HashMap.h>
#include <AK/String.h>
#include <AK/Vector.h>
#include <assert.h>
#include <string.h>
#include <termcap.h>

extern "C" {

char PC;
char* UP;
char* BC;

int tgetent([[maybe_unused]] char* bp, [[maybe_unused]] const char* name)
{
    warnln_if(TERMCAP_DEBUG, "tgetent: bp={:p}, name='{}'", bp, name);
    PC = '\0';
    BC = const_cast<char*>("\033[D");
    UP = const_cast<char*>("\033[A");
    return 1;
}

static HashMap<String, const char*>* caps = nullptr;

static void ensure_caps()
{
    if (caps)
        return;
    caps = new HashMap<String, const char*>;
    caps->set("DC", "\033[%p1%dP");
    caps->set("IC", "\033[%p1%d@");
    caps->set("ce", "\033[K");
    caps->set("cl", "\033[H\033[J");
    caps->set("cr", "\015");
    caps->set("dc", "\033[P");
    caps->set("ei", "");
    caps->set("ic", "");
    caps->set("im", "");
    caps->set("kd", "\033[B");
    caps->set("kl", "\033[D");
    caps->set("kr", "\033[C");
    caps->set("ku", "\033[A");
    caps->set("ks", "");
    caps->set("ke", "");
    caps->set("le", "\033[D");
    caps->set("mm", "");
    caps->set("mo", "");
    caps->set("pc", "");
    caps->set("up", "\033[A");
    caps->set("vb", "");
    caps->set("am", "");
    caps->set("@7", "");
    caps->set("kH", "");
    caps->set("kI", "\033[L");
    caps->set("kh", "\033[H");
    caps->set("vs", "");
    caps->set("ve", "");
    caps->set("E3", "");
    caps->set("kD", "");
    caps->set("nd", "\033[C");

    caps->set("co", "80");
    caps->set("li", "25");
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

char* tgetstr(const char* id, char** area)
{
    ensure_caps();
    warnln_if(TERMCAP_DEBUG, "tgetstr: id='{}'", id);
    auto it = caps->find(id);
    if (it != caps->end()) {
        char* ret = *area;
        const char* val = (*it).value;
        strcpy(*area, val);
        *area += strlen(val) + 1;
        return ret;
    }
    warnln_if(TERMCAP_DEBUG, "tgetstr: missing cap id='{}'", id);
    return nullptr;
}

}
